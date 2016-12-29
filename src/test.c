#include <stdio.h>
#include "include/cJSON.h"
#include "include/structs.h"

int main(int argc, char const *argv[]) {
  TRIE *trie = trie_new();
  int a = trie_add(trie, "warehouse", (void *)18293);
  int b = trie_contains(trie, "warehouse");
  void *v = trie_get(trie, "warehouse");

  int a2 = trie_add(trie, "warehous", "subterfuge");
  int b3 = trie_contains(trie, "warehous");
  void *v4 = trie_get(trie, "warehous");

  printf("%d %d %d\n", a, b, v);
  printf("%d %d %s\n", a2, b3, v4);

  struct stack *stack = stack_new(10);
  for (size_t i = 1; i < 15; i++)
    printf("%d\n", stack_push(stack, (void *)i));

  for (size_t i = 0; i < 11; i++)
    printf("%d ", (size_t)stack_pop(stack));

  printf("\n");
  return 0;
}
