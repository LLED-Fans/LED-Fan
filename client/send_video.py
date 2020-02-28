import io
import random
import time
from typing import Callable

import requests
import socket
from PIL import Image
from mss import mss

from artnet import ArtnetProvider
from util import grouper, flatmap, bilinear
from datetime import datetime

import argparse


def get_white_noise_image(width, height):
    pil_map = Image.new("RGB", (width, height), 255)
    random_grid = map(lambda x: (
        int(random.random() * 256),
        int(random.random() * 256),
        int(random.random() * 256)
    ), [0] * width * height)
    pil_map.putdata(random_grid)
    return pil_map


def pixel_at(image: Image, x: float, y: float):
    r = bilinear(image, x * (image.width - 1), y * (image.height - 1))
    return r


def run(
    ip: str,
    endpoint: str,
    image_provider: Callable[[], Image.Image],
    simulated_rotation_seconds: int = 0,
    frames_per_second: int = 30
):
    print("Getting Server Info")

    server_info = requests.get(f"http://{ip}/i/cc").json()
    #print(server_info)

    artnet_info = server_info[endpoint]
    port = int(artnet_info["port"])

    if endpoint == "concentric":
        pixels = list(grouper(2, artnet_info["pixels"]))

    sock = socket.socket(
        socket.AF_INET,    # Internet
        socket.SOCK_DGRAM  # UDP
    )

    artnet_provider = ArtnetProvider(
        universe=0,
        subnet=0,
        net=0
    )

    seconds_per_frame = 1.0 / frames_per_second

    print(f"Sending Art-Net Data to: {ip}:{port}!")
    sequence_start = datetime.now()

    simulated_rotation = 0

    try:
        while True:
            frame_start = datetime.now()

            img = image_provider()

            if endpoint == "concentric":
                data = bytes(flatmap(
                    lambda t: pixel_at(img, *t),
                    pixels
                ))
            else:
                img = img.resize((64, 64))
                data = img.tobytes("raw")

            packets = artnet_provider(data)
            for packet in packets:
                sock.sendto(
                    bytes(packet),
                    (ip, port)
                )

            if artnet_provider.sequence == 0:
                print(f"Sequence Pushed! RGB Pixels: {len(data) / 3}, Packets p.f.: {len(packets)}, FPS: {255.0 / (frame_start - sequence_start).total_seconds()}")
                sequence_start = frame_start

            if simulated_rotation_seconds > 0:
                requests.post(f"http://{ip}/rotation/set", data={"rotation": simulated_rotation})
                simulated_rotation = (simulated_rotation + seconds_per_frame / simulated_rotation_seconds) % 1.0

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


command_parser = argparse.ArgumentParser()

command_parser.add_argument(
    "ip", default="192.168.1.4",
)
command_parser.add_argument(
    "--endpoint", "-e",
    default="concentric"
)
command_parser.add_argument(
    "--image", "-i",
    default="test_image.png"
)
command_parser.add_argument(
    "--capture-window", "-w",
    help="Capture Screen Recording. Input: top,left,width,height"
)
command_parser.add_argument(
    "--monitor", "-m",
    help="Capture Screen Recording on a specific monitor (number).",
    type=int
)
command_parser.add_argument(
    "--simulated_rotation_seconds",
    help="Simulate a rotation that takes x seconds.",
    type=int, default=0
)
command_parser.add_argument(
    "--frames_per_second",
    type=int, default=30
)


def run_main(args):
    if args.capture_window or args.monitor is not None:
        capturer = mss()

        monitor_index = args.monitor or 0
        monitor = capturer.monitors[monitor_index]

        if args.capture_window:
            left, top, width, height = list(map(int, args.capture_window.split(",")))
            monitor = {
                'top': monitor["top"] + top,
                'left': monitor["left"] + left,
                'width': width,
                'height': height,
                "mon": monitor_index
            }

        def capture_image():
            image = capturer.grab(monitor)
            return Image.frombytes('RGB', image.size, bytes(image.rgb))

        image_provider = capture_image
    else:
        image = Image.open(args.image).convert("RGB")
        image_provider = lambda: image

    run(
        ip=args.ip,
        endpoint=args.endpoint,
        image_provider=image_provider,
        simulated_rotation_seconds=args.simulated_rotation_seconds,
        frames_per_second=args.frames_per_second
    )


if __name__ == "__main__":
    run_main(command_parser.parse_args())

