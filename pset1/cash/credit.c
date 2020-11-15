#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Prompt user for credit card number input
    long cc_number = get_long ("Number: ");
    
    // Luhn's algorithm
    
    //Getting rid of last digit to enable 1st modulo
    long cc_number_start = cc_number / 10;
    long cc_number_start2 = cc_number;
    long cc_number_2 = cc_number;
    //Defining digit to be obtained
    int cc_number_mod = 0;
    int cc_number_mod2 = 0;
    //Defining product's digits
    int cc_number_dig1 = 0;
    int cc_number_dig2 = 0;
    int cc_number_dig3 = 0;
    //sum of product's digits
    int cc_number_sum = 0;
 
    //Obtaining every other digit
    while (cc_number_start2 > 0)
    {
        //step 1
        cc_number_mod = cc_number_start % 10;
        cc_number_mod2 = cc_number_start2 % 10;
        
        if (cc_number_mod >= 5)
        {
            cc_number_mod = cc_number_mod * 2;
            cc_number_dig1 = cc_number_mod % 10;
            cc_number_dig2 = cc_number_mod / 10;
        }
        else 
        {
            cc_number_mod = cc_number_mod * 2;
            cc_number_dig1 = cc_number_mod;
            cc_number_dig2 = 0;
        }
        
        //step 2
        if (cc_number_mod2 >= 0)
        {
            cc_number_dig3 = cc_number_mod2;
        }
        
        //sum off step 1 and step 2 (to have last digit 0 if legit)
        cc_number_sum = cc_number_sum + cc_number_dig1 + cc_number_dig2 + cc_number_dig3;
        
        //Next iteration
        cc_number_start = cc_number_start / 100;
        cc_number_start2 = cc_number_start2 / 100;
        
    }

    // Counting amount of digits in the credit card number: 
        int digits = 1;
        int modulo = 0;
    
        while ( cc_number >= 10)
        {
            cc_number = cc_number / 10;
            digits++;
        }
        
    //Calculating 2nd digit of credit card
    int cc_number_2dig = 0;
    while (cc_number_2 >= 100)
    {
        cc_number_2 = cc_number_2/10;
        cc_number_2dig = cc_number_2 % 10;
        
    }
    
    //Calculating last digit of Luhn's formula
    int cc_number_sum_dig = cc_number_sum % 10;
    
    
     //Determining type of card
    if ((cc_number_sum_dig == 0) && (digits == 15 || digits == 16 || digits == 13 ))
    {
        if ((digits == 15 && cc_number == 3) && (cc_number_2dig == 4 || cc_number_2dig == 7))
        {
           printf("AMEX\n");
        }

        else if ((digits == 16 && cc_number == 5) && (cc_number_2dig == 1 || cc_number_2dig == 2 || cc_number_2dig == 3 || cc_number_2dig == 4 || cc_number_2dig == 5))
        {
            printf("MASTERCARD\n");
        }

        else if ((digits == 13 || digits == 16) && (cc_number == 4))
       {
            printf("VISA\n");
        }
        
        else 
        {
            printf("INVALID\n");
        }

    }

else 
        {
            printf("INVALID\n");
        }
    
}