

from typing import List

from vm import VirtualMachine
from vm import Object, Number, String, Null, Function, Operation

import inspect
import sys

def __operation_add(_, objects: List[Object]) -> Object:
    if len(objects) != 2:
        raise ValueError()
    if objects[0].type != 'number' or objects[1].type != 'number':
        raise ValueError()
    nums: List[Number] = objects
    return Number(nums[0].value + nums[1].value)
def __operation_print(_, objects: List[Object]) -> None:
    print(*[o for o in objects])

def populate_globals(vm: VirtualMachine):
    _len = len('__operation_')
    for name, obj in inspect.getmembers(sys.modules[__name__], inspect.isfunction):
        if name.startswith('__operation_'):
            vm.globals.string_dict['!internals.'+name[_len:]] = Operation(obj)
