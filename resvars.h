#include "hashmap.h"
int substring_count(char* string, char* substring);
char* replace_word(const char* s, const char* oldW, const char* newW);
char* resolve_vars(hashmap* map, char* ptr, char* value);
bool check_occurrence(char* value);
char* sanitize_input(char* input);
hashmap buildMap(char* str);
char* resvars(char* str);
hashmap create_map(char* str);
