//use the code either for the groove-LCD RGB backlight or the liquid crystal display
#include "xc.h"
#include "string.h"
#include "LCD.h"

int location =0;
//This will intialize the LCD display and set all the necassary values. I till also reset the 
//LCD
void delay_ms(unsigned int ms){
    while (ms-- > 0){
        asm("repeat #15998");
        asm("nop");
    }
}  
void lcd_init(void){
    // Set the I2C baud rate generator
    I2C2BRG = 0x9D;
    
    // Clear the I2C master interrupt flag
    IFS3bits.MI2C2IF = 0;
    
    // Enable the I2C module
    I2C2CONbits.I2CEN = 1;
    
    // Allow I2C module to run in idle mode
    _I2CSIDL = 0;
    
    // Delay to allow initialization
    delay_ms(40);
    
    // Send initialization commands to the LCD
    lcd_cmd(0b00111000); // Function Set (8-bit interface, 2 lines, 5x8 dots)
    delay_ms(1);
    lcd_cmd(0b00111001); // Function Set (same as before)
    delay_ms(1);
    lcd_cmd(0b00010100); // Internal OSC frequency (bias and contrast) - Intermediate values
    delay_ms(1);
    lcd_cmd(0b01110000); // Power/ICON control/Contrast set - Intermediate values
    delay_ms(1);
    lcd_cmd(0b01010110); // Follower control
    delay_ms(1);
    lcd_cmd(0b01101100); // Power/ICON control/Contrast set - Intermediate values
    delay_ms(200); // Delay to allow stabilization
    
    // Finalize initialization
    lcd_cmd(0b00111000); // Function Set (8-bit interface, 2 lines, 5x8 dots)
    delay_ms(1);
    lcd_cmd(0b00001100); // Display ON/OFF control (Display ON, Cursor OFF, Blink OFF)
    delay_ms(1);
    lcd_cmd(0b00000010); // Clear Display
    delay_ms(1);
    lcd_cmd(0b00000001); // Clear Display
    delay_ms(1);
}

/*
   Function: lcd_cmd
   
   Description:
   Sends a command package to the LCD over the I2C communication protocol.
   The package consists of a control byte followed by a data byte.
   
   Parameters:
   - Package: The command package to be sent to the LCD.
*/
void lcd_cmd(char Package){
    // Send start condition
    IFS3bits.MI2C2IF = 0; // Clear I2C master interrupt flag
    I2C2CONbits.SEN = 1; // Start condition
    
    // Wait for start condition to finish
    while(I2C2CONbits.SEN == 1);
    
    IFS3bits.MI2C2IF = 0; // Clear I2C master interrupt flag
    
    // Send slave address and R/W bit
    I2C2TRN = 0b01111100;
    
    // Wait for transmission to complete
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    // Send control byte
    I2C2TRN = 0b00000000;
    
    // Wait for transmission to complete
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    // Send data byte
    I2C2TRN = Package;
    
    // Wait for transmission to complete
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    // Send stop condition
    I2C2CONbits.PEN = 1;
    
    // Wait for stop condition to complete
    while(I2C2CONbits.PEN);
}
/*
   Function: lcd_printChar
   
   Description:
   Prints a single character on the LCD using the I2C communication protocol.
   
   Parameters:
   - myChar: The character to be printed on the LCD.
*/
void lcd_printChar(char myChar) {
    // Send start condition
    IFS3bits.MI2C2IF = 0; // Clear I2C master interrupt flag
    I2C2CONbits.SEN = 1; // Start condition
    
    // Wait for start condition to finish
    while(I2C2CONbits.SEN);
    
    IFS3bits.MI2C2IF = 0; // Clear I2C master interrupt flag
    
    // Send slave address
    I2C2TRN = 0b01111100; // Slave address
        
    // Wait for transmission to complete
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    // Set RS bit to 1 (data mode)
    I2C2TRN = 0b01000000;
    
    // Wait for transmission to complete
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    // Send the character to be printed
    I2C2TRN = myChar;
        
    // Wait for transmission to complete
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    // Send stop condition
    I2C2CONbits.PEN = 1; // Stop
        
    // Wait for stop condition to complete
    while(I2C2CONbits.PEN);
}
/*
   Function: lcd_printStr
   
   Description:
   Prints a string on the LCD using the I2C communication protocol.
   
   Parameters:
   - Str: The string to be printed on the LCD.
*/
void lcd_printStr(char Str[]){
    // Get the size of the string
    int size = strlen(Str);
    
    // Send start condition
    IFS3bits.MI2C2IF = 0; // Clear I2C master interrupt flag
    I2C2CONbits.SEN = 1; // Start condition
    
    // Wait for start condition to finish
    while(I2C2CONbits.SEN);
    
    IFS3bits.MI2C2IF = 0; // Clear I2C master interrupt flag
    
    // Send slave address
    I2C2TRN = 0b01111100; // Slave address
        
    // Wait for transmission to complete
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    int i = 0;
    for(i = 0; i < (size - 1); i++){
        // Set RS bit to 1 (data mode)
        I2C2TRN = 0b11000000;
        
        // Wait for transmission to complete
        while(IFS3bits.MI2C2IF == 0);
        IFS3bits.MI2C2IF = 0;
        
        // Send each character of the string
        I2C2TRN = Str[i];
        
        // Wait for transmission to complete
        while(IFS3bits.MI2C2IF == 0);
        IFS3bits.MI2C2IF = 0;
    }
    
    // Set RS bit to 0 (instruction mode)
    I2C2TRN = 0b01000000;
    
    // Wait for transmission to complete
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    // Send the last character of the string
    I2C2TRN = Str[size - 1];
    
    // Wait for transmission to complete
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    // Send stop condition
    I2C2CONbits.PEN = 1; // Stop
        
    // Wait for stop condition to complete
    while(I2C2CONbits.PEN);
}
/*
   Function: lcd_setCursor
   
   Description:
   Sets the cursor position on the LCD screen.
   
   Parameters:
   - x: The column position (0 to 7).
   - y: The row position (0 or 1).
*/
void lcd_setCursor(char x, char y) {
    // Calculate the memory location based on row and column
    location = 0x40 * y + x;
    
    // Send command to set cursor position
    lcd_cmd(128 + location);
}
