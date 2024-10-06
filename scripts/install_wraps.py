import subprocess
import os


if not os.path.exists("subprojects"):
    os.mkdir("subprojects")


wraps = [
    "sdl2",
    "sdl2_image",
    "sdl2_ttf",
    "freetype2"
]

for wrap in wraps:
    print(f"Installing wrap: {wrap}")
    subprocess.run(f"meson wrap install {wrap}", shell=True)