from argparse import ArgumentParser
from typing import Callable, Any

import requests


def post(command: ArgumentParser, name: str, help: str, uri: Callable[[Any], str]):
    command.add_argument(
        name, help=help
    )

    def run(args):
        res = requests.post(f"http://{args.ip}/{uri(args)}")
        if res.status_code != 200:
            print(res)
            exit(res.status_code)

        exit(0)

    command.set_defaults(func=run)


def setup_set_behavior(command: ArgumentParser):
    post(
        command, "id", "The behavior ID",
        lambda args: f"behavior/set?id={args.id}"
    )
