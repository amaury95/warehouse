#ifndef WEBSERVER_H
#define WEBSERVER_H

#define SIZE_LISTEN_QUEUE 1024
#define MAXLINE 4096

char *server(char *port, TRIE *products, void(*process)(char *, TRIE *, int));
char *client(char *host, char *port, char *request, char *(*process)(char *));

int open_listenfd(char *port);
int open_clientfd(char *hostname, char *port);

#endif //WEBSERVER_H
