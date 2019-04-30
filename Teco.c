#include <stdio.h>
#include <unistd.h>
#include "irslinger.h"
#define pulseSize 8*3

int main(int argc, char *argv[])
{
	uint32_t outPin = 17;           // The Broadcom pin number the signal will be sent on
	int frequency = 38000;          // The frequency of the IR signal in Hz
	double dutyCycle = 0.5;         // The duty cycle of the IR signal. 0.5 means for every cycle,
	                                // the LED will turn on for half the cycle time, and off the other half
        int teco_leader = 3346;
        int teco_tail = 32590;
        int teco_space = 840;
        int teco_zero = 840;
        int teco_one = 2500;

        int keyCode_bits[pulseSize];
        int codes[(8*3*2+2+2)*4];

        int keyCode;
        int i, j, k;
        int result;

        // prompt to provide ir code
        if (argc == 1){
            printf("Please assign a key code(0x00-0x3F)!\n");
            return -1;
        }

        for (i = 1; i < argc; ++i) {
            sscanf(argv[i], "%x", &keyCode);
            if(keyCode > 0x3F){
                printf("key code not in the range 0~0x3F!\n");
                return -1;
            }
            if(keyCode <40)
            keyCode <<= 6;
            keyCode |= 0xA;
            keyCode |= (((keyCode << 12)) ^ 0xFFF000);
            //printf("KeyCode = %6X\n",keyCode);

            for (j = 0; j<24; ++j)
                keyCode_bits[j] = ((keyCode & (1 << j)) ? 1 : 0);

            // generate table for ir sending
            for (k = 0; k < 4; ++k) {
                codes[k*(pulseSize*2+2+2)] = teco_leader;
                codes[k*(pulseSize*2+2+2)+1] = teco_leader;
                for (j = 0; j < pulseSize; ++j) {
                    codes[k*(pulseSize*2+2+2)+2+j*2] = teco_space;
                    codes[k*(pulseSize*2+2+2)+2+j*2+1] = (keyCode_bits[j] ? teco_one : teco_zero);
                }
                codes[k*(pulseSize*2+2+2)+50] = teco_space;
                codes[k*(pulseSize*2+2+2)+51] = teco_tail;
            }

	    result = irSlingRaw(
		outPin,
		frequency,
		dutyCycle,
		codes,
		sizeof(codes) / sizeof(int));

        }
	return result;
}
