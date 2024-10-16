import math

def factorial(n):
    if n == 0 or n == 1:
        return 1

    return n * factorial(n-1)


if __name__ == '__main__':
    X = pow(2, 16) - 1

    print(bin(X))
    print(bin(factorial(8)))