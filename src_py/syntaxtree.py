"""

    This file is a part of the Scratch to C
    project and distributed under the MIT license.

    Copyright © Kadir Aksoy
    https://github.com/kadir014/scratch2c


"""

from dataclasses import dataclass
from enum import Enum

from .common import Block, Primitive, PrimitiveType


@dataclass
class Node:
    ...

@dataclass
class Variable(Node):
    name: str
    id: str

    def __repr__(self) -> str:
        return f"Variable<{self.name}>"
    
@dataclass
class List(Node):
    name: str
    id: str

    def __repr__(self) -> str:
        return f"List<{self.name}>"

class LiteralType(Enum):
    STRING = 0
    NUMBER = 1
    COLOR = 2

@dataclass
class Literal(Node):
    type: LiteralType
    value: str | float

    def __repr__(self) -> str:
        return f"Literal<{self.type.name}, {self.value}>"

@dataclass
class Assignment(Node):
    variable: Variable
    expression: Node

    def __repr__(self) -> str:
        return "Assignment"

class BinOpType(Enum):
    ADD = 0
    SUB = 1
    MUL = 2
    DIV = 3
    MOD = 4
    EQ  = 5
    LT  = 6
    GT  = 7
    AND = 8
    OR  = 9

@dataclass
class BinOp(Node):
    type: BinOpType
    left: Node
    right: Node

    def __repr__(self) -> str:
        return f"BinOp<{self.type.name}>"

@dataclass
class Repeat(Node):
    times: Node
    body: list[Node]

    def __repr__(self) -> str:
        return f"Repeat"
    
@dataclass
class If(Node):
    condition: Node
    body: list[Node]

    def __repr__(self) -> str:
        return f"If"
    
class ListActionType(Enum):
    CLEAR = 0
    LENGTH = 1
    SHOW = 2
    HIDE = 3

    ADD = 4
    REMOVE = 5
    ITEM = 6
    ITEM_NO = 7
    CONTAINS = 8
    
@dataclass
class ListAction(Node):
    type: ListActionType
    list: List
    param1: Node | None = None
    param2: Node | None = None

    def __repr__(self) -> str:
        return f"ListAction<{self.type.name}, {self.list}>"

class FunctionArgumentType(Enum):
    ANY = 0
    BOOL = 1

@dataclass
class FunctionArgument(Node):
    type: FunctionArgumentType
    name: str
    id: str = ""

    def __repr__(self) -> str:
        return f"FunctionArgument<{self.type.name}, {self.name}, {self.id}>"

@dataclass
class FunctionCall(Node):
    signature: str
    function: str
    arguments: list[Node]

    def __repr__(self) -> str:
        return f"FunctionCall<{self.function}>"

@dataclass
class FunctionDefinition(Node):
    signature: str
    function: str
    arguments: list[Node]
    body: list[Node] | None = None

    def __repr__(self) -> str:
        return f"FunctionDefinition<{self.function}>"
    
@dataclass
class MotionTurn(Node):
    angle: Node
    cw: bool

    def __repr__(self) -> str:
        return f"MotionTurn"
    
class PenActionType(Enum):
    DOWN = 0
    UP = 1
    CLEAR = 2
    STAMP = 3

@dataclass
class PenAction(Node):
    type: PenActionType

    def __repr__(self) -> str:
        return f"PenAction<{self.type.name}>"
    
@dataclass
class PenAssignment(Node):
    ...

@dataclass
class PenColorAssignment(Node):
    color: str

    def __repr__(self) -> str:
        return f"PenColorAssignment"


