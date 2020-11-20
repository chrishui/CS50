from cs50 import get_float

def main():
    # Cents from user input
    cents = 100*get_positive_float()

    # Coins counter
    c = 0

    while cents >= 25:
        cents = cents - 25
        c += 1

    while cents >= 10:
        cents = cents - 10
        c += 1

    while cents >= 5:
        cents = cents - 5
        c += 1

    while cents >= 1:
        cents = cents - 1
        c += 1

    print(c)

# Get positive float from user
def get_positive_float():
    while True:
        i = get_float("Change owed ($): ")
        if i > 0:
            break
    return i

main()