import argparse

command_parser = argparse.ArgumentParser()

command_parser.add_argument(
    "--blades",
    required=True
)
command_parser.add_argument(
    "--leds-per-m",
    required=True
)
command_parser.add_argument(
    "--rpm",
    required=True
)
command_parser.add_argument(
    "--radius", help="in cm",
    required=True
)
command_parser.add_argument(
    "--mirror", action="store_true"
)

command_parser.add_argument("--led-ampere", default=0.06)
command_parser.add_argument("--led-voltage", default=5)
command_parser.add_argument("--led-clock", default=2000)

if __name__ == "__main__":
    args = command_parser.parse_args()

    blades = int(args.blades)
    leds_per_m = int(args.leds_per_m)
    mirror = bool(args.mirror)
    radius = int(args.radius)
    rpm = float(args.rpm)

    led_ampere = float(args.led_ampere)
    led_voltage = float(args.led_voltage)
    led_clock = float(args.led_clock)

    total_leds = int(blades * radius * leds_per_m / 100)
    total_ampere = led_ampere * total_leds
    pixel_density = 2 if mirror else 1

    print(f"LED Specs: {led_ampere}A, {led_voltage}v, {led_clock}Hz")

    print()

    print(f"Total LEDs: {total_leds}")
    print(f"Total Power Needed: {total_ampere * led_voltage : .5}W = {total_ampere : .5}A x {led_voltage}v")
    print(f"Pixel Lightness: {pixel_density}")

    print()

    rings = total_leds * (0.5 if mirror else 1)
    ring_distance = radius / rings
    print(f"Rings: {rings}")
    print(f"Ring Distance: {ring_distance : .5}cm")

    print(f"Pixel Passes: {rpm / 60.0 * (pixel_density) : .5} / s")
    print(f"Any Pixel Passes: {rpm / 60.0 * blades : .5} / s")

    print()

    circumference = 2 * 3.141 * radius
    outside_speed = circumference / 100 * (rpm / 60)
    print(f"Outside Speed: {outside_speed : .5} m/s")
    print(f"Outside Motion: {outside_speed / led_clock * 100 : .5} cm/tick")
    print(f"Outside Pixel Density: {(circumference * ring_distance) / pixel_density : .5} cm^2/l")
