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


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    return apology("TODO")

#==========================================================================
# 3: Buy
#==========================================================================
@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    return apology("TODO")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    return apology("TODO")


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
        data = lookup(symbol)

        # if no data returned
        if not data:
            return apology("Symbol doesn't exist",403)

        # If data is returned
        else:
            # Assigning lookup dict into the name/price/symbol
            name = data["name"]
            price = data["price"]
            symbol = data["symbol"]
            #sector = data["sector"]
            #industry = data["industry"]
            #website = data["website"]
            #description = data["description"]

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
        query = db.execute("SELECT username FROM users WHERE username = :username", username = request.form.get("username"))

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

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    return apology("TODO")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
