
#include "xc.h"

#define BUFFER_SIZE 1024                //size of the array
#define sample_size 128                //number of elements to take the avg of, might be too big but I think having this much will reduce the noise a little more

volatile uint16_t buffer[BUFFER_SIZE];        //size 16 because our ADC is 16 bits
int bufferindex = 0;



//this will set up the buffer to be initialized to 0
void initBuffer(){
    int i;
    for (i=0; i<BUFFER_SIZE; i++) {
        buffer[i]=0;
    }
    bufferindex = 0;
}
//this will write the value of the data recieved to the specified value inside the array, utilizing a circular buffer
void putVal(uint16_t newValue){
    int bufferindex = 0;
    buffer[bufferindex++] = newValue;
    if (bufferindex > BUFFER_SIZE)
        bufferindex = 0;
}


//the avg will take the values of the array sum them and divide to get the avg, because we are using it for a scale we can
//use a bigger buffer because it should not be utilized in a changing enviornment. should get a static value, if we decide that the 
//buffer is too big then just reduce sample_size
uint16_t getAvg(){
    int i, sum = 0, t=0;
    for (i = 0; i<sample_size; i++) {
        sum += buffer[bufferindex--];
        if(bufferindex <= 0){
            bufferindex = BUFFER_SIZE;
        }
    } return sum / BUFFER_SIZE;
}
