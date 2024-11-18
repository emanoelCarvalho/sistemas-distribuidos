#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <time.h>

#define SOCKET_PATH "/tmp/socket_server.sock"

int main() {
    int client_sock;
    struct sockaddr_un server_addr;
    char buffer[256];
    char client_id[256];

    client_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_sock < 0) {
        perror("Erro ao criar o socket");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SOCKET_PATH);

    if (connect(client_sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_un)) < 0) {
        perror("Erro ao conectar ao servidor");
        close(client_sock);
        exit(EXIT_FAILURE);
    }

    snprintf(client_id, sizeof(client_id), "Cliente-%ld", time(NULL));

    send(client_sock, client_id, strlen(client_id) + 1, 0);

    printf("Conectado ao servidor com o identificador: %s\n", client_id);

    while (1) {
        printf("Digite uma mensagem (ou 'sair' para encerrar): ");
        fgets(buffer, sizeof(buffer), stdin);
        
        buffer[strcspn(buffer, "\n")] = 0;

        if (strcmp(buffer, "sair") == 0) {
            break;
        }

        send(client_sock, buffer, strlen(buffer) + 1, 0);
    }

    close(client_sock);
    return 0;
}
