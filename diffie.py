import random

def main():
    p, g = [int(x) for x in input("Enter p and g ").split()]

    sa = random.randint(1, 1000)
    sb = random.randint(1, 1000)
    
    TA = (g ** sa)%p
    TB = (g ** sb)%p

    A = (TB ** sa)%p
    B = (TA ** sb)%p


    if(A==B):
        print("Secret key is ", A)
    else :
        print("ERROR")

if __name__ == "__main__":
    main()
