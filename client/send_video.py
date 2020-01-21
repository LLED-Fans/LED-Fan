import io
import random
import time

import requests
from PIL import Image


def get_white_noise_image(width, height):
    pil_map = Image.new("RGB", (width, height), 255)
    random_grid = map(lambda x: (
        int(random.random() * 256),
        int(random.random() * 256),
        int(random.random() * 256)
    ), [0] * width * height)
    pil_map.putdata(random_grid)
    return pil_map


while True:
    img = get_white_noise_image(64, 64)

    data = img.tobytes("raw")

    # data = io.BytesIO()
    # img.save(data, format='JPEG', progression=False)
    # print(data.tell())
    # data.seek(0, 0)

    r = requests.post(
        "http://192.168.2.126/img/rgb",
        data=data
    )
    print(r)

    time.sleep(0.1)
