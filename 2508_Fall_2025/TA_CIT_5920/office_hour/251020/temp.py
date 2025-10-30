import math

if __name__ == "__main__":
    # Books
    num_python = 10
    num_math = 6
    num_java = 4
    # Shelves
    num_shelves = 4
    
    # Your sol
    your_sol = math.comb(num_python+num_shelves-1, num_shelves-1) * \
               math.comb(num_math+num_shelves-1, num_shelves-1) * \
               math.comb(num_java+num_shelves-1, num_shelves-1)
    print(your_sol)
    
    # Suggested method
    # 1. Arrangement using stars and bars method
    step1_comb = math.comb(
        num_python + num_math + num_java + num_shelves - 1,
        num_shelves - 1
    )    
    
    # 2. Permutation of books
    step2_perm = math.factorial(num_python + num_math + num_java)
    step2_perm /= math.factorial(num_python)
    step2_perm /= math.factorial(num_math)
    step2_perm /= math.factorial(num_java)    
    
    suggested_method = step1_comb * step2_perm
    print(suggested_method)