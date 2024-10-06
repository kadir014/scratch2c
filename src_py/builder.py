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
INCLUDE_PATH = BASE_PATH / "include"
EXTERNAL_PATH = BASE_PATH / "external"


srcs = [BASE_PATH / "generated.c"]

for root, _, files in os.walk(SRC_PATH):
    for file in files:
        if file.endswith(".c"):
            srcs.append(os.path.join(root, file))

for root, _, files in os.walk(EXTERNAL_PATH / "src"):
    for file in files:
        if file.endswith(".c"):
            srcs.append(os.path.join(root, file))


def build(args: Namespace) -> None:
    info("Compiling")

    if os.path.exists(BUILD_PATH):
        shutil.rmtree(BUILD_PATH)

    os.mkdir(BUILD_PATH)
    os.chdir(BUILD_PATH)

    compiler = "gcc"
    options = "-std=c11 -O1 -g3 -Wall -Wextra -march=native"
    binary = "project.exe"

    libs = [
        EXTERNAL_PATH / "lib" / "SDL2",
        EXTERNAL_PATH / "lib" / "SDL2_ttf",
        EXTERNAL_PATH / "lib" / "SDL2_image"
    ]

    links = "-lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lopengl32"

    includes = [
        INCLUDE_PATH,
        EXTERNAL_PATH / "include"
    ]

    compile_cmd = f"{compiler} {options} -o {binary} {' '.join(str(i) for i in srcs)} -I{' -I'.join(str(i) for i in includes)} -L{' -L'.join(str(i) for i in libs)} {links}"
    if args.verbose: print(f"\n{compile_cmd}\n")

    start = perf_counter()
    if not args.verbose:
        result = subprocess.run(compile_cmd, shell=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    else:
        result = subprocess.run(compile_cmd, shell=True)
    end = perf_counter()

    if result.returncode == 0:
        if args.verbose: print()
        done(f"Compilation finished in {FG.yellow}{round(end - start, 2)}s{RESET}")

        if os.path.exists(binary):
            shutil.copyfile(EXTERNAL_PATH / "bin" / "SDL2" / "SDL2.dll", BUILD_PATH / "SDL2.dll")
            shutil.copyfile(EXTERNAL_PATH / "bin" / "SDL2_ttf" / "SDL2_ttf.dll", BUILD_PATH / "SDL2_ttf.dll")
            shutil.copyfile(EXTERNAL_PATH / "bin" / "SDL2_image" / "SDL2_image.dll", BUILD_PATH / "SDL2_image.dll")

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
            result = subprocess.run(binary)
            if result.returncode == 0:
                done(f"Project binary exited with code {result.returncode}")
            else:
                fail(f"Project binary exited with code {result.returncode}")
    
    else:
        if args.verbose: print()
        fail(f"Compilation failed with code {FG.yellow}{result.returncode}{RESET}")