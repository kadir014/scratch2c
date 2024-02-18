"""

    This file is a part of the Scratch to C
    project and distributed under the MIT license.

    Copyright © Kadir Aksoy
    https://github.com/kadir014/scratch2c

"""

from __future__ import annotations

import zipfile
import json
from dataclasses import dataclass


@dataclass
class Block:
    opcode: str
    parent: Block | None = None
    next: Block | None = None


PRETTY_PRINT_MAP = {
    "event_whenflagclicked": "When Green Flag Clicked"
}

def pretty_print(block: Block, indent: int = 0):
    if block.next is None:
        print(f"{' ' * indent}{block.opcode}")
        print(f"{' ' * indent}next -> None")

    else:
        print(f"{' ' * indent}{block.opcode}")
        pretty_print(block.next)
        #pretty_print(block.next, indent+1)


def get_next(sprite: dict, block_data: dict) -> Block | None:
    next_block_p = block_data["next"]

    if next_block_p is None:
        return None
    
    else:
        next_block = sprite["blocks"][block_data["next"]]
        return Block(next_block["opcode"], next=get_next(sprite, next_block))


if __name__ == "__main__":
    with zipfile.ZipFile("../test.sb3") as sb3zip:
        project = json.loads(sb3zip.read("project.json"))

    sprite = project["targets"][1]
    
    root_block = None
    for block in sprite["blocks"]:
        if sprite["blocks"][block]["topLevel"]:
            root_block = sprite["blocks"][block]
            break

    if root_block is None: raise Exception("No root block.")

    root = Block(root_block["opcode"], parent=None, next=get_next(sprite, root_block))

    pretty_print(root)