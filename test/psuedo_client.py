

import os
import sys
import socket
import json

def send_msg(client: socket.socket, obj: dict):
    client.send((json.dumps(obj) + '\n').encode(encoding='utf8'))

def run_test():
    
    base = '.' if len(sys.argv) < 2 else sys.argv[1]

    # Create a connection
    print("Create socket")
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    print("Connecting")
    client.connect(("localhost", 3540))

    print("Send code")
    for channel in os.listdir(base):
        print(f"Found channel {channel}")
        for msg in os.listdir(os.path.join(base, channel)):
            with open(os.path.join(base, channel, msg)) as f:
                send_msg(client, {
                    "Name": "ParseTree",
                    "Code": f.read()
                })

    """
    send_msg(client, {
        "Name": "Run",
        "Function": "main",
        "Arguments": []
    })
    """

    # print("Receiving")
    # data = client.recv(1028)

    # print(data)

    # print("done")
    client.close()

if __name__ == "__main__":
    run_test()

