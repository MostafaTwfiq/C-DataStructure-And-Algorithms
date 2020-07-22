#include "../Headers/HashMap.h"
#include "../../../System/Utils.h"
#include <stdio.h>
#include <stdlib.h>
#include "limits.h"
#include <string.h>



unsigned int fHashCal(unsigned int address, unsigned int sizeOfKey, unsigned int length);


unsigned int sHashCal(unsigned int address, unsigned int sizeOfKey, unsigned int bPrime);


unsigned int calIndex(unsigned int fHash, unsigned int sHash, unsigned int index, unsigned int length);


int calBPrime(int length);


int getNextPrime(int num);







/** The freeing entry function.
 * @param entry the entry address.
 */

void freeEntry(void *entry) {
    Entry *entryToFree = (Entry *) entry;
    entryToFree->freeItemFun(entryToFree->item);
    free(entryToFree->item);
    entryToFree->freeKeyFun(entryToFree->key);
    free(entryToFree);

}








/** This function will take the freeing item function address, the freeing key function address, and the key comparator function as a parameters,
   then it will initialize a new hash map,
   then the function will return the address of the hash map.
 * @param freeKey the freeing key function address that will be called to free the items keys
 * @param freeItem the freeing item function address that will be called to free the hash map items
 * @param keyComp the function that will be called to compare the keys
 * @return it will return the new initialized hash map address
 */

HashMap *hashMapInitialization(
        void (*freeKey)(void *)
        , void (*freeItem)(void *)
        , int (*keyComp)(const void *, const void *)
        ) {

    if (freeKey == NULL) {
        fprintf(stderr, INVALID_ARG_MESSAGE, "free key function pointer", "hash map data structure");
        #ifdef CU_TEST_H
     		DUMMY_TEST_DATASTRUCTURE->errorCode = INVALID_ARG;
     	#else
     		exit(INVALID_ARG);
     	#endif

    } else if (freeItem == NULL) {
        fprintf(stderr, INVALID_ARG_MESSAGE, "free item function pointer", "hash map data structure");
        #ifdef CU_TEST_H
     		DUMMY_TEST_DATASTRUCTURE->errorCode = INVALID_ARG;
     	#else
     		exit(INVALID_ARG);
     	#endif

    } else if (keyComp == NULL) {
        fprintf(stderr, INVALID_ARG_MESSAGE, "key comparator function pointer", "hash map data structure");
        #ifdef CU_TEST_H
     		DUMMY_TEST_DATASTRUCTURE->errorCode = INVALID_ARG;
     	#else
     		exit(INVALID_ARG);
     	#endif

    }

    HashMap *map = (HashMap *) malloc(sizeof(HashMap));
    if (map == NULL) {
        fprintf(stderr, FAILED_ALLOCATION_MESSAGE, "hash map", "hash map data structure");
        #ifdef CU_TEST_H
     		DUMMY_TEST_DATASTRUCTURE->errorCode = FAILED_ALLOCATION;
     	#else
     		exit(FAILED_ALLOCATION);
     	#endif

    }

    map->length = getNextPrime(10); //the length of the map array should always be a prime number.
    map->arr = (Entry **) calloc(sizeof(Entry *), map->length);
    if (map == NULL) {
        fprintf(stderr, FAILED_ALLOCATION_MESSAGE, "hash map entries array", "hash map data structure");
        #ifdef CU_TEST_H
     		DUMMY_TEST_DATASTRUCTURE->errorCode = FAILED_ALLOCATION;
     	#else
     		exit(FAILED_ALLOCATION);
     	#endif

    }

    map->count = 0;
    map->bPrime = calBPrime(map->length);
    map->freeItemFun = freeItem;
    map->freeKeyFun = freeKey;
    map->keyComp = keyComp;

    return map;

}





/** This function will take the map address, the key address, the size of the key in bytes, and the item address as a parameters,
 * then it will insert the item in the map.
 * Note: if the key is already in the map then the map will override the data and free the old item and it's key.
 * @param map the hash map address
 * @param key the key address
 * @param sizeOfKey the size of the key in bytes
 * @param item the item address
 */

