#include <stdio.h>
#include "irslinger.h"

int main(int argc, char *argv[])
{
	uint32_t outPin = 17;            // The Broadcom pin number the signal will be sent on
	int frequency = 38000;           // The frequency of the IR signal in Hz
	double dutyCycle = 0.5;          // The duty cycle of the IR signal. 0.5 means for every cycle,
	                                 // the LED will turn on for half the cycle time, and off the other half
	int leadingPulseDuration = 9000; // The duration of the beginning pulse in microseconds
	int leadingGapDuration = 4500;   // The duration of the gap in microseconds after the leading pulse
	int onePulse = 562;              // The duration of a pulse in microseconds when sending a logical 1
	int zeroPulse = 562;             // The duration of a pulse in microseconds when sending a logical 0
	int oneGap = 1688;               // The duration of the gap in microseconds when sending a logical 1
	int zeroGap = 562;               // The duration of the gap in microseconds when sending a logical 0
	int sendTrailingPulse = 1;       // 1 = Send a trailing pulse with duration equal to "onePulse"
	                                 // 0 = Don't send a trailing pulse

        char codes[32+1];
        int keyCode, address, command;
	int i, j;
	int result;

	if (argc == 1) printf("Please assign a key code(0000-FFFF)!\n");

        for (i = 1; i < argc; i++) {
            sscanf(argv[i], "%x", &keyCode);
	    address = (keyCode >> 8) & 0xff;
	    command = keyCode & 0xff;
	    
	    for(j = 0; j < 8; j++) {
                if ((address >> j) & 0x01 == 1) {
		    codes[j] = '1';
		    codes[j+8] = '0';
		} else {
         	    codes[j] = '0';
		    codes[j+8] = '1';
		}
	    }

	    for (j=0; j < 8; j++){
	        if ((command >> j) & 0x01 == 1) {
	            codes[j+16] = '1';
                    codes[j+24] = '0';
                } else {
                    codes[j+16] = '0';
                    codes[j+24] = '1';
                }

	    }

	    codes[32]=0;

            printf("%s\n", codes);

	    result = irSling(
		outPin,
		frequency,
		dutyCycle,
		leadingPulseDuration,
		leadingGapDuration,
		onePulse,
		zeroPulse,
		oneGap,
		zeroGap,
		sendTrailingPulse,
		codes);
	}
        return result;
}
