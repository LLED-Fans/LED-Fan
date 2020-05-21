import math
from typing import Callable

import requests
import socket
from PIL import Image, ImageEnhance

from datetime import datetime, timedelta

from artnet import ArtnetProvider

from util import grouper, flatmap, bilinear, BufferedResource, RegularClock


def pixel_at(image: Image, x: float, y: float):
    r = bilinear(image, x * (image.width - 1), y * (image.height - 1))
    return r


def run(
    ip: str,
    endpoint: str,
    image_provider: Callable[[], Image.Image],
    frames_per_second: int = 30,
    contrast=2
):
    print("Getting Server Info")

    server_info = requests.get(f"http://{ip}/i").json()
    # print(server_info)

    endpoint_info = server_info[endpoint]
    net = int(endpoint_info["net"])

    port = 6454  # default artnet port

    if endpoint == "concentric":
        import numexpr

        def adding_var(name, formula, infos):
            return [
                {name: numexpr.evaluate(formula, info).item(), **info}
                for info in infos
            ]

        infos = [{'pi': math.pi}]
        for name, equation in zip(endpoint_info['names'], endpoint_info['equations']):
            if name.startswith("|"):
                name = name[1:]
                infos = adding_var(name, equation, infos)
                infos = [
                    {**info, name: i}
                    for info in infos
                    for i in range(info[name])
                ]
            else:
                infos = adding_var(name, equation, infos)
        pixels = [(info['x'], info['y']) for info in infos]
        resolution = (infos[0]["rings"]*2,) * 2
        print(f"Sampled Pixels: {len(pixels)}")

        def grab_frame():
            img = image_provider().resize(resolution)

            return bytes(flatmap(
                lambda t: pixel_at(img, *t),
                pixels
            ))

    elif endpoint == "cartesian":
        resolution = (endpoint_info["width"], endpoint_info["height"])
        print(f"Screen Size: {resolution[0]}x{resolution[1]}")

        def grab_frame():
            img = image_provider().resize(resolution)
            if contrast > 1:
                img = ImageEnhance.Contrast(img).enhance(contrast)
            # img.save("backup.png")
            return img.tobytes("raw")
    else:
        raise Exception()

# ------------------------------------------------------
    # --- Frame Conversion
    # ------------------------------------------------------

    data_resource = BufferedResource(max_buffer_size=2)
    data_resource.start_async_factory(grab_frame)

    # ------------------------------------------------------
    # --- Packet Assembly
    # ------------------------------------------------------

    artnet_provider = ArtnetProvider(
        universe=0,
        subnet=0,
        net=net
    )

    def grab_artnet_packets():
        return list(map(bytes, artnet_provider(data_resource.pop())))

    packets_resource = BufferedResource(max_buffer_size=2)
    packets_resource.start_async_factory(grab_artnet_packets)

    # ------------------------------------------------------
    # --- Loop
    # ------------------------------------------------------

    sock = socket.socket(
        socket.AF_INET,  # Internet
        socket.SOCK_DGRAM  # UDP
    )

    time_per_frame = timedelta(seconds=1.0 / frames_per_second)

    print(f"Sending Art-Net Data to: {ip}:{port} — {artnet_provider.net}|{artnet_provider.subnet}|{artnet_provider.universe}!")
    start = datetime.now()

    clock = RegularClock()
    frame_start = clock.mark()

    last_status_update = start
    frame_update_time = timedelta(seconds=10)
    frames_since_status_update = 0

    while True:
        packets = packets_resource.pop()
        for packet in packets:
            sock.sendto(
                packet,
                (ip, port)
            )

        frames_since_status_update += 1

        if frame_start - last_status_update > frame_update_time:
            print(
                f"===> "
                f"kB: {sum(map(len, packets)) / 3}"
                f", Packets p.f.: {len(packets)}"
                f", FPS: {frames_since_status_update / (frame_start - last_status_update).total_seconds()}"
            )
            frames_since_status_update = 0
            last_status_update = frame_start

        # For plotting coords
        # log = requests.get(f"http://{ip}/log")
        # img2 = img.copy()
        # for idx, x, y in grouper(3, log.text.split("\n")):
        #     if x and y:
        #         print(idx, x, y)
        #         img2.putpixel((int(x), int(y)), (int(idx) * 14, 0, 255))
        # img2.save("rs.png")

        frame_start = clock.elapse(time_per_frame)
