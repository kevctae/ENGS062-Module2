/*
 * led.c -- led module
 *
 */
#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <xgpio.h>		  	/* axi gpio */
#include <xgpiops.h>		/* processor gpio */
#include "xparameters.h"  	/* constants used by the hardware */
#include "xil_types.h"		/* types used by xilinx */

/* led states */
#define LED_ON true
#define LED_OFF false

#define ALL 0xFFFFFFFF		/* A value designating ALL leds */

#define OUTPUT 0x0							/* setting GPIO direction to output */
#define CHANNEL1 1							/* channel 1 of the GPIO port */

static XGpio port;									/* GPIO port connected to the leds */
static XGpio port1;

/*
 * Initialize the led module
 */
void led_init(void) {
	XGpio_Initialize(&port, XPAR_AXI_GPIO_0_DEVICE_ID);	/* initialize device AXI_GPIO_0 */
	XGpio_SetDataDirection(&port, CHANNEL1, OUTPUT);	    /* set tristate buffer to output */
	XGpio_DiscreteWrite(&port, CHANNEL1, 0x0);
}

/*
 * Set <led> to one of {LED_ON,LED_OFF,...}
 *
 * <led> is either ALL or a number >= 0
 * Does nothing if <led> is invalid
 */
void led_set(u32 led, bool tostate) {
	u32 led_state = XGpio_DiscreteRead(&port, CHANNEL1);

	if (led == ALL) {
		if (tostate) {
			XGpio_DiscreteWrite(&port, CHANNEL1, 0xF);
		} else {
			XGpio_DiscreteWrite(&port, CHANNEL1, 0x0);
		}
	} else if (led == 0) {
		if (tostate) {
			u32 value = 0x1 | led_state;
			XGpio_DiscreteWrite(&port, CHANNEL1, value);
		} else {
			u32 value = 0xe & led_state;
			XGpio_DiscreteWrite(&port, CHANNEL1, value);
		}
	} else if (led == 1) {
		if (tostate) {
			u32 value = 0x2 | led_state;
			XGpio_DiscreteWrite(&port, CHANNEL1, value);
		} else {
			u32 value = 0xd & led_state;
			XGpio_DiscreteWrite(&port, CHANNEL1, value);
		}
	} else if (led == 2) {
		if (tostate) {
			u32 value = 0x4 | led_state;
			XGpio_DiscreteWrite(&port, CHANNEL1, value);
		} else {
			u32 value = 0xb & led_state;
			XGpio_DiscreteWrite(&port, CHANNEL1, value);
		}
	} else if (led == 3) {
		if (tostate) {
			u32 value = 0x8 | led_state;
			XGpio_DiscreteWrite(&port, CHANNEL1, value);
		} else {
			u32 value = 0x7 & led_state;
			XGpio_DiscreteWrite(&port, CHANNEL1, value);
		}
	}
}

/*
 * Get the status of <led>
 *
 * <led> is a number >= 0
 * returns {LED_ON,LED_OFF,...}; LED_OFF if <led> is invalid
 */
bool led_get(u32 led) {
	u32 led_state = XGpio_DiscreteRead(&port, CHANNEL1);

	if (led == 0) {
		u32 value = 0x1 & led_state;

		if (value == 0x1) {
			return LED_ON;
		} else {
			return LED_OFF;
		}
	} else if (led == 1) {
		u32 value = 0x2 & led_state;

		if (value == 0x2) {
			return LED_ON;
		} else {
			return LED_OFF;
		}
	} else if (led == 2) {
		u32 value = 0x4 & led_state;

		if (value == 0x4) {
			return LED_ON;
		} else {
			return LED_OFF;
		}
	} else if (led == 3) {
		u32 value = 0x8 & led_state;

		if (value == 0x8) {
			return LED_ON;
		} else {
			return LED_OFF;
		}
	}
	return LED_OFF;
}

/*
 * Toggle <led>
 *
 * <led> is a value >= 0
 * Does nothing if <led> is invalid
 */
void led_toggle(u32 led) {
	bool led_state = led_get(led);

	if (led_state == LED_ON) {
		led_set(led, LED_OFF);
	} else {
		led_set(led, LED_ON);
	}
}

void led_init_rgb(void) {
	XGpio_Initialize(&port1, XPAR_AXI_GPIO_1_DEVICE_ID);	/* initialize device AXI_GPIO_0 */
	XGpio_SetDataDirection(&port1, CHANNEL1, OUTPUT);	    /* set tristate buffer to output */
	XGpio_DiscreteWrite(&port1, CHANNEL1, 0x0);
}

void led_rgb_set(char color){

	if(color == 'r'){
		XGpio_DiscreteWrite(&port1, CHANNEL1, 0x4);
	}else if(color == 'g'){
		XGpio_DiscreteWrite(&port1, CHANNEL1, 0x2);
	}else if(color == 'b'){
		XGpio_DiscreteWrite(&port1, CHANNEL1, 0x1);
	}else if(color == 'y'){
		XGpio_DiscreteWrite(&port1, CHANNEL1, 0x6);
	}
}

void led_rgb_off(void){
	XGpio_DiscreteWrite(&port1, CHANNEL1, 0x0);
}
