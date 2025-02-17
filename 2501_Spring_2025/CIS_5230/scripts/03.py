
def bolt_on_equal_test(e1, e2, filp_coin=0.5):
    return (e2-e1) / (filp_coin - e1)

def bolt_on_gamma_test(e1, e2, gamma, flip_coin=0.5):
    return (e2-e1-gamma) / (flip_coin - e1)

def bolt_on_overall_error(e1, e2, gamma, p1, flip_coin=0.5):
    return -p1 * gamma + e2

if __name__ == '__main__':
    print(1)
    print(bolt_on_equal_test(0.05, 0.2))

    print(bolt_on_overall_error(0.05, 0.2, 0.03, 0.7))