#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int letters(string t);
int words(string t);
int sentence(string t);

int main(void)
{
    //Prompt user for text input
    string text = get_string("Text: ");

    //Average number per 100 words in text
    float L;
    L = (float) letters(text) / (float) words(text) * 100;

    //Average number of sentences per 100 words in the text
    float S;
    S = (float) sentence(text) / (float) words(text) * 100;

    //Coleman-Liau Index;
    float index;
    index = 0.0588 * L - 0.296 * S - 15.8;
    
    if (round(index) <= 1)
    {
        printf("Before Grade 1\n");
    }
    
    if (round(index) < 16 && round(index) > 1)
    {
        printf("Grade %.0f\n", round(index));
    }
    
    if (round(index) >= 16)
    {
        printf("Grade 16+\n");
    }
}


//function to count number of letters, uppercase or lowercase
int letters(string t)
{
    int letter_count = 0;
    for (int i = 0; i < strlen(t); i++)
    {
        if ((t[i] >= 'a' && t[i] <= 'z') || (t[i] >= 'A' && t[i] <= 'Z'))
        {
            letter_count++;
        }
    }
    return letter_count;
}

//function to count number of words, counting number of spaces between words. Initial int at 1 as single word does not contain any spacing
int words(string t)
{
    int word_count = 1;
    for (int i = 0; i < strlen(t); i++)
    {
        if (t[i] == ' ')
        {
        word_count++;
        }
    }
    return word_count;
}

//function to count number of sentences, using '.' '!' '?' as counts
int sentence(string t)
{
    int sen_count = 0;
    for (int i = 0; i < strlen(t); i++)
    {
        if (t[i] == '.' || t[i] == '!' || t[i] == '?')
        {
        sen_count++;
        }
    }
    return sen_count;
}