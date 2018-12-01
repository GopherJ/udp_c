/**
 * Author: cheng.jiang@utt.fr
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <netinet/in.h>

#include "utils.h"

#define BUF_LEN       1024
#define EXIT_PROGRAM   0
#define EXIT_FAILURE   1

/**
 * usage
 */
void usage() {
    printf(
            "\n"
            "  Usage: udp_client [ip_address] [port]\n"
            "\n"
    );
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    struct sockaddr_in server_addr;
    uint16_t port;
    int socket_fd;
    char *buf;
    size_t len;
    unsigned int socket_len;
    ssize_t recv_len;

    if (argc == 1)
        usage();
    else if (argc < 3)
        printf("\nError: no enough arguments\n\n"), usage();
    else if (inet_pton(AF_INET, argv[1], &(server_addr.sin_addr)) == 0)
        printf("\nError: invalid address\n\n"), usage();
    else if (!str_to_uint16(argv[2], &port))
        printf("\nError: invalid port\n\n"), usage();

    memset(&server_addr, 0, sizeof(server_addr));


    if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        error("Socket creation");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(port);

    while (1) {
        if (getline(&buf, &len, stdin) != -1) {
            if (sendto(socket_fd, buf, len, MSG_CONFIRM, (const struct sockaddr *) &server_addr, sizeof(server_addr)) == -1){
                close(socket_fd);
                break;
            }

            memset(buf, 0, sizeof(buf));

            recv_len = recvfrom(socket_fd, buf, BUF_LEN, MSG_WAITALL, (struct sockaddr *) &server_addr, &socket_len);
            if (recv_len == -1) {
                close(socket_fd);
                break;
            }

            printf("Recv: %s", buf);
            memset(buf, 0, sizeof(buf));
        }
    }
}
