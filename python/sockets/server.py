import socket
import threading
import signal
import sys

bind_ip = '0.0.0.0'
bind_port = 9999

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind((bind_ip, bind_port))
server.listen(5)  # max backlog of connections

def sigint_handler(sig, no):
    print("\nClosing server socket")
    server.close()
    sys.exit(0)

signal.signal(signal.SIGINT, sigint_handler)

print('Listening on {0}:{1}'.format(bind_ip, bind_port))

def handle_client_connection(client_socket):
    while True:
        request = client_socket.recv(1024).decode()
        print('Received {0}'.format(request))
        client_socket.send('ACK'.encode())
        if (request == "EXIT"):
            print("Closing client connection.")
            break
    client_socket.close()

while True:
    client_sock, address = server.accept()
    print('Accepted connection from {0}:{0}.'.format(address[0], address[1]))
    client_handler = threading.Thread(
        target=handle_client_connection,
        args=(client_sock,)
    )
    client_handler.start()