void hashMapInsert(HashMap *map, void *key, int sizeOfKey, void *item) {
    if (map == NULL) {
        fprintf(stderr, NULL_POINTER_MESSAGE, "hash map pointer", "hash map data structure");
        #ifdef CU_TEST_H
     		DUMMY_TEST_DATASTRUCTURE->errorCode = NULL_POINTER;
     	#else
     		exit(NULL_POINTER);
     	#endif

    } else if (key == NULL) {
        fprintf(stderr, INVALID_ARG_MESSAGE, "key pointer", "hash map data structure");
        #ifdef CU_TEST_H
     		DUMMY_TEST_DATASTRUCTURE->errorCode = INVALID_ARG;
     	#else
     		exit(INVALID_ARG);
     	#endif

    } else if (item == NULL) {
        fprintf(stderr, INVALID_ARG_MESSAGE, "item pointer", "hash map data structure");
        #ifdef CU_TEST_H
     		DUMMY_TEST_DATASTRUCTURE->errorCode = INVALID_ARG;
     	#else
     		exit(INVALID_ARG);
     	#endif

    }

    if (map->count == map->length) {
        map->length = getNextPrime(map->length * 2); //the length of the map array should always be a prime number.
        map->arr = (Entry **) realloc(map->arr, sizeof(Entry *) * map->length);
        if (map->arr == NULL) {
            fprintf(stderr, FAILED_REALLOCATION_MESSAGE, "entries array", "hash map data structure");
            #ifdef CU_TEST_H
     		DUMMY_TEST_DATASTRUCTURE->errorCode = FAILED_REALLOCATION;
     	#else
     		exit(FAILED_REALLOCATION);
     	#endif

        }

        for (int i = map->count; i < map->length; i++)
            map->arr[i] = NULL;

        map->bPrime = calBPrime(map->length);
    }

    unsigned int
            fHash = fHashCal( (unsigned int) key, sizeOfKey, map->length)
            , sHash = sHashCal( (unsigned int) key, sizeOfKey, map->bPrime);

    unsigned int pHashIndex = 1;
    unsigned int index = calIndex(fHash, sHash, pHashIndex, map->length);

    while (map->arr[index] != NULL) {

        if (map->keyComp(key, map->arr[index]->key) == 0) {
            map->freeItemFun(map->arr[index]->item);
            map->freeKeyFun(map->arr[index]->key);
            map->arr[index]->key = key;
            map->arr[index]->item = item;
            return;
        }

        pHashIndex++;
        index = calIndex(fHash, sHash, pHashIndex, map->length);

    }

    Entry *newEntry = (Entry *) malloc(sizeof(Entry));
    if (newEntry == NULL) {
        fprintf(stderr, FAILED_ALLOCATION_MESSAGE, "new entry", "hash map data structure");
        #ifdef CU_TEST_H
     		DUMMY_TEST_DATASTRUCTURE->errorCode = FAILED_ALLOCATION;
     	#else
     		exit(FAILED_ALLOCATION);
     	#endif

    }

    newEntry->key = key;
    newEntry->item = item;
    newEntry->freeItemFun = map->freeItemFun;
    newEntry->freeKeyFun = map->freeKeyFun;
    map->arr[index] = newEntry;
    map->count++;

}






/** This function will take the map address, the key address, and the key size as a parameters,
    then it will return (1) if the key is in the map,
    other wise it will return zero (0).
 * Note: this function will not free the passed key.
 * @param map the hash map address
 * @param key the key address
 * @param sizeOfKey the size of the key in bytes
 * @return it will return one if the provided key is in the hash map, other wise it will return zero
 */

int hashMapContains(HashMap *map, void *key, int sizeOfKey) {
    if (map == NULL) {
        fprintf(stderr, NULL_POINTER_MESSAGE, "hash map pointer", "hash map data structure");
        #ifdef CU_TEST_H
     		DUMMY_TEST_DATASTRUCTURE->errorCode = NULL_POINTER;
     	#else
     		exit(NULL_POINTER);
     	#endif

    } else if (key == NULL) {
        fprintf(stderr, INVALID_ARG_MESSAGE, "key pointer", "hash map data structure");
        #ifdef CU_TEST_H
     		DUMMY_TEST_DATASTRUCTURE->errorCode = INVALID_ARG;
     	#else
     		exit(INVALID_ARG);
     	#endif

    }

    unsigned int
            fHash = fHashCal( (unsigned int) key, sizeOfKey, map->length)
    , sHash = sHashCal( (unsigned int) key, sizeOfKey, map->bPrime);

    unsigned int pHashIndex = 1;
    unsigned int index = calIndex(fHash, sHash, pHashIndex, map->length);
    unsigned int firstIndex = index;

    do {

        if (map->arr[index] != NULL) {
            if (map->keyComp(key, map->arr[index]->key) == 0)
                return 1;

        }

        pHashIndex++;
        index = calIndex(fHash, sHash, pHashIndex, map->length);

    } while (firstIndex != index);

    return 0;

}





