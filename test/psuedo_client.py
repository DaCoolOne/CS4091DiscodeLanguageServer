

import os
import sys
import time
import socket
import json

import signal
import subprocess
import threading

CREATE_SERVER = False

def send_msg(client: socket.socket, obj: dict):
    client.send((json.dumps(obj) + '\n').encode(encoding='utf8'))

def run_test():
    
    base = input('Test: ') if len(sys.argv) < 2 else sys.argv[1]

    # Launch the server
    if CREATE_SERVER:
        server_object = subprocess.Popen(["../src/code_server/server.exe", '--single-inst'], bufsize=1, stdout=subprocess.PIPE, stderr=subprocess.PIPE, encoding='utf8')
    
    try:
        time.sleep(1)

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
                    """send_msg(client, {
                        "Name": "ParseTree",
                        "Code": f.read()
                    })"""
                    send_msg(client, {
                        "Name": "Load",
                        "Server_id": "tester",
                        "Channel_id": channel,
                        "Code": f.read()
                    })

        while True:
            cmd = input("Enter command: ")
            if cmd == 'quit' or cmd == '':
                break
            print("Sending")
            send_msg(client, {
                "Name": "Run",
                "Server_id": "tester",
                "Function": cmd,
                "Message": {  },
            })
            print("Sent!")

        print("Shutting down")
        client.close()

    finally:
        if CREATE_SERVER:
            stdout, stderr = server_object.communicate()

            print("Server output:")
            print(stdout)

            if server_object.returncode != 0:
                print(f"Process reported error:")
                print(stderr)


if __name__ == "__main__":
    os.chdir(os.path.dirname(__file__))
    run_test()

