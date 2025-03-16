/*
 * File:   main.c
 * Author: sethrosenberg
 *
 * Created on April 12, 2024, 11:29 AM
 */

#include "xc.h"
#include "SPI.h"
#include "data_buffer.h"
#include "LCD.h"
#include "string.h"
#include "stdint.h"
#include <stdio.h>

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

volatile uint16_t data = 0;
volatile int length = 0;
volatile static char ADC_val[16];
volatile char binary_val[17];

void delay_ms(unsigned int ms) {
    while (ms-- > 0) {
        asm("repeat #15998");
        asm("nop");
    }
}

void numToBinaryString(uint16_t num)
{
    for (int i = 0; i < 16; i++) {
            binary_val[i] = (num & (1 << (15 - i))) ? '1' : '0';
        }
    binary_val[16] = '\0';  // Null-terminate the string  
}

//void __attribute__((interrupt, auto_psv)) _CNInterrupt(void) {
//    IFS1bits.CNIF = 0;  // Clear interrupt flag
//    data = ADC_read();  // Read data
//    putVal(data);                // this will put the new value into the array where we will get the avg value after
//
//}
//
//void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt(void) {
//   _T2IF = 0; TMR2 = 0;
//    //This will create a char array and get the uint32_t avg, and then translate that to a char array to pring to the LCD
//    char adStr[20];
//    double average = getAvg();
//    sprintf(adStr, "%8.4f", average);
//    lcd_printStr(adStr);
//}

void
setup(){
    _RCDIV = 0;
    AD1PCFG = 0xffff;
    for(int i = 0; i < 20; i++)
    {
        ADC_val[i] = '0';
    }
    
    // -- I2C STUFF --
//    I2C2BRG = 0x9D;
//    I2C2CONbits.I2CEN = 1;
//    _I2CSIDL = 0;
//    IFS3bits.MI2C2IF=0;    
    // -- TIMER 2 STUFF -- 
//    T2CON = 0;
//    TMR2 = 0;
//    T2CONbits.TCKPS = 0b10;
//    PR2 = 782;
//    T2CONbits.TON = 1;
//      
////     -- INTERRUPT ENABLES --
//    IEC0bits.AD1IE = 1;
//    IFS0bits.AD1IF = 0;
//    ADC Interrupt (Enabled)
//    IEC0bits.T2IE = 1;
//    IFS0bits.T2IF = 0;
    // T2 Interrupt (Enabled)
}

int  main(){
    setup();
    lcd_setup();
    SPI_INIT();
    unsigned int dummy;
    //ADC_INIT();
    lcd_printStr("Here!");
    while(1){
        dummy = ADC_read();
        numToBinaryString(dummy);     
//        sprintf(ADC_val,"%d", dummy);
        lcd_printSPI(binary_val);
    }
    return 0;
}
          

