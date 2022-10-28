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
#define HASH_MAGIC 5381
#define HASH_BITWISE_MULTIPLIER 5
#define HASHMAP_LEN 5000000
#define HASHMAP_BUFFER 5000000
// Struct for the hashmap node
typedef struct {
    char *key;
    char *value;
} hashmap_entry;
// Struct for the hashmap itself
typedef struct {
    hashmap_entry *entries;
    int size;
    int count;
    int error;
} hashmap;

// Hashmap functions
hashmap *hashmap_create(int size); // Create a new hashmap
void hashmap_destroy(hashmap *map); // Destroy a hashmap
void hashmap_put(hashmap *map, char *key, char *value); // Put a key/value pair into the hashmap
char *hashmap_get(hashmap *map, char *key); // Get the value from the hashmap for a given key
bool hashmap_contains_key(hashmap *map, char *key); // Check if the hashmap contains a key
void hashmap_remove(hashmap *map, char *key); // Remove a key from the hashmap
void hashmap_print(hashmap *map); // Print the hashmap
char* hashmap_to_string(hashmap *map); // Print the hashmap to a string
int hashmap_size(hashmap *map); // Get the size of the hashmap
void hashmap_set_error(hashmap *map, int error); // Set the error flag for the hashmap
int hashmap_get_error(hashmap *map); // Get the error flag for the hashmap
