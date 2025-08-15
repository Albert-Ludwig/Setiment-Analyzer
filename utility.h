#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define ARRAY_SIZE 10
#define MAX_STRING_LENGTH 50 // Adjust the maximum length of words
#define DEFAULT_SIZE 20
#define MAX_TOKENS 50
#define MAX_INPUT 100

// Structure to store each word's associated sentiment values and its integer array
typedef struct
{
    char word[MAX_STRING_LENGTH]; // The word itself
    float value1;                 // First sentiment value (positive/negative)
    float value2;                 // Second sentiment value (optional, for other metric)
    int intArray[ARRAY_SIZE];     // Integer array for additional data (e.g., related features)
} WordData;

// Function to load and read the Vader lexicon from a file
int readFile(const char *filename, WordData **lexicon);

// Function to parse a sentence into individual tokens, handling punctuation as separate tokens
void tokenizeSentence(char *input, char *tokens[], int maxTokens);

// Function to count the number of words found in the lexicon that match the given tokens
int numLexicons(char *tokens[], WordData *lexicon, int lexiconSize);

// Function to compute the total sentiment score based on a list of tokens
float calculateTotalScore(WordData *lexicon, int lexiconSize, char *tokens[]);

// Function to calculate the average sentiment score for the given score and lexicon size
float computeAverageScore(float score, int numLexicons);

// Function to compute the compound sentiment score, adjusting the total score for a more normalized result
float computeCompoundScore(float score);
