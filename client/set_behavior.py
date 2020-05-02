import argparse

import requests

command_parser = argparse.ArgumentParser()

command_parser.add_argument(
    "ip", default="192.168.1.4",
)

command_parser.add_argument(
    "id", help="The behavior ID to set."
)


def run_main(args):
    ip = args.ip
    id = args.id
    requests.post(f"http://{ip}/behavior/set?id={id}")


if __name__ == "__main__":
    run_main(command_parser.parse_args())
