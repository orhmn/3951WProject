/*
 * File:   main.c
 * Author: sethrosenberg
 *
 * Created on April 12, 2024, 11:29 AM
 */

#include "xc.h"
#include "LCD.h"
#include "internal_data_buffer.h"
#include "string.h"
#include "stdint.h"
#include <stdio.h>
#include <math.h>

// CW1: FLASH CONFIGURATION WORD 1 (see PIC24 Family Reference Manual 24.1)
#pragma config ICS = PGx1          // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config FWDTEN = OFF        // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config GWRP = OFF          // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF           // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF        // JTAG Port Enable (JTAG port is disabled)


// CW2: FLASH CONFIGURATION WORD 2 (see PIC24 Family Reference Manual 24.1)
#pragma config I2C1SEL = PRI       // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF       // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = ON       // Primary Oscillator I/O Function (CLKO/RC15 functions as I/O pin)
#pragma config FCKSM = CSECME      // Clock Switching and Monitor (Clock switching is enabled, 
                                       // Fail-Safe Clock Monitor is enabled)
#pragma config FNOSC = FRCPLL      // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))

char output[20];

#define BUFSIZE 1024 
#define NUMSAMPLES 20

void delay_ms(unsigned int ms) {
    while (ms-- > 0) {
        asm("repeat #15998");
        asm("nop");
    }
}

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

void adc_init()
{
    TRISAbits.TRISA1 = 1;   // sets to input
    TRISAbits.TRISA0 = 1;   // sets to input
    AD1PCFGbits.PCFG0 = 0;  // setup I/O
    AD1PCFGbits.PCFG1 = 0;  // setup I/O 
    AD1CHSbits.CH0SA = 0b0001;
    
    AD1CON2bits.VCFG = 0b001;   // Uses External Pin (2.5V) and AVSS (0V) as max/min
    
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
    //float val = ((2.5 / 1024) * (float)getAvg());
    float voltage_reading = roundf((float) getAvg() * 100) / 100;
    float val = (((2.5/1024)*voltage_reading) - (0.3369))/(0.0020606);
    val = roundf(val);
    sprintf(output, "%3.0f g", val);
    lcd_printStr(output);
}

void __attribute__((interrupt, auto_psv)) _ADC1Interrupt(void) {
    _AD1IF = 0;
    putVal(ADC1BUF0);
}

void setup(){
    CLKDIVbits.RCDIV = 0; //set frequency to 16 MHz
    AD1PCFG = 0xffff;   //set all pins to digital
}

int  main(){
    setup();
    lcd_setup();
    timer2_init();
    adc_init();
    initBuffer();
    while(1){
    }
    return 0;
}
          

