R = 45e3
C = 4.7e-9
tau = R*C
T = 4.96*tau # 0-99.3%
f = 1/T
f_motor = 15.3 # Hz/V
Vcc = 12
n_pols = 7
D = f / (n_pols * f_motor * Vcc) # 6 canvis de fase
print(D)