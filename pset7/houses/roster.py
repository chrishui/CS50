import sys
from cs50 import SQL

def main():

    # Check command line arguments
    if len(sys.argv) != 2:
        print("usage: python roster.py 'house'")
        sys.exit()

    # Connect database students.db with SQL
    db = SQL("sqlite:///students.db")

    # Select table contents, returning as a list of Python dicts
    list = db.execute("SELECT first, middle, last, birth FROM students WHERE house = %s ORDER BY last ASC, first ASC", sys.argv[1])

    for row in list:
        if row['middle'] != None:
            print (row['first'], " ", row['middle']," ", row['last'], ", born ", row['birth'], sep='')
        if row['middle'] == None:
            print (row['first'], " ", row['last'], ", born ", row['birth'], sep='')

main()