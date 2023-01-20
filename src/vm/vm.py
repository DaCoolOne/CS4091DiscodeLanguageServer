from __future__ import annotations
from typing import Dict, List, Callable, Optional

class FCall:
    def __init__(self, identifier: str, ret_to: Optional[int], args: List[int]) -> None:
        self.ident = identifier
        self.ret = ret_to
        self.args = args

class Object:
    def __init__(self, type: str) -> None:
        self.type = type

class Null(Object):
    def __init__(self) -> None:
        super().__init__('null')
    def __str__(self) -> str:
        return "<NULL>"

class Number(Object):
    def __init__(self, value: float = 0) -> None:
        super().__init__('number')
        self.value: float = value
    def __str__(self) -> str:
        return str(self.value)

class String(Object):
    def __init__(self, value: str) -> None:
        super().__init__('string')
        self.value: str = value
    def __str__(self) -> str:
        return self.value

# Basically a function except it's not user defined
class Operation(Object):
    def __init__(self, f: Callable[["VirtualMachine", List[Object]], Optional[Object]]) -> None:
        super().__init__('internal')
        self.f = f
    def run(self, vm: "VirtualMachine", args: List[Object]) -> Object:
        ret = self.f(vm, args)
        if ret is None:
            ret = Null()
        return ret

class Function(Operation):
    def __init__(self) -> None:
        self.type = 'function'
        self.instructions: List[FCall] = []
        self.memspace = 0
    def newIns(self, f: FCall) -> None:
        self.instructions.append(f)
        self.memspace = max(self.memspace - 1, f.ret if f.ret is not None else 0, *f.args) + 1
    def run(self, vm: "VirtualMachine", args: List[Object]) -> Object:
        if len(args) < self.memspace:
            args += [ Null() for i in range(self.memspace - len(args)) ]
        for ins in self.instructions:
            funct = vm.resolve_identifier(ins.ident)
            if isinstance(funct, Operation):
                if ins.ret is None:
                    funct.run(vm, [ args[arg_idx] for arg_idx in ins.args ])
                else:
                    args[ins.ret] = funct.run(vm, [ args[arg_idx] for arg_idx in ins.args ])
            else:
                raise ValueError(f"Cannot execute value of type {funct.type}")
        return Null()

class Array(Object):
    def __init__(self) -> None:
        super().__init__('array')
        self.value: List[Object] = []
    def __str__(self) -> str:
        return f"[{[o for o in self.value].join(',')}]"

class Dictionary(Object):
    def __init__(self) -> None:
        super().__init__('dict')
        self.string_dict: Dict[str, Object] = {}
    def at(self, key: str) -> Object:
        if key not in self.string_dict:
            raise ValueError(f"Could not find identifier {key}")
        return self.string_dict[key]

class VirtualMachine:
    def __init__(self) -> None:
        self.stack: List[Object] = []
        self.globals = Dictionary()

    def execute(self, command: str, args: List[Object]) -> None:
        f = self.resolve_identifier('#commands.'+command)
        if isinstance(f, Function):
            f.run(self, args)

    def resolve_identifier(self, name: str):
        return self.globals.at(name)
