
#include "xc.h"

#define BUFFER_SIZE 1024
#define sample_size 128

volatile uint32_t buffer[BUFFER_SIZE];
int bufferindex = 0;

void initBuffer(){
    int i;
    for (i=0; i<BUFFER_SIZE; i++) {
        buffer[i]=0;
    }
    bufferindex = 0;
}

void putVal(long newValue){
    int bufferindex = 0;
    buffer[bufferindex++] = newValue;
    if (bufferindex > BUFFER_SIZE)
        bufferindex = 0;
}

uint32_t getAvg(){
    int i, sum = 0, t=0;
    for (i = 0; i<sample_size; i++) {
        sum += buffer[bufferindex--];
        if(bufferindex <= 0){
            bufferindex = BUFFER_SIZE;
        }
    } return sum / BUFFER_SIZE;
}
