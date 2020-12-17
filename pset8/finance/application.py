#==========================================================================
# Rev0.1 - 15/12/2020 - Chris Hui
#==========================================================================

import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd
#import datetime
from datetime import datetime

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")

#==========================================================================
# 4: Index
#==========================================================================
@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Obtain logged in user's user_id
    user_id = session.get("user_id")

    # pull symbols, quantities and average prices from finance.db
    rows = db.execute("SELECT symbol, SUM(quantity), AVG(price) FROM buy WHERE id=:id GROUP BY symbol", id=user_id)

    # To obtain current share prices
    # Obtain symbols stored in finance.db --------------------------------To be updated to 'current porfolio' table?
    symbol_db = db.execute("SELECT symbol FROM buy WHERE id=:id GROUP BY symbol", id=user_id)

    # No. of different shares bought by user
    num = len(symbol_db)

    # stocks total value
    tot_stocks = 0

    # Iterate over each stock
    for i in range(num):
        # select symbol
        sbl = symbol_db[i]['symbol']
        # Lookup data using lookup()
        current = lookup(sbl)
        # Obtain current price
        c_price = current["price"]
        # Add current price, as dict, to rows
        rows[i]['currentprice'] = c_price

        # total purchase price
        tot_p = (rows[i]["AVG(price)"])*(rows[i]["SUM(quantity)"])
        # Add total purchase price, as dict, to rows
        rows[i]['totalpurchaseprice'] = tot_p

        # total current value
        tot_v = c_price*(rows[i]["SUM(quantity)"])
        # Add total current value, as dict, to rows
        rows[i]['totalcurrentprice'] = tot_v

        # Add total value to stocks total value
        tot_stocks += tot_v

    # User remaining cash balance
    cash = (db.execute("SELECT cash FROM users WHERE id=:id", id=user_id))[0]["cash"]

    # Grand total (stocks' total current value plus cash)
    gtot = cash + tot_stocks

    #User reach route via GET
    if request.method == "GET":
        return render_template("index.html", rows=rows, cash=cash, gtot=gtot)

    else:
        return apology("Error",403)

#==========================================================================
# 3: Buy
#==========================================================================
@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reach route via GET
    if request.method == "GET":
        return render_template("buy.html")

    # User reach route via POST (as by submitting a fortm via POST)
    if request.method == "POST":

        # Obtain user's symbol input
        symbol = request.form.get("symbol")

        # if no input
        if not symbol:
            return apology("Please input symbol", 403)

        # Obtain instrument from user's input
        instrument = lookup(symbol)

        # If instrument does not exist
        if not instrument:
            return apology("Instrument doesn't exist",403)

        # If instrument exists, check share price
        else:
            # Assign lookup dict for instrument price
            price = instrument["price"]

        # Obtain user's no. of shares input
        shares = int(request.form.get("shares"))

        # if no. of shares is not positive integer
        if shares < 0:
            return apology("No. of shares must be positive",403)

        # Total cost of user's shares purchase
        cost = price * shares

        # Obtain logged in user's user_id (??????) -------------------------------- is user_id correct?
        user_id = session.get("user_id")

        # Check user's remaining cash (??????) -------------------------------- as above
        cash = (db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=user_id))[0]["cash"]

        # Log current time
        date = datetime.now().date()
        time = datetime.now().time()

        # Check if sufficient funds to complete purchase
        if cost > cash:
            return apology("Insufficient funds",403)

        # Insert data into table ------------------------------------- This table should be 'history' table?
        else:
            db.execute("INSERT INTO buy (id, direction, symbol, quantity, price, total_amount, trading_day, trading_time) VALUES (:id, :direction, :symbol, :quantity, :price, :total_amount, :trading_day, :trading_time)",
            id=user_id, direction="BUY", symbol=symbol, quantity=shares, price=price, total_amount=cost, trading_day=date, trading_time=time)

        # To update cash -------------------------------------- TBC
        cash_remain = cash - cost
        db.execute("UPDATE users SET cash = :cash WHERE id = :id", cash=cash_remain, id=user_id)

        # Redirect user to index page -------------------------------------- TBC
        return redirect("/")

@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    return apology("error",403)

#==========================================================================
# Preloaded: login
#==========================================================================
@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

#==========================================================================
# Preloaded: logout
#==========================================================================
@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")

