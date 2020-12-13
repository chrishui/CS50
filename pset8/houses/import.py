import sys
import csv
from cs50 import SQL

def main():
    # Check command-line arguments
    if len(sys.argv) != 2:
        print("Usage: python import.py 'csv file'.csv")
        sys.exit()

    # Connect database students.db with SQL
    open(f"students.db", "w").close()
    db = SQL("sqlite:///students.db")

    # Create table called 'students'
    db.execute("CREATE TABLE students (%s TEXT, %s TEXT, %s TEXT, %s TEXT, %s NUMERIC)", "first", "middle", "last", "house", "birth")

    # Open CSV file given by command-line argument
    with open(sys.argv[1], mode='r') as csvfile:
        reader = csv.DictReader(csvfile)

        # Parse name for each row
        for row in reader:
            # Split names
            name_split = row["name"].split();

            # For students with first/last names
            if len(name_split) == 2:
                first = name_split[0]
                middle = None
                last = name_split[1]

            # For students with first/mid/last names
            if len(name_split) == 3:
                first = name_split[0]
                middle = name_split[1]
                last = name_split[2]

            #Insert students by substituting values into each ? placeholder
            db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)", first, middle, last, row["house"], row["birth"])

main()
