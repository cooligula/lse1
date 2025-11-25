#ifndef MOSFETS_H
#define MOSFETS_H

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

// Comment out this line to use GPIOs for the low-side
// Uncomment this line to use PWMs for the low-side.
//#define USE_PWM_LOW_SIDE

#ifdef USE_PWM_LOW_SIDE
// CONFIGURATION FOR PWM LOW-SIDE
#define LOW_1B_PWM_BASE      PWM0_BASE
#define LOW_1B_PWM_OUT_BIT   PWM_OUT_4_BIT // PE4 (M0PWM4)
#define LOW_2B_PWM_BASE      PWM0_BASE
#define LOW_2B_PWM_OUT_BIT   PWM_OUT_2_BIT // PB4 (M0PWM2)
#define LOW_3B_PWM_BASE      PWM1_BASE
#define LOW_3B_PWM_OUT_BIT   PWM_OUT_3_BIT // PA7 (M1PWM3)

#else
// CONFIGURATION FOR GPIO LOW-SIDE
#define LOW_1B_GPIO_BASE     GPIO_PORTE_BASE
#define LOW_1B_GPIO_PIN      GPIO_PIN_4
#define LOW_2B_GPIO_BASE     GPIO_PORTB_BASE
#define LOW_2B_GPIO_PIN      GPIO_PIN_4
#define LOW_3B_GPIO_BASE     GPIO_PORTA_BASE
#define LOW_3B_GPIO_PIN      GPIO_PIN_7
#endif

// High-Side Driver Configuration (PWM)
#define HIGH_1A_PWM_BASE     PWM0_BASE
#define HIGH_1A_PWM_OUT_BIT  PWM_OUT_3_BIT
#define HIGH_2A_PWM_BASE     PWM0_BASE
#define HIGH_2A_PWM_OUT_BIT  PWM_OUT_5_BIT
#define HIGH_3A_PWM_BASE     PWM1_BASE
#define HIGH_3A_PWM_OUT_BIT  PWM_OUT_2_BIT

/**
 * @brief Control the 6 MOSFETs for the BLDC driver.
 *
 * This class abstracts the high-side (PWM) and low-side (GPIO or PWM)
 * control into simple Ph1A(bool), Ph1B(bool), etc. methods.
 */
class MosfetController
{
public:
    /**
     * @brief Configures all peripherals, pins, and PWM generators for the MOSFETs.
     * @param pwm_freq The desired PWM frequency in Hz (e.g., 20000).
     * @param duty_cycle The initial duty cycle as a float (e.g., 0.1 for 10%).
     */
    void setup(uint32_t pwm_freq, float duty_cycle);

    /**
     * @brief Control Phase 1 High-Side MOSFET (1A)
     * @param enable true to enable PWM output, false to disable.
     */
    void Ph1A(bool enable) {
        MAP_PWMOutputState(HIGH_1A_PWM_BASE, HIGH_1A_PWM_OUT_BIT, enable);
    }

    /**
     * @brief Control Phase 2 High-Side MOSFET (2A)
     * @param enable true to enable PWM output, false to disable.
     */
    void Ph2A(bool enable) {
        MAP_PWMOutputState(HIGH_2A_PWM_BASE, HIGH_2A_PWM_OUT_BIT, enable);
    }

    /**
     * @brief Control Phase 3 High-Side MOSFET (3A)
     * @param enable true to enable PWM output, false to disable.
     */
    void Ph3A(bool enable) {
        MAP_PWMOutputState(HIGH_3A_PWM_BASE, HIGH_3A_PWM_OUT_BIT, enable);
    }

    /**
     * @brief Control Phase 1 Low-Side MOSFET (1B)
     * @param enable true to turn on (GPIO high or PWM enabled), false to turn off.
     */
    void Ph1B(bool enable) {
    #ifdef USE_PWM_LOW_SIDE
        MAP_PWMOutputState(LOW_1B_PWM_BASE, LOW_1B_PWM_OUT_BIT, enable);
    #else
        uint8_t val = enable ? LOW_1B_GPIO_PIN : 0x00;
        MAP_GPIOPinWrite(LOW_1B_GPIO_BASE, LOW_1B_GPIO_PIN, val);
    #endif
    }

    /**
     * @brief Control Phase 2 Low-Side MOSFET (2B)
     * @param enable true to turn on (GPIO high or PWM enabled), false to turn off.
     */
    void Ph2B(bool enable) {
    #ifdef USE_PWM_LOW_SIDE
        MAP_PWMOutputState(LOW_2B_PWM_BASE, LOW_2B_PWM_OUT_BIT, enable);
    #else
        uint8_t val = enable ? LOW_2B_GPIO_PIN : 0x00;
        MAP_GPIOPinWrite(LOW_2B_GPIO_BASE, LOW_2B_GPIO_PIN, val);
    #endif
    }

    /**
     * @brief Control Phase 3 Low-Side MOSFET (3B)
     * @param enable true to turn on (GPIO high or PWM enabled), false to turn off.
     */
    void Ph3B(bool enable) {
    #ifdef USE_PWM_LOW_SIDE
        MAP_PWMOutputState(LOW_3B_PWM_BASE, LOW_3B_PWM_OUT_BIT, enable);
    #else
        uint8_t val = enable ? LOW_3B_GPIO_PIN : 0x00;
        MAP_GPIOPinWrite(LOW_3B_GPIO_BASE, LOW_3B_GPIO_PIN, val);
    #endif
    }

    /**
     * @brief Update duty cycle value
     * @param duty_cycle The width of the desired pulse (in seconds).
     */
    void updateDuty(float ui32Width) {

        // Pulse Width for all three outputs
        MAP_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3, ui32Width); // 1A (PB5)
        MAP_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_5, ui32Width); // 2A (PE5)
        MAP_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_2, ui32Width); // 3A (PA6)

        // Set pulse widths for low-side if they are PWMs
        #ifdef USE_PWM_LOW_SIDE
        MAP_PWMPulseWidthSet(LOW_1B_PWM_BASE, LOW_1B_PWM_OUT_BIT, ui32Width); // 1B (PE4)
        MAP_PWMPulseWidthSet(LOW_2B_PWM_BASE, LOW_2B_PWM_OUT_BIT, ui32Width); // 2B (PB4)
        MAP_PWMPulseWidthSet(LOW_3B_PWM_BASE, LOW_3B_PWM_OUT_BIT, ui32Width); // 3B (PA7)
        #endif
    }

    /**
     * @brief Turn all 6 MOSFETs off.
     */
    void allOff() {
        Ph1A(false);
        Ph2A(false);
        Ph3A(false);
        Ph1B(false);
        Ph2B(false);
        Ph3B(false);
    }
};

#endif // MOSFETS_H
