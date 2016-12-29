#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>

#include "include/webserver.h"

int open_listen_fd(char *port) {
    int listen_fd;
    int opt_val = 1;
    struct sockaddr_in server_addr;
    /*create a socket descriptor*/
    if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return -1;
    }
    /* Eliminates "Address already in use" error from bind */
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR,
                   (const void *) &opt_val, sizeof(int)) < 0) {
        perror("setsockopt");
        return -1;
    }
    /*clean server_addr structure*/
    bzero(&server_addr, sizeof(server_addr));
    /*listen_fd will be an end point for all requests to port
     *on any IP address for this host */
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = ntohs((unsigned short) atoi(port));
    if (bind(listen_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        return -1;
    }
    /* Make it a listening socket ready to accept connection requests */
    if (listen(listen_fd, SIZE_LISTEN_QUEUE) < 0) {
        perror("listen");
        return -1;
    }
    return listen_fd;
}
