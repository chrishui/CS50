from cs50 import get_int
from cs50 import get_string

def main():
    # User credit card no.
    cc = get_credit_card_number()
    # No. of digits in credit card
    digits = len(str(cc))
    # Luhn's algorithm to determine validity
    sum = Luhn(cc)
    if sum%10 != 0:
        print("INVALID")
    # Check credit card type
    # Amex
    if ((cc>340000000000000 and cc<349999999999999) or (cc>370000000000000 and cc<379999999999999)) and digits==15:
        print("AMEX")
    # Mastercard
    if (cc>5100000000000000 and cc<5599999999999999) and digits==16:
        print("Mastercard")
    # Visa
    if ((cc//10**12==4) or (cc//10**15==4)) and (digits==13 or digits==16):
        print("Visa")

# Function: Credit card number input prompt
def get_credit_card_number():
    while True:
        cc = int(input("Credit card number: "))
        if cc > 1:
            break
    return cc

# Function: Luhn's algorithm
def Luhn(cc):
    sum_even = 0
    sum_odd = 0
    sum = 0
    while cc > 0:
        # Even numbers
        even = cc % 10
        sum_even += even
        cc //= 10
        # Odd numbers
        i = (cc % 10) * 2
        odd = (i%10) + (i//10)
        sum_odd += odd
        cc //= 10

    sum = round(sum_even + sum_odd)
    return sum

main ()