/* 
 * File:   SPI.h
 * Author: nicke
 *
 * Created on March 1, 2025, 6:23 PM
 */

#ifndef SPI_H
#define	SPI_H

#ifdef	__cplusplus
extern "C" {
#endif
    void delay_us(unsigned int us);
    void SPI_INIT();
    uint16_t ADC_read();
    unsigned int ADC_INIT();
    unsigned int ADC_INIT2();


#ifdef	__cplusplus
}
#endif

#endif	/* SPI_H */

