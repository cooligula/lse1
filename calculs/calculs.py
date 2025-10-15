import numpy as np

#%% Cboot i temps 
Dmax = 1 # (qüestionable)
Qg = 65e-9 # 65 nC
Ihbs = 50e-6 # 50 uA
Ihb = 100e-6 # 100 uA
fsw = 20e3 # 16 Mhz (qüestionable)

Vdd = 10 # V
Vdh = 1 # V
Vhbl = 4.7 # V

Vboot = 12 # V

Qtotal = Qg + Ihbs * Dmax/fsw + Ihb/fsw
dVhv = Vdd - Vdh - Vhbl

Cboot = Qtotal/dVhv
print(f"Cboot = {Cboot}")

Idiode = 150e-3 # 150 mA
IVbs = np.array([20, 60, 100])*1e-6 # Min, typ, max

tUp = Vboot * Cboot / Idiode
tDown = Vboot * Cboot / IVbs
print(f"T_up = {tUp}, T_down = {tDown}")
#%% Consums driver i MOSFET