#include "comparators.h"

void ComparatorController::setup(void (*interruptHandler)(void))
{
    // Enable peripherals for Comparators
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // Wait for comparator peripherals to be ready
    while(!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC) ||
          !MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));

    // Enable comparator GPIOs as inputs
    MAP_GPIOPinTypeGPIOInput(COMP1_GPIO_BASE, COMP1_GPIO_PIN); // Comp1
    MAP_GPIOPinTypeGPIOInput(COMP2_GPIO_BASE, COMP2_GPIO_PIN); // Comp2
    MAP_GPIOPinTypeGPIOInput(COMP3_GPIO_BASE, COMP3_GPIO_PIN); // Comp3

    // Disable interrupts while configuring
    MAP_GPIOIntDisable(COMP1_GPIO_BASE, COMP_PORTC_PINS);
    MAP_GPIOIntDisable(COMP3_GPIO_BASE, COMP_PORTF_PINS);

    // Clear any existing pending interrupts
    MAP_GPIOIntClear(COMP1_GPIO_BASE, COMP_PORTC_PINS);
    MAP_GPIOIntClear(COMP3_GPIO_BASE, COMP_PORTF_PINS);

    // Register the provided handler function for both ports
    MAP_GPIOIntRegister(COMP1_GPIO_BASE, interruptHandler);
    MAP_GPIOIntRegister(COMP3_GPIO_BASE, interruptHandler);

    // Set interrupts to trigger on both rising and falling edges by default
    MAP_GPIOIntTypeSet(COMP1_GPIO_BASE, COMP_PORTC_PINS, GPIO_BOTH_EDGES);
    MAP_GPIOIntTypeSet(COMP3_GPIO_BASE, COMP_PORTF_PINS, GPIO_BOTH_EDGES);

    // Enable interrupts on the GPIO pins
    MAP_GPIOIntEnable(COMP1_GPIO_BASE, COMP_PORTC_PINS);
    MAP_GPIOIntEnable(COMP3_GPIO_BASE, COMP_PORTF_PINS);

    // Enable the master interrupt for each port in the interrupt controller
    MAP_IntEnable(INT_GPIOC); // 18
    MAP_IntEnable(INT_GPIOF); // 46
}

void ComparatorController::disableAllInterrupts()
{
    MAP_GPIOIntDisable(COMP1_GPIO_BASE, COMP_PORTC_PINS); // Disables 1 and 2
    MAP_GPIOIntDisable(COMP3_GPIO_BASE, COMP_PORTF_PINS); // Disables 3
}

void ComparatorController::configComparatorInt(ComparatorID id, bool enable, uint32_t edgeType)
{
    uint32_t gpioBase;
    uint32_t gpioPin;

    // Map ID to the specific hardware definitions
    switch(id)
    {
        case COMP_1:
            gpioBase = COMP1_GPIO_BASE; // GPIO_PORTC_BASE
            gpioPin  = COMP1_GPIO_PIN;  // GPIO_PIN_4
            break;
        case COMP_2:
            gpioBase = COMP2_GPIO_BASE; // GPIO_PORTC_BASE
            gpioPin  = COMP2_GPIO_PIN;  // GPIO_PIN_6
            break;
        case COMP_3:
            gpioBase = COMP3_GPIO_BASE; // GPIO_PORTF_BASE
            gpioPin  = COMP3_GPIO_PIN;  // GPIO_PIN_2
            break;
        default:
            return; // Invalid ID
    }

    // Configure the edge type
    // We only set the type if we are enabling, or we can set it regardless.
    // Setting it regardless is safer for the next time it is enabled.
    MAP_GPIOIntTypeSet(gpioBase, gpioPin, edgeType);

    // 3. Enable or Disable the specific pin interrupt
    if (enable)
    {
        MAP_GPIOIntEnable(gpioBase, gpioPin);
    }
    else
    {
        MAP_GPIOIntDisable(gpioBase, gpioPin);
    }
}


/**
 * @brief Sets the interrupt trigger type for all comparator pins.
 */
 /*
void ComparatorController::setInterruptType(uint32_t ui32IntType)
{
    // Set the interrupt type for both ports
    MAP_GPIOIntTypeSet(COMP1_GPIO_BASE, COMP_PORTC_PINS, ui32IntType);
    MAP_GPIOIntTypeSet(COMP3_GPIO_BASE, COMP_PORTF_PINS, ui32IntType);
}
*/
/**
 * @brief Enables or disables the interrupts on all comparator GPIO pins.
 */
 /*
void ComparatorController::enableInterrupts(bool enable)
{
    if (enable)
    {
        // Enable interrupts on the GPIO pins
        MAP_GPIOIntEnable(COMP1_GPIO_BASE, COMP_PORTC_PINS);
        MAP_GPIOIntEnable(COMP3_GPIO_BASE, COMP_PORTF_PINS);
    }
    else
    {
        // Disable interrupts on the GPIO pins
        MAP_GPIOIntDisable(COMP1_GPIO_BASE, COMP_PORTC_PINS);
        MAP_GPIOIntDisable(COMP3_GPIO_BASE, COMP_PORTF_PINS);
    }
}
*/