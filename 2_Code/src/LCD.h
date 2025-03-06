/* 
 * File:   LCD.h
 * Author: nick erickson
 *
 * Created on April 3, 2024, 7:32 PM
 */

#ifndef LCD_H
#define	LCD_H


#ifdef	__cplusplus
extern "C" {
#endif


void lcd_init(void);
void lcd_setCursor(char x, char y);

//void lcd_printChar(char myChar); this was for a test to see if I can actually print to the LCD

void lcd_printStr(const char s[]);
void lcd_cmd(char Package);


#ifdef	__cplusplus
}
#endif

#endif	/* ERIC4767_LCD_V001_H */
