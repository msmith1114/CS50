import cs50

height = 0
print("Height: ", end="")
height = cs50.get_int()
if height < 0 or height > 23:
    while height < 0 or height > 23:
        print("Height: ", end="")
        height = cs50.get_int()

for i in range(0,height):
    for a in range(0,height-i-1):
        print(" ", end="")
    for b in range(0,i+1):
        print("#", end="")
    print(" ", end="")
    for c in range(0,i+1):
        print("#", end="")
    print("")
            
    