

import sys
import socket

from CodeServer import CodeServer

def cmd_arg(idx, default=None):
    if len(sys.argv) > idx:
        return sys.argv[idx]
    return default

def run_server(address="localhost", port=0):
    print("Starting server")
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    server.bind(("localhost", port))

    server.listen()

    # Todo: connect to multiple clients
    print("Waiting for connection")
    (clientsocket, address) = server.accept()

    code_server = CodeServer()

    running = True
    while running:
        try:
            command = clientsocket.recv(4028)
            if command == b'':
                running = False
            else:
                code_server.parse_command(command.decode(encoding='utf8'))
        except KeyboardInterrupt as kbi:
            running = False
        except Exception as e:
            print(e)

if __name__ == "__main__":
    run_server(port=int(cmd_arg(1, "3540")))

