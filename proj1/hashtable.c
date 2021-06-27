#include "hashtable.h"
#include <stdlib.h>

/*
 * This creates a new hash table of the specified size and with
 * the given hash function and comparison function.
 */
HashTable *createHashTable(int size, unsigned int (*hashFunction)(void *),
                           int (*equalFunction)(void *, void *)) {
  int i = 0;
  HashTable *newTable = malloc(sizeof(HashTable));//null?
  newTable->size = size;//number of buckets
  newTable->data = malloc(sizeof(struct HashBucket *) * size);//null?
  for (i = 0; i < size; ++i) {
    newTable->data[i] = NULL;//init
  }
  newTable->hashFunction = hashFunction;
  newTable->equalFunction = equalFunction;
  return newTable;
}

unsigned int hash(HashTable* table, void* key) {
  return table->hashFunction(key) % table->size;
}

/*
 * This inserts a key/data pair into a hash table.  To use this
 * to store strings, simply cast the char * to a void * (e.g., to store
 * the string referred to by the declaration char *string, you would
 * call insertData(someHashTable, (void *) string, (void *) string).
 * Because we only need a set data structure for this spell checker,
 * we can use the string as both the key and data.
 */
void insertData(HashTable *table, void *key, void *data) {
  // -- TODO --
  // HINT:
  // 1. Find the right hash bucket location with table->hashFunction.
  // 2. Allocate a new hash bucket struct.
  // 3. Append to the linked list or create it if it does not yet exist.
  unsigned int location = hash(table, key);
  //fprintf(stderr, "\"%s\" was inserted at %d\n", (char*)key, location);
  HashBucket *newBucket = malloc(sizeof(HashBucket));
  newBucket->key = key;
  newBucket->data = data;
  newBucket->next = NULL;
  if (!((table->data)[location])) {
    (table->data)[location] = newBucket;
  } else {
    HashBucket *predecessor = (table->data)[location];
    while(predecessor->next) {
      predecessor = predecessor->next;
    }
    predecessor->next = newBucket;
  }
}

/*
 * This returns the corresponding data for a given key.
 * It returns NULL if the key is not found. 
 */
void *findData(HashTable *table, void *k) {
  // -- TODO --
  // HINT:
  // 1. Find the right hash bucket with table->hashFunction.
  // 2. Walk the linked list and check for equality with table->equalFunction.
  //fprintf(stderr, "[checking: %s]\n", (char*)key);
  //char* k = (char*)key;
  unsigned int location = hash(table, k);
  if ((!(table->data)[location])) {
    return NULL;
  } else {
    HashBucket *node = (table->data)[location];
    //fprintf(stderr, "in node %s\n", (char*)node->key);
    do {
      //fprintf(stderr, "finding %s...%s, %s\n", (char*)key, (char*)node->key, (char*)node->data);
      if ((table->equalFunction)(node->key, k)) {
        //fprintf(stderr, "finally find %s in node %s\n", (char*)k, (char*)node->key);
        return node->data;
      }
      node = node->next;
    } while (node);
    //fprintf(stderr, "cannot find %s\n", (char*)key);
    return NULL;
  }
}
