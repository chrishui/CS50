import sys
import csv

def main():

    ############ Step 1 ############
    #Check for command line arguemtns argv[2]
    if len(sys.argv) != 3:
        print("Usage: python dna.py 'database CSV' 'sequences txt'")
        sys.exit()

    #Load database from user input, read into memory
    with open(sys.argv[1], mode='r') as csvfile:
        database = csv.reader(csvfile)
        db = []
        for row in database:
            db.append(row)

    #Load sequence from user input, read into memory
    sq_open = open(sys.argv[2], "r")
    sq = sq_open.read()
    #Length of DNA sequence in loaded sequence
    sq_len = (len(sq))

    ############ Step 2 ############
    #check sucessive substrings for STR repeat

    #Lists of arrays to store matches
    STR_match_list = []
    for x in range(len(db[0])-1):
        STR_match_list.append([0]*(sq_len))

    #loop for STR (e.g. AGATC, AATC etc.)
    for j in range(len(db[0])-1):
        #loop for sequence
        for i in range(sq_len):
            #loop for successive substrings
            for k in range(i, sq_len, len(db[0][j+1])):
                if sq[k:k+len(db[0][j+1])] == db[0][j+1]:
                    STR_match_list[j][i] += 1
                else:
                    break

    #print(STR_match_list)


    ############ Step 3 ############
    #Comparing against data

    #Obtain highest consecutive matches in STR_match_list
    STR_match_count = [0]*(len(db[0])-1)
    for i in range(len(STR_match_count)):
        STR_match_count[i]=max(STR_match_list[i])

    #Array for database in int
    db_int = []
    for x in range(len(db[0])-1):
        db_int.append([0]*(len(db)-1))

    match = 0
    #Compare matches against database
    for i in range(len(db[0])-1):
        #Convert database into int
        db_int[i]=list(map(int, db[i+1][1:]))
        #Print matched individual
        if (STR_match_count == db_int[i]) == True:
            print (db[i+1][0])
            match = 1

    if (match == 0):
        print("No match")

main()


