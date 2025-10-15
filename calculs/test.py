import time
import math

# Define the values of x, y, z
x = 2
y = 3
z = 4

iterations = 1000000

# Time the native exponentiation operator
start_native = time.time()
for i in range(iterations):
    result_native = x ** (y ** z)
end_native = time.time()
native_time = end_native - start_native

# Time the math.pow function
start_math_pow = time.time()
for i in range(iterations):
    result_math_pow = math.pow(x, math.pow(y, z))
end_math_pow = time.time()
math_pow_time = end_math_pow - start_math_pow

# Output the results
print(f"Native ** result: {result_native}, Time taken: {native_time:.10f} seconds")
print(f"math.pow result: {result_math_pow}, Time taken: {math_pow_time:.10f} seconds")
print(f"Ratio: {math_pow_time/native_time*100}")