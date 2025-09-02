import numpy as np
from scipy.optimize import minimize

# Objective function    
def f(z):
    x, y = z
    return x**2+y**2-6*x*y-4*x-5*y

# Constraints    
# y <= -(x-2)^2+4
cons1 = {'type':'ineq','fun':lambda z: -(z[1] - (-(z[0]-2)**2+4))} 
# y >= -x+1
cons2 = {'type':'ineq','fun':lambda z: z[1] + z[0] -1} 

res = minimize(f,[0,0],constraints=[cons1,cons2])
print(res.x,res.fun)