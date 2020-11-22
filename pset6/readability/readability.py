from cs50 import get_string

def main():

    #Prompt user for text input
    text = get_string("Text: ")

    #Average no. of letters per 100 words
    L = (letters(text) / words(text)) * 100

    #Average no. of sentences per 100 words
    S = (sen(text) / words(text)) * 100

    #Coleman-Liau Index
    index = round(0.0588 * L - 0.296 * S - 15.8)

    if index <= 1:
        print("Before Grade 1")
    elif index > 1 and index <= 16:
        print(f"Grade {index}")
    elif index > 16:
        print("Grade 16+")

#No. of letters
def letters(s):
    letter_count = 0
    #Iterate over each character in a string
    for c in s:
        #Check if the letter is alphabat
        if c.isalpha() == True:
            letter_count += 1
    return letter_count

#No. of words
def words(s):
    word_count = 1
    #Iterate over each character in a string
    for c in s:
        #Check for space, to indicate words
        if c==" ":
            word_count +=1
    return word_count

#No. of sentences
def sen(s):
    sen_count = 0
    #Iterate over each character in a string
    for c in s:
        #Check for "." "!" and "?"
        if c=="." or c=="!" or c=="?":
            sen_count += 1
    return sen_count

main()
