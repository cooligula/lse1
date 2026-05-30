#include "driverlib/adc.h"

/**
 * @brief ADCSetup
 * Inicialitza el pin PE3 com una entrada ADC
 */
void ADCSetup(void)
{
    // Enable clocks
    MAP_SysCtlPeripheralEnable (SYSCTL_PERIPH_ADC0);
    MAP_SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOE);

    while (!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0)
            || !MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE))
    {
    }

    // Configure pin
    MAP_GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);

    // Configure conversion sequence
    MAP_ADCSequenceDisable(ADC0_BASE, 3); // Disable SS3 before configuration
    MAP_ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0); // Configure SS3 for processor trigger, highest priority
    // Configure SS3 step 0 to read AIN0
    // ADC_CTL_END marks the end of the sequence
    MAP_ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);
    MAP_ADCSequenceEnable(ADC0_BASE, 3);   // Enable SS3
    MAP_ADCIntClear(ADC0_BASE, 3);
}

/**
 * @brief ADC0_ReadAvg
 * Retorna un valor de l'ADC0 fent el promig de N mesures
 */
uint32_t ADC0_ReadAvg(uint8_t N)
{
    uint32_t sum = 0;
    uint32_t value;
    volatile uint8_t i = 0;
    for (i = 0; i < N; i++)
    {
        // Trigger SS3 conversion
        MAP_ADCProcessorTrigger(ADC0_BASE, 3);

        while (!MAP_ADCIntStatus(ADC0_BASE, 3, false))
        {
        }
        MAP_ADCIntClear(ADC0_BASE, 3);

        // Read the ADC value
        MAP_ADCSequenceDataGet(ADC0_BASE, 3, &value);
        sum += value;  // Use single-read function
    }
    return sum / N;
}
