#ifndef WEBSERVER_H
#define WEBSERVER_H

#define SIZE_LISTEN_QUEUE 1024
#define MAXLINE 5120

void *server(void *);
void *client(void *);

int open_listenfd(char *port);
int open_clientfd(char *hostname, char *port);

#endif //WEBSERVER_H
