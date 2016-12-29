#ifndef STRUCTS_H
#define STRUCTS_H
/*
PRODUCT Implementation
TODO:HERE
*/
struct product{
  int product_id;
  char provider_id[3];
  char product_name[3];
  char product_content[4083];
};

/*
STACH Implementation
TODO:HERE
*/
struct stack{
  int pos;
  int len;
  struct product **elements;
};

struct stack *stack_new(int len);
void *stack_pop(struct stack* stack);
int stack_push(struct stack *stack, void *product);

/*
TRIE Implementation
TODO:HERE
*/
#define TRIE_LEN 36

typedef struct trie_t{
  void *value;
  struct trie_t **children;
}TRIE;

TRIE *trie_new();
int trie_pos(char c);
void *trie_get(TRIE *trie, char *chain);
int trie_contains(TRIE *trie, char *chain);
int trie_add(TRIE *trie, char *chain, void *value);

#endif
