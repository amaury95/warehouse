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

#include "include/cJSON.h"
#include "include/webserver.h"
#include "include/producer.h"

void *generator(void *params)
{    
    static int id = 1;

    while(1)
    {
        sem_wait(&sem_products);
        
        for(int i = 0; i < products->pos; i++)

            ((struct production *)products->elements[i])->pendding += 
            ((struct production *)products->elements[i])->generate;                         
             
        sem_post(&sem_products);
        
        sleep(1); 
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
 
    //Starting generator thread
    pthread_t ptid;
    pthread_create(&ptid, NULL, generator, NULL);

    while(1){
        sem_wait(&sem_products);
        
        for(int i = 0; i < products->pos; i++)

            if(((struct production *)products->elements[i])->pendding > 0)
            {
                

                pthread_t pid;
                pthread_create(&pid, NULL, send, &args);
            }                         
             
        sem_post(&sem_products);
    }
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
    cJSON *request = cJSON_Parse((char *)argv);

    if(request!=NULL)
    {

    }

    return NULL;
}