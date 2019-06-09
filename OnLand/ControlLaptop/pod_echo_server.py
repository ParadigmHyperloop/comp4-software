import socket

HOST = '127.0.0.1'
PORT = 3001 

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen()
    print("listening...")
    conn, addr = s.accept()
    with conn:
        print('connected: {conn} from address: {addr}')
        while True:
            try:
                data = conn.recv(1024)
                print('data: {data}')
                if not data:
                    break
                conn.sendall('ok'.encode('utf-8'))
            except ConnectionResetError as e:
                print("Disconnection...")
                break
