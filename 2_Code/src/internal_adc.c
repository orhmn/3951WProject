#include <xc.h>
#include "stdint.h"
#include <stdio.h>
#include "internal_adc.h"

char output[20];

#define BUFSIZE 1024 
#define NUMSAMPLES 128

void timer2_init()
{
    T2CON = 0;
    T2CONbits.TCKPS = 0b11;
    PR2 = 6275; //100 milliseconds
    TMR2 = 0;
    
    _T2IE = 1;
    _T2IF = 0;
    
    T2CONbits.TON = 1;
}

/**
 * Delays the program execution for a specified duration.
 * 
 * This function delays the program execution for the specified duration in milliseconds.
 * 
 * @param delay_in_ms: The duration of the delay in milliseconds.
 * @return None
 */
void delay_ms_internal(unsigned int ms) {
    while (ms-- > 0) {
        asm("repeat #15998");
        asm("nop");
    }
}

void adc_init()
{
    TRISAbits.TRISA0 = 1;   // sets to input
    
    AD1PCFGbits.PCFG0 = 0;  // setup I/O 
    
    AD1CON2bits.VCFG = 0b000;   // Uses AVDD (3.3V) and AVSS (0V) as max/min
    AD1CON3bits.ADCS = 0b00000001;   // TAD = 2 * 62.5 ns
    AD1CON1bits.SSRC = 2;   // sample on timer3 events (page 5 of section 17)
    AD1CON3bits.SAMC = 0b00001;   // auto sample time bit (page 9 of section 17)
    AD1CON1bits.FORM = 0;   // data output form is unsigned int
                            //results in 0V = 0b0000000000, 3.3V = 1111111111 
 
    AD1CON1bits.ASAM = 1;   // Sampling begins when SAMP bit is set
    AD1CON2bits.SMPI = 0;   
    AD1CON1bits.ADON = 1;   // turn on ADC
    
    _AD1IF = 0; // clear IF
    _AD1IE = 1; // enable interrupts
    
    TMR3 = 0;   // timer 3 setup
    T3CON = 0;
    T3CONbits.TCKPS = 0b10;
    PR3 = 15624;
    T3CONbits.TON = 1;
    
}

void __attribute__((interrupt, auto_psv)) _T3Interrupt(void) {
    _T3IF = 0;
}

void __attribute__((interrupt, auto_psv)) _T2Interrupt(void) {
    _T2IF = 0;
    float val = (3.3/1024)*getAvg();
    sprintf(output, "%6.4f V", val);
    lcd_printStr(output);
}

void __attribute__((interrupt, auto_psv)) _ADC1Interrupt(void) {
    _AD1IF = 0;
    putVal(ADC1BUF0);
}
