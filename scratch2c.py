"""

    This file is a part of the Scratch to C
    project and distributed under the MIT license.

    Copyright © Kadir Aksoy
    https://github.com/kadir014/scratch2c

"""

import argparse

from src_py.transpiler import transpile
from src_py.builder import build


if __name__ == "__main__":
    cli = argparse.ArgumentParser(
        prog="scratch2c",
        description="Scratch3 to C transpiler",
        epilog="For support & issues: https://github.com/kadir014/scratch2c",
        add_help=False
    )

    cli.add_argument("-h", "--help", action="help", default=argparse.SUPPRESS, help="Show help message and exit")
    cli.add_argument("filepath", help="Path to .sb3 file")
    cli.add_argument("-v", "--verbose", action="store_true", help="Verbose compilation logs")
    cli.add_argument("-c", action="store_true", help="Build without transpilation, mainly for debugging")

    args = cli.parse_args()

    transpile(args)
    build(args)