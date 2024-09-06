import numpy as np
import math

A = np.array([[1,2,3],
              [3,2,1],
              [2,1,3]])

A_eig_val, A_eig_vec = np.linalg.eig(A)

print(A_eig_val)
print(A_eig_vec)

print(math.sqrt(2))