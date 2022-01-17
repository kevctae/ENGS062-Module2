/*
 * blinky.c -- working with Serial I/O and GPIO
 *
 * Assumes the LED's are connected to AXI_GPIO_0, on channel 1
 *
 * Terminal Settings:
 *  -Baud: 115200
 *  -Data bits: 8
 *  -Parity: no
 *  -Stop bits: 1
 */
#include <stdio.h>							/* printf(), getchar() */
#include "xil_types.h"					/* u32, u16 etc */
#include "platform.h"						/* ZYBOboard interface */
#include <xgpio.h>							/* Xilinx GPIO functions */
#include "xparameters.h"				/* constants used by the hardware */
#include <stdlib.h>
#include <strings.h>
#include "led.h"

#define OUTPUT 0x0							/* setting GPIO direction to output */
#define CHANNEL1 1							/* channel 1 of the GPIO port */

int main() {
	XGpioPs port;									/* GPIO port connected to the leds */

	init_platform();							/* initialize the hardware platform */

	 /* 
	  * set stdin unbuffered, forcing getchar to return immediately when
	  * a character is typed.
	  */
	setvbuf(stdin,NULL,_IONBF,0);
	 
	printf("[Hello]\n");
	 
//	XGpio_Initialize(&port, XPAR_AXI_GPIO_0_DEVICE_ID);	/* initialize device AXI_GPIO_0 */
//	XGpio_SetDataDirection(&port, CHANNEL1, OUTPUT);	    /* set tristate buffer to output */
//	XGpio_DiscreteWrite(&port, CHANNEL1, 0x0);						/* turn on led0 */

	XGpioPs_Config *config = XGpioPs_LookupConfig(XPAR_XGPIOPS_0_DEVICE_ID);
	XGpioPs_CfgInitialize(&port, config, config->BaseAddr);
	XGpioPs_SetDirectionPin(&port, 7, 1);
	XGpioPs_SetOutputEnablePin(&port, 7, 1);
	XGpioPs_WritePin(&port, 7, 1);

	led_init();
	led_init_rgb();
	led_set(0, LED_OFF);

	while (1) {
		printf(">");
		char c, str[64] = "";
		int i = 0;

		// get characters one by one into str
		while ((c = getchar()) != '\r') {
			str[i++] = c;
			printf("%c", c);
		}

		char *ptr;

		// print "[#]" if is number 0 to 3
		if (strcmp(str, "0") == 0 || strcmp(str, "1") == 0 || strcmp(str, "2") == 0 || strcmp(str, "3") == 0) {
			int num = strtol(str, &ptr, 10);

			led_toggle(num);
			bool led_state = led_get(num);

			printf("\n[%d ", num);
			if (led_state) {
				printf("on");
			} else {
				printf("off");
			}
			printf("]");

		// exit if is 'q' character
		} else if (strcmp(str, "q") == 0) {
			printf("\n");

			led_set(0, false);
			XGpioPs_WritePin(&port, 7, 0);

			break;
		}else if(strcmp(str, "r") == 0 || strcmp(str, "b") == 0 || strcmp(str, "g") == 0 || strcmp(str, "y") == 0){

			char color = str[0];
			led_rgb_set(color);
		}

		printf("\n");
	}

	led_set(ALL, LED_OFF);
	led_rgb_off();

	cleanup_platform();					/* cleanup the hardware platform */
	return 0;
}
