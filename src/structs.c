#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "include/structs.h"
/*
WAREHOUSE Implementation
TODO:HERE
*/
struct stack *stack_new(int len)
{
  struct stack *stack = malloc(sizeof(struct stack));

  stack->elements = malloc(sizeof(void*) * len);

  stack->len = len;

  stack->pos = 0;

  return stack;
}
void *stack_pop(struct stack* stack)
{
  if(stack->pos-- == 0)return NULL;

  return stack->elements[stack->pos];
}
int stack_push(struct stack *stack, void *product)
{
  if(stack->pos == stack->len)return 0;

  return (stack->elements[stack->pos++] = product) != NULL;
}

/*
TRIE Implementation
TODO:HERE
*/
int trie_pos(char c)
{
  c = tolower(c);

  return c < 58 ? c - 48 : c < 123 ? c - 87 : c;
}
TRIE *trie_new()
{
  TRIE *trie = malloc(sizeof(TRIE));

  trie->children = malloc(sizeof(TRIE) * TRIE_LEN);

  return trie;
}

int trie_contains_p(TRIE *trie, char *chain, int pos, int len)
{
  if(pos == len)return trie->value != NULL;

  return trie->children[trie_pos(chain[pos])] != NULL &&

         trie_contains_p(trie->children[trie_pos(chain[pos])], chain, pos + 1, len);
}
int trie_contains(TRIE *trie, char *chain)
{
  return trie_contains_p(trie, chain, 0, strlen(chain));
}

int trie_add_p(TRIE *trie, char *chain, void *value, int pos, int len)
{
  if(pos == len) return (trie->value = value) != NULL;

  if(trie->children[trie_pos(chain[pos])] == NULL)

    trie->children[trie_pos(chain[pos])] = trie_new();

  return trie_add_p(trie->children[trie_pos(chain[pos])], chain, value, pos + 1, len);
}
int trie_add(TRIE *trie, char *chain, void *value)
{
  return trie_add_p(trie, chain, value, 0, strlen(chain));
}

void *trie_get_p(TRIE *trie, char *chain, int pos, int len)
{
  if(pos == len)return trie->value;

  return trie_get_p(trie->children[trie_pos(chain[pos])], chain, pos + 1, len);
}
void *trie_get(TRIE *trie, char *chain)
{
  if(trie_contains(trie, chain))

    return trie_get_p(trie, chain, 0, strlen(chain));
    
  return NULL;
}
