#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#include "include/structs.h"

struct stack *servers;
struct stack *products;

sem_t sem_servers;
sem_t sem_products;

int ppid = 1;

#include "include/cJSON.h"
#include "include/webserver.h"
#include "include/producer.h"

void *generator(void *params)
{    
    while(1)
    {
        sem_wait(&sem_products);
        
        for(int i = 0; i < products->pos; i++)

            if(((struct production *)products->elements[i])->pendding == 0)
            {
                ((struct production *)products->elements[i])->pendding += 
                ((struct production *)products->elements[i])->generate;   
                for(int j = 0; j < ((struct production *)products->elements[i])->generate; j++)
                    printf("<--> %s\n", ((struct production *)products->elements[i])->id);
            }                      
             
        sem_post(&sem_products);
        
        sleep(1); 
    } 
}

void *singlep(void *args)
{
    int i = *((int *)args);

    while(1)

        if(((struct production *)products->elements[i])->pendding > 0)
                
            for(int j = 0; j < servers->pos; j++){
                
                printf("requesting..\n");

                struct product product;
                product.product_id = ppid;
                strcpy(product.provider_id, "AD");
                strcpy(product.product_name, ((struct production *)products->elements[i])->id);
                strcpy(product.product_content, "basura");

                struct thread args;
                args.port = ((struct thread *)servers->elements[j])->port;
                args.hostname = ((struct thread *)servers->elements[j])->hostname;
                args.request = conform_request(product);
                args.process = client_process;

                if( *(int *)client(&args) ) { ppid ++; break; }
            }
}

int main(int argc, char const *argv[])
{
    //Initialize local variables
    sem_init(&sem_servers, 0, 1);
	sem_init(&sem_products, 0, 1);

    servers = stack_new(STACK_MAX);
    products = stack_new(STACK_MAX);

    //Simuling parameters
    struct thread server;
    server.port = "3000";
    server.hostname = "192.168.99.100";

    stack_push(servers, &server);
 
    struct production prod;
    prod.id = "A";
    prod.generate = 2;
    prod.pendding = 0;

    stack_push(products, &prod);

    struct thread server2;
    server2.port = "4000";
    server2.hostname = "192.168.99.100";

    stack_push(servers, &server2);
 
    struct production prod2;
    prod2.id = "B";
    prod2.generate = 3;
    prod2.pendding = 0;

    stack_push(products, &prod2);

    //Starting generator thread
    pthread_t ptid;

    pthread_create(&ptid, NULL, generator, NULL);
    
    for(int i = 0; i < products->pos; i++)
    {
        int *p = malloc(sizeof(int)); *p = i;
        pthread_create(&ptid, NULL, singlep, p);
    }
    pthread_exit(NULL);

    return 0;
}

char *conform_request(struct product product)
{
    cJSON *request = cJSON_CreateObject();
    cJSON_AddStringToObject(request, "client", "producer");
    cJSON_AddStringToObject(request, "type", product.product_name);
    cJSON_AddItemToObject(request, "value", product_to_json(product));

    return cJSON_Print(request);
}

cJSON *product_to_json(struct product p)
{
  cJSON *json = cJSON_CreateObject();
 
  cJSON_AddStringToObject(json, "name", p.product_name);
  cJSON_AddStringToObject(json, "pid", p.provider_id);
  cJSON_AddStringToObject(json, "content", p.product_content);
  cJSON_AddNumberToObject(json, "id", p.product_id);

  return json;
}

void *client_process(void *argv)
{
    cJSON *response = cJSON_Parse((char *)argv);

    int *retval = malloc(4);

    *retval = 0;

    if(response != NULL)
    
        if(strcmp(cJSON_GetObjectItem(response, "result")->valuestring, "accept") == 0)
        {
            char *id = cJSON_GetObjectItem(response, "id")->valuestring;

            sem_wait(&sem_products);

            for(int i = 0; i < products->pos; i++)

                if(strcmp(((struct production *)products->elements[i])->id, id) == 0)
                 
                    ((struct production *)products->elements[i])->pendding--;

            sem_post(&sem_products);     
                
            *retval = 1;

            printf("---> %s\n", id);
        }
    
    return retval;
}