#ifndef C_DATASTRUCTURES_TRIE_H
#define C_DATASTRUCTURES_TRIE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Trie {
    struct Node *root;
} Trie;

Trie *trieInitialization();

void trieAddWord(Trie *trie, char *word);

int trieContains(Trie *trie, char *word);

void trieRemoveWord(Trie *trie, char *word);

struct ArrayList *trieAutoCompletion(Trie *trie, char *word, int numOfSuggestion);

struct ArrayList *trieSuggestion(Trie *trie, char *word, int numOfSuggestion);

void triePrintAllWords(Trie *trie);

void clearTrie(Trie *trie);

void destroyTrie(Trie *trie);

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_TRIE_H