def generate_node(block: Block | Primitive) -> Node:
    if isinstance(block, Primitive):
        if block.type == PrimitiveType.VARIABLE:
            return Variable(block.value, block.id)
        
        elif block.type == PrimitiveType.LIST:
            return List(block.value, block.id)
        
        elif block.type == PrimitiveType.STRING:
            return Literal(LiteralType.STRING, block.value)
        
        elif block.type == PrimitiveType.COLOR:
            return Literal(LiteralType.COLOR, block.value)
        
        else:
            return Literal(LiteralType.NUMBER, float(block.value))
        
    if block.opcode == "motion_turnleft":
        return MotionTurn(generate_node(block.inputs["DEGREES"]), False)
    
    if block.opcode == "motion_turnright":
        return MotionTurn(generate_node(block.inputs["DEGREES"]), True)

    elif block.opcode == "data_setvariableto":
        return Assignment(Variable(block.fields["VARIABLE"][0], block.fields["VARIABLE"][1]), generate_node(block.inputs["VALUE"]))
    
    elif block.opcode == "data_changevariableby":
        var = Variable(block.fields["VARIABLE"][0], block.fields["VARIABLE"][1])
        return Assignment(var, BinOp(BinOpType.ADD, var, generate_node(block.inputs["VALUE"])))

    elif block.opcode == "control_repeat":
        inner_body = []
        generate_stack(inner_body, block.inputs["SUBSTACK"])
        return Repeat(generate_node(block.inputs["TIMES"]), inner_body)
    
    elif block.opcode == "control_if":
        inner_body = []
        generate_stack(inner_body, block.inputs["SUBSTACK"])
        return If(generate_node(block.inputs["CONDITION"]), inner_body)

    elif block.opcode == "procedures_call":
        arguments = []
        for arg in block.inputs:
            arguments.append(generate_node(block.inputs[arg]))

        func = block.procedure.replace("%s", "").replace("%b", "").replace(" ", "").strip()
        return FunctionCall(block.procedure, func, arguments)
    
    elif block.opcode == "argument_reporter_string_number":
        funcarg = FunctionArgument(FunctionArgumentType.ANY, block.fields["VALUE"][0])
        return funcarg
    
    elif block.opcode == "argument_reporter_boolean":
        funcarg = FunctionArgument(FunctionArgumentType.BOOL, block.fields["VALUE"][0])
        return funcarg
    
    elif block.opcode == "operator_add":
        return BinOp(BinOpType.ADD, generate_node(block.inputs["NUM1"]), generate_node(block.inputs["NUM2"]))
    
    elif block.opcode == "operator_subtract":
        return BinOp(BinOpType.SUB, generate_node(block.inputs["NUM1"]), generate_node(block.inputs["NUM2"]))
    
    elif block.opcode == "operator_multiply":
        return BinOp(BinOpType.MUL, generate_node(block.inputs["NUM1"]), generate_node(block.inputs["NUM2"]))
    
    elif block.opcode == "operator_divide":
        return BinOp(BinOpType.DIV, generate_node(block.inputs["NUM1"]), generate_node(block.inputs["NUM2"]))
    
    elif block.opcode == "operator_equals":
        return BinOp(BinOpType.EQ, generate_node(block.inputs["OPERAND1"]), generate_node(block.inputs["OPERAND2"]))
    
    elif block.opcode == "data_deletealloflist":
        return ListAction(ListActionType.CLEAR, List(block.fields["LIST"][0], block.fields["LIST"][1]))
    
    elif block.opcode == "data_addtolist":
        return ListAction(ListActionType.ADD, List(block.fields["LIST"][0], block.fields["LIST"][1]), generate_node(block.inputs["ITEM"]))
    
    elif block.opcode == "pen_penDown":
        return PenAction(PenActionType.DOWN)
    
    elif block.opcode == "pen_penUp":
        return PenAction(PenActionType.UP)
    
    elif block.opcode == "pen_clear":
        return PenAction(PenActionType.CLEAR)
    
    elif block.opcode == "pen_stamp":
        return PenAction(PenActionType.STAMP)
    
    elif block.opcode == "pen_setPenColorToColor":
        return PenColorAssignment(generate_node(block.inputs["COLOR"]))

def generate_stack(body: list, block: Block) -> None:
    while True:
        node = generate_node(block)
        body.append(node)

        block = block.next
        if block is None: return

def generate_ast(script: Block) -> list[Node]:
    body = []

    generate_stack(body, script)

    return body

def generate_ast_procdef(block: Block) -> FunctionDefinition:
    proto = block.inputs["custom_block"]

    arguments = []
    for arg in proto.inputs:
        argnode = generate_node(proto.inputs[arg])
        argnode.id = arg
        arguments.append(argnode)

    funcname = proto.procedure.replace("%s", "").replace("%b", "").replace(" ", "").strip()
    funcdef = FunctionDefinition(proto.procedure, funcname, arguments)

    funcdef.body = []
    generate_stack(funcdef.body, block.next)

    return funcdef