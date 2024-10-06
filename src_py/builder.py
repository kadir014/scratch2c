"""

    This file is a part of the Scratch to C
    project and distributed under the MIT license.

    Copyright © Kadir Aksoy
    https://github.com/kadir014/scratch2c

"""

import os
import subprocess
import shutil
from pathlib import Path
from time import perf_counter
from argparse import Namespace

from .terminal import info, done, fail, FG, RESET


BASE_PATH = Path(os.getcwd())
BUILD_PATH = BASE_PATH / "build"
SRC_PATH = BASE_PATH / "src_c"


def build(args: Namespace) -> None:
    info("Building with Meson")

    start = perf_counter()
    subprocess.run("meson setup build --buildtype=debug", shell=True)
    result = subprocess.run("meson compile -C build")
    end = perf_counter()

    if result.returncode == 0:
        if args.verbose: print()
        done(f"Compilation finished in {FG.yellow}{round(end - start, 2)}s{RESET}")

        if os.path.exists(BUILD_PATH):
            os.mkdir(BUILD_PATH / "assets")
            for *_, files in os.walk(BASE_PATH / "assets"):
                for file in files:
                    shutil.copyfile(
                        BASE_PATH / "assets" / file,
                        BUILD_PATH / "assets" / file
                    )

            os.replace(BASE_PATH / "project_data", BUILD_PATH / "project_data")

            os.mkdir(BUILD_PATH / "shaders")
            for *_, files in os.walk(SRC_PATH / "shaders"):
                for file in files:
                    shutil.copyfile(
                        SRC_PATH / "shaders" / file,
                        BUILD_PATH / "shaders" / file
                    )

            info("Running the project binary")
            result = subprocess.run(BUILD_PATH / "project.exe")
            if result.returncode == 0:
                done(f"Project binary exited with code {result.returncode}")
            else:
                fail(f"Project binary exited with code {result.returncode}")
    
    else:
        if args.verbose: print()
        fail(f"Compilation failed with code {FG.yellow}{result.returncode}{RESET}")