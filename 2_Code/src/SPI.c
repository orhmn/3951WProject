
#include "xc.h"

void
SPI_INIT(){
    
    //disable the SPI peripheral so you can change the SCK and ADI pins
    SPI1STATbits.SPIEN = 0;
    SPI1CON1bits.DSSDO = 0;         //set the SDO pin controlled by module
    
    RPINR20bits.SCK1R0 = 1;         //Sets RP0 to SCK
    RPINR20bits.SDI1R6 = 1;         //Sets RP2 to the SPI data input
    
    RPOR3bits.RP7R = 7;             //Sets RP1 to the SDO
    
    TRISBbits.TRISB0 = 0;           //this set the SCK pin to an output
    TRISBbits.TRISB6= 1;            //this sets the SDI pin as an input
    TRISBbits.TRISB7= 0;            //this sets the SDO pin as an output
   
    
    
    SPI1CON1bits.MODE16 = 1;        //This will have the data be word length or 16 bits because our ADC is 16bits
    SPI1CON1bits.CKP = 1;           //This sets the clock to idle high, our ADC needs this
    SPI1CON1bits.MSTEN = 1;         //We need our microcontroller to be in master mode because it will control the 
                                    // SCK and the communication with the ADC
    
    
    //this will set the SCK to 1000kHz if Fcy is 16Mhz
    //this is the primary and secondary scalars set to 4:1
    SPI1CON1bits.SPRE = 0b01;       
    SPI1CON1bits.PPRE = 0b01;
    
    
    SPI1STATbits.SPIEN = 1;         //enables the SPI pin and sets the 
    
    
    IEC0bits.SPI1IE = 1;        //transfer complet interrupt
    IFS0bits.SPF1IF = 0;        //event intterrupt flag status
    IPC2bits.SPF1IP = 4;        //priorty 4    
}

void
ADC_INIT(){

    //this is the activation for the ADC to be in continuous read mode
    SPIBUF = 0b0011110;
    while(!SPI1STATbits.SPIBUF);        //wait until it sends
    
    
    dummy = SPIBUF;
    while(!SPI1STATbits.SPIBUF);        //read the buffer to clear it
}


uint16_t ADC_read(){
    udouble16_t data = 0;

    // Wait for pin to go low (data ready)
    while (PORTBbits.RB6 == 1);  // Replace RBx with the pin we decide to use probably based on the pcb design

    // Perform one 16-bit SPI read
    // Dummy write to generate clock because the AD7788 does not have a continuous 
    //SPI clock mode we have to initialize a data transfer to the ADC to generate a clock
    SPI1BUF = 0x0000;
    while (!SPI1STATbits.SPIRBF);  // Wait for receive buffer full
    data = SPI1BUF;  // Read 16-bit data

    //we have 2^15 - 1 bits or 32767 values available to us and we know that the strain gauge values range from 0 to 3mv with 100g being at 3mv.
    //so we just have to divide the data value by 2^15 - 1 and multiply by 100 to get the value in grams

    data = ((double)2.08E-3)* data + (double)0.324;

    return data;  // Return the 16-bit data   
}

