#ifndef PRODUCER_H
#define PRODUCER_H

void *client_process(void *);
struct product get_product();
char *conform_request(struct product);
cJSON *product_to_json(struct product);

#endif