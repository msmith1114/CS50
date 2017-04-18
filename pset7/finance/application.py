from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import gettempdir

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = gettempdir()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    """Index page"""
    if request.method == "GET":
        #get username
        username = db.execute("select username from users where id = :id", id=session["user_id"])
        #get symbol list from portfolio from logged in user
        symbols = db.execute("SELECT symbol,sum(numshare) as shares FROM portfolio where username = :username GROUP BY symbol", username=username[0]["username"])
        positions = []
        total_worth = 0
        for i in range(len(symbols)):
            temp_dict = lookup(symbols[i]["symbol"])
            worth = temp_dict["price"]*symbols[i]["shares"]
            total_worth += worth
            dict_val = {'symbol': temp_dict["symbol"], 'value': worth,'name': temp_dict["name"],'shares':symbols[i]["shares"],'price':temp_dict["price"]}
            positions.append(dict_val.copy())
        total_cash = db.execute("select cash from users where username = :username", username=username[0]["username"])
        total_value = total_cash[0]["cash"]+total_worth
        return render_template("index.html",stocks=positions,total=total_cash[0]["cash"],holdings=total_value)  
    else:
        return render_template("index.html")

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy Stocks"""
    if request.method == "GET":
        return render_template("buy.html")
    else:
        # ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide symbol")
        elif not request.form.get("shares"):
            return apology("shares must be a positive integer")
        elif (int(request.form.get("shares")) <= 0):
            return apology("shares must be a positive integer")
            
        # returns dict:
        # name
        # price
        # symbol
        quote = lookup(request.form.get("symbol"))
        # check that symbol is valid
        if not quote:
            return apology("stock does not exist")
        rows = db.execute("SELECT * FROM users WHERE id = :id", id=session["user_id"])
        if (int(rows[0]['cash'])) > (int(request.form.get("shares"))*quote['price']):
            insert_return = db.execute("INSERT INTO PORTFOLIO (username,symbol,price,numshare) VALUES (:username,:symbol,:price,:numshare)", username=rows[0]['username'],symbol=quote['symbol'],price=quote['price'],numshare=int(request.form.get("shares")))
            buy_history = db.execute("INSERT INTO history (username,symbol,price,numshare,type) VALUES(:username,:symbol,:price,:numshare,:type)", username=rows[0]['username'],symbol=quote['symbol'],price=quote['price'],numshare=int(request.form.get("shares")),type="buy")
            update_return = db.execute("UPDATE users set cash = :value where username = :username",value=(rows[0]['cash']-int(request.form.get("shares"))*quote['price']),username=rows[0]['username'])
            return redirect(url_for("index"))
        else:
            return apology("Cannot afford this transaction")
      
    

@app.route("/history")
@login_required
def history():
    """History"""
    if request.method == "GET":
        #get username
        username = db.execute("select username from users where id = :id", id=session["user_id"])
        history_list = db.execute("select * from history where username = :username", username=username[0]["username"])
        return render_template("history.html",history_list=history_list)  
    else:
        return render_template("index.html")

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")
    else:
        # ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide symbol")
        
        # returns dict:
        # name
        # price
        # symbol
        quote = lookup(request.form.get("symbol"))
        # check that symbol is valid
        if not quote:
            return apology("stock does not exist")
        return render_template("quoted.html", name=quote['name'],price=quote['price'],symbol=quote['symbol'])
        

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register User."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")
    
        # ensure password 2 was submitted
        elif not request.form.get("password2"):
            return apology("must provide password")
        
        # ensure passwords match
        elif not request.form.get("password") == request.form.get("password2"):
            return apology("passwords must match")
        
        # hash password for database insertion    
        hash = pwd_context.encrypt(request.form.get("password"))

        # insert into database new user
        result = db.execute("INSERT INTO users (username,hash) VALUES(:username,:hash)", username=request.form.get("username"),hash=hash)

        # ensure username exists and password is correct
        if not result:
            return apology("invalid username and/or password")

         # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")



@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    if request.method == "GET":
        #get username
        username = db.execute("select username from users where id = :id", id=session["user_id"])
        #get symbol list from portfolio from logged in user
        symbols = db.execute("SELECT symbol,sum(numshare) as shares FROM portfolio where username = :username GROUP BY symbol", username=username[0]["username"])
        positions = []
        total_worth = 0
        for i in range(len(symbols)):
            temp_dict = lookup(symbols[i]["symbol"])
            worth = temp_dict["price"]*symbols[i]["shares"]
            total_worth += worth
            dict_val = {'symbol': temp_dict["symbol"], 'value': worth,'name': temp_dict["name"],'shares':symbols[i]["shares"],'price':temp_dict["price"]}
            positions.append(dict_val.copy())
        total_cash = db.execute("select cash from users where username = :username", username=username[0]["username"])
        total_value = total_cash[0]["cash"]+total_worth
        return render_template("sell.html",stocks=positions,total=total_cash[0]["cash"],holdings=total_value)  
    else:
        #get username
        username = db.execute("select username from users where id = :id", id=session["user_id"])
        #get symbol list from portfolio from logged in user
        symbols = db.execute("SELECT symbol,sum(numshare) as shares FROM portfolio where username = :username GROUP BY symbol", username=username[0]["username"])
        positions = []
        total_worth = 0
        for i in range(len(symbols)):
            temp_dict = lookup(symbols[i]["symbol"])
            worth = temp_dict["price"]*symbols[i]["shares"]
            total_worth += worth
            dict_val = {'symbol': temp_dict["symbol"], 'value': worth,'name': temp_dict["name"],'shares':symbols[i]["shares"],'price':temp_dict["price"]}
            positions.append(dict_val.copy())
        total_cash = db.execute("select cash from users where username = :username", username=username[0]["username"])
        total_value = total_cash[0]["cash"]+total_worth
        owned = db.execute("SELECT symbol,price,sum(numshare) as shares FROM portfolio where username = :username and symbol = :symbol GROUP BY symbol", username=username[0]["username"], symbol=request.form.get("symbol"))
        if not request.form.get("symbol"):
            return apology("must provide symbol")
        elif not owned:
            return apology("must have shares in that company")
        else:
            new_lookup = lookup(owned[0]["symbol"])
            sell_worth = owned[0]["shares"]*new_lookup["price"]
            get_cash = db.execute("SELECT cash from USERS where username = :username", username=username[0]["username"])
            new_cash = get_cash[0]["cash"]+sell_worth
            update_cash = db.execute("UPDATE users SET cash = :cash_amount WHERE username = :username",cash_amount=new_cash,username=username[0]["username"])
            sell_history = db.execute("INSERT INTO history (username,symbol,price,numshare,type) VALUES(:username,:symbol,:price,:numshare,:type)",username=username[0]["username"],symbol=owned[0]["symbol"],price=new_lookup["price"],numshare=owned[0]["shares"],type="sell")
            reduce_cash = db.execute("DELETE from PORTFOLIO where SYMBOL = :symbol",symbol=owned[0]["symbol"])
            return redirect(url_for("index")) 
            #make sure user owns that many shares
            
@app.route("/change", methods=["GET", "POST"])
@login_required
def change():
    """Change Password"""
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        username = db.execute("select username from users where id = :id", id=session["user_id"])
        hash = db.execute("select hash from users where username = :username", username=username[0]["username"])
        ok = pwd_context.verify(request.form.get("cur_password"), hash[0]["hash"])
        if not ok:
            return apology("current password is not correct")
            
        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")
    
        # ensure password 2 was submitted
        elif not request.form.get("password2"):
            return apology("must provide password confirm")
        
        # ensure passwords match
        elif not request.form.get("password") == request.form.get("password2"):
            return apology("passwords must match")
        
        # hash password for database insertion    
        hash_new = pwd_context.encrypt(request.form.get("password"))

        # insert into database new user
        result = db.execute("UPDATE users SET hash = :hash where username = :username", username=username[0]["username"],hash=hash_new)

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("change.html")
        
