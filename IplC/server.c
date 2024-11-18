#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>

#define SOCKET_PATH "/tmp/socket_server.sock"

void *handle_client(void *arg) {
    int client_sock = *(int *)arg;
    char buffer[256];
    char client_id[256];

    if (recv(client_sock, client_id, sizeof(client_id), 0) < 0) {
        perror("Erro ao receber o identificador do cliente");
        close(client_sock);
        return NULL;
    }

    printf("Cliente conectado: %s\n", client_id);

    while (1) {
        int bytes_received = recv(client_sock, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            if (bytes_received == 0) {
                printf("Cliente %s desconectado.\n", client_id);
            } else {
                perror("Erro ao receber mensagem do cliente");
            }
            break;
        }

        buffer[bytes_received] = '\0';  

        printf("Cliente %s: %s\n", client_id, buffer);
    }

    close(client_sock);
    return NULL;
}

int main() {
    int server_sock, client_sock;
    struct sockaddr_un server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    pthread_t thread_id;

    server_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("Erro ao criar o socket");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SOCKET_PATH);

    unlink(SOCKET_PATH);

    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_un)) < 0) {
        perror("Erro ao associar o socket");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    if (listen(server_sock, 5) < 0) {
        perror("Erro ao escutar");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    printf("Servidor aguardando conexões...\n");

    while (1) {
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_sock < 0) {
            perror("Erro ao aceitar conexão");
            continue;
        }

        if (pthread_create(&thread_id, NULL, handle_client, &client_sock) != 0) {
            perror("Erro ao criar thread");
            close(client_sock);
        } else {
            pthread_detach(thread_id);
        }
    }

    close(server_sock);
    return 0;
}
