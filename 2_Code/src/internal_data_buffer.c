#include "xc.h"
#include "internal_data_buffer.h"


#define BUFSIZE 1024 
#define NUMSAMPLES 30

volatile int adc_buffer [BUFSIZE]; 
volatile int buffer_index = 0;


/* Set all buffer entries to 0 */ 
void initBuffer()
{
    for (int i=0; i < BUFSIZE; i++)
    {
        adc_buffer[i] = 0;
    }
}
                                     
void putVal (int ADCvalue)
{
    adc_buffer[buffer_index] = ADCvalue;
    buffer_index += 1;
    if (buffer_index>= BUFSIZE)
    {
        buffer_index = 0;
    }
}

int getAvg() {
    unsigned long sum = 0;
    
    int start_index = 0;
    
    if (buffer_index > NUMSAMPLES)
    {
        start_index = buffer_index - NUMSAMPLES;
        for (int i = start_index; i < buffer_index; i++)
        {
            sum += adc_buffer[i];
        }
    } else {
        start_index = BUFSIZE + (buffer_index - NUMSAMPLES);
        for (int i = 0; i < buffer_index; i++)
        {
            sum += adc_buffer[i];
        }
        
        for (int i = start_index; i < BUFSIZE; i++)
        {
            sum += adc_buffer[i];
        }
    }
    
    return sum / NUMSAMPLES;
}