import socket

HOST = '127.0.0.1'
PORT = 65432

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen()
    print ("listening...")
    conn, addr = s.accept()
    with conn:
        print (f'connected: {conn} from address: {addr}')
        while True:
            data = conn.recv(1024)
            print(f'data: {data}')
            if not data:
                break
            conn.sendall(data)
