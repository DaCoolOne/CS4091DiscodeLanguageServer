

import json

class CodeServer:
    def __init__(self) -> None:
        pass

    def parse_command(self, command: str) -> str:
        command_obj = json.loads(command)

        if 'name' not in command_obj:
            raise ValueError('Expected "name" property in command object')

        command_type = command_obj['name']

        if command_type == 'ECHO':
            return command
        if command_type == 'load':
            return

