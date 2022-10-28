/**
 * @file resvars.c
 * @author Ziad Lteif
 * @brief Allow internal variable resolution.
 * All variables are resolved in the order they are defined.
 * @version 1.2
 * @date 2022-06-03
 */

/***********************************************
 * 
 * Exit codes:
 * 
 * 0 - Success
 * 1 - Unknown error
 * 2 - Undefined variable resolved (varA=${varB} but varB is not defined)
 * 3 - Invalid variable name (unmatched brackets ${ without matching })
 * 4 - Malloc failed.
 **********************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "resvars.h"
#define HASHMAP_BUFFER_SIZE 1024
/**
 * @brief Counts the occurrences of substring in a string
 * 
 * @param string The string to search in
 * @param substring The substring to search for
 * @return int The number of occurrences
 */
int substring_count(char* string, char* substring) {
    int i, j, l1, l2;
    int count = 0;
    int found = 0;
    l1 = strlen(string);
    l2 = strlen(substring);
    for(i = 0; i < l1 - l2 + 1; i++) {
      found = 1;
      for(j = 0; j < l2; j++) {
        if(string[i+j] != substring[j]) {
          found = 0;
          break;
        }
      }
      if(found) {
        count++;
        i = i + l2 -1;
      }
    }
    return count;
}

/**
 * @brief Replace all occurrences of a substring in a string
 * 
 * @param s Full string
 * @param oldW Word to replace
 * @param newW Replacement word
 * @return char* New modified string
 */
char* replace_word(const char* s, const char* oldW, const char* newW) {
    char* result;
    int i, cnt = 0;
    int newWlen = strlen(newW);
    int oldWlen = strlen(oldW);
    //printf("[replace_word] s = %s\n", s);
    //printf("[replace_word] oldW = %s\n", oldW);
    //printf("[replace_word] newW = %s\n", newW);
    // Counting the number of times old word
    // occur in the string
    for (i = 0; s[i] != '\0'; i++) {
        if (strstr(&s[i], oldW) == &s[i]) {
            cnt++;
            // Jumping to index after the old word.
            i += oldWlen - 1;
        }
    }
    // Making new string of enough length
    result = (char*)malloc(i + cnt * (newWlen - oldWlen) + 1);
    i = 0;
    while (*s) {
        // compare the substring with the result
        if (strstr(s, oldW) == s) {
            strcpy(&result[i], newW);
            i += newWlen;
            s += oldWlen;
        }
        else
            result[i++] = *s++;
    }
    result[i] = '\0';
    //printf("[DEBUG][replace_word] result = %s\n", result);
    return result;
}

/**
 * @brief Resolve variables by replacing all occurrences of the variable name with its value
 * fetched from the hashmap of variables
 * 
 * @param map The main hashmap of variables
 * @param ptr Pointer to the index of the ${} variable
 * @param value The inital value of the variable to be resolved (ex: ${var})
 * @return char* The resolved value of the variable (ex: 123)
 */
char* resolve_vars(hashmap* map, char* ptr, char* value) {
    //printf("[DEBUG][resolve_vars] ptr = %s\n", ptr);
    while(ptr) {
        char *endptr = strstr(ptr, "}");
        char *var = malloc(endptr - ptr + 1);
        if(!var) {
            printf("[ERROR][resolve_vars] Malloc failed\n");
            return NULL;
        }
        strncpy(var, ptr + 2, endptr - ptr - 2);
        var[endptr - ptr - 2] = '\0';
        //printf("[DEBUG][resolve_vars] var = %s\n", var);
        char *fullvar = malloc(strlen(var) + 4);
        strcpy(fullvar, "${");
        strcat(fullvar, var);
        strcat(fullvar, "}");
        strcat(fullvar, "\0");
        //printf("[DEBUG][resolve_vars] fullvar = %s\n", fullvar);
        char* resolved = hashmap_get(map, var);
        //printf("[DEBUG][resolved_vars] resolved = %s\n", resolved);
        if (resolved == NULL) {
            printf("[ERROR] Undefined variable resolved: %s\n", fullvar);
            return NULL; // Variable not found
        }
        //printf("[DEBUG][resolve_vars] Replacing %s with %s\n", fullvar, resolved);
        value = replace_word(value, fullvar, resolved);
        ptr = strstr(ptr+1, "${");
        free(fullvar);
    }
    //printf("[DEBUG][resolve_vars] value = %s\n", value);
    return value;
}

