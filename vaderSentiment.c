#include "utility.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define ALPHA 15

int readFile(const char *filename, WordData **lexicon) // Reads a file and stores its contents in an array of WordData structures
{
    FILE *file = fopen(filename, "r");
    if (file == NULL) // check for fail
    {
        printf("Error: File not found\n");
        return -1;
    }

    int arraySize = DEFAULT_SIZE;
    int i = 0;
    // Read data from the file into the WordData structure
    while (fscanf(file, "%49[^\t]\t%f\t%f\t[%d, %d, %d, %d, %d, %d, %d, %d, %d, %d]\n",
                  (*lexicon)[i].word, &(*lexicon)[i].value1, &(*lexicon)[i].value2,
                  &(*lexicon)[i].intArray[0], &(*lexicon)[i].intArray[1], &(*lexicon)[i].intArray[2],
                  &(*lexicon)[i].intArray[3], &(*lexicon)[i].intArray[4], &(*lexicon)[i].intArray[5],
                  &(*lexicon)[i].intArray[6], &(*lexicon)[i].intArray[7], &(*lexicon)[i].intArray[8],
                  &(*lexicon)[i].intArray[9]) != EOF)
    {
        i++;
        if (i >= arraySize) // Check if array needs resizing
        {
            arraySize += 20;                                                              // Increase array size
            WordData *temp = (WordData *)realloc(*lexicon, arraySize * sizeof(WordData)); // reassign the size
            if (temp == NULL)
            {
                printf("Error: Memory allocation failed\n"); // check for fail memory allocation
                fclose(file);
                return -1;
            }
            *lexicon = temp; // Update lexicon pointer
        }
    }
    fclose(file);

    return i; // Number of items read
}

// Splits an input sentence into an array of words and punctuation
void tokenizeSentence(char *input, char *tokens[], int maxTokens)
{
    int i = 0;   // Token index
    int pos = 0; // Current position in input
    while (input[pos] != '\0' && i < maxTokens - 1)
    {
        // Skip leading whitespace
        while (isspace(input[pos]))
            pos++;

        // Check if the character is a sequence of punctuation (excluding single quotes)
        if (ispunct(input[pos]) && input[pos] != '\'')
        {
            int start = pos;
            // Collect consecutive punctuation characters
            while (ispunct(input[pos]) && input[pos] != '\'')
                pos++;
            int length = pos - start;
            tokens[i] = strndup(input + start, length); // Copy punctuation as a token
            i++;
        }
        // Otherwise, collect a regular word (including single quotes)
        else if (isalnum(input[pos]) || input[pos] == '\'')
        {
            int start = pos;
            // Collect characters until a space or punctuation is found
            while (input[pos] != '\0' && !isspace(input[pos]) &&
                   (isalnum(input[pos]) || input[pos] == '\''))
                pos++;
            int length = pos - start;
            tokens[i] = strndup(input + start, length); // Copy word as a token
            i++;
        }
    }

    tokens[i] = NULL; // End the token array with NULL
}

static int isIntensifier(char *token)
{
    const char *positiveIntensifiers[] = {
        "very",
        "extremely",
        "incredibly",
        "so",
        "totally",
        "completely",
        "remarkably",
        "really",
        "particularly"};
    const char *negativeIntensifiers[] = {
        "barely",
        "hardly",
        "scarcely",
        "somewhat",
        "mildly",
        "slightly",
        "partially",
        "fairly",
        "pretty much"};

    int numPositiveIntensifiers = 9;
    int numNegativeIntensifiers = 9;
    // Check if the token is a positive intensifier
    for (int i = 0; i < numPositiveIntensifiers; i++)
    {
        if (strcasecmp(token, positiveIntensifiers[i]) == 0)
        {
            return 1; // Return 1 for positive intensifier
        }
    }
    // Check if the token is a negative intensifier
    for (int i = 0; i < numNegativeIntensifiers; i++)
    {
        if (strcasecmp(token, negativeIntensifiers[i]) == 0)
        {
            return -1; // Return -1 for negative intensifier
        }
    }

    return 0; // Return 0 if not an intensifier
}
// Counts the number of matching words in the input tokens found in the lexicon
int numLexicons(char *token[], WordData *lexicon, int lexiconSize)
{
    int count = 0;

    for (int i = 0; token[i] != NULL; i++)
    {
        for (int j = 0; j < lexiconSize; j++)
        {
            if (strcasecmp(token[i], lexicon[j].word) == 0)
            {
                count++;
                break; // Stop checking once a match is found
            }
        }
    }

    return count;
}
// Checks if a word is a negation
static int isNegation(char *word)
{
    const char *negations[] = {
        "not",
        "isn't",
        "doesn't",
        "wasn't",
        "shouldn't",
        "won't",
        "cannot",
        "can't",
        "nor",
        "neither",
        "without",
        "lack",
        "missing"};

    int numNegations = 13;

    for (int i = 0; i < numNegations; i++)
    {
        if (strcasecmp(word, negations[i]) == 0)
        {
            return 1; // Return 1 for negation
        }
    }
    return 0; // Return 0 if not a negation
}
// Calculates the boost in sentiment score from exclamation marks
static float getExclamationBoost(char *token)
{
    int count = 0;
    while (token[count] == '!')
    {
        count++;
    }
    if (count > 3)
        return pow(1 + 0.292, 3); // Cap the boost at 3 exclamation marks
    else
        return pow(1 + 0.292, count); // Slightly increase sentiment for each exclamation mark
}
// Calculates the penalty to the sentiment score from question marks
static float getQuestionMarkPenalty(char *token)
{
    int count = 0;
    while (token[count] == '?')
    {
        count++;
    }
    if (count > 3)
        return pow(1 - 0.293, 3); // More than 3 question marks doesn't decrease the sentiment further
    else if (count == 1)          // single question mark is neutral
        return 1;
    else
        return pow(1 - 0.29, count); // Each question mark decreases the sentiment slightly
}

float calculateTotalScore(WordData *lexicon, int lexiconSize, char *token[])
{
    float score = 0;
    float factor = 1; // The multiplier to adjust sentiment (e.g., due to intensifiers or negations)
    int negate = 0;   // Flag to track whether negation is active

    for (int i = 0; token[i] != NULL; i++)
    {
        int isAllCaps = 1;

        for (int k = 0; token[i][k] != '\0'; k++)
        {
            if (!isupper(token[i][k]))
            {
                isAllCaps = 0;
                break;
            }
        }

        // Check for negation (if negation is found, it inverts the sentiment)
        if (isNegation(token[i]))
        {
            negate = 1;
            continue;
        }

        if (isIntensifier(token[i]) == 1)
        {
            factor *= (1 + 0.293);
            continue;
        }
        else if (isIntensifier(token[i]) == -1)
        {
            factor *= (1 - 0.293);
            continue;
        }

        for (int j = 0; j < lexiconSize; j++)
        {
            if (strcasecmp(token[i], lexicon[j].word) == 0)
            {
                float wordScore = lexicon[j].value1;

                if (isAllCaps)
                {
                    factor *= 1.5;
                }

                if (negate)
                {
                    factor *= -0.5; // Apply negation inversion
                }

                factor *= getExclamationBoost(token[i + 1]);
                factor *= getQuestionMarkPenalty(token[i + 1]);

                score += wordScore * factor;

                factor = 1; // Reset factor for next word
                break;
            }
        }
    }

    return score;
}

float computeAverageScore(float score, int numLexicons)
{
    return score / numLexicons;
}

float computeCompoundScore(float score)
{
    return score / sqrt(score * score + ALPHA);
}
