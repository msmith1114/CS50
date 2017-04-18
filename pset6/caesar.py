import cs50
import sys

if len(sys.argv) != 2:
    print("Usage: ./caesar k")
    exit(1)
else:
    k = sys.argv[1]
    plain = ""
    print("plaintext: ", end="")
    plain = cs50.get_string()
    print("ciphertext: ",end="")
    n = len(plain)
    for i in range(n):
        if plain[i].islower():
            print("{}".format(chr(((((ord(plain[i]) + int(k)) - 97) % 26) + 97))),end="")
        elif plain[i].isupper():
            print("{}".format(chr(((((ord(plain[i]) + int(k)) - 65) % 26) + 65))),end="")
        else:
            print("{}".format(plain[i]),end="")
    print("")
            