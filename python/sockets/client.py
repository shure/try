import socket

# create an ipv4 (AF_INET) socket object using the tcp protocol (SOCK_STREAM)
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# connect the client
client.connect(('0.0.0.0', 9999))

def communicate(message):
    client.send(message.encode())
    response = client.recv(4096).decode()
    print(response)

communicate("TEST")
communicate("EXIT")