/** This function will take the map address, the key address, and the key size as a parameter,
    then it will return the item address if the key existed,
    other wise it will return NULL.
 * Note: this function will not free the passed key
 * @param map the hash map address
 * @param key the key address
 * @param sizeOfKey the size of the key in bytes
 * @return it will return the item with the provided key if found other wise it will return NULL
 */

void *hashMapGet(HashMap *map, void *key, int sizeOfKey) {
    if (map == NULL) {
        fprintf(stderr, NULL_POINTER_MESSAGE, "hash map pointer", "hash map data structure");
        #ifdef CU_TEST_H
     		DUMMY_TEST_DATASTRUCTURE->errorCode = NULL_POINTER;
     	#else
     		exit(NULL_POINTER);
     	#endif

    } else if (key == NULL) {
        fprintf(stderr, INVALID_ARG_MESSAGE, "key pointer", "hash map data structure");
        #ifdef CU_TEST_H
     		DUMMY_TEST_DATASTRUCTURE->errorCode = INVALID_ARG;
     	#else
     		exit(INVALID_ARG);
     	#endif

    }

    unsigned int
            fHash = fHashCal( (unsigned int) key, sizeOfKey, map->length)
    , sHash = sHashCal( (unsigned int) key, sizeOfKey, map->bPrime);

    unsigned int pHashIndex = 1;
    unsigned int index = calIndex(fHash, sHash, pHashIndex, map->length);
    unsigned int firstIndex = index;

    do {

        if (map->arr[index] != NULL) {
            if (map->keyComp(key, map->arr[index]->key) == 0)
                return map->arr[index]->item;

        }

        pHashIndex++;
        index = calIndex(fHash, sHash, pHashIndex, map->length);

    } while (firstIndex != index);

    return NULL;

}






/** This function will take the map address, the key address, and the key size as a parameter,
 * then it will return the key address if the key existed,
 * other wise it will return NULL.
 * Note: this function will not free the passed key
 * @param map the hash map address
 * @param key the key address
 * @param sizeOfKey the size of the key in bytes
 * @return it will return the item with the provided key if found other wise it will return NULL
 */

void *hashMapGetKey(HashMap *map, void *key, int sizeOfKey) {
    if (map == NULL) {
        fprintf(stderr, NULL_POINTER_MESSAGE, "hash map pointer", "hash map data structure");
        #ifdef CU_TEST_H
     		DUMMY_TEST_DATASTRUCTURE->errorCode = NULL_POINTER;
     	#else
     		exit(NULL_POINTER);
     	#endif

    } else if (key == NULL) {
        fprintf(stderr, INVALID_ARG_MESSAGE, "key pointer", "hash map data structure");
        #ifdef CU_TEST_H
     		DUMMY_TEST_DATASTRUCTURE->errorCode = INVALID_ARG;
     	#else
     		exit(INVALID_ARG);
     	#endif

    }

    unsigned int
            fHash = fHashCal( (unsigned int) key, sizeOfKey, map->length)
    , sHash = sHashCal( (unsigned int) key, sizeOfKey, map->bPrime);

    unsigned int pHashIndex = 1;
    unsigned int index = calIndex(fHash, sHash, pHashIndex, map->length);
    unsigned int firstIndex = index;

    do {

        if (map->arr[index] != NULL) {
            if (map->keyComp(key, map->arr[index]->key) == 0)
                return map->arr[index]->key;

        }

        pHashIndex++;
        index = calIndex(fHash, sHash, pHashIndex, map->length);

    } while (firstIndex != index);

    return NULL;

}





/** This function will take the map address, the key address, and the size of the key as a parameters,
    then it will delete and free the key and the item that linked to the key.
 * Note: if the key didn't found in the hash map, then the function will fo nothing.
 * @param map the hash map address
 * @param key the key address
 * @param sizeOfKey the size of the key in bytes
 */

