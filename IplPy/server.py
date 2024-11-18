import socket
import threading
import os

socket_path = "/tmp/socket_server.sock"

def handle_client(client_socket, client_id):
    while True:
        message = client_socket.recv(1024).decode()
        if not message:
            break
        print(f"Cliente {client_id}: {message}")
    client_socket.close()


def server():
    if os.path.exists(socket_path):
        os.remove(socket_path)

    server_socket = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    server_socket.bind(socket_path)
    server_socket.listen(5)
    print("Servidor aguardando conexões...")

    while True:
        client_socket, _ = server_socket.accept()
        
        client_id = client_socket.recv(1024).decode()
        print(f"Novo cliente conectado: {client_id}")

        client_thread = threading.Thread(target=handle_client, args=(client_socket, client_id))
        client_thread.start()

if __name__ == "__main__":
    server()
