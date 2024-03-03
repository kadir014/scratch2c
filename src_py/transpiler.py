"""

    This file is a part of the Scratch to C
    project and distributed under the MIT license.

    Copyright © Kadir Aksoy
    https://github.com/kadir014/scratch2c

"""

import zipfile
import json
import re

from common import Block, PrimitiveType, Primitive, TargetVariable, TargetList, Costume, Target
from codegen import generate_code
from visualize import plot_ast
from syntaxtree import generate_ast


def get_next(sprite: dict, block_data: str | dict | list) -> Block | Primitive | None:
    if isinstance(block_data, list):
        if block_data[0] < 11 and block_data[1].isdigit():
            block_data[0] = 4

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
            block = Block(next_block_p, next_block["opcode"], next=get_next(sprite, next_block), root=next_block["topLevel"])
            block.inputs = {}
            block.fields = {}

            if next_block["opcode"] in ("procedures_call", "procedures_prototype"):
                block.procedure = next_block["mutation"]["proccode"]
                block.arguments = []
                argumentids = next_block["mutation"]["argumentids"][1:-1]
                args = re.findall("\"([^\"]*)\"", argumentids)
                for arg in args:
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
        variables = [TargetVariable(key, target_d["variables"][key][0], target_d["variables"][key][1]) for key in target_d["variables"]]
        lists = [TargetList(key, target_d["lists"][key][0], target_d["lists"][key][1]) for key in target_d["lists"]]
        target = Target(target_d["isStage"], costumes, variables)
        target.scripts = []
        target.procedures = []

        if not target.is_stage:
            target.x = target_d["x"]
            target.y = target_d["y"]
            target.size = target_d["size"]
            target.direction = target_d["direction"]
            target.draggable = target_d["draggable"]

        green_flags_ids = []
        procedures_ids = []

        for block in target_d["blocks"]:
            if target_d["blocks"][block]["topLevel"]:
                if target_d["blocks"][block]["opcode"] == "event_whenflagclicked":
                    green_flags_ids.append(block)
                
                #elif target_d["blocks"][block]["opcode"] == "procedures_definition":
                #    procedures_ids.append(block)

        for block_id in green_flags_ids:
            target.scripts.append(get_next(target_d, block_id))

        for proc_id in procedures_ids:
            target.procedures.append(get_next(target_d, proc_id))

        targets.append(target)
        
    #ast = generate_ast(targets[1].scripts[0].next)
    #plot_ast(ast)

    with open("generated.c", "w") as file:
        file.write(generate_code(targets))