void hashMapDelete(HashMap *map, void *key, int sizeOfKey) {
    if (map == NULL) {
        fprintf(stderr, NULL_POINTER_MESSAGE, "hash map pointer", "hash map data structure");
        #ifdef CU_TEST_H
     		DUMMY_TEST_DATASTRUCTURE->errorCode = NULL_POINTER;
     	#else
     		exit(NULL_POINTER);
     	#endif

    } else if (key == NULL) {
        fprintf(stderr, INVALID_ARG_MESSAGE, "key pointer", "hash map data structure");
        #ifdef CU_TEST_H
     		DUMMY_TEST_DATASTRUCTURE->errorCode = INVALID_ARG;
     	#else
     		exit(INVALID_ARG);
     	#endif

    }

    unsigned int
            fHash = fHashCal( (unsigned int) key, sizeOfKey, map->length)
    , sHash = sHashCal( (unsigned int) key, sizeOfKey, map->bPrime);

    unsigned int pHashIndex = 1;
    unsigned int index = calIndex(fHash, sHash, pHashIndex, map->length);
    unsigned int firstIndex = index;

    do {

        if (map->arr[index] != NULL) {
            if (map->keyComp(key, map->arr[index]->key) == 0) {
                freeEntry(map->arr[index]);
                map->arr[index] = NULL;
                map->count--;
                return;
            }

        }

        pHashIndex++;
        index = calIndex(fHash, sHash, pHashIndex, map->length);

    } while (firstIndex != index);

}










/** This function will take the map address, the key address, and the size of the key as a parameters,
    then it will delete and free the key without freeing the item that linked to that key.
 * Note: if the key didn't found in the hash map, then the function will do nothing.
 * @param map the hash map address
 * @param key the key address
 * @param sizeOfKey the size of the key in bytes
 * @return it will return the deleted item pointer if found, other wise it will return NULL
 */

void *hashMapDeleteWtoFr(HashMap *map, void *key, int sizeOfKey) {
    if (map == NULL) {
        fprintf(stderr, NULL_POINTER_MESSAGE, "hash map pointer", "hash map data structure");
        #ifdef CU_TEST_H
     		DUMMY_TEST_DATASTRUCTURE->errorCode = NULL_POINTER;
     	#else
     		exit(NULL_POINTER);
     	#endif

    } else if (key == NULL) {
        fprintf(stderr, INVALID_ARG_MESSAGE, "key pointer", "hash map data structure");
        #ifdef CU_TEST_H
     		DUMMY_TEST_DATASTRUCTURE->errorCode = INVALID_ARG;
     	#else
     		exit(INVALID_ARG);
     	#endif

    }

    unsigned int
            fHash = fHashCal( (unsigned int) key, sizeOfKey, map->length)
    , sHash = sHashCal( (unsigned int) key, sizeOfKey, map->bPrime);

    unsigned int pHashIndex = 1;
    unsigned int index = calIndex(fHash, sHash, pHashIndex, map->length);
    unsigned int firstIndex = index;

    do {

        if (map->arr[index] != NULL) {
            if (map->keyComp(key, map->arr[index]->key) == 0) {
                void *returnItem = map->arr[index]->item;
                map->freeKeyFun(map->arr[index]->key);
                free(map->arr[index]);
                map->arr[index] = NULL;
                map->count--;
                return returnItem;
            }

        }

        pHashIndex++;
        index = calIndex(fHash, sHash, pHashIndex, map->length);

    } while (firstIndex != index);

    return NULL;

}





/** This function will take the map address, the key address, and the size of the key as a parameters,
    then it will delete the entry without freeing the key and the item that linked to that key.
 * Note: if the key didn't found in the hash map, then the function will do nothing.
 * @param map the hash map address
 * @param key the key address
 * @param sizeOfKey the size of the key in bytes
 * @return it will return the entry pointer if found, other wise it will return NULL
 */

