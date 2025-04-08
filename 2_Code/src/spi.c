
#include <p24FJ64GA002.h>

#include "xc.h"

void delay_us(unsigned int us) {
    while (us-- > 0) {
        asm("repeat #1593");
        asm("nop");
    }
}


void
SPI_INIT(){
    SPI1STATbits.SPIEN = 0;         //disable the SPI peripheral so you can change the SCK and ADI pins
    SPI1BUF = 0;                    // clear the buffer
    
    // Clear the configuration registers.
    SPI1STAT = 0;
    SPI1CON1 = 0;
    SPI1CON2 = 0;
    
    // Initialize the pins
    __builtin_write_OSCCONL(OSCCON & 0xBF);
    RPOR2bits.RP4R = 8;             // Sets RP4 to SCK
    RPINR20bits.SDI1R = 3;          // Sets RP3 to the SPI data input
    RPOR3bits.RP7R = 7;             // Sets RP7 to the SDO (comment this if you
                                    // want to try Din idle high)
    __builtin_write_OSCCONL(OSCCON | 0x40);
    
    // SPI1 Initialization Shenanigans
    SPI1CON1bits.DISSDO = 0;        //set the SDO pin controlled by module
    SPI1CON1bits.DISSCK = 0;        // enable the internal SPI clock
    SPI1CON1bits.SMP = 1;           // Input data sampled in the middle
    SPI1CON1bits.SPRE = 4;          // 4:1 Prescaler
    SPI1CON1bits.PPRE = 2;          // 4:1 Prescaler, will result in a 1 MHz Fsck
    SPI1CON1bits.CKP = 1;           // Idle high
    SPI1CON1bits.CKE = 0;           // data changes from idle to active
    SPI1CON1bits.MODE16 = 1;        // Data will be 16 bits long
    SPI1CON1bits.MSTEN = 1;         // PIC24 will be the master
    
    TRISBbits.TRISB4 = 0;           //this set the SCK pin to an output
    TRISBbits.TRISB3 = 1;           //this sets the SDI pin as an input
    TRISBbits.TRISB7 = 0;           //this sets the SDO pin as an output
    
//    LATBbits.LATB2 = 1;             // Slave select pin (we'll just toggle it manually)
//    LATBbits.LATB7 = 1;
//    TRISBbits.TRISB2 = 0;           //this sets the slave select pin as an output
    
    // Initialize Interrupts
    IEC0bits.SPI1IE = 0;        //transfer complete interrupt (supposed to be 1)
    IFS0bits.SPF1IF = 0;        //event interrupt flag status
    IPC2bits.SPF1IP = 4;        //priority 4    
    
    SPI1STATbits.SPIROV = 0;
    SPI1STATbits.SPIEN = 1;         //enables the SPI pin 
 
}


unsigned int ADC_INIT1(){
//    //this is the activation for the ADC to be in continuous read mode
    SPI1BUF = 0b0000000000010000;
    while(!SPI1STATbits.SPIRBF);        // wait until buffer is empty
    return SPI1BUF;                     // clear the buffer by interacting with it 
}

unsigned int ADC_INIT2()
{
    SPI1BUF = 0b0000000000000110;
    while(!SPI1STATbits.SPIRBF);        // wait until buffer is empty
    return SPI1BUF;                     // clear the buffer by interacting with it 
}

//uint16_t ADC_read(){
//    double data = 0;
//    SPI1STATbits.SPIRBF = 0;
//    
//    // Wait for pin to go low (data ready)
//    while (PORTBbits.RB6 == 1);  // Replace RBx with the pin we decide to use probably based on the pcb design
//
//    // Perform one 16-bit SPI read
//    // Dummy write to generate clock because the AD7788 does not have a continuous 
//    // SPI clock mode we have to initialize a data transfer to the ADC to generate a clock
//    SPI1BUF = 0x0000;
//    
//    while (!SPI1STATbits.SPIRBF);  // Wait for receive buffer full
//    data = SPI1BUF;  // Read 16-bit data
//
//    //we have 2^15 - 1 bits or 32767 values available to us and we know that the strain gauge values range from 0 to 3mv with 100g being at 3mv.
//    //so we just have to divide the data value by 2^15 - 1 and multiply by 100 to get the value in grams
//
//    data = ((double)2.08E-3)* data + (double)0.324;
//
//    return data;  // Return the 16-bit data   
//}


// This ADC read is configured so that Din idles high. Doesn't work.
//uint16_t ADC_read() {
//    LATBbits.LATB7 = 0;
//    LATBbits.LATB2 = 0;
//    
//    __builtin_write_OSCCONL(OSCCON & 0xBF);
//    RPOR3bits.RP7R = 7;             // Sets RP7 to the SDO
//    
//    SPI1BUF = 0x38;                    // Initiate SPI read by writing dummy data to generate clock
//    while(!SPI1STATbits.SPIRBF);   // Wait for SPI receive buffer to be full
//    
//    RPOR3bits.RP7R = 0;             // Sets RP7 to the SDO
//    __builtin_write_OSCCONL(OSCCON | 0x40);
//    
//    LATBbits.LATB7 = 1;
//    LATBbits.LATB2 = 1;
//    return SPI1BUF;                 // Return the ADC value
//}

uint16_t ADC_read() {
    SPI1BUF = 0x38;                    // Initiate SPI read by writing dummy data to generate clock
    while(!SPI1STATbits.SPIRBF);   // Wait for SPI receive buffer to be full
    return SPI1BUF;                 // Return the ADC value
}
