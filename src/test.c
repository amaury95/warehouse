#include <stdio.h>
#include "include/cJSON.h"
#include "include/structs.h"

int main(int argc, char const *argv[]) {
  TRIE *trie = trie_new();
  int a = trie_add(trie, "warehouse", 18293);
  int b = trie_contains(trie, "warehouse");
  void *v = trie_get(trie, "warehouse");
  printf("%d %d %d\n", a, b, v);
  return 0;
}
