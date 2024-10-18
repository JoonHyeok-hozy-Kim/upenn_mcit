import math

def factorial(n):
    if n < 0:
        return -1
    if n == 0 or n == 1:
        return 1

    return n * factorial(n-1)


if __name__ == '__main__':
    x = [6, 5, 8, 10, -5]

    for i in x:
        print(hex(factorial(i)))