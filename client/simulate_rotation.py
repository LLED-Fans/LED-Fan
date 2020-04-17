import argparse
from datetime import datetime, timedelta

import requests

from util import RegularClock

command_parser = argparse.ArgumentParser()

command_parser.add_argument(
    "ip", default="192.168.4.1",
)
command_parser.add_argument(
    "--seconds_per_rotation",
    help="Seconds per rotation",
    type=int, default=2, required=False
)
command_parser.add_argument(
    "--frames_per_second",
    help="FPS for rotation sending",
    type=float, default=30, required=False
)


def run_main(args):
    seconds_per_rotation = args.seconds_per_rotation
    ip = args.ip
    time_per_frame = timedelta(seconds=1.0 / args.frames_per_second)

    clock = RegularClock(None)
    start = clock.mark()

    print(f"Simulating Rotation: {seconds_per_rotation} seconds per rotation, ({time_per_frame.total_seconds()} FPS)")

    try:
        while True:
            uptime = datetime.now() - start
            simulated_rotation = (uptime.total_seconds() / seconds_per_rotation) % 1.0
            requests.post(f"http://{ip}/rotation/set", data={"rotation": simulated_rotation})

            clock.elapse(time_per_frame)
    finally:
        if seconds_per_rotation > 0:
            requests.post(f"http://{ip}/rotation/set", data={"rotation": "-1"})


if __name__ == "__main__":
    run_main(command_parser.parse_args())
