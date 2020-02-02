import io
import random
import time

import requests
from PIL import Image

from util import grouper, flatmap, bilinear


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

server_info = requests.get("http://192.168.2.126/i/cc").json()
pixels = list(grouper(2, server_info["pixels"]))
print(server_info)
print(f"Remote Pixels ({len(pixels)}): {pixels}")


while True:
    #img = get_white_noise_image(64, 64)
    img = get_image_file("two_color_square.png")
    img = img.resize((64, 64))

    # data = img.tobytes("raw")
    data = io.BytesIO(
        bytes(flatmap(
            lambda t: pixel_at(img, *t),
            pixels
        ))
    )

    # data = io.BytesIO()
    # img.save(data, format='JPEG', progression=False)
    # print(data.tell())
    # data.seek(0, 0)

    r = requests.post(
        "http://192.168.2.126/i/cc/rgb",
        data=data
    )
    print(r)

    time.sleep(0.1)
