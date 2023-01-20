

import socket
import json

def run_test():
    
    # Create a connection
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    client.connect(("localhost", 3540))

    client.send(json.dumps({
        "Test": "Hello World"
    }).encode(encoding='utf8'))

    client.close()

if __name__ == "__main__":
    run_test()

