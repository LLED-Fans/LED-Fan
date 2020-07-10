#!/usr/local/Cellar/platformio/4.3.4/libexec/bin/python3.8

import sys
import os
from pathlib import Path
from os.path import expanduser

home = expanduser("~")

idf_path = (Path.home() / Path(".platformio/packages/framework-espidf")).absolute()
os.environ['IDF_PATH'] = str(idf_path)

sys.path.append(str(idf_path / Path("components/app_update")))
sys.path.append(str(idf_path / Path("components/partition_table")))

if "-f" not in sys.argv:
    sys.argv.insert(1, "-f")
    partition_file = Path("./partitions_8mb.csv").absolute()
    sys.argv.insert(2, str(partition_file))
    print(f"[Auto] Using Partitions File: {partition_file}")

import otatool
otatool.main()
