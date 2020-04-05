import io
import random
import time
from typing import Callable

import requests
import socket
from PIL import Image
from mss import mss

from datetime import datetime
from threading import Thread

from artnet import ArtnetProvider
from util import grouper, flatmap, bilinear


def pixel_at(image: Image, x: float, y: float):
    r = bilinear(image, x * (image.width - 1), y * (image.height - 1))
    return r


def run(
    ip: str,
    endpoint: str,
    image_provider: Callable[[], Image.Image],
    simulated_rotation_seconds: int = 0,
    rotate_input_seconds: int = 0,
    frames_per_second: int = 30
):
    print("Getting Server Info")

    server_info = requests.get(f"http://{ip}/i").json()
    # print(server_info)

    endpoint_info = server_info[endpoint]
    net = int(endpoint_info["net"])

    port = 6454  # default artnet port

    if endpoint == "concentric":
        pixels = list(grouper(2, endpoint_info["pixels"]))
    elif endpoint == "cartesian":
        resolution = (endpoint_info["width"], endpoint_info["height"])

    sock = socket.socket(
        socket.AF_INET,  # Internet
        socket.SOCK_DGRAM  # UDP
    )

    artnet_provider = ArtnetProvider(
        universe=0,
        subnet=0,
        net=net
    )

    seconds_per_frame = 1.0 / frames_per_second

    print(f"Sending Art-Net Data to: {ip}:{port} — {artnet_provider.net}|{artnet_provider.subnet}|{artnet_provider.universe}!")
    start = datetime.now()
    sequence_start = start

    try:
        while True:
            frame_start = datetime.now()
            uptime = (frame_start - start).total_seconds()

            img = image_provider()

            if rotate_input_seconds > 0:
                img = img.rotate(uptime * 360 / rotate_input_seconds)

            if endpoint == "concentric":
                data = bytes(flatmap(
                    lambda t: pixel_at(img, *t),
                    pixels
                ))
            else:
                img = img.resize(resolution)
                data = img.tobytes("raw")

            packets = artnet_provider(data)
            for packet in packets:
                sock.sendto(
                    bytes(packet),
                    (ip, port)
                )

            if artnet_provider.sequence == 0:
                print(
                    f"Sequence Pushed! RGB Pixels: {len(data) / 3}, Packets p.f.: {len(packets)}, FPS: {255.0 / (frame_start - sequence_start).total_seconds()}")
                sequence_start = frame_start

            if simulated_rotation_seconds > 0:
                simulated_rotation = (uptime / simulated_rotation_seconds) % simulated_rotation_seconds
                requests.post(f"http://{ip}/rotation/set", data={"rotation": (simulated_rotation)})

            # For plotting coords
            # log = requests.get(f"http://{ip}/log")
            # img2 = img.copy()
            # for idx, x, y in grouper(3, log.text.split("\n")):
            #     if x and y:
            #         print(idx, x, y)
            #         img2.putpixel((int(x), int(y)), (int(idx) * 14, 0, 255))
            # img2.save("rs.png")

            time_this_frame = datetime.now() - frame_start
            if time_this_frame.total_seconds() < seconds_per_frame:
                time.sleep(seconds_per_frame - time_this_frame.total_seconds())
    finally:
        if simulated_rotation_seconds > 0:
            requests.post(f"http://{ip}/rotation/set", data={"rotation": "-1"})
