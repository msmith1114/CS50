/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>

#include "dictionary.h"


//Node Struct Definition
typedef struct node
{
    char word[LENGTH+1]; //+1 accounts for /0 (Null terminator)
    struct node *next;
}node;

node *hashtable[HASHTABLESIZE]; //Array of pointers that point to a node (struct) type


/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{ 
    char new_word[LENGTH+1];
    for(int i = 0; i < LENGTH+1; i++)
        new_word[i] = tolower(word[i]);
        
    int index = hashValue(new_word);
    node *ptr = hashtable[index];
    if(hashtable[index] == NULL)
    {
            return false;
     }
    while(ptr != NULL)
    {
       if(strcmp(ptr->word,new_word) == 0)
       {
            return true;
       }
        ptr = ptr->next;
    }
    return false;
}

/**
 * Hash Function: found at http://cs50.stackexchange.com/questions/19705/how-to-make-the-check-function-faster
 */
int hashValue(const char *word)
{
    unsigned int hash = 0;
    for (int i=0, n=strlen(word); i<n; i++) {
        hash = (hash << 2) ^ word[i];
    }
    return hash % HASHTABLESIZE;
}


/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary) //Works
{
    FILE *file;
    file = fopen (dictionary, "r");
    char word[LENGTH+1];
    int index;
    for(int i = 0;i < HASHTABLESIZE; i++)
    {
        hashtable[i] = NULL;
    }
    
    while(fscanf(file, "%s", word) != EOF)
    {
      node *new_node = malloc(sizeof(node));
      if(new_node == NULL)
      {
          unload();
          return false;
      }
      strcpy(new_node->word, word);   
      index = hashValue(word);
      new_node->next = hashtable[index];
      hashtable[index] = new_node;
      
    }
    //printHash();
    return true;
    
}


/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    node *ptr = NULL;
    int counter = 0;
    for(int i = 0; i < HASHTABLESIZE; i++)
    {
            ptr = hashtable[i];

                while(ptr != NULL)
                {
                    counter++;
                    ptr=ptr->next;
                }
            
    }
    
    return counter;
    if(counter == 0)
    {
        return 0;
    }
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    node *ptr = NULL;
    for(int i = 0; i < HASHTABLESIZE; i++)
    {
            ptr = hashtable[i];

                while(ptr != NULL)
                {
                    node *temp = ptr;
                    ptr=ptr->next;
                    free(temp);
                }
        
    }
    return true;
}

void printHash()
{
    node *ptr = NULL;
    for(int i = 0; i < HASHTABLESIZE; i++)
    {
            ptr = hashtable[i];
            while(ptr != NULL)
            {
                printf("Word:%s  Index:%i\n", ptr->word,i); 
                ptr=ptr->next;
            }
    }
}
