import math
from typing import Iterable, TypeVar, List, Callable
from itertools import zip_longest

from PIL import Image

from dataclasses import dataclass, field
from threading import Thread, Lock, Condition, Timer

# Declare type variables
T = TypeVar('T')
U = TypeVar('U')
V = TypeVar('V')


def grouper(n, iterable, fillvalue=None):
    "grouper(3, 'ABCDEFG', 'x') --> ABC DEF Gxx"
    args = [iter(iterable)] * n
    return zip_longest(fillvalue=fillvalue, *args)


def flatten(iterables: Iterable[Iterable[T]]) -> List[T]:
    return [y for x in iterables for y in x]


def flatmap(fun: Callable[[U], Iterable[T]], iterables: Iterable[U]) -> List[T]:
    return flatten(map(fun, iterables))


def lerp(a, b, coord):
    if isinstance(a, tuple):
        return tuple([lerp(c, d, coord) for c, d in zip(a, b)])
    ratio = coord - math.floor(coord)
    return int(round(a * (1.0 - ratio) + b * ratio))


def bilinear(im: Image, x, y):
    x1, y1 = int(math.floor(x)), int(math.floor(y))
    x2, y2 = int(math.ceil(x1)), int(math.ceil(y))
    left = lerp(im.getpixel((x1, y1)), im.getpixel((x1, y2)), y)
    right = lerp(im.getpixel((x2, y1)), im.getpixel((x2, y2)), y)
    return lerp(left, right, x)


class RepeatTimer(Timer):
    def run(self):
        while not self.finished.wait(self.interval):
            self.function(*self.args, **self.kwargs)


@dataclass
class BufferedResource:
    max_buffer_size: int
    buffer: List = field(default_factory=list)

    condition = Condition()

    def push(self, resource):
        with self.condition:
            while len(self.buffer) >= self.max_buffer_size:
                self.condition.wait()

            self.buffer.append(resource)
            self.condition.notify()

    def pop(self):
        with self.condition:
            while len(self.buffer) == 0:
                self.condition.wait()

            resource = self.buffer.pop(0)
            self.condition.notify()

        return resource
