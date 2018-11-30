/**
 *  Cheng JIANG
 *
 *  simple udp client
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

#define MAXLINE       1024
#define EXIT_FAILURE  1

static volatile bool KEEP_RUNNING = true;

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

/**
 *
 * convert string to uint16_t
 *
 * @param str
 * @param n
 * @return
 */
bool str_to_uint16(const char *str, uint16_t *n) {
    char *end;
    errno = 0;
    intmax_t val = strtoimax(str, &end, 10);
    if (errno == ERANGE || val < 0 || val > UINT16_MAX || end == str || *end != '\0')
        return false;
    *n = (uint16_t) val;
    return true;
}

/**
 *  SIGINT signal handler
 *
 */
void sig_handler(int signo) {
    KEEP_RUNNING = false;
}

int main(int argc, char **argv) {
    struct sockaddr_in server_addr;
    uint16_t *port;

    if (argc == 1)
        usage();
    else if (argc < 3)
        printf("\nError: no enough arguments\n\n"), usage();
    else if (inet_pton(AF_INET, argv[1], &(server_addr.sin_addr)) == 0)
        printf("\nError: invalid address\n\n"), usage();
    else if (!str_to_uint16(argv[2], port))
        printf("\nError: invalid port\n\n"), usage();

    int socket_fd;

    char *buf;
    size_t len;
    unsigned int n;

    if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("\nError: socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(*port);

    signal(SIGINT, sig_handler);

    while (KEEP_RUNNING && getline(&buf, &len, stdin) != -1) {
        sendto(socket_fd, buf, len, MSG_CONFIRM, (const struct sockaddr *) &server_addr, sizeof(server_addr));
        recvfrom(socket_fd, buf, MAXLINE, MSG_WAITALL, (struct sockaddr *) &server_addr, &n);
        printf("Recv: %s", buf);
    }

    close(socket_fd);
}
