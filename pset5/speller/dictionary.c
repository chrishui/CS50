// Implements a dictionary's functionality

#include <stdbool.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 60;

// Count numbers of words loaded, initially set to 0
int wordCount = 0;

// List (or bucket) of [N], all poistions initially not pointing to anything
node *list[N] = {NULL};

// 4)
// Returns true if word is in dictionary else false
bool check(const char *word)
{
    unsigned int hashedWord = hash(word);

    // set up node trav, assigning to hashed value list
    node *trav = list[hashedWord];

    while (trav != NULL)
    {
        // Check if current current word matches
        if (strcasecmp(trav->word, word) == 0)
        {
            return true;
            break;
        }

        // Set traverse to the next node
        trav = trav->next;

    }
    // Word not found at end of list
    return false;
}

// 2)
// Hashes word to a number
unsigned int hash(const char *word)
{
    // Assign a number to the first char of buffer from 0-25
    // Source: Daniel J. Bernstein, djb2, http://www.cse.yorku.ca/~oz/hash.html
    unsigned long hash = 5381;
    int c = *word;
    c = tolower(c);
    while (*word != 0)
    {
        hash = ((hash << 5) + hash) + c;
        c = *word++;
        c = tolower(c);
    }
    return hash % N;
}

// 1)
// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Open dictionary file; currently opening >small< dictionary
    FILE *file = fopen(dictionary, "r");

    // Check if able to successfully open file
    if (file == NULL)
    {
        printf("unable to load dictionary");
        fclose(file);
        return false;
    }

     // Allocate memory to read words to
    char word[LENGTH+1];

    // Read strings from file, and stores them in variable >word<, until EOF is encountered
    while (fscanf(file, "%s", word) != EOF)
    {
        // Allocate memory for node
        node *newptr = malloc(sizeof(node));
        if (newptr == NULL)
        {
            fclose(file);
            return false;
        }

        // copy string from word, to node newptr -> word
        strcpy(newptr->word , word);
        newptr->next = NULL;

        // Increase wordcount
        wordCount++;

        // hash the word into an index, using hash function
        unsigned int hashedWord = hash(newptr->word);

        // check for empty list, if yes, assign node newptr to list
        if (list[hashedWord] == NULL)
        {
            list[hashedWord] = newptr;
        }
        // Check for insertion at beginning
        else
        {
            // Insertion at beginning of linked list
             newptr->next = list[hashedWord];
             list[hashedWord] = newptr;
        }
    }

    // Close the dictionary file
    fclose(file);
    return true;
}

// 3)
// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
        return wordCount;
}

// 5)
// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // Loop over buckets
    for ( int i = 0; i < N; i++)
    {
        // set up node trav, assigning to hashed value list
        node *trav_1 = list[i];
        node *trav_2 = trav_1;

        // Check if list has nodes
        if (trav_1 == NULL)
        {
            free(trav_2);
            continue;
        }

        while (trav_2->next != NULL)
        {
            // Assign trav_1 to next node
            trav_1 = trav_1->next;

            // Free trav_2
            free(trav_2);

            // Assign trav_2 to trav_1
            trav_2 = trav_1;
        }
        // Free final node
        free(trav_2);
    }
    return true;
}
