#include "utility.h"
#include <stdio.h>
void test(WordData *lexicon, int lexiconSize)
{
    char *input[] = {
        "VADER is smart, handsome, and funny.",
        "VADER is smart, handsome, and funny!",
        "VADER is very smart, handsome, and funny.",
        "VADER is VERY SMART, handsome, and FUNNY.",
        "VADER is VERY SMART, handsome, and FUNNY!!!",
        "VADER is VERY SMART, uber handsome, and FRIGGIN FUNNY!!!",
        "VADER is not smart, handsome, nor funny.",
        "At least it isn't a horrible book.",
        "The plot was good, but the characters are uncompelling and the dialog is not great.",
        "Make sure you :) or :D today!",
        "Not bad at all"};

    for (int i = 0; i < 11; i++)
    {
        char *tokens[MAX_TOKENS];
        tokenizeSentence(input[i], tokens, MAX_TOKENS);
        float score = calculateTotalScore(lexicon, lexiconSize, tokens);
        float compScore = computeCompoundScore(score);

        printf("Input sentence: %s\n", input[i]);
        printf("Compound score: %f\n", compScore);
        if (compScore > 0)
        {
            printf("The sentiment socre is positive (pos)");
        }
        else if (compScore == 0)
        {
            printf("The sentiment socre is neutral (neu)");
        }
        else
        {
            printf("The sentiment socre is negative (neg)");
        }
        printf("\n");
    }
}

int main()
{
    const char *filename = "vader_lexicon.txt";
    WordData *lexicon = (WordData *)malloc(DEFAULT_SIZE * sizeof(WordData)); // allocation for the copy of the lexicon
    if (lexicon == NULL)                                                     // allocation is failed
    {
        printf("Error: Initial memory allocation failed\n");
        return 1; // return
    }

    int lexiconSize = readFile(filename, &lexicon);
    if (lexiconSize == -1)
    {
        free(lexicon); // check for the lsxicon is valid, if size is not valid
        return 1;
    }
    char input[MAX_INPUT] = "VADER is very smart, handsome, and funny.";
    char *tokens[MAX_TOKENS];

    tokenizeSentence(input, tokens, MAX_TOKENS);
    test(lexicon, lexiconSize);

    free(lexicon);
    return 0;
}
