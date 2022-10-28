/*
MIT License

Copyright (c) 2022 Ziad Lteif

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdint.h>
#include <unistd.h>
#include <stddef.h>
#include "hashmap.h"

/*
* @brief Creates a new hashmap of the given size.
* @param size The size of the hashmap
* @return A pointer to the hashmap
*/
hashmap *hashmap_create(int size) {
    hashmap *map = (hashmap *) malloc(sizeof(hashmap));
    if(map == NULL) {
        printf("[hashmap_create] Error allocating memory for hashmap\n");
        return NULL;
    }
    map->entries = (hashmap_entry *) malloc(sizeof(hashmap_entry) * size);
    if(map->entries == NULL) {
        printf("[hashmap_create] Error allocating memory for hashmap entries\n");
        return NULL;
    }
    map->size = size;
    map->count = 0;
    map->error = 0;
    return map;
}
/*
* @brief Destroys a hashmap.
* @param map The hashmap to destroy
*/
void hashmap_destroy(hashmap *map) {
    free(map->entries);
    free(map);
}
/*
* @brief Converts the given key to a hash integer for classic hashmap usage.
* @param key The key string to convert
* @return The hash integer
*/
int hashmap_hash_string(char *str) {
    int hash = HASH_MAGIC;
    int c;
    while ((c = *str++)) {
        hash = ((hash << HASH_BITWISE_MULTIPLIER) + hash) + c;
    }
    return hash;
}   

/*
* @brief Adds a new pair key/value to the given hashmap.
* @param map The hashmap to add the pair to
* @param key The key of the pair
* @param value The value of the pair
*/
void hashmap_put(hashmap *map, char *key, char *value) {
    int hash = hashmap_hash_string(key);
    int index = abs(hash % map->size);
    while (map->entries[index].key != NULL) {
        if (strcmp(map->entries[index].key, key) == 0) {
            map->entries[index].value = value;
            return;
        }
        index = (index + 1) % map->size;
    }
    map->entries[index].key = key;
    map->entries[index].value = value;
    map->count++;
}
/*
* @brief Gets the value of the given key from the given hashmap.
* @param map The hashmap to get the value from
* @param key The key to get the value of
* @return The value of the key or NULL if the key does not exist
*/
char *hashmap_get(hashmap *map, char *key) {
    int hash = hashmap_hash_string(key);
    int index = abs(hash % map->size);
    while (map->entries[index].key != NULL) {

        if (strcmp(map->entries[index].key, key) == 0) {
            return map->entries[index].value;
        }
        index = (index + 1) % map->size;
    }
    return NULL;
}
/*
* @brief Prints all the key/value pairs in the given hashmap.
* @param map The hashmap to print
*/
void hashmap_print(hashmap *map) {
    int i;
    for (i = 0; i < map->size; i++) {
        if (map->entries[i].key != NULL) {
            printf("%s: %s\n", map->entries[i].key, map->entries[i].value);
        }
    }
}
/*
* @brief Removes the given key from the given hashmap.
* @param map The hashmap to remove the key from
* @param key The key to remove
*/
void hashmap_remove(hashmap *map, char *key) {
    int hash = hashmap_hash_string(key);
    int index = hash % map->size;
    while (map->entries[index].key != NULL) {
        if (strcmp(map->entries[index].key, key) == 0) {
            map->entries[index].key = NULL;
            map->entries[index].value = NULL;
            map->count--;
            return;
        }
        index = (index + 1) % map->size;
    }
}

/*
* @brief Prints all the key/value pairs in the given hashmap to a string buffer.
* @param map The hashmap to print
*/
char* hashmap_to_string(hashmap *map) {
    char *str = (char *) malloc(sizeof(char) * (map->count * 5000));
    if(str == NULL) {
        return NULL;
    }
    int i = 0;
    for (int j = 0; j < map->size; j++) {
        if (map->entries[j].key != NULL) {
            sprintf(str + i, "%s=%s\n", map->entries[j].key, map->entries[j].value);
            i += strlen(str + i);
        }
    }
    //remove last newline
    str[i-1] = '\0';
    return str;
}

/*
* @brief Returns the size in bytes of everything in the hashmap.
* @param map The hashmap to get the size of
* @return The size of the hashmap
*/
int hashmap_size(hashmap *map) {
    int size = 0;
    size += sizeof(hashmap_entry);
    for (int i = 0; i < map->size; i++) {
        if (map->entries[i].key != NULL) {
            size += strlen(map->entries[i].key) + 1;
            size += strlen(map->entries[i].value) + 1;
        }
    }
    return size;
}

/*
* @brief Sets the error flag in the hashmap.
* @param map The hashmap to set the error flag in
*/

void hashmap_set_error(hashmap *map, int error) {
    map->error = error;
}

/*
* @brief Returns the error flag in the hashmap.
* @param map The hashmap to get the error flag from
* @return The error flag
*/

int hashmap_get_error(hashmap *map) {
    return map->error;
}
