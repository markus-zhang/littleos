#include "timer.h"

void 
delay(unsigned long long count) 
{
    while (count--) {
        // Do nothing, just waste CPU cycles
        __asm__ volatile ("nop");
    }
}