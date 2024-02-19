"""

    This file is a part of the Scratch to C
    project and distributed under the MIT license.

    Copyright © Kadir Aksoy
    https://github.com/kadir014/scratch2c

"""

from __future__ import annotations

from dataclasses import dataclass
from enum import Enum


class PrimitiveType(Enum):
    NUMBER = 4
    POSITIVE_NUMBER = 5
    POSITIVE_INTEGER = 6
    INTEGER = 7
    ANGLE = 8
    COLOR = 9
    STRING = 10
    BROADCAST = 11
    VARIABLE = 12
    LIST = 13

@dataclass
class Primitive:
    type: PrimitiveType
    value: str
    id: str = ""


@dataclass
class Block:
    opcode: str
    root: bool = False
    parent: Block | None = None
    next: Block | None = None
    substack: Block | None = None
    procedure: str = ""
    arguments: list[Block | Primitive] | None = None
    inputs: dict | None = None
    fields: dict | None = None


@dataclass
class Variable:
    id: str
    name: str
    value: str


@dataclass
class Costume:
    name: str
    file: str


@dataclass
class Target:
    is_stage: bool
    costumes: list[Costume]
    variables: list[Variable]
    scripts: list[Block] | None = None
    procedures: list[Block] | None = None