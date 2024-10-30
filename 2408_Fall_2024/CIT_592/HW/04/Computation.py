from itertools import permutations

def trial_1(A):    
    P = permutations(A)
    val = 0
    s1 = []
    s2 = []
    cnt = 0
    for p in P:
        cnt += 1
        # print(p)
        v = (p[0] * 10 + p[1]) * (p[2] * 10 + p[3])
        val += v
        s1.append('({}{}'.format(p[0], p[1]))
        s1.append('\\times')
        s1.append('{}{})'.format(p[2], p[3]))
        s1.append('+')
        s2.append('{}'.format(v))
        s2.append('+')
    val /= cnt
    s1.pop()
    s2.pop()
    print(''.join(s1))
    print(''.join(s2))
    print(val)

def trial_2(A):
    M = {}
    for p in permutations(A):
        v = (p[0] * 10 + p[1]) * (p[2] * 10 + p[3])
        if v not in M:
             M[v] = 0
        M[v] += 1
    
    for k in M.keys():
         print(k,M[k])
    
          


if __name__ == '__main__':
    A = [1,2,3,4]
    trial_1(A)
    # print((408 + 516 + 312 + 546 + 322+448 + 714 + 903 + 322 + 943+312 + 744 + 744 + 1302 + 448+1312 + 408 + 714 + 943 + 1312+546 + 1302 + 516 + 903)/24)
