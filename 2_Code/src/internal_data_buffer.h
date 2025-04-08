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

#ifndef INTERNAL_DATA_BUFFER_H
#define INTERNAL_DATA_BUFFER_H

#include <xc.h>

#ifdef	__cplusplus
extern "C" {
#endif
void initBuffer();                                   
void putVal (int ADCvalue);
int getAvg();
#ifdef	__cplusplus
}
#endif

#endif

