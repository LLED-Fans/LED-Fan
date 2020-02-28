import argparse

import time
from datetime import datetime
import requests

command_parser = argparse.ArgumentParser()

command_parser.add_argument(
    "ip", default="192.168.1.4",
)

command_parser.add_argument(
    "--frames-per-second",
    type=float, default=1
)


def run_main(args):
    ip = args.ip

    seconds_per_frame = 1.0 / args.frames_per_second

    full_log = ""

    while True:
        frame_start = datetime.now()
        log = requests.get(f"http://{ip}/log").text

        index = 0
        for i in reversed(range(1, min(len(full_log), len(log)) + 1)):
            if log.startswith(full_log[-i:]):
                index = i
                break

        log = log[index:]

        if log:
            print(log, end='')
            full_log += log

        time_this_frame = datetime.now() - frame_start
        if time_this_frame.total_seconds() < seconds_per_frame:
            time.sleep(seconds_per_frame - time_this_frame.total_seconds())


if __name__ == "__main__":
    run_main(command_parser.parse_args())

