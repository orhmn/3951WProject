#ifndef DATA_BUFFER_H
#define	DATA_BUFFER_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    
void putVal(uint32_t newValue);  // add a new value to the buffer
long getAvg();               // average all buffer vals
void initBuffer();          // set all buffer vals to zero



#ifdef	__cplusplus
}
#endif

#endif	/* DATA_BUFFER_H */