Entry *hashMapDeleteWtoFrAll(HashMap *map, void *key, int sizeOfKey) {
    if (map == NULL) {
        fprintf(stderr, NULL_POINTER_MESSAGE, "hash map pointer", "hash map data structure");
        #ifdef CU_TEST_H
     		DUMMY_TEST_DATASTRUCTURE->errorCode = NULL_POINTER;
     	#else
     		exit(NULL_POINTER);
     	#endif

    } else if (key == NULL) {
        fprintf(stderr, INVALID_ARG_MESSAGE, "key pointer", "hash map data structure");
        #ifdef CU_TEST_H
     		DUMMY_TEST_DATASTRUCTURE->errorCode = INVALID_ARG;
     	#else
     		exit(INVALID_ARG);
     	#endif

    }

    unsigned int
            fHash = fHashCal( (unsigned int) key, sizeOfKey, map->length)
    , sHash = sHashCal( (unsigned int) key, sizeOfKey, map->bPrime);

    unsigned int pHashIndex = 1;
    unsigned int index = calIndex(fHash, sHash, pHashIndex, map->length);
    unsigned int firstIndex = index;

    do {

        if (map->arr[index] != NULL) {
            if (map->keyComp(key, map->arr[index]->key) == 0) {
                Entry *returnEntry = map->arr[index];
                map->arr[index] = NULL;
                map->count--;
                return returnEntry;
            }

        }

        pHashIndex++;
        index = calIndex(fHash, sHash, pHashIndex, map->length);

    } while (firstIndex != index);

    return NULL;

}






/** This function will take the map address as a parameter,
    then it will return double void pointer that has a copy of all the items in the map.
 * @param map the hash map address
 * @return it will return a double void array that contains a copy of the hash map items
 */

void **hashMapToArray(HashMap *map) {
    if (map == NULL) {
        fprintf(stderr, NULL_POINTER_MESSAGE, "hash map pointer", "hash map data structure");
        #ifdef CU_TEST_H
     		DUMMY_TEST_DATASTRUCTURE->errorCode = NULL_POINTER;
     	#else
     		exit(NULL_POINTER);
     	#endif

    }

    void **arr = (void **) malloc(sizeof(void *) * map->count);
    if (arr == NULL) {
        fprintf(stderr, FAILED_ALLOCATION_MESSAGE, "to array", "hash map data structure");
        #ifdef CU_TEST_H
     		DUMMY_TEST_DATASTRUCTURE->errorCode = FAILED_ALLOCATION;
     	#else
     		exit(FAILED_ALLOCATION);
     	#endif

    }

    for (int i = 0, index = 0; i < map->length; i++) {

        if (map->arr[i] != NULL)
            arr[index++] = map->arr[i]->item;

    }

    return arr;

}



/** This function will take the map address as a parameter,
    then it will return an entry array that contains a copy of all the items and it's key in the map.
 * Note: the returned Entries will not be a really copy of the keys and items, it will reference to the same addresses
 * @param map the hash map address
 * @return it will return a double Entry array that contains all the hash map entries
 */

Entry **hashMapToEntryArray(HashMap *map) {
    if (map == NULL) {
        fprintf(stderr, NULL_POINTER_MESSAGE, "hash map pointer", "hash map data structure");
        #ifdef CU_TEST_H
     		DUMMY_TEST_DATASTRUCTURE->errorCode = NULL_POINTER;
     	#else
     		exit(NULL_POINTER);
     	#endif

    }

    Entry **arr = (Entry **) malloc(sizeof(Entry *) * map->count);
    if (arr == NULL) {
        fprintf(stderr, FAILED_ALLOCATION_MESSAGE, "to entries array", "hash map data structure");
        #ifdef CU_TEST_H
     		DUMMY_TEST_DATASTRUCTURE->errorCode = FAILED_ALLOCATION;
     	#else
     		exit(FAILED_ALLOCATION);
     	#endif

    }

    for (int i = 0, index = 0; i < map->length; i++) {
        if (map->arr[i] != NULL)
            arr[index++] = map->arr[i];

    }

    return arr;

}




/** This function will take the map address as a parameter,
    then it will return the number of items in the map.
 * @param map the hash map address
 * @return it will return the number of entries (items) in the hash map
 */

int hashMapGetLength(HashMap *map) {
    if (map == NULL) {
        fprintf(stderr, NULL_POINTER_MESSAGE, "hash map pointer", "hash map data structure");
        #ifdef CU_TEST_H
     		DUMMY_TEST_DATASTRUCTURE->errorCode = NULL_POINTER;
     	#else
     		exit(NULL_POINTER);
     	#endif

    }

    return map->count;
}





/** This function will take the map address as a parameter,
    then it will return one (1) if the map is empty,
    other wise it will return zero (0).
 * @param map the hash map address
 * @return it will return one if the hash map is empty, other wise it will return zero
 */

