import numpy as np

#%% Specs
Vin = np.array([10, 12, 14])
Vout = 5 # V
Pnom = 5 # W
fPWM = 32000 # Hz
dI_rel = 0.05
dVo = max(Vin / (2**8)) # V
f_samp = 1 / 100e-6 # Hz

#%% 1) Duty
D = Vout / Vin

#%% 2) Corrent
Inom = Pnom / Vout

#%% 3) Inductancia minima
Lmin = ( Vout*(1-D) ) / ( 2*Inom*fPWM )
RL = Vout / Inom

#%% 4) Inductancia
dI = dI_rel * Inom
L_dI = ( Vout*(1-D) ) / ( dI*fPWM )
L = max(max(L_dI), max(Lmin))

#%% 5) Condensador
C_dVo = dI / ( 8*dVo*fPWM )

#%% Dinamica natural
f0 = 1 / ( 2*np.pi*np.sqrt(L*C_dVo) )

k = 1
L_scaled = L
C_scaled = C_dVo

# Busqueda iterativa
while (40*f0 > f_samp):
    L_scaled = L*k
    C_scaled = C_dVo*k
    f0 = 1 / ( 2*np.pi*np.sqrt(L_scaled*C_scaled) )
    k += 0.001
   
print(f"Vin = {Vin} V") 
print(f"Duty = {D}")
print(f"L = {L_scaled} H")
print(f"C = {C_scaled} F")
print(f"f0 = {f0} Hz, fsamp = {f_samp} Hz, k = {k}")

#%% 7) Nombre de voltes
mu_r = 4000
mu0 = 4*np.pi*1e-7
L = 5e-3 # H

din = 14e-3
dout = 26.7e-3
h = 10e-3

S = (dout - din)/2 * h
r = (dout + din)/2 /2

N = np.sqrt( (2*np.pi*L) / (mu0*mu_r*h*np.log(dout/din)) )
print(f"N_voltes = {N}")