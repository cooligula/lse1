#ifndef COMPARATORS_H
#define COMPARATORS_H

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"

#define COMP1_GPIO_BASE     GPIO_PORTC_BASE
#define COMP1_GPIO_PIN      GPIO_PIN_4

#define COMP2_GPIO_BASE     GPIO_PORTC_BASE
#define COMP2_GPIO_PIN      GPIO_PIN_6

#define COMP3_GPIO_BASE     GPIO_PORTF_BASE
#define COMP3_GPIO_PIN      GPIO_PIN_2

#define COMP_PORTC_PINS     (COMP1_GPIO_PIN | COMP2_GPIO_PIN)
#define COMP_PORTF_PINS     (COMP3_GPIO_PIN)

#define INT_GPIOC 18
#define INT_GPIOF 46

/**
 * @brief A class to abstract reading and setting up the hardware comparators.
 *
 * This simplifies reading comparator values to simple A/B/C methods and
 * encapsulates all the complex GPIO and interrupt setup.
 */
class ComparatorController
{
public:
    /**
     * @brief Configures all peripherals, pins, and interrupts for the comparators.
     * @param interruptHandler A pointer to the function to be called on a comparator interrupt.
     */
    void setup(void (*interruptHandler)(void));

    /**
     * @brief Sets the interrupt trigger type for all comparator pins.
     * @param ui32IntType The type of interrupt trigger:
     * - GPIO_RISING_EDGE
     * - GPIO_FALLING_EDGE
     * - GPIO_BOTH_EDGES
     */
    void setInterruptType(uint32_t ui32IntType);

    /**
     * @brief Enables or disables the interrupts on all comparator GPIO pins.
     * @param enable true to enable interrupts, false to disable.
     */
    void enableInterrupts(bool enable);

    /**
     * @brief Reads the state of Comparator 1 (A)
     * @return true if the pin is high, false if low.
     */
    bool readA() {
        // The original code implicitly casts the result to bool (non-zero is true)
        // We do the same here for consistency.
        return MAP_GPIOPinRead(COMP1_GPIO_BASE, COMP1_GPIO_PIN) != 0;
    }

    /**
     * @brief Reads the state of Comparator 2 (B)
     * @return true if the pin is high, false if low.
     */
    bool readB() {
        return MAP_GPIOPinRead(COMP2_GPIO_BASE, COMP2_GPIO_PIN) != 0;
    }

    /**
     * @brief Reads the state of Comparator 3 (C)
     * @return true if the pin is high, false if low.
     */
    bool readC() {
        return MAP_GPIOPinRead(COMP3_GPIO_BASE, COMP3_GPIO_PIN) != 0;
    }

    /**
     * @brief Clears the pending interrupt flags for all comparator pins.
     * This should be called inside the interrupt handler.
     */
    void clearInterrupts() {
        MAP_GPIOIntClear(COMP1_GPIO_BASE, COMP_PORTC_PINS);
        MAP_GPIOIntClear(COMP3_GPIO_BASE, COMP_PORTF_PINS);
    }
};

//=============================================================================
// GLOBAL COMPARATOR CONTROLLER INSTANCE (Declaration)
//=============================================================================

// This makes the 'comparators' object visible to any .cpp file that includes this header.
extern ComparatorController comparators;

#endif // COMPARATORS_H