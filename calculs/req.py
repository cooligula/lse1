import numpy as np

def parallel_resistance(R1, R2):
    """
    Calculate the equivalent resistance of two resistors in parallel.
    Handles the case where the sum is zero to prevent division by zero.
    """
    if (R1 + R2) == 0:
        return 0
    return (R1 * R2) / (R1 + R2)

def parallel_resistance_three(R1, R2, R3):
    """Calculate the equivalent resistance of three resistors in parallel."""
    # R_eq = 1 / ((1/R1) + (1/R2) + (1/R3))
    
    # Handle non-zero denominators
    sum_reciprocals = 0
    if R1 != 0:
        sum_reciprocals += 1 / R1
    if R2 != 0:
        sum_reciprocals += 1 / R2
    if R3 != 0:
        sum_reciprocals += 1 / R3
    
    return 1 / sum_reciprocals if sum_reciprocals != 0 else 0


def equivalent_resistor(R_m_A, R_m_B, R_m_C, R_star, R_div_H, R_div_L):
    """
    Calculates R_eq based on the NEW equation: 
    R_eq = ((R_m^A + (R_m^B || R_m^C)) || (R_star + (R_star || R_star || (R_div^H + R_div^L)))) + R_div^H + R_div^L
    """

    # --- 1. Calculate R_branch1 (The left branch of the main parallel section) ---
    # (R_m^A + (R_m^B || R_m^C))
    R_B_parallel_C = parallel_resistance(R_m_B, R_m_C)
    R_branch1 = R_m_A + R_B_parallel_C

    # --- 2. Calculate R_branch2 (The right branch of the main parallel section) ---
    # R_{star} + (R_{star} || R_{star} || (R_{div}^H + R_{div}^L))
    
    # Inner series sum: R_div^H + R_div^L
    R_div_series = R_div_H + R_div_L
    
    # Three-way parallel: (R_star || R_star || R_div_series)
    R_three_parallel = parallel_resistance_three(R_star, R_star, R_div_series)

    # R_branch2 is R_star in series with the three-way parallel term
    R_branch2 = R_star + R_three_parallel

    # --- 3. Calculate R_parallel_core (The main parallel section) ---
    # R_branch1 || R_branch2
    R_parallel_core = parallel_resistance(R_branch1, R_branch2)

    # --- 4. Final equivalent resistance (R_eq) ---
    # R_eq = R_parallel_core + R_div^H + R_div^L (Final series addition)
    R_eq = R_parallel_core + R_div_H + R_div_L
    
    return R_eq

# Example values (Kept the same for comparison)
R_m_A = 0.8     # Ohms
R_m_B = 0.8     # Ohms
R_m_C = 0.8     # Ohms
R_star = 1e3    # 1 kOhm
R_div_H = 33e3  # 33 kOhm
R_div_L = 12e3  # 12 kOhm

# Calculate equivalent resistor
R_eq_value = equivalent_resistor(R_m_A, R_m_B, R_m_C, R_star, R_div_H, R_div_L)
print(f"The equivalent resistance R_eq is: {R_eq_value:.2f} Ohms")

C = 4.7e-9

fmax = 1287.72 # Hz
f = 1 / (2*np.pi*R_eq_value*C)

R = 45e3
tau = R*C
T = 4.96*tau # 0-99.3%
f = 1/T
f_motor = 15.3 # Hz/V
Vcc = 12
n_pols = 7
D = f / (n_pols * f_motor * Vcc) # 6 canvis de fase
print(f"ftall = {f}, duty = {D}, fmax = {D*fmax}")

print(f)