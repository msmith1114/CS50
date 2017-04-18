import cs50

amount = 0.0
cents = 0
quarters = 0
dimes = 0
nickels = 0
leftover = 0

print("O hai! How much change is owed? ", end="")
amount = cs50.get_float()
if amount <= 0 or amount == 0:
    while amount <= 0 or amount == 0:
        print("O hai! How much change is owed? ", end="")
        amount = cs50.get_float()  
        
cents = amount*int(100)

quarters = int(cents) // 25
leftover = int(cents) % int(25)


dimes = int(leftover) // int(10)
leftover = int(leftover) % int(10)


nickels = int(leftover) // int(5)
leftover = int(leftover) % int(5)


print("{}".format(quarters+dimes+nickels+leftover))
    