#==========================================================================
# 2: Quote
#==========================================================================
@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # User reached route via GET
    if request.method == "GET":
        return render_template("quote.html")

    # User reached route via POST (submitted a form via POST)
    if request.method == "POST":

        # Obtain user's symbol input
        symbol = request.form.get("symbol")

        # If no symbol entered
        if not symbol:
            return apology("Please input symbol",403)

        # Obtain symbol data via lookup helper function
        instrument = lookup(symbol)

        # if no data returned
        if not instrument:
            return apology("Symbol doesn't exist",403)

        # If data is returned
        else:
            # Assigning lookup dict into the name/price/symbol
            name = instrument["name"]
            price = instrument["price"]
            symbol = instrument["symbol"]
            #sector = instrument["sector"]
            #industry = instrument["industry"]
            #website = instrument["website"]
            #description = instrument["description"]

            # Otherwise, link name, symbol, and price to quoted.html
            return render_template("quoted.html", name=name, price=price, symbol=symbol)

#==========================================================================
# 1: Register
#==========================================================================
@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via GET (clicking on link)
    if request.method == "GET":
        return render_template("register.html")

    # User reached route via POST (submitted a form via POST)
    if request.method == "POST":

        # Obtain user's username input
        username = request.form.get("username")

        # If no username is provided
        if not username:
            return apology("Please input username",403)

        # Check if username already exists
        # Query database for username
        query = db.execute("SELECT username FROM users WHERE username = :username", username = username)

        # If query returns a username, that username has been taken
        if len(query) != 0:
            return apology("Username already taken",403)

        # Obtain user's password input
        password = request.form.get("password")

        # Obtain user's password confirmation
        confirmation = request.form.get("confirmation")

        # If either input is blank
        if not password:
            return apology("Please input password",403)
        if not confirmation:
            return apology("Please input password confirmation",403)

        # If password doesn't match
        if password != confirmation:
            return apology("passwords do not match",403)

        # Hash password
        hash = generate_password_hash(password,method='pbkdf2:sha256',salt_length=8)

        # Insert new user into users (in finance.db)
        db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)", username=username, hash=hash)

        # Return to login form
        return redirect("/")

#==========================================================================
# 5: Sell
#==========================================================================
@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

     # User reach route via GET
    if request.method == "GET":

        # Obtain logged in user's user_id
        user_id = session.get("user_id")

        # Obtain symbols stored in finance.db (user purchased)
        symbol_db = db.execute("SELECT symbol FROM buy WHERE id=:id GROUP BY symbol", id=user_id)

        # link symbol_db to GET
        return render_template("sell.html", symbol_db = symbol_db)

    # User reach route via POST (as by submitting a fortm via POST)
    if request.method == "POST":

        # Obtain logged in user's user_id
        user_id = session.get("user_id")

        # Obtain user's chosen symbol
        symbol = request.form.get("tosell")

        # If no symbol chosen
        if not symbol:
            return apology("No symbol selected",403)

        # Obtain symbols stored in finance.db (user purchased) -------------------- Alter this to obtain data from updated ' buy' table?
        symbol_db = db.execute("SELECT symbol, SUM(quantity) FROM buy WHERE id=:id GROUP BY symbol", id=user_id)

        num = len(symbol_db)

        # Obtain user's chosen no. of shares to sell
        shares = int(request.form.get("shares"))

        # Check if user owns chosen stock, and if no. of shares to sell exceeds shares owned
        symbol_check = 0
        for i in range(num):
            if symbol in symbol_db[i]['symbol']:
                # Symbol found
                symbol_check = 1
                # No. of shares check
                if shares > int(symbol_db[i]['SUM(quantity)']):
                    return apology ("Invalid shares input",403)
                break
        if symbol_check != 1:
            return apology("Stock not owned",403)

        # Obtain instrument from user's input using lookup()
        instrument = lookup(symbol)

        # Get current share price
        price = instrument["price"]

        # Sell (USD$)
        sale = price * shares

        # Log current time
        date = datetime.now().date()
        time = datetime.now().time()

        # Check user's remaining cash
        cash = (db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=user_id))[0]["cash"]

        # To update cash -------------------------------------- TBC
        cash_remain = cash + sale
        db.execute("UPDATE users SET cash = :cash WHERE id = :id", cash=cash_remain, id=user_id)

        # Insert data into table ------------------------------------- This table should be 'history' table?
        db.execute("INSERT INTO buy (id, direction, symbol, quantity, price, total_amount, trading_day, trading_time) VALUES (:id, :direction, :symbol, :quantity, :price, :total_amount, :trading_day, :trading_time)",
        id=user_id, direction="SELL", symbol=symbol, quantity=shares, price=price, total_amount=sale, trading_day=date, trading_time=time)

        return redirect("/")






        #return apology("TODO")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
