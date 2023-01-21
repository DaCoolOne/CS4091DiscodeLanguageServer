

import socket
import json

def run_test():
    
    # Create a connection
    print("Create socket")
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    print("Connecting")
    client.connect(("localhost", 3540))

    print("Send data")
    client.send((json.dumps({
        "Test": "Hello World"
    })).encode(encoding='utf8'))

    print("Receiving")
    data = client.recv(1028)

    print(data)

    print("done")
    client.close()

if __name__ == "__main__":
    run_test()

