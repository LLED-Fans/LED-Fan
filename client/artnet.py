from dataclasses import dataclass
from typing import List


def split_bytes(number: int, count: int) -> List[int]:
    return list(map(
        lambda i: ((number >> (8 * i)) & 0xFF),
        range(count)
    ))


@dataclass
class ArtnetProvider:
    universe: int
    subnet: int
    net: int

    sequence: int = 0

    def generate(self, payload: bytearray) -> bytearray:
        packet = bytearray()
        packet.extend(bytearray('Art-Net', 'utf8'))
        packet.append(0x0)
        # 8 - opcode (2 x 8 low byte first)
        packet.append(0x00)
        packet.append(0x50)  # ArtDmx data packet
        # 10 - prototocol version (2 x 8 high byte first)
        packet.append(0x0)
        packet.append(14)
        # 12 - sequence (int 8), NULL for not implemented
        packet.append(self.sequence)
        # 13 - physical port (int 8)
        packet.append(0x00)
        # 14 - universe, (2 x 8 low byte first)

        # as specified in Artnet 4 (remember to set the value manually after):
        # Bit 3  - 0 = Universe (1-16)
        # Bit 7  - 4 = Subnet (1-16)
        # Bit 14 - 8 = Net (1-128)
        # Bit 15     = 0
        # this means 16 * 16 * 128 = 32768 universes per port
        # a subnet is a group of 16 Universes
        # 16 subnets will make a net, there are 128 of them
        packet.append(self.subnet << 4 | self.universe)
        packet.append(self.net & 0xFF)

        # 16 - packet size (2 x 8 high byte first)
        packet.extend(reversed(split_bytes(len(payload), count=2)))

        packet.extend(payload)

        return packet

    def __call__(self, *args, **kwargs):
        packet = self.generate(args[0])
        self.sequence = (self.sequence + 1) % 256
        return packet
