import argparse

AMPERE_PER_LED = 0.06
LED_VOLTAGE = 5
CLOCK_HZ = 2000

command_parser = argparse.ArgumentParser()

command_parser.add_argument(
    "--blades",
    required=True
)
command_parser.add_argument(
    "--leds-per-blade",
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

if __name__ == "__main__":
    args = command_parser.parse_args()

    blades = int(args.blades)
    leds_per_blade = int(args.leds_per_blade)
    mirror = bool(args.mirror)
    radius = int(args.radius)
    rpm = float(args.rpm)

    total_leds = blades * leds_per_blade
    total_ampere = AMPERE_PER_LED * total_leds

    print(f"Total LEDs: {total_leds}")
    print(f"Total Power Needed: {total_ampere * LED_VOLTAGE}W = {total_ampere}A x {LED_VOLTAGE}v")
    print(f"Pixel Lightness: {2 if mirror else 1}")

    print()

    rings = total_leds * (0.5 if mirror else 1)
    print(f"Rings: {rings}")
    print(f"Ring Distance: {radius / rings : .5}cm")

    print(f"Pixel Passes: {rpm / 60.0 * (2 if mirror else 1) : .5} / s")
    print(f"Any Pixel Passes: {rpm / 60.0 * blades : .5} / s")

    print()

    outside_speed = 2 * 3.141 * radius / 100 * (rpm / 60)
    print(f"Outside Speed: {outside_speed : .5} m/s")
    print(f"Outside Motion: {outside_speed / CLOCK_HZ * 100 : .5} cm/tick")
