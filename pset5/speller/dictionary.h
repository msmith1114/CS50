/**
 * Declares a dictionary's functionality.
 */

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 45
#define HASHTABLESIZE 65536

/**
 * Returns true if word is in dictionary else false.
 */
int hashValue(const char *word); 


/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word);


/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary);


/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void);

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void);

void printHash();

#endif // DICTIONARY_H
