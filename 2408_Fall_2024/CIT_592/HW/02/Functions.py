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
    x = 2 * math.pow(10, 4) * perm(10,3)
    print(x)