/**
 *
 * Author: cheng.jiang@utt.fr
 *
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>

#include <sys/types.h> 
#include <sys/socket.h>

#include <arpa/inet.h> 
#include <netinet/in.h>

#include "utils.h"

#define EXIT_PROGRAM  0
#define EXIT_FAILURE  1
#define BUF_LEN       1024

/**
 * usage
 */
void usage() {
    printf(
            "\n"
            "  Usage: udp_server ip_address port\n"
            "\n"
    );
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    struct sockaddr_in server_addr, client_addr;
    uint16_t port;
    char buf[BUF_LEN];
    ssize_t n;
    socklen_t addr_size;
    char trans_buf[BUF_LEN];
    int opt_val = 1, socket_fd, offset = 0;

    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    if (argc == 1)
        usage();
    else if (argc < 3)
        printf("\nError: no enough arguments\n\n"), usage();
    else if (inet_pton(AF_INET, argv[1], &(server_addr.sin_addr)) == 0)
        printf("\nError: invalid address\n\n"), usage();
    else if (!str_to_uint16(argv[2], &port))
        printf("\nError: invalid port\n\n"), usage();

    if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        error("Socket creation");
        exit(EXIT_FAILURE); 
    }

    setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt_val, sizeof(int));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
      
    if (bind(socket_fd, (const struct sockaddr *) &server_addr,  sizeof(server_addr)) < 0) {
        error("Socket binding");
        exit(EXIT_FAILURE);
    }

    printf("Server listen on %s:%d\n", argv[1], port);

    while (true) {
        n = recvfrom(socket_fd, buf, BUF_LEN, 0, (struct sockaddr *)&client_addr, &addr_size);
        if (n == -1) break;
        buf[n] = '\0';
        printf("Recv: %s\n", buf);

        for (int i = 0; buf[i] != '\0'; i += 1) {
            if ((buf[i] >= 'A' && buf[i] <= 'Z') || (buf[i] >= 'a' && buf[i] <= 'z')) {
                trans_buf[offset] = buf[i];
                offset += 1;
            }
        }

        trans_buf[offset] = '\0';
        offset = 0;
        printf("TransferredTo: %s\n", trans_buf);

        sendto(socket_fd, trans_buf, strlen(trans_buf), 0, (const struct sockaddr *)&client_addr, sizeof(client_addr));
    }

    close(socket_fd);
    return EXIT_PROGRAM;
}
