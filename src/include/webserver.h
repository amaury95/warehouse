#ifndef WEBSERVER_H
#define WEBSERVER_H

#define SIZE_LISTEN_QUEUE 1024
#define MAXLINE 4096

void *server(void *);
char *client(char *, char *, char *);

int open_listenfd(char *port);
int open_clientfd(char *hostname, char *port);

#endif //WEBSERVER_H
