#include <stdio.h>
#include "include/cJSON.h"
#include "include/structs.h"

int main(int argc, char const *argv[]) {
  TRIE *trie = trie_new();
  int a = trie_add(trie, "warehouse", 18293);
  int b = trie_contains(trie, "warehouse");
  void *v = trie_get(trie, "warehouse");
  printf("%d %d %d\n", a, b, v);

  for(char i = 0; i < 123; i++){
    if(trie_pos(i) != i)
      printf("%c\n", i);
  }
  return 0;
}
