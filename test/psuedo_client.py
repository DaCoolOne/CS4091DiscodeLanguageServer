

import os
import sys
import time
import socket
import json

import queue
import subprocess
import threading

CREATE_SERVER = True

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
        print("Create sockets")
        client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        response_client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        print("Connecting")
        client.connect(("localhost", 3540))
        response_client.connect(("localhost", 3541))

        client.setblocking(False)
        response_client.setblocking(False)

        EXISTING_COMMANDS = set()

        def recvcommands(q: queue.Queue):
            while q.empty():
                # Todo: make data have more length
                try:
                    data = response_client.recv(4096)
                    obj = json.loads(data.decode(encoding='utf8'))
                    print(obj)
                    if obj['Name'] == 'Add Func':
                        server_id = obj['Server_ID']
                        name = obj['Function_name']
                        args = obj['Arguments']
                        arg_str = ','.join(args)
                        print(f"Register command {name}({arg_str})")
                        EXISTING_COMMANDS.add(name)
                    elif obj['Name'] == 'Send Message':
                        chid = obj['Channel_ID']
                        msg = obj['Message']
                        print(f'#{chid}: {msg}')
                except BlockingIOError:
                    pass
                
                time.sleep(0.01)

        def sendcommands():
            time.sleep(1) # Give time for VM to sort itself
            while True:
                cmd = input("Enter command: ")
                if cmd == 'quit' or cmd == '':
                    break
                if cmd in EXISTING_COMMANDS:
                    send_msg(client, {
                        "Name": "Run",
                        "Server_ID": "psuedo",
                        "Server_Name": "pseudo",
                        "Channel_ID": "tester",
                        "Channel_Name": "tester",
                        "Function": cmd,
                        "Message": {  }
                    })
                else:
                    print(f"{cmd} is not a command")
                time.sleep(0.05)

        stopQueue = queue.Queue()
        t1 = threading.Thread(target=sendcommands)
        t2 = threading.Thread(target=recvcommands, args=(stopQueue,))

        t2.start()

        print("Send code")
        for channel in os.listdir(base):
            print(f"Found channel {channel}")
            for msg in os.listdir(os.path.join(base, channel)):
                with open(os.path.join(base, channel, msg)) as f:
                    code = f.read()
                    send_msg(client, {
                        "Name": "ParseTree",
                        "Code": code
                    })
                    send_msg(client, {
                        "Name": "Load",
                        "Server_ID": "psuedo",
                        "Server_Name": "pseudo",
                        "Channel_ID": channel,
                        "Channel_Name": channel,
                        "Code": code
                    })
                    time.sleep(1)

        t1.start()
        t1.join()

        print("Shutting down")
        stopQueue.put("Stop")
        t2.join()
        client.close()
        response_client.close()

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

