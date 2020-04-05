from dataclasses import dataclass
from typing import List


def split_bytes(number: int, count: int) -> List[int]:
    return list(map(
        lambda i: ((number >> (8 * i)) & 0xFF),
        range(count)
    ))

def chunks(lst, n):
    """Yield successive n-sized chunks from lst."""
    for i in range(0, len(lst), n):
        yield lst[i:i + n]

@dataclass
class ArtnetProvider:
    universe: int
    subnet: int
    net: int

    sequence: int = 0

    def generate(self, payload: bytearray, universe_offset: int = 0) -> bytearray:
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

        # as specified in Artnet 4
        # Bit 3  - 0 = Universe (1-16)
        # Bit 7  - 4 = Subnet (1-16)
        # Bit 14 - 8 = Net (1-128)
        # Bit 15     = 0
        # this means 16 * 16 * 128 = 32768 universes per port
        # a subnet is a group of 16 Universes
        # 16 subnets will make a net, there are 128 of them
        total_universe = self.net << 8 | self.subnet << 4 | self.universe
        total_universe += universe_offset
        packet.extend(split_bytes(total_universe, count=2))

        # 16 - packet size (2 x 8 high byte first)
        packet.extend(reversed(split_bytes(len(payload), count=2)))

        packet.extend(payload)

        return packet

    def __call__(self, *args, **kwargs):
        total_bytes = args[0]
        payloads = chunks(total_bytes, 512)

        packets = list(map(lambda t: self.generate(t[1], t[0]), enumerate(payloads)))
        self.sequence = (self.sequence + 1) % 256
        return packets
