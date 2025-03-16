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

#ifndef LCD_H
#define LCD_H

#include <xc.h>

#ifdef	__cplusplus
extern "C" {
#endif
void lcd_cmd(char Package);
void lcd_setup(void);
void lcd_printChar(char package);
void lcd_setCursor(int row, int column);
void lcd_printStr(char s[]);
void lcd_printSPI(char s[]);
#ifdef	__cplusplus
}
#endif

#endif