#include "MKL05Z4.h"
#include "lcd1602.h"
#include "leds.h"
#include "klaw.h"
#include "TPM.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Definicje dla HC-SR04
#define TRIG_PIN_MASK   (1 << 1) 
#define ECHO_PIN_MASK   (1 << 5) 


volatile uint8_t S1_press = 0;   
volatile uint32_t echo_start = 0;
volatile uint32_t echo_end = 0;
volatile float distance = 0;

void Reset_TPM1(void)
{
    SIM->SCGC6 &= ~SIM_SCGC6_TPM1_MASK; 
    SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;  
    TPM1->SC = 0;                       
}

// Funkcja do inicjalizacji PWM
void PWM_Init(void)
{
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK; 
    SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;  
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);  

    TPM1->SC = 0;                      
    TPM1->MOD = 13107;                

    PORTB->PCR[13] = PORT_PCR_MUX(2);  

    TPM1->CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK; 
    TPM1->CONTROLS[1].CnV = 983;       

    TPM1->SC = TPM_SC_PS(6) | TPM_SC_CMOD(1); 
}
// Funkcja do ustawiania pozycji serwomechanizmu
void Servo_SetAngle(int angle)
{
    if (angle < 0) angle = 0;
    if (angle > 180) angle = 180;

    uint32_t pulse_width = 1000 + ((angle * 1000) / 180); 
    TPM1->CONTROLS[1].CnV = (pulse_width * TPM1->MOD) / 20000; 
}

// Inicjalizacja HC-SR04
void HC_SR04_Init(void) {
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;       

    // Ustaw TRIG jako wyjście
    PORTB->PCR[1] = PORT_PCR_MUX(1);          
    PTB->PDDR |= TRIG_PIN_MASK;
    PTB->PCOR |= TRIG_PIN_MASK;               

    // Ustaw ECHO jako wejście
    PORTB->PCR[5] = PORT_PCR_MUX(1) | PORT_PCR_IRQC(0b1011); 
    PTB->PDDR &= ~ECHO_PIN_MASK;
}

// Funkcja do pomiaru odległości
void HC_SR04_Trigger(void) {
    PTB->PSOR |= TRIG_PIN_MASK;
    DELAY(10);                                
    PTB->PCOR |= TRIG_PIN_MASK;             
}

// Funkcja obliczająca odległość
void HC_SR04_CalculateDistance(void) {
    uint32_t echo_time = echo_start - echo_end; 
    distance = (float)echo_time* 0.00034 ;  
}

// Obsługa przerwania dla ECHO
void PORTB_IRQHandler(void) {
    if (PORTB->ISFR & ECHO_PIN_MASK) {
        if (PTB->PDIR & ECHO_PIN_MASK) { 
				  SysTick->VAL = 0; 	// ECHO stan wysoki
            echo_start = SysTick->VAL;        
        } else {                              
            echo_end = SysTick->VAL;          
            HC_SR04_CalculateDistance();
        }
        PORTB->ISFR |= ECHO_PIN_MASK;
NVIC_ClearPendingIRQ(PORTB_IRQn);				
    }
} 

// Obsługa przycisku S1
void PORTA_IRQHandler(void) {
    if (PORTA->ISFR & S1_MASK) {
        if (!(PTA->PDIR & S1_MASK)) {         
            DELAY(100);
            if (!(PTA->PDIR & S1_MASK)) {     
               if(!S1_press)
											{
												S1_press=1;
											}
            }
        }
        PORTA->ISFR |= S1_MASK;
NVIC_ClearPendingIRQ(PORTA_IRQn);				
    }
}

// Funkcja inicjalizacji systemu
void System_Init(void) {
    LED_Init();
    LCD1602_Init();
    LCD1602_Backlight(TRUE);
    HC_SR04_Init();
    Klaw_Init();
    Klaw_S2_4_Int();

    NVIC_EnableIRQ(PORTA_IRQn);               
    NVIC_EnableIRQ(PORTB_IRQn);              

    SysTick_Config(SystemCoreClock/10); 
}



     

		int main(void) {
    System_Init();
    Reset_TPM1();
    PWM_Init();

    LCD1602_SetCursor(0, 0);
    LCD1602_Print("Brama gotowa");

    while (1) {
        HC_SR04_Trigger();

        char buf[16];
        memset(buf, 0, sizeof(buf)); 
        sprintf(buf, "Dist: %.1f cm", distance);

        LCD1602_SetCursor(0, 1);
        LCD1602_Print("                "); 
        LCD1602_SetCursor(0, 1);
        LCD1602_Print(buf);

        if (distance > 0 && distance < 3.0) {
            PTB->PCOR |= LED_R_MASK;
            PTB->PSOR |= LED_G_MASK;
            Servo_SetAngle(180); 
        } else {
            PTB->PCOR |= LED_G_MASK;
            PTB->PSOR |= LED_R_MASK;
            Servo_SetAngle(0); 
        }

        for (volatile uint32_t i = 0; i < 60000; i++) __NOP();
    }
}