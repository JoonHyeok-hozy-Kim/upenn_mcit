import math

def sign_calculator(f: float) -> str:
    return "1" if f<0 else "0"

def decimal_integer_to_binary(n, pop_MSB=False):
    result_list = []
    while n > 0:
        result_list.append(str(n%2))
        n //= 2

    if pop_MSB:
        result_list.pop()   # MSB will not be used!
    
    return ''.join(result_list[::-1])

def decimal_fraction_to_binary(f, digit_limit=math.inf):
    result_list = []
    
    while True:
        precision_len = len(str(f))-2
        f *= 2
        if f >= 1:
            result_list.append("1")
            f -= 1
        else:
            result_list.append("0")
        
        # print(f, result_list)
        f = round(f, precision_len+1)

        digit_limit -= 1
        if (f == 0 or digit_limit == 0): break
    
    return ''.join(result_list)

def fraction_calculator(f):
    f = abs(f)
    left_decimal_point = decimal_integer_to_binary(int(f), True)
    # print(left_decimal_point)

    original_exponent = len(left_decimal_point) # MSB has been already popped!
    # print(original_exponent)

    right_decimal_point = decimal_fraction_to_binary(f-int(f), 23-original_exponent)
    # print(right_decimal_point)

    return original_exponent, left_decimal_point + right_decimal_point


def exponent_binary_string(n):
    ''' Return the 8-bit binary exponent '''
    return decimal_integer_to_binary(n + 127)


def IEEE_fraction_calculator(f):
    result_list = []
    result_list.append(sign_calculator(f))

    original_exponent, unpadded_fraction = fraction_calculator(f)

    result_list.append(exponent_binary_string(original_exponent))

    result_list.append(unpadded_fraction)
    # print(len(unpadded_fraction))
    for _ in range(23-len(unpadded_fraction)):
        result_list.append("0")

    return ''.join(result_list)



if __name__ == '__main__':
    ''' Test decimal_integer_to_binary'''
    # print(decimal_integer_to_binary(55))
    # print(decimal_integer_to_binary(132))

    # print(IEEE_fraction_calculator(3.75))
    # print(IEEE_fraction_calculator(55+23/64))
    print(IEEE_fraction_calculator(3.1415927))

    '''
    0
    10000000
    1001 0010 0001 1111 1011 010

    01000000010010010000111111011010
    01000000010010010000111111011011
    '''

    print(IEEE_fraction_calculator(pow(2,128)))

    print(pow(2,15))