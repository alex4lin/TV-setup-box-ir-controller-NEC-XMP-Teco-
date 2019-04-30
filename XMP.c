#include <stdio.h>
#include <unistd.h>
#include "irslinger.h"
#define pulseSize (8+1)*2*4-1

int main(int argc, char *argv[])
{
	uint32_t outPin = 17;           // The Broadcom pin number the signal will be sent on
	int frequency = 38000;          // The frequency of the IR signal in Hz
	double dutyCycle = 0.5;         // The duty cycle of the IR signal. 0.5 means for every cycle,
	                                // the LED will turn on for half the cycle time, and off the other half
        int xmp_unit = 137;
        int xmp_leader = 212;
        int xmp_nibble_prefix = 758;
        int xmp_halfframe_space = 13560;
        int xmp_trailer_space = 82392;
        
        int codes[pulseSize];
        int template[] = {
                 1, 11, 0, 15, 4, 4, 5 , 8,
                 1, 0, 0, 0, 0, 0, 0, 0,
                 1, 11, 0, 15, 4, 4, 5 , 8,
                 1, 0, 8, 0, 0, 0, 0, 0 };
   
        int keyCode;
        int sum;
        int i, j;
        int result;

        if (argc == 1) printf("Please assign a key code(00-FF)!\n");

        for (i = 1; i < argc; ++i) {
            sscanf(argv[i], "%x", &keyCode);
            template[12] = keyCode >> 4;
            template[13] = keyCode & 15;
            template[28] = keyCode >> 4;
            template[29] = keyCode & 15;
            
            // set checksum
            template[9] = 0;
            sum = 0;
            for(j = 8; j < 16; ++j) sum += template[j];
            template[9] = (16 - (sum & 15)) & 15;
            
            // set checksum 
            template[25] = 0;
            sum = 0;
            for(j = 24; j < 32; ++j) sum += template[j];
            template[25] = (16 - (sum & 15)) & 15;

            // set leader
	    for(j = 0; j < pulseSize; j+=2) codes[j] = xmp_leader;
            
            // set space 
            for(j = 0; j < 8; ++j) codes[j*2+1] = xmp_nibble_prefix + xmp_unit * template[j];
            codes[17] = xmp_halfframe_space;
            for(j = 0; j < 8; ++j) codes[j*2+19] = xmp_nibble_prefix + xmp_unit * template[j+8];
            codes[35] = xmp_trailer_space;
            for(j = 0; j < 8; ++j) codes[j*2+37] = xmp_nibble_prefix + xmp_unit * template[j+16];
            codes[53] = xmp_halfframe_space;
            for(j = 0; j < 8; ++j) codes[j*2+55] = xmp_nibble_prefix + xmp_unit * template[j+24];

	    result = irSlingRaw(
		outPin,
		frequency,
		dutyCycle,
		codes,
		sizeof(codes) / sizeof(int));
        }
	return result;
}
