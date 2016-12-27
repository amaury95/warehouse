#include <stdio.h>
#include "include/cJSON.h"
#include "include/structs.h"

int main(int argc, char const *argv[]) {
  TRIE *trie = trie_new();
  int a = trie_add(trie, "warehouse", 18293);
  int b = trie_contains(trie, "warehouse");
  void *v = trie_get(trie, "warehouse");

  int a2 = trie_add(trie, "warehous", "subterfuge");
  int b3 = trie_contains(trie, "warehous");
  void *v4 = trie_get(trie, "warehous");
  
  printf("%d %d %d\n", a, b, v);
  printf("%d %d %s\n", a2, b3, v4);

  return 0;
}
