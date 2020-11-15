#include <math.h>
#include <cs50.h>
#include <stdio.h>

float get_positive_float(void);

int main(void)
{
    // Obtain positive amount, from function at bottom
    float dollars = get_positive_float();
    
    // Convert dollars to cents (and round to nearest penny)
    int cents = round(dollars * 100);
    
    // Define integer coins, starting from 0
    int coins = 0;

    // computing amount of coins to change
    while (cents >= 25)
    {
        cents = cents - 25;
        coins++;
    }
    
    while (cents >= 10)
    {
        cents = cents - 10;
        coins++;
    }
    
    while (cents >= 5)
    {
        cents = cents - 5;
        coins++;
    }
    
    while (cents >= 1)
    {
        cents = cents - 1;
        coins++;
    }
    
    printf ("%i\n", coins);
 
}
  




// Propmt user for positive amount of change owed
float get_positive_float(void)
{
    float d;
    do
    {
        d = get_float("Amount of change owed: $ ");
    }
    while (d < 0);
    return d;
}