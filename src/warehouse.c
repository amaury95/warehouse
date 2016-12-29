#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#include "include/structs.h"

TRIE *warehouse;
sem_t sem_warehouse;

#include "include/cJSON.h"
#include "include/webserver.h"
#include "include/warehouse.h"


int main(int argc, char const *argv[])
{
	// struct ports_t{
	// 	int count = 3;
	// 	char *portno[] = {"3000", "4000", "5000"};
	// }ports;
	//
	// for(int i = 0; i < ports.count; i++)
	// 	/*thread*/
	// 	server(ports.portno[i]);

	return 0;
}

int has_product(char *port, char *product)
{
	return 0;
}

int can_product(char *port, char *product)
{
	return 0;
}

cJSON *get_product(char *port, char *product)
{
	return NULL;
}

void set_product(char *port, char *product, cJSON *value)
{

}

void *server_process(void *argv){
	
	cJSON *args, *request, *respond;

	args = (cJSON*)argv;
	
	request = cJSON_Parse(cJSON_GetObjectItem(args, "request")->valuestring);
	
	respond = cJSON_CreateObject();

	char *port = cJSON_GetObjectItem(args, "port")->valuestring;
	
	int connfd = cJSON_GetObjectItem(args, "connfd")->valueint;

	if(strcmp(cJSON_GetObjectItem(request, "client")->valuestring, "consumer") == 0)
	{
		char *product = cJSON_GetObjectItem(request, "product")->valuestring;
		 
		if(has_product(port, product))
		{
			cJSON_AddStringToObject(respond, "result", "accept");
			
			cJSON *value = get_product(port, product);
			
			cJSON_AddItemToObject(respond, "value", value);
		}
		else
			cJSON_AddStringToObject(respond, "result", "dennied");
		
	}

	if(strcmp(cJSON_GetObjectItem(request, "client")->valuestring, "producer") == 0)
	{
		char *product = cJSON_GetObjectItem(request, "product")->valuestring;
		 
		if(can_product(port, product))
		{
			cJSON_AddStringToObject(respond, "result", "accept");
			
			set_product(port, product, (cJSON*)cJSON_GetObjectItem(request, "value"));
		}
		else
			cJSON_AddStringToObject(respond, "result", "dennied");
	}

	char *rendered = cJSON_Print(respond);

	write(connfd, rendered, strlen(rendered));
	
	close(connfd);
	
	pthread_detach(pthread_self());
	
	return NULL;
}