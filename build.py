"""

    This file is a part of the Scratch to C
    project and distributed under the MIT license.

    Copyright © Kadir Aksoy
    https://github.com/kadir014/scratch2c

"""

from src_py.transpiler import transpile
from src_py.builder import build


if __name__ == "__main__":
    transpile("test.sb3")
    #transpile("SCRATCHVMTEST002.sb3")
    build()