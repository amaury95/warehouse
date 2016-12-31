#ifndef CONSUMER_H
#define CONSUMER_H

void *client_process(void *);
struct product get_product();
char *conform_request(struct product);
struct product json_to_product(cJSON *);

#endif