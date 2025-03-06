
#include "xc.h"
#include "delay.h"

//delay function from asmLib
void delay_1ms(int ms) {
    for (int i = 0; i < ms; i++){
        wait_1ms();
    }
}

void delay_100us(int us) {
    for (int i = 0; i < us; i++)
        wait_100us();
}