int hashMapIsEmpty(HashMap *map) {
    if (map == NULL) {
        fprintf(stderr, NULL_POINTER_MESSAGE, "hash map pointer", "hash map data structure");
        #ifdef CU_TEST_H
     		DUMMY_TEST_DATASTRUCTURE->errorCode = NULL_POINTER;
     	#else
     		exit(NULL_POINTER);
     	#endif

    }

    return map->count == 0;
}





/** This function will take the map address as a parameter,
 * then it will clear and free all the items and it's key from the map without freeing the map.
 * @param map the hash map address
 */

void clearHashMap(HashMap *map) {
    if (map == NULL) {
        fprintf(stderr, NULL_POINTER_MESSAGE, "hash map pointer", "hash map data structure");
        #ifdef CU_TEST_H
     		DUMMY_TEST_DATASTRUCTURE->errorCode = NULL_POINTER;
     	#else
     		exit(NULL_POINTER);
     	#endif

    }

    for (int i = 0; i < map->length; i++) {
        if (map->arr[i] != NULL) {
            freeEntry(map->arr[i]);
            map->arr[i] = NULL;
        }

    }

    map->count = 0;

}





/** This function will take the map address as a parameter,
    then it will destroy and free the map and all it's entries.
 * @param map the hash map address
 */

void destroyHashMap(HashMap *map) {
    clearHashMap(map);
    free(map->arr);
    free(map);

}





/** This function will take the key address as an integer, the size of the key and the length of the map,
    as a parameters, then it will return the first hash of this key.
 * Note: this function should only be called from the hash map functions.
 * @param address the address of the key represented as an unsigned integer
 * @param sizeOfKey the size of the key in bytes
 * @param length the length of the hash map array
 * @return it will return the first hashed key
 */

unsigned int fHashCal(unsigned int address, unsigned int sizeOfKey, unsigned int length) {
    unsigned int *key = (int *) calloc(sizeof(int), 1);
    memcpy(key, (void *) address, sizeOfKey > 4 ? 4 : sizeOfKey);
    unsigned int hash = (*key % length);
    free(key);
    return hash;
}





/** This function will take the key address as an integer, the size of the key and the biggest prime number,
    that smaller than the map array length as a parameters, then it will return the second hash of this key.
 * Note: this function should only be called from the hash map functions.
 * @param address the address of the key represented as an unsigned integer
 * @param sizeOfKey the size of the key in bytes
 * @param length the length of the hash map array
 * @return it will return the second hashed key
 */

unsigned int sHashCal(unsigned int address, unsigned int sizeOfKey, unsigned int bPrime) {
    unsigned int *key = (int *) calloc(sizeof(int), 1);
    memcpy(key, (void *) address, sizeOfKey > 4 ? 4 : sizeOfKey);
    unsigned int hash = (bPrime - (*key % bPrime));
    free(key);
    return hash;

}





/** This function will take the first hash of the key, the second hash of the key, the index, and the length of the map array
    as a parameters, then it will return the next index that should be available.
 * Note: this function should only be called from the hash map functions.
 * @param fHash the first hashed key
 * @param sHash the second hashed key
 * @param index the current index in the array that wasn't empty
 * @param length the length of the hash map array
 * @return it will return the next index that should be empty
 */

unsigned int calIndex(unsigned int fHash, unsigned int sHash, unsigned int index, unsigned int length) {
    return ((fHash + (index * sHash))  % length);
}




/** This function will take the length of the map array as a parameter,
    then it will return the biggest prime number that is smaller than the length.
 * @param length the length of the hash map array
 * @return it will return the biggest prime number that is smaller than the length
 */

int calBPrime(int length) {
    for (int i = length - 1; i >= 1; i--) {
        for (int j = i / 2; j > 1; j--) {
            if (i % j == 0)
                break;
            else if (i % j != 0 && j == 2)
                return i;

        }
    }

    return 1;

}




/** This function will take an integer number as a parameter,
    then it will return the next prime number that bigger than or equal to the passed parameter,
    other wise it will return 1 if the function didn't found any prime numbers.
 * @param num the number that the function will start to check from
 * @return it will return the first prime number that is bigger than or equal to the provided number, and if
 * it didn't found any number smaller that or equal to the MAX_INT, then the function will return one
 */

int getNextPrime(int num) {
    for (int i = num; i <= INT_MAX; i++) {
        for (int j = i / 2; j > 1; j--) {

            if (i % j == 0)
                break;
            else if (j == 2 && i % j != 0)
                return i;

        }

    }

    return 1;

}