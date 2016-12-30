#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#include "include/structs.h"

struct capacity_t{
	int max;
	int pos;
}capacity;

TRIE *warehouse;

sem_t sem_warehouse;
sem_t sem_capacity;

#include "include/cJSON.h"
#include "include/webserver.h"
#include "include/warehouse.h"

//Structs to fill with tokenizer input
int   count = 2;
char* production_id[] = {"A", "B"};
int   production_sz[] = { 9 ,  8 };

int   pcount = 1;
char* ports[] = {"3000"};

int main(int argc, char const *argv[])
{
	sem_init(&sem_warehouse, 0, 1);
	sem_init(&sem_capacity, 0, 1);

	warehouse = trie_new();
	capacity.max = 10;
	capacity.pos = 0;

	for(int i = 0; i < pcount; i++)
	{
		TRIE *ptrie = trie_new();
		trie_add(warehouse, ports[i], ptrie);
		
		for(int j = 0; j < count; j++)
			trie_add(ptrie, production_id[j], stack_new(production_sz[j]));

		struct thread params;
		params.port = ports[i];
		params.process = server_process;

		pthread_t tid;		
		pthread_create(&tid, NULL, server, &params);
	}
	
	pthread_exit(NULL);

	return 0;
}

struct stack *get_stack(char *port, char *product)
{
	if(trie_contains(warehouse, port))
	{
		TRIE *temp = (TRIE *)trie_get(warehouse, port);

		if(trie_contains(temp, product))
		
			return (struct stack *)trie_get(temp, product);
		
		sem_wait(&sem_capacity);

		struct stack *stack = stack_new(capacity.max);
		
		sem_post(&sem_capacity);		

		trie_add(temp, product, stack);

		return stack;
	}
	return NULL;
}

int has_product(char *port, char *product)
{
	struct stack *stack;

	if( (stack = get_stack(port, product)) != NULL )

		return stack->pos > 0;

	return 0;
}

int can_product(char *port, char *product)
{
	struct stack *stack;
	
	sem_wait(&sem_capacity);

	int cap = capacity.max - capacity.pos;

	sem_post(&sem_capacity);

	if(cap > 0 && (stack = get_stack(port, product)) != NULL )

		return stack->pos != stack->len;

	return 0;
}

cJSON *get_product(char *port, char *product)
{
	struct stack *stack;

	if( (stack = get_stack(port, product)) != NULL )
	{
		sem_wait(&sem_capacity);

		capacity.pos--;

		sem_post(&sem_capacity);		
		
		return stack_pop(stack);
	}

	return NULL;
}

void set_product(char *port, char *product, cJSON *value)
{
	struct stack *stack;

	if( (stack = get_stack(port, product)) != NULL )
	{
		sem_wait(&sem_capacity);

		capacity.pos++;

		sem_post(&sem_capacity);
		
		stack_push(stack, value);
	}
}

void *server_process(void *argv)
{
	cJSON *args, *request, *respond;
	
	args = (cJSON*)argv;

	request = cJSON_Parse(cJSON_GetObjectItem(args, "request")->valuestring);
	
	respond = cJSON_CreateObject();
	

	char *port = cJSON_GetObjectItem(args, "port")->valuestring;
	
	int connfd = cJSON_GetObjectItem(args, "connfd")->valueint;
 
	sem_wait(&sem_warehouse);

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
		char *product = cJSON_GetObjectItem(request, "type")->valuestring;
		
		if(can_product(port, product))
		{
			cJSON_AddStringToObject(respond, "result", "accept");
			
			set_product(port, product, (cJSON*)cJSON_GetObjectItem(request, "value"));
		}
		else
			cJSON_AddStringToObject(respond, "result", "dennied");
	}

	sem_post(&sem_warehouse);

	char *rendered = cJSON_Print(respond);

	printf("%s\n", rendered);

	write(connfd, rendered, strlen(rendered));
	
	close(connfd);
	
	pthread_detach(pthread_self());
	
	return NULL;
}