import numpy as np
import sys

N = int(sys.argv[1])

def generate_c_code(matrix, name):
    s = 'float ' + name + '[' + str(N) + '][' + str(N) + '] = {\n'
    for row  in matrix:
        s += '    {'
        for x in row:
            s += str(x) + ', '
        s = s[:-2]
        s += '},\n'
    s = s[:-2]
    s += '\n};'
    return s

s = 'const int N = ' + str(N) + ';\n\n'

A = np.random.randint(1, 10, size=(N, N))
A = A/10
s += generate_c_code(A, "a")

B = np.random.randint(1, 10, size=(N, N))
B = B/10
s += '\n'
s += generate_c_code(B, "b")

print('\n\nAB =')
C = np.matmul(A,B)
print(C)

f = open("input_matrix.h", "w")
f.write(s)
f.close()

s = 'AB = \n'
s += str(C)

f = open("mat_mul_out.txt", "w")
f.write(s)
f.close()
