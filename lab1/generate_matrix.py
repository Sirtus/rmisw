import numpy as np
import random

N = 3

def generate_C_code(matrix, name):
    s = 'int ' + name + '[' + str(N) + '][' + str(N) + '] = {\n'
    for row  in matrix:
        s += '  {'
        for x in row:
            s += str(x) + ', '
        s = s[:-2]
        s += '},\n'
    s = s[:-2]
    s += '\n};'
    print(s)

A = np.random.randint(1, 10, size=(N, N))
generate_C_code(A, "a")
B = np.random.randint(1, 10, size=(N, N))
generate_C_code(B, "b")

print('\n\nAB =')
C = np.matmul(A,B)
print(C)
