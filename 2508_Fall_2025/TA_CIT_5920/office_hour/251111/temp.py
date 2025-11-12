import math

def p1():
    a = math.comb(13, 3) * math.comb(9, 3) * math.comb(7, 3)
    b = math.comb(23, 3) * math.perm(20) / math.perm(10) / math.perm(6) / math.perm(4)
    
    print(a==b)

def p2():
    a = 1
    for i in range(10):
        a *= math.comb(60 - 6*i, 6)
        
    # a *= math.perm(11) ** 10
    # print(a)
    
    b = math.perm(60)
    b /= math.perm(6) ** 10
    # b *= math.perm(11) ** 10
    
    print(a)
    print(b)
    print(a==b)

if __name__ == '__main__':
    p2()