import io
import random
import time

import requests
import socket
from PIL import Image

from artnet import ArtnetProvider
from util import grouper, flatmap, bilinear
from datetime import datetime


def get_white_noise_image(width, height):
    pil_map = Image.new("RGB", (width, height), 255)
    random_grid = map(lambda x: (
        int(random.random() * 256),
        int(random.random() * 256),
        int(random.random() * 256)
    ), [0] * width * height)
    pil_map.putdata(random_grid)
    return pil_map


def get_image_file(path) -> Image:
    return Image.open(path)


def pixel_at(image: Image, x: float, y: float):
    r = bilinear(image, x * (image.width - 1), y * (image.height - 1))
    return r

print("Getting Server Info")

ip = "192.168.2.126"
endpoint = "concentric"

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

seconds_per_frame = 1.0 / 30.0
simulated_rotation_seconds = 3

print(f"Sending Art-Net Data to: {ip}:{port}!")
sequence_start = datetime.now()

simulated_rotation = 0

try:
    while True:
        frame_start = datetime.now()

        #img = get_white_noise_image(64, 64)
        img = get_image_file("two_color_square.png")

        if endpoint == "concentric":
            data = bytes(flatmap(
                lambda t: pixel_at(img, *t),
                pixels
            ))
        else:
            img = img.resize((64, 64))
            data = img.tobytes("raw")

        # data = io.BytesIO()
        # img.save(data, format='JPEG', progression=False)
        # print(data.tell())
        # data.seek(0, 0)
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

        # r = requests.post(
        #     f"http://{url}/i/cc/rgb",
        #     data=data
        # )
        # print(r)

        time_this_frame = datetime.now() - frame_start
        if time_this_frame.total_seconds() < seconds_per_frame:
            time.sleep(seconds_per_frame - time_this_frame.total_seconds())
finally:
    if simulated_rotation_seconds > 0:
        requests.post(f"http://{ip}/rotation/set", data={"rotation": "-1"})
