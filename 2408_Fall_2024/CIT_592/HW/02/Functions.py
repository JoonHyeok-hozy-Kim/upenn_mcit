import math

def factorial(n):
    if n== 0 or n==1:
        return 1
    return n * factorial(n-1)

def perm(n, r):
    return factorial(n) // factorial(n-r)

def comb(n, r):
    return perm(n, r) // factorial(r)


if __name__ == '__main__':
    # x = factorial(23) / factorial(10) / factorial(6) / factorial(4) / factorial(3)
    # print(x)
    a = comb(17,3) - comb(8,3) - 3*comb(7,3)
    b = 900
    c = comb(16,2) - comb(7,2) -2*comb(6,2)
    print(a+b-c)