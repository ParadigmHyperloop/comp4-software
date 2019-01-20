import socket


UDP_IP = '127.0.0.1'
UDP_PORT = 6000

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))
while True:
    message, add = sock.recvfrom(1024)
    # message(str(message))
    print(message)
    print(message.ParseFromString())
