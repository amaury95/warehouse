#include <stdlib.h>
#include <string.h>

#include "include/structs.h"

char trie_pos(char c){
  c = tolower(c);
  return c < 58 ? c - 48 : c < 123 ? c - 87;
}
TRIE *trie_new(){
  TRIE *trie = malloc(sizeof(TRIE));
  TRIE->children = malloc(sizeof(TRIE) * TRIE_LEN);
  return trie;
}

int trie_contains(TRIE *trie, char *chain, int pos, int len){
  if(pos == len)return trie->value != NULL;
  return trie->children[trie_pos(chain[pos])] != NULL &&
         trie_contains(trie->children[trie_pos(chain[pos])], chain, pos + 1, len);
}
int trie_contains(TRIE *trie, char *chain){
  return trie_contains(trie, chain, 0, strlen(chain));
}

int trie_add(TRIE *trie, char *chain, void *value, int pos, int len){
  if(pos == len) return (trie->value = value) != NULL;
  if(trie->children[trie_pos(chain[pos])] == NULL)
    trie->children[trie_pos(chain[pos])] = trie_new();
  return trie_add(trie->children[trie_pos(chain[pos])], chain, value, pos + 1, len);
}
int trie_add(TRIE *trie, char *chain, void *value){
  return trie_add(trie, chain, value, 0, strlen(chain));
}

void *trie_get(TRIE *trie, char *chain, int pos, int len){
  if(pos == len)return trie->value;
  return trie_get(trie->children[trie_pos(chain[pos])], chain, pos + 1, len);
}
void *trie_get(TRIE *trie, char *chain){
  if(trie_contains(trie, chain))
    return trie_get(trie, chain, 0, strlen(chain));
  return NULL;
}
