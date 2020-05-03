import time
from argparse import ArgumentParser
from datetime import timedelta
import requests

from util import RegularClock


def run(args):
    ip = args.ip

    time_per_frame = timedelta(seconds=(1.0 / args.frames_per_second))
    clock = RegularClock()

    full_log = ""

    while True:
        clock.elapse(time_per_frame)

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


def setup(command: ArgumentParser):
    command.add_argument(
        "--frames-per-second",
        type=float, default=1
    )
    command.set_defaults(func=run)
