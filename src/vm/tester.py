

from operations import populate_globals

from vm import VirtualMachine, FCall
from vm import Function, Number, String

# Testing
if __name__ == "__main__":
    vm = VirtualMachine()

    populate_globals(vm)

    test_f = Function()
    test_f.newIns(FCall('!internals.add', 0, [0, 1]))
    test_f.newIns(FCall('!internals.print', None, [0]))
    vm.globals.string_dict['#commands.test'] = test_f

    hello_world_test = Function()
    hello_world_test.newIns(FCall('!internals.print', None, [0]))
    vm.globals.string_dict['#commands.hello'] = hello_world_test

    vm.execute('test', [ Number(1.0), Number(1.0) ])
    vm.execute('hello', [ String('Hello World!') ])

