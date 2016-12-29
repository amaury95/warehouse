#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>

#include <signal.h>
#include <poll.h>

#include "include/structs.h"
#include "include/webserver.h"

char *server(char *port, TRIE *products, void(*process)(char *, TRIE *, int))
{
  int listenfd, connfd, clientlen;
  struct sockaddr_in clientaddr;
  // struct hostent *hp;
  // char *haddrp;

  listenfd = open_listenfd(port);
  while (1) {
    clientlen = sizeof(clientaddr);
    connfd = accept(listenfd, (struct sockaddr *)&clientaddr, (socklen_t *)&clientlen);

    // hp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr,
    // sizeof(clientaddr.sin_addr.s_addr), AF_INET);
    // haddrp = inet_ntoa(clientaddr.sin_addr);

    /*POLL*/

    char buff[MAXLINE];
    read(connfd, buff, MAXLINE);

    /*THREAD*/
    process(buff, products, connfd);
  }
  exit(0);
}

char *client(char *host, char *port, char *request, char *(*process)(char *))
{
  int clientfd = open_clientfd(host, port);
  write(clientfd, request, strlen(request));

  /*POLL*/


  char buff[MAXLINE];
  read(clientfd, buff, MAXLINE);
  close(clientfd);

  return process(buff);
}

int open_listenfd(char *port)
{
  int listen_fd;
  int opt_val = 1;
  struct sockaddr_in server_addr;
  if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      perror("socket");
      return -1;
  }
  if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR,
                 (const void *) &opt_val, sizeof(int)) < 0) {
      perror("setsockopt");
      return -1;
  }
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = ntohs((unsigned short) atoi(port));
  if (bind(listen_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
      perror("bind");
      return -1;
  }
  if (listen(listen_fd, SIZE_LISTEN_QUEUE) < 0) {
      perror("listen");
      return -1;
  }
  return listen_fd;
}

int open_clientfd(char *hostname, char *port)
{
  int clientfd;
  struct hostent *hp;
  struct sockaddr_in serveraddr;

  if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    return -1;

  if ((hp = gethostbyname(hostname)) == NULL)
    return -2;
  bzero((char *) &serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  bcopy((char *)hp->h_addr_list[0],
  (char *)&serveraddr.sin_addr.s_addr, hp->h_length);
  serveraddr.sin_port = htons(atoi(port));

  if (connect(clientfd, (const struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0)
    return -1;
  return clientfd;
}
