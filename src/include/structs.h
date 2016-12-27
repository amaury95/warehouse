#ifndef STRUCTS_H
#define STRUCTS_H

#ifndef TRIE_LEN
#define TRIE_LEN 36
#endif

typedef struct trie_t{
  void *value;
  struct trie_t **children;
}TRIE;

TRIE *trie_new();
char trie_pos(char c);
void *trie_get(TRIE *trie, char *chain);
int trie_contains(TRIE *trie, char *chain);
int trie_add(TRIE *trie, char *chain, void *value);
#endif
