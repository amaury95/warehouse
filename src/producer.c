#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#include "include/structs.h"

struct stack *servers;
struct stack *products;

#include "include/cJSON.h"
#include "include/webserver.h"
#include "include/producer.h"

int main(int argc, char const *argv[])
{
    servers = stack_new(STACK_MAX);
    products = stack_new(STACK_MAX);

    struct thread server;
    server.port = "3000";
    server.hostname = "192.186.99.100";

    stack_push(servers, &server);
    
    while(1){
        struct product product = get_product();
        char *request = conform_request(product);
        for(int i = 0; i < servers->pos; i++){
            struct thread param;
            param.hostname = ((struct thread *)servers->elements[i])->hostname; 
            param.port = ((struct thread *)servers->elements[i])->port;
            param.request = request;
            param.process = client_process;

            pthread_t tid;
            pthread_create(&tid, NULL, client, &param);
        }
        sleep(1);
    }
    return 0;
}

void *client_process(void *argv){
    cJSON *request = cJSON_Parse((char *)argv);

    printf("%s\n", cJSON_Print(request));

    pthread_detach(pthread_self());
    return NULL;
}

struct product get_product()
{
    struct product p;
    p.product_id = 10;
    strcpy(p.provider_id, "A0");
    strcpy(p.product_name, "A0");
    strcpy(p.product_content, "Ilikedressingup");
    return p;
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

