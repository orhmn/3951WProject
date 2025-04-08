/* 
 * Date: 03/20/2024
 * Name: Omar Rahman
 * Student ID number: 5770051
 * Course number: EE 2361
 * Term: Spring
 * Lab/assignment number: Lab 5
 * Short Program Description: 
 * Animate an LCD screen with whatever string you want
 */


#include "xc.h"
#include "stdint.h"
#include "LCD.h"

volatile int length;

/**
 * Sends a command to the LCD display.
 * 
 * @param package the command package to be sent
 */


void lcd_cmd(char Package)
{  
    IFS1bits.MI2C1IF = 0; 
    I2C1CONbits.SEN = 1; // send START 
    while(I2C1CONbits.SEN == 1); // wait for SEN == 0
    
    IFS1bits.MI2C1IF = 0; 
    I2C1TRN = 0b01111100; // 8-bits consisting of the slave address and the R/nW bit
    while(IFS1bits.MI2C1IF == 0); //wait for IFS1bits.MI2C1IF == 1
    
    IFS1bits.MI2C1IF = 0;
    I2C1TRN = 0b00000000; // 8-bits consisting of control byte
    while(IFS1bits.MI2C1IF == 0);  //wait for IFS1bits.MI2C1IF == 1
    
    IFS1bits.MI2C1IF = 0;
    I2C1TRN = Package; // 8-bits consisting of the data byte
    while(IFS1bits.MI2C1IF == 0); //wait for IFS1bits.MI2C1IF == 1
    
    IFS1bits.MI2C1IF = 0;
    I2C1CONbits.PEN = 1;
    while(I2C1CONbits.PEN == 1); // PEN will clear when Stop bit is complete
}

/**
 * Initializes the LCD display with specific setup instructions.
 */
void lcd_setup(void)
{
    I2C1BRG = 0x9D; //sets I2C to 100 kHz, as per pg 153 in device docs
    IFS1bits.MI2C1IF = 0; //clear interrupt flag
    I2C1CONbits.I2CEN = 1; //enable I2C mode on I2C1
    
    delay_ms(40);
    lcd_cmd(0b00111000); // function set, normal instruction mode
    lcd_cmd(0b00111001); // function set, extended instruction mode
    lcd_cmd(0b00010100); // interval osc
    lcd_cmd(0b01110000); // contrast C3-C0
    lcd_cmd(0b01011110); // Ion, Bon, C5-C4
    lcd_cmd(0b01101100); // follower control

    
    delay_ms(200);
    lcd_cmd(0b00111000); // function set, normal instruction mode
    lcd_cmd(0b00001100); // Display On
    lcd_cmd(0b00000001); // Clear Display
    
    delay_ms(1);
}

/**
 * Prints a character on the LCD display.
 * 
 * @param package the character to be printed
 */
void lcd_printChar(char package) {
    IFS1bits.MI2C1IF = 0; //clear
    I2C1CONbits.SEN = 1; // send START 
    while(I2C1CONbits.SEN == 1); // wait for SEN == 0
    
    IFS1bits.MI2C1IF = 0;
    I2C1TRN = 0b01111100; // 8-bits consisting of the slave address and the R/nW bit
    while(IFS1bits.MI2C1IF == 0); //wait for IFS1bits.MI2C1IF == 1
    
    IFS1bits.MI2C1IF = 0;
    I2C1TRN = 0b01000000; // 8-bits consisting of control byte /w RS=1
    while(IFS1bits.MI2C1IF == 0);//wait for IFS1bits.MI2C1IF == 1
    
    IFS1bits.MI2C1IF = 0;
    I2C1TRN = package; // 8-bits consisting of the data byte
    while(IFS1bits.MI2C1IF == 0); //wait for IFS1bits.MI2C1IF == 1
    
    I2C1CONbits.PEN = 1;
    while(I2C1CONbits.PEN == 1); // PEN will clear when Stop bit is complete
}

/**
 * Sets the cursor position on the LCD display.
 * 
 * @param row the row where the cursor should be set
 * @param column the column where the cursor should be set
 */
void lcd_setCursor(int row, int column)
{   
    IFS1bits.MI2C1IF = 0; 
    I2C1CONbits.SEN = 1; // send START 
    while(I2C1CONbits.SEN == 1); // wait for SEN == 0
    
    IFS1bits.MI2C1IF = 0; 
    I2C1TRN = 0b01111100; // 8-bits consisting of the slave address and the R/nW bit
    while(IFS1bits.MI2C1IF == 0); //wait for IFS1bits.MI2C1IF == 1
    
    IFS1bits.MI2C1IF = 0;
    I2C1TRN = 0b00000000; // 8-bits consisting of control byte
    while(IFS1bits.MI2C1IF == 0);  //wait for IFS1bits.MI2C1IF == 1
    
    IFS1bits.MI2C1IF = 0;
    I2C1TRN = ((0x40 * row) + column + 0x80); // 8-bits consisting of the data byte
    while(IFS1bits.MI2C1IF == 0); //wait for IFS1bits.MI2C1IF == 1
    
    IFS1bits.MI2C1IF = 0;
    I2C1CONbits.PEN = 1;
    while(I2C1CONbits.PEN == 1); // PEN will clear when Stop bit is complete
}

/**
 * Prints a string on the LCD display.
 * 
 * @param s the string to be printed
 */
void lcd_printStr(char s[])
{
    for (length = 0; s[length] != '\0'; length++){}
    for (int i = 0; i < length; i++)
    {
        lcd_setCursor(0,i);
        lcd_printChar(s[i]);
    }
}
