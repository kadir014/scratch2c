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


BASE_PATH = Path(os.getcwd())
BUILD_PATH = BASE_PATH / "build"
SRC_PATH = BASE_PATH / "src_c"
INCLUDE_PATH = BASE_PATH / "include"


srcs = [BASE_PATH / "example.c"]

for root, _, files in os.walk(SRC_PATH):
    for file in files:
        if file.endswith(".c"):
            srcs.append(os.path.join(root, file))


if os.path.exists(BUILD_PATH):
    shutil.rmtree(BUILD_PATH)

os.mkdir(BUILD_PATH)
os.chdir(BUILD_PATH)

compiler = "gcc"
options = "-std=c11 -O3 -Wall -march=native"
binary = "project.exe"

libs = [
    BASE_PATH / "deps" / "lib" / "SDL2",
    BASE_PATH / "deps" / "lib" / "SDL2_ttf",
    BASE_PATH / "deps" / "lib" / "SDL2_image"
]

links = "-lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image"

includes = [
    INCLUDE_PATH,
    BASE_PATH / "deps" / "include"
]

compile_cmd = f"{compiler} {options} -o {binary} {' '.join(str(i) for i in srcs)} -I{' -I'.join(str(i) for i in includes)} -L{' -L'.join(str(i) for i in libs)} {links}"
print(compile_cmd)

start = perf_counter()
result = subprocess.run(compile_cmd, shell=True)
end = perf_counter()

print(f"\nCompilation finished in {round(end - start, 2)}s.")

if os.path.exists(binary):
    shutil.copyfile(BASE_PATH / "deps" / "bin" / "SDL2" / "SDL2.dll", BUILD_PATH / "SDL2.dll")
    shutil.copyfile(BASE_PATH / "deps" / "bin" / "SDL2_ttf" / "SDL2_ttf.dll", BUILD_PATH / "SDL2_ttf.dll")
    shutil.copyfile(BASE_PATH / "deps" / "bin" / "SDL2_image" / "SDL2_image.dll", BUILD_PATH / "SDL2_image.dll")

    os.mkdir(BUILD_PATH / "assets")
    for *_, files in os.walk(BASE_PATH / "assets"):
        for file in files:
            shutil.copyfile(
                BASE_PATH / "assets" / file,
                BUILD_PATH / "assets" / file
            )

    result = subprocess.run(binary)
    print(f"{binary} exited with code {result.returncode}")