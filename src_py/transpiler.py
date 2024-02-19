"""

    This file is a part of the Scratch to C
    project and distributed under the MIT license.

    Copyright © Kadir Aksoy
    https://github.com/kadir014/scratch2c

"""

import zipfile
import json

from common import Block, PrimitiveType, Primitive, Variable, Costume, Target
from codegen import generate_code


PP_MAP = {
    "motion_glidesecstoxy": "Glide",

    "event_whenflagclicked": "When flag clicked",

    "control_repeat": "Repeat",
    "control_if": "If",

    "operator_add": "Add",
    "operator_equals": "Equals",

    "data_setvariableto": "Set variable to",

    "procedures_call": "Proc call",
    "procedures_definition": "Proc definition",
    "procedures_prototype": "Proc prototype",

    "argument_reporter_string_number": "Arg<str/num>",
    "argument_reporter_boolean": "Arg<bool>"
}

PP_SPACES = 2

def pretty_dict(d: dict, indent: int = 0) -> None:
    # https://stackoverflow.com/a/3229493

    for key, value in d.items():
        print(" " * indent + str(key))

        if isinstance(value, dict):
            pretty_dict(value, indent + PP_SPACES)

        else:
            if isinstance(value, (Block, Primitive)):
                pretty_print(value, indent + PP_SPACES)

            else:
                print(" " * (indent + PP_SPACES) + str(value))

def pretty_print(block: Block | Primitive | None, indent: int = 0) -> None:
    if isinstance(block, Primitive):
        print(f"{' ' * (indent)}Primitive<{block.type}, {block.value}, {block.id}>")
        return

    if block is None:
        #print(f"{' ' * (indent - PP_SPACES)}End")
        return

    if block.opcode in ("procedures_call", "procedures_prototype"):
        print(f"{' ' * indent}{PP_MAP[block.opcode]} {block.procedure}")
        for i, arg in enumerate(block.arguments):
            print(f"{' ' * (indent + PP_SPACES)}Arg#{i}")
            pretty_print(arg, indent=indent + PP_SPACES * 2)

    else:
        print(f"{' ' * indent}{PP_MAP[block.opcode]}")
        if block.inputs is not None:
            pretty_dict(block.inputs, indent + PP_SPACES)

    if block.root: indent += PP_SPACES
        
    pretty_print(block.next, indent)


def get_next(sprite: dict, block_data: str | dict | list) -> Block | Primitive | None:
    if isinstance(block_data, list):
        primitive = Primitive(
            PrimitiveType(block_data[0]),
            value=block_data[1]
        )

        if len(block_data) > 2:
            primitive.id = block_data[2]

        return primitive
    
    else:
        if isinstance(block_data, str):
            next_block_p = block_data
        else:
            next_block_p = block_data["next"]

        if next_block_p is None:
            return None
        
        else:
            next_block = sprite["blocks"][next_block_p]
            block = Block(next_block["opcode"], next=get_next(sprite, next_block), root=next_block["topLevel"])
            block.inputs = {}
            block.fields = {}

            if next_block["opcode"] in ("procedures_call", "procedures_prototype"):
                block.procedure = next_block["mutation"]["proccode"]
                block.arguments = []
                argumentids = next_block["mutation"]["argumentids"][1:-1].split(",")
                argumentids = [v[1:-1] for v in argumentids]
                for arg in argumentids:
                    block.arguments.append(get_next(sprite, next_block["inputs"][arg][1]))

            for key in next_block["inputs"]:
                block.inputs[key] = get_next(sprite, next_block["inputs"][key][1])

            for key in next_block["fields"]:
                block.fields[key] = next_block["fields"][key]

            return block


if __name__ == "__main__":
    with zipfile.ZipFile("../test.sb3") as sb3zip:
        project = json.loads(sb3zip.read("project.json"))

    targets = []

    for target_d in project["targets"]:
        costumes = [Costume(c["name"], c["md5ext"]) for c in target_d["costumes"]]
        variables = [Variable(key, target_d["variables"][key][0], target_d["variables"][key][1]) for key in target_d["variables"]]
        target = Target(target_d["isStage"], costumes, variables)
        target.scripts = []
        target.procedures = []

        green_flags_ids = []
        procedures_ids = []

        for block in target_d["blocks"]:
            if target_d["blocks"][block]["topLevel"]:
                if target_d["blocks"][block]["opcode"] == "event_whenflagclicked":
                    green_flags_ids.append(block)
                
                elif target_d["blocks"][block]["opcode"] == "procedures_definition":
                    procedures_ids.append(block)

        for block_id in green_flags_ids:
            target.scripts.append(get_next(target_d, block_id))

        for proc_id in procedures_ids:
            target.procedures.append(get_next(target_d, proc_id))

        targets.append(target)

    with open("generated.c", "w", encoding="utf-8") as file:
        file.write(generate_code(targets))