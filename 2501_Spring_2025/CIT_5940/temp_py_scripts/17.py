def power_lsb(n):
    M = {}
    for i in range(10):
        M[i] = None
    i = 0
    while i <= n:
        ii = i*i
        ii %= 10
        if ii in M:
            print('i: {}, ii: {}'.format(i, ii))
            del M[ii]
        if len(M) == 0:
            return i
        i += 1
    
    for k in M.keys():
        print('{}, '.format(k), end="")
    return None



if __name__ == '__main__':
    print(power_lsb(1000))