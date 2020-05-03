#!/usr/bin/env python3
from argparse import ArgumentParser
import os
import sys

import observe_log
import send_video
import commands as subcommands
import simulate_rotation

assert (3, 0) <= sys.version_info

path = os.path.dirname(os.path.realpath(__file__))
# Move to the directory that this file is located in,
# for determinable scripts
os.chdir(path)

command_parser = ArgumentParser()

command_parser.add_argument(
    "--ip", default="192.168.4.1",
)

commands = command_parser.add_subparsers(dest='subcommand')

send_video.setup(commands.add_parser(
    "send-video", help="Send a streaming video."
))

observe_log.setup(commands.add_parser(
    "observe-log", help="Actively observe the log."
))

subcommands.setup_set_behavior(commands.add_parser(
    "behavior", help="Set the current behavior."
))
simulate_rotation.setup(commands.add_parser(
    "simulate-rotation", help="Simulate a rotation at no movement."
))


def run_main(args):
    try:
        fun = args.func
    except AttributeError:
        command_parser.print_help()
        exit(1)

    fun(args)


if __name__ == "__main__":
    run_main(command_parser.parse_args())
