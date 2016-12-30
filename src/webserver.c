#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <pthread.h>

#include <signal.h>
#include <poll.h>

#include "include/cJSON.h"
#include "include/structs.h"
#include "include/webserver.h"

void *server(void *argv)
{
  struct server_param *params = (struct server_param *)argv;
  
  int listenfd, connfd, clientlen;

  struct sockaddr_in clientaddr;
 
  listenfd = open_listenfd(params->port);

  while (1) 
  {
    clientlen = sizeof(clientaddr);

    printf("Waitting for connections at %s\n", params->port);

    connfd = accept(listenfd, (struct sockaddr *)&clientaddr, (socklen_t *)&clientlen);
  
    /*POLL*/
  
    char buff[MAXLINE];

    read(connfd, buff, MAXLINE);
  
    cJSON *req = cJSON_CreateObject();
    cJSON_AddStringToObject(req, "client", "consumer");
    cJSON_AddStringToObject(req, "product", "A");

    char *requ = cJSON_Print(req);

    cJSON *request = cJSON_CreateObject();
    cJSON_AddStringToObject(request, "request", requ);
    cJSON_AddStringToObject(request, "port", params->port);
    cJSON_AddNumberToObject(request, "connfd", connfd);

    pthread_t tid;
    pthread_create(&tid, NULL, params->process, request);
  }
  return NULL;
}

char *client(char *hostname, char *port, char *request)
{
  int clientfd = open_clientfd(hostname, port);

  write(clientfd, request, strlen(request));
  
  /*POLL*/  
  
  char *buff = malloc(MAXLINE);

  read(clientfd, buff, MAXLINE);
  
  close(clientfd);

  return buff;
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
