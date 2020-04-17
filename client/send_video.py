import random

from PIL import Image
from mss import mss

import argparse

import fan_interface
from util import BufferedResource, RepeatTimer


def get_white_noise_image(width, height):
    pil_map = Image.new("RGB", (width, height), 255)
    random_grid = map(lambda x: (
        int(random.random() * 256),
        int(random.random() * 256),
        int(random.random() * 256)
    ), [0] * width * height)
    pil_map.putdata(random_grid)
    return pil_map


command_parser = argparse.ArgumentParser()

command_parser.add_argument(
    "ip", default="192.168.4.1",
)
command_parser.add_argument(
    "--endpoint", "-e",
    default="concentric"
)
command_parser.add_argument(
    "--image", "-i",
    default="img/test.png"
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
        else:
            # Capture square window since we have a square view
            width, height = monitor['width'], monitor['height']
            size = min(width, height)
            monitor = {
                'top': monitor['top'] + (height - size) // 2,
                'left': monitor['left'] + (width - size) // 2,
                'width': size,
                'height': size,
                "mon": monitor_index
            }

            raw_image_resource = BufferedResource(max_buffer_size=2)
            raw_capture_thread = RepeatTimer(
                interval=0,
                function=lambda: raw_image_resource.push(capturer.grab(monitor))
            )
            raw_capture_thread.start()

        def capture_image():
            image = raw_image_resource.pop()
            return Image.frombytes('RGB', image.size, bytes(image.rgb))

        image_provider = capture_image
    else:
        image = Image.open(args.image).convert("RGB")
        image_provider = lambda: image

    resource = BufferedResource(max_buffer_size=2)
    capture_thread = RepeatTimer(
        interval=0,
        function=lambda: resource.push(image_provider())
    )
    capture_thread.start()

    fan_interface.run(
        ip=args.ip,
        endpoint=args.endpoint,
        image_provider=resource.pop,
        frames_per_second=args.frames_per_second
    )


if __name__ == "__main__":
    run_main(command_parser.parse_args())
