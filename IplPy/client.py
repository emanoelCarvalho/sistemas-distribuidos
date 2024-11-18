import socket
import time

socket_path = "/tmp/socket_server.sock"

def send_message(client_socket):
    while True:
        message = input("Digite uma mensagem: ")
        if message == "sair":
            break
        client_socket.send(message.encode())

def client():
    client_socket = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)

    client_socket.connect(socket_path)

    client_id = f"Cliente-{int(time.time())}"
    client_socket.send(client_id.encode())
    print(f"Conectado ao servidor com o identificador: {client_id}")

    send_message(client_socket)

    client_socket.close()

if __name__ == "__main__":
    client()
