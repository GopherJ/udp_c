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
            "  Usage: udp_client ip_address port\n"
            "\n"
    );
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    struct sockaddr_in server_addr;
    uint16_t port;
    int socket_fd;
    char *buf = NULL;
    size_t len;
    socklen_t addr_size;
    ssize_t recv_len;

    memset(&server_addr, 0, sizeof(server_addr));

    if (argc == 1)
        usage();
    else if (argc < 3)
        printf("\nError: no enough arguments\n\n"), usage();
    else if (inet_pton(AF_INET, argv[1], &(server_addr.sin_addr)) == 0)
        printf("\nError: invalid address\n\n"), usage();
    else if (!str_to_uint16(argv[2], &port))
        printf("\nError: invalid port\n\n"), usage();

    if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        error("Socket creation");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    while (true) {
        printf("\nEnter your message: ");
        if (getline(&buf, &len, stdin) != -1) {
            if (sendto(socket_fd, buf, len, 0, (const struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
                break;
            }

            memset(buf, 0, len);

            recv_len = recvfrom(socket_fd, buf, BUF_LEN, 0, (struct sockaddr *)&server_addr, &addr_size);
            if (recv_len == -1) break;

            printf("Recv: %s", buf);
        }
    }

    close(socket_fd);
    return EXIT_PROGRAM;
}
