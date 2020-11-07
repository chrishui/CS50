#include <cs50.h>
#include <stdio.h>
//string.h provides length of string check function: strlen
#include <string.h>
//ctyoe.h provides digits function: isdigit
#include <ctype.h>
//stdlib.h provides string to number conversion: atoi
#include <stdlib.h>

int main (int argc, string argv[])
{
    //define 2nd argument in input as string "validate"
    string validate = argv[1];
    //define integer, used for string (an array of chars) to int conversion
    int val;

    //Validating the Key
    //check if input has two arguments i.e. ./caesar asd
    if (argc == 2)
    {
        //creating loop for input string
        for (int i = 0, n = strlen(validate); i < n; i++)
        {
            //check if input is digits
            if (isdigit(validate[i]))
            {
                
            }

            //input not digits
            else
            {
                printf("Usage: ./caesar key\n");
                //return 1 stops the loop (?), if input is not digits
                return 1;
            }
        }
        //using atoi function to convert input to number (int)
        val = atoi(validate);
        
        //Prompt user for plaintext input
        string pt = get_string ("plaintext: ");
        
        //ciphertext
        printf("ciphertext: ");
        
        //Adjusted value 
        int adj = 0;
        for (int j = 0, m = strlen(pt); j < m; j++)
        {
            //producing cipher text
            
            //if punctuation, print punctuation
            if ((pt[j] <= 64) || (pt[j] >= 91 && pt[j] <= 96) || (pt[j]>=123))
            {
                printf ("%c", pt[j]);
            }
            
            //result of value adjustment calculation, lower case (Refer to asciichart.com for int). 97 because lowercase a starts at 97.
            if (pt[j] >= 97)
            {
            adj = 97 + ((pt[j] + val - 97) % 26);
            printf("%c", adj );
            }
            
            //result of value adjustment calculation,  Upper case.
            if (pt[j] <= 90 && pt[j] >= 65)
            {
            adj = 65 + ((pt[j] + val - 65) % 26);
            printf("%c", adj );
            }
            
        }
        printf("\n");

    }

    //input does not have 2 arguments
    else
    {
        printf("Usage: ./caesar key\n");
        //return 1 stops the loop (?), if input is not digits
        return 1;
    }
}