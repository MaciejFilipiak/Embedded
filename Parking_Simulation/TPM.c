#include "TPM.h"
void InCap_OutComp_Init()
{
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK; // Włączenie zegara dla portu B
    PORTB->PCR[13] |= PORT_PCR_MUX(2);  // PTB13 jako TPM1_CH1 (PWM Output)

    SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;  // Włączenie zegara dla TPM1
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);  // Wybór źródła zegara (MCGFLLCLK = 41,943,040 Hz)

    TPM1->SC = 0;                       // Wyłączenie TPM1 przed konfiguracją
    TPM1->MOD = 13107;                  // Okres PWM (20 ms = 50 Hz)

    // Ustawienia kanału TPM1_CH1 w trybie PWM
    TPM1->CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK; // High-true pulses
    TPM1->CONTROLS[1].CnV = 983;        // Neutralna pozycja (1,5 ms)

    TPM1->SC = TPM_SC_PS(6) | TPM_SC_CMOD(1); // Preskaler = 64, włączenie licznika TPM1
}
