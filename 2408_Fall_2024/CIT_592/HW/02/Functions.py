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
    print(1000000000/(50**6))