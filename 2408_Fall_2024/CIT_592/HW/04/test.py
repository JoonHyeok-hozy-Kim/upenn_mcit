def logical_right_shift(x, n):
  return (x >> n) & 0xFFFFFFFF  # 32-bit mask

if __name__ == '__main__':
  

    x = 0b10101010
    n = 2
    print(bin(x))
    result = logical_right_shift(x, n)
    print(bin(result))

    print(bin(x>>2))