/*
 * File:   main.c
 * Author: sethrosenberg
 *
 * Created on April 12, 2024, 11:29 AM
 */

#include "xc.h"
#include "delay.h"
#include "SPI.h"
#include "data_buffer.h"
#include "LCD.h"

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


void __attribute__((interrupt, auto_psv)) _CNInterrupt(void) {
    IFS1bits.CNIF = 0;  // Clear interrupt flag
    data = ADC_read();  // Read data
    putVal(data);                // this will put the new value into the array where we will get the avg value after

}

void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt(void) {
   _T2IF = 0; TMR2 = 0;
    //This will create a char array and get the uint32_t avg, and then translate that to a char array to pring to the LCD
    char adStr[20];
    uint32_t average = getavg();
    sprintf(adStr, "%lu", average);
    lcd_printStr(adStr);
}

void
  main(){
      //setup code
      SPI_INIT();
      initBuffer();
      ADC_INIT();

      char test[4] = "test";
      lcd_printStr(test);
      while(1);


  }

