volatile uint16_t faza = 0;
volatile int32_t Sinus[20] = { 0, 634, 1181, 1570, 1789, 1829, 1689, 1379, 919, 339, -339, -919, -1379, -1689, -1829, -1789, -1570, -1181, -634, 0 };
volatile uint16_t ster = 0;

void SysTick_Handler(void)
{ 
    if (ster < 8000) { // Sygnalizacja buzzera
        DAC_Load_Trig((Sinus[faza] / 100) + 0x0800);
        faza++;
        if (faza == 20) {
            faza = 0;
        }
    } else {
        DAC_Load_Trig(0); // Wy³¹cz sygna³
    }

    ster++;
    if (ster == 16000) {
        ster = 0; // Reset licznika
    }
}
