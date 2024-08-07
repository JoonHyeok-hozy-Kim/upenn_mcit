from random import randint

def maximum(array):
    if len(array) == 0:
      return None
    elif len(array) == 1:
      return array[0]
      
    return max(array[0], maximum(array[1:]))


if __name__ == '__main__':
   A = [randint(0, 100) for _ in range(5)]
   print(A, maximum(A))