/**
 * @brief Checks if a string has matched the pattern of a variable definition.
 * A variable is defined by the pattern ${var}. The function check if there is
 * a matching "}" for each "${" in the string.
 * 
 * @param value Value to check for variables
 * @return true if the occurrences of "${" and "}" are equal
 * @return false otherwise
 */
bool check_occurrence(char* value) {
    int occurrences = substring_count(value, "${");
    int closingoccurences = substring_count(value, "}");
    bool occequals = true;
    if (occurrences != closingoccurences) {
        occequals = false;
    }
    return occequals;
}

/**
 * @brief Sanitize the input by stripping newline characters
 * 
 * @param input The input string to sanitize
 * @return char* The sanitized string
 */
char* sanitize_input(char* input) {
    if (input[strlen(input) - 1] == '\n') {
        input[strlen(input) - 1] = '\0';
    }
    return input;
}
/**
 * @brief Build a hashmap of all variables with resolved values.
 * It reads from stdin and follows the following logic:
 * - If the line starts with a '#' it is a comment and is ignored.
 * - If the line contains a '${' it is a variable definition if it contains a matching closing '}'.
 * - If the line starts with a newline or is empty, it is ignored.
 * 
 * @return hashmap* - The built hashmap (or table of variables)
 */
hashmap buildMap(char* str) {
    char *curLine = str;
    size_t len = 0;
    ssize_t read;
    int totalbytes = 0; // Needed for allocation of the precise size of the 
    hashmap *map = hashmap_create(strlen(str) * HASHMAP_BUFFER_SIZE);
    if(!map) {
        printf("[ERROR] Hashmap creation failed\n");
        exit(4);
    }
    while(curLine) {
        printf("[DEBUG] curLine = %s\n", curLine);
        const char * nextLine = strstr(curLine, "\n");
        int curLineLen = nextLine ? (nextLine - curLine) : strlen(curLine);
        totalbytes += curLineLen;
        char* tempStr = (char *)malloc(curLineLen + 1);
        if(tempStr) {
            memcpy(tempStr, curLine, curLineLen);
            tempStr[curLineLen] = '\0';
            if (tempStr[0] != '#' && tempStr[0] != '\n' && tempStr[0] != '\0' && strchr(tempStr, '=') != NULL && tempStr != NULL) {
                char *key = strtok(tempStr, "=");
                char *value = strtok(NULL, "=");
                // If value contains ${}, resolve it and add the rest to the value
                if (!check_occurrence(value)) {
                    printf("[ERROR] Unmatched '${' and '}' in variable definition: %s\n", tempStr);
                    hashmap_set_error(map, 1);
                    return *map; // Variable definition error (missing closing '}')
                }
                char *ptr = strstr(value, "${");
                value = resolve_vars(map, ptr, value);
                if (value == NULL) {
                    printf("[ERROR] Variable resolution failed: %s\n", tempStr);
                    hashmap_set_error(map, 1);
                    return *map; // Variable resolution failed
                }
                // Add key/value pair copy to hashmap
                // remove newline character at end of value
                key = sanitize_input(key);
                value = sanitize_input(value);
                hashmap_put(map, strdup(key), strdup(value));
            }
            free(tempStr);
        } else {
            printf("[ERROR] Malloc failed\n");
            hashmap_set_error(map, 1);
            return *map; // Memory allocation error
        }
        printf("[DEBUG]curLine: %s\n", curLine);
        curLine = nextLine ? (nextLine + 1) : NULL;
    }
    // Add null terminator to the end of the hashmap
    return *map;
}
/**
 * @brief Builds a hashmap of all variables with resolved values from stdin.
 * Transforms the hashmap to a string,
 * 
 * @param argc Number of arguments
 * @param argv Array of arguments
 * @return char* - The built hashmap (or table of variables) in string format
 */
char* resvars(char* str) {
    //printf("[DEBUG] resvars called\n");
    //printf("[DEBUG] str = %s\n", str);
    hashmap map = buildMap(str);
    //printf("[DEBUG] finished building map\n");
    if(map.size == 0) {
        return NULL;
    }
    int err = hashmap_get_error(&map);
    if(err == 1) {
        return NULL;
    }
    char* fullstr = hashmap_to_string(&map);
    //printf("[DEBUG] fullstr = %s\n", fullstr);
    return fullstr;
}

hashmap create_map(char* str) {
    printf("[DEBUG] create_map called with str = %s", str);
    //Check if str has a null terminator
    if(str[strlen(str) - 1] != '\0') {
        // add null terminator
        str[strlen(str)] = '\0';
    }
    hashmap map = buildMap(str);
    printf("[DEBUG] finished building map\n");
    return map;
}
