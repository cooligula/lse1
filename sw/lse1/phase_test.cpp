#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "mosfets.h"
#include "comparators.h"

#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

#define STOP 7
#define S1 0
#define S2 1
#define S3 2
#define S4 3
#define S5 4
#define S6 5

#define RISE 0
#define FALL 1

// Controller instance.
MosfetController mosfets;
ComparatorController comps;

// Comencem apagant tots els MOSFETs
int state = STOP;

uint8_t RiseFall = 0;

const int N = 10;

float freqfase[N+1];
float dutys[N+1];

/**
 * @brief updateClosedLoopState
 */
/*
void updateClosedLoopState() {//(bool A, bool B, bool C) {

    mosfets.1A(A && !B);
    mosfets.2A(B && !C);
    mosfets.3A(C && !A);
    
    mosfets.1B(!A && B);
    mosfets.2B(!B && C);
    mosfets.3B(!C && A);
}
*/

/**
 * @brief updateState
 * Configura sortides de GPIOs pels sis estats d'alimentació del motor
 */
void updateState(int state){
    switch (state)
    {
        case STOP:
            mosfets.allOff();
            break;
            
        case S1:
            mosfets.1A(true);
            mosfets.2A(false);
            mosfets.3A(false);
            
            mosfets.1B(false);
            mosfets.2B(true);
            mosfets.3B(false);
            break;
            
        case S2:
            mosfets.1A(true);
            mosfets.2A(false);
            mosfets.3A(false);
            
            mosfets.1B(false);
            mosfets.2B(false);
            mosfets.3B(true);
            break;
            
        case S3:
            mosfets.1A(false);
            mosfets.2A(true);
            mosfets.3A(false);
            
            mosfets.1B(false);
            mosfets.2B(false);
            mosfets.3B(true);
            break;
            
        case S4:
            mosfets.1A(false);
            mosfets.2A(true);
            mosfets.3A(false);
            
            mosfets.1B(true);
            mosfets.2B(false);
            mosfets.3B(false);
            break;
            
        case S5:
            mosfets.1A(false);
            mosfets.2A(false);
            mosfets.3A(true);
            
            mosfets.1B(true);
            mosfets.2B(false);
            mosfets.3B(false);
            break;
            
        case S6:
            mosfets.1A(false);
            mosfets.2A(false);
            mosfets.3A(true);
            
            mosfets.1B(false);
            mosfets.2B(true);
            mosfets.3B(false);
            break;
    }
}


void testPhase(int phase, int on){

    switch (phase){
        case 1:
            mosfets.1A(on);
            mosfets.1B(!on);
            break;

        case 2:
            mosfets.2A(on);
            mosfets.2B(!on);
            break;

        case 3:
            mosfets.3A(on);
            mosfets.3B(!on);
            break
    }

}


//=============================================================================
// MAIN FUNCTION
//=============================================================================
int
main(void)
{

    // Set the clocking to run directly from the external crystal/oscillator.
    MAP_SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);

    // Set the PWM clock to the system clock.
    MAP_SysCtlPWMClockSet(SYSCTL_PWMDIV_1);

    // MOSFET Setup
    float initial_duty = 0.4;
    int pwm_freq = 20e3;
    mosfets.setup(pwm_freq, initial_duty);
    float phase_period = 1.4e-3;

    // Disable interrupts, setup and reenable interrupts
    MAP_IntMasterDisable();
    comps.setup(GPIOInt);

    mosfets.allOff()

    // Phase select
    phase_test = 1;
    phase_on = 0;

    // Begin phase test loop

    while(1){

        testPhase(phase_test, phase_on);
        phase_on ^= 1;

        // Wait half a phase period
        MAP_SysCtlDelay((MAP_SysCtlClockGet() * phase_period) / 6);

        mosfets.allOff();

        // Wait half a phase period
        MAP_SysCtlDelay((MAP_SysCtlClockGet() * phase_period) / 6);
        
    }

    
}
