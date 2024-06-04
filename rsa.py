import math
import functools
# ------------ returns true if p is not prime ------------
def ifnprime(p) -> int:
    if(p <= 2):
        return 0
    d = 2
    while(d <= math.sqrt(p)):
        if(p%d==0):
            return 1
        d+=1
    return 0

def euclidean(p, q):
    if (q == 0):
        return p

    while q > 1:
        q = p%q
        p = q
    return p

def getrelative(pq, n):
    d = 2;
    while(d < pq):
        if(euclidean(pq, d) == 1):
            return d
        d+=1

def getD(e, p, q):
    d = 1
    while True:
        if((e*d)%((p-1)*(q-1)) == 1):
            return d
        d +=1

def addpows(e, m, n) -> int:
    b = "{:b}".format(e)
    m = [(m **(2**i)%n) for (i, x) in enumerate(b[::-1]) if x == "1"]
    return functools.reduce(lambda acc, x: acc * x, m, 1)

def encrypt(m, e, n) -> int:
    return addpows(e, m, n)%n

def decrypt(c, d, n) -> int:
    return addpows(d, c, n)%n


def main ():
    # ------------ GET PRIME VALUES ------------
    p, q = [int(x) for x in input("Enter prime numbers: ").split()]

    if(ifnprime(p) or ifnprime(q)):
        print("not a prime")
        return 0

    print("Calculating RSA values")

    # ------------ GET PRODUCT OF P & Q ------------
    n = p * q

    # ----------- GET RELATIVE PRIME OF(P-1)(Q-1)
    e = getrelative(((p-1) * (q-1)), n)

    # ------------ GET D SUCH THAT ED MOD (P-1)(Q-1) == 1 ------------
    d = getD(e, p, q)

    # ------------ PRINTING ----------------
    print(f'Public RSA Key ({e}, {n})')
    print(f'Private RSA key ({d}, {n})')

    m = int(input("Enter the plaintext message m (an integer): "))

    print("Encrypting m...")

    c = encrypt(m, e, n)

    print(f'The cyphertext is {c}')

    print("Decrypting c...")

    d = decrypt(c, d, n)

    print(f'The plaintext is {d}')


if __name__ == "__main__":
    main()
