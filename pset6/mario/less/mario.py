from cs50 import get_int

# Main function
def main():
    # Obtain half-pyramid's height from user
    height = get_positive_int()
    # Initialize constants:
    i = 0
    h = height
    # Make pyramid
    # Printing Spaces
    while h > 0 and i < height:
        print(" "*h + "#"*(i+1), end="\n")
        h -= 1
        i += 1
    # Printing #s
    #while i < height:
        #print("#"*(i+1), end="\n")
        #i += 1

# Check if input is positive
def get_positive_int():
    while True:
        n = get_int("Height: ")
        if n > 0 and n < 8:
            break
    return n

main()

