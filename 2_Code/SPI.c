
#include "xc.h"

void
SPI_INIT(){
    
    // SPI1 Initialization Shenanigans
    SPI1STATbits.SPIEN = 0;         //disable the SPI peripheral so you can change the SCK and ADI pins
    SPI1CON1bits.DISSDO = 0;        //set the SDO pin controlled by module
    SPI1BUF = 0;                    // clear the buffer
   
    SPI1CON1bits.SPRE = 4;          //this will set the SCK to 100kHz if Fcy is 16Mhz
    SPI1CON1bits.PPRE = 2;          //this is the primary and secondary scalars set to 4:1
    SPI1STATbits.SPIEN = 1;         //enables the SPI pin and sets the 
    
    TRISBbits.TRISB4 = 0;           //this set the SCK pin to an output
    TRISBbits.TRISB3 = 1;           //this sets the SDI pin as an input
    TRISBbits.TRISB7= 0;            //this sets the SDO pin as an output
    
    // Initialize the pins
    __builtin_write_OSCCONL(OSCCON & 0xBF);
    RPOR2bits.RP4R = 8;             //Sets RP4 to SCK
    RPINR20bits.SDI1R3 = 1;         //Sets RP3 to the SPI data input
    RPOR3bits.RP7R = 7;             //Sets RP7 to the SDO
    __builtin_write_OSCCONL(OSCCON | 0x40);
    
    // Initialize Interrupts
    IEC0bits.SPI1IE = 1;        //transfer complete interrupt
    IFS0bits.SPF1IF = 0;        //event interrupt flag status
    IPC2bits.SPF1IP = 4;        //priority 4    
}

void
ADC_INIT(){
    //this is the activation for the ADC to be in continuous read mode
    while(SPI1STATbits.SPITBF);        //wait until buffer is empty
    SPI1BUF = 0x3C;
    while(SPI1STATbits.SPITBF);        //wait until buffer is empty
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

int16_t ADC_read() {
    uint16_t data = 0;  // Use uint16_t for 16-bit ADC data

    // Wait for data ready pin to go low
    while (RPINR7 == 1);  // Replace RB6 with the correct pin

    // Initiate SPI read by writing dummy data to generate clock
    SPI1BUF = 0x0000; 
    
    // Wait until transmit is complete
    while (SPI1STATbits.SPITBF);
    
    // Wait for SPI receive buffer to be full
    while (!SPI1STATbits.SPIRBF);

    data = SPI1BUF;  // Read 16-bit data

    return data;  // Return the ADC value
}