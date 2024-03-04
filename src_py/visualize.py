"""

    This file is a part of the Scratch to C
    project and distributed under the MIT license.

    Copyright © Kadir Aksoy
    https://github.com/kadir014/scratch2c

"""

import pydot
import PIL.Image

from .common import Block, Primitive, PrimitiveType
from .syntaxtree import *


PNG_OUT = "out.png"


def build_block_graph(graph: pydot.Graph, block: Block) -> None:
    graph.add_node(pydot.Node(block.id, label=f"{block.id} - {block.opcode}"))

    # Statement sequence
    if block.next is not None:
        graph.add_edge(pydot.Edge(block.id, block.next.id, dir="forward"))
        build_block_graph(graph, block.next)

    for i, input_key in enumerate(block.inputs.keys()):
        input_val = block.inputs[input_key]
        if block.opcode == "procedures_call": input_key = f"Arg#{i}"

        if isinstance(input_val, Block):
            graph.add_edge(pydot.Edge(block.id, input_val.id, labelfloat=True, label=input_key, fontsize=10.0, style="dashed"))
            build_block_graph(graph, input_val)

        elif isinstance(input_val, Primitive):
            if input_val.type == PrimitiveType.VARIABLE:
                graph.add_node(pydot.Node(id(input_val), label=f"Variable<{input_val.value}>"))
            elif input_val.type == PrimitiveType.STRING:
                graph.add_node(pydot.Node(id(input_val), label=f"String<{input_val.value}>"))
            else:
                graph.add_node(pydot.Node(id(input_val), label=f"Number<{input_val.value}>"))

            graph.add_edge(pydot.Edge(block.id, id(input_val), labelfloat=True, label=input_key, fontsize=10.0, style="dashed"))

def plot_script(script: Block) -> None:
    graph = pydot.Dot("tree_graph", graph_type="graph", bgcolor="white", rankdir="LR")

    build_block_graph(graph, script)

    graph.write_png(PNG_OUT)
    PIL.Image.open(PNG_OUT).show()


def build_graph(graph: pydot.Graph, node: Node) -> int:
    node_id = id(node)
    graph.add_node(pydot.Node(node_id, label=str(node)))

    if isinstance(node, Assignment):
        inner_id = build_graph(graph, node.expression)
        graph.add_edge(pydot.Edge(node_id, inner_id, labelfloat=True, label="expression", fontsize=9.0, color="gray"))

        inner_id = build_graph(graph, node.variable)
        graph.add_edge(pydot.Edge(node_id, inner_id, labelfloat=True, label="variable", fontsize=9.0, color="gray"))

    elif isinstance(node, Repeat):
        inner_id = build_graph(graph, node.times)
        graph.add_edge(pydot.Edge(node_id, inner_id, labelfloat=True, label="times", fontsize=9.0, color="gray"))

        inner_id = build_stack(graph, node.body)
        graph.add_edge(pydot.Edge(node_id, inner_id, labelfloat=True, label="body", fontsize=9.0, color="gray"))

    elif isinstance(node, If):
        inner_id = build_graph(graph, node.condition)
        graph.add_edge(pydot.Edge(node_id, inner_id, labelfloat=True, label="condition", fontsize=9.0, color="gray"))

        inner_id = build_stack(graph, node.body)
        graph.add_edge(pydot.Edge(node_id, inner_id, labelfloat=True, label="body", fontsize=9.0, color="gray"))

    elif isinstance(node, BinOp):
        inner_id = build_graph(graph, node.left)
        graph.add_edge(pydot.Edge(node_id, inner_id, labelfloat=True, label="left", fontsize=9.0, color="gray"))

        inner_id = build_graph(graph, node.right)
        graph.add_edge(pydot.Edge(node_id, inner_id, labelfloat=True, label="right", fontsize=9.0, color="gray"))
    
    elif isinstance(node, ListAction):
        if node.param1 is not None:
            inner_id0 = build_graph(graph, node.param1)
            graph.add_edge(pydot.Edge(node_id, inner_id0, labelfloat=True, label="param1", fontsize=9.0, color="gray"))

        if node.param2 is not None:
            inner_id1 = build_graph(graph, node.param2)
            graph.add_edge(pydot.Edge(node_id, inner_id1, labelfloat=True, label="param2", fontsize=9.0, color="gray"))

    elif isinstance(node, FunctionCall):
        for i, arg in enumerate(node.arguments):
            inner_id = build_graph(graph, arg)
            graph.add_edge(pydot.Edge(node_id, inner_id, labelfloat=True, label=f"arg#{i}", fontsize=9.0, color="gray"))

    elif isinstance(node, PenColorAssignment):
        inner_id = build_graph(graph, node.color)
        graph.add_edge(pydot.Edge(node_id, inner_id, labelfloat=True, label="color", fontsize=9.0, color="gray"))

    return node_id

def build_stack(graph: pydot.Graph, body: list[Node]) -> int:
    first_node = body[0]
    first_node_id = build_graph(graph, first_node)
    really_first_node_id = first_node_id

    for node in body[1:]:
        node_id = build_graph(graph, node)
        graph.add_edge(pydot.Edge(first_node_id, node_id, style="dashed", dir="forward", color="navy"))
        first_node_id = node_id

    return really_first_node_id

def plot_ast(body: list[Node]) -> None:
    graph = pydot.Dot("tree_graph", graph_type="graph", bgcolor="white", rankdir="LR")

    graph.add_node(pydot.Node("root"))

    node_id = build_stack(graph, body)
    graph.add_edge(pydot.Edge("root", node_id, style="dashed", dir="forward", color="navy"))

    graph.write_png(PNG_OUT)
    PIL.Image.open(PNG_OUT).show()