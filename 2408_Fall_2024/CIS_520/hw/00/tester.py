def f1(n):
    return (1-1/n) ** n

if __name__ == '__main__':
    j = 1
    for i in range(10):
        print(f1(j))
        j *= 2