import numpy as np
from scipy.optimize import minimize

# Objective function    
def f(z):
    x, y = z
    return x**2 + y**2 - 6*x*y - 4*x - 5*y

# Constraints    
# y <= -(x-2)^2+4
cons1 = {'type':'ineq','fun':lambda z: 4 -(z[0]-2)**2 - z[1]} 
# y >= -x+1
cons2 = {'type':'ineq','fun':lambda z: z[0] + z[1] - 1} 

initial_point = [1, 3]

res = minimize(f, initial_point, constraints=[cons1,cons2], method='SLSQP')
print(f'Optimal Point : {res.x}, Value : {res.fun}')

cons1_changed = {'type':'ineq','fun':lambda z: 4.1 -(z[0]-2)**2 - z[1]} 

res = minimize(f, initial_point,constraints=[cons1_changed,cons2], method='SLSQP')
print(f'Optimal Point : {res.x}, Value : {res.fun}')