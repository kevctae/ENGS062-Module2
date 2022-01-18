/*
 * main.c -- A program to print a dot each time button 0 is pressed.
 *
 *  Some useful values:
 *  -- XPAR_AXI_GPIO_1_DEVICE_ID -- xparameters.h
 *  -- XPAR_FABRIC_GPIO_1_VEC_ID -- xparameters.h
 *  -- XGPIO_IR_CH1_MASK         -- xgpio_l.h (included by xgpio.h)
 */
#include <stdio.h>		/* getchar,printf */
#include <stdlib.h>		/* strtod */
#include <stdbool.h>		/* type bool */
#include <unistd.h>		/* sleep */
#include <string.h>

#include "platform.h"		/* ZYBO board interface */
#include "xil_types.h"		/* u32, s32 etc */
#include "xparameters.h"	/* constants used by hardware */

#include "gic.h"		/* interrupt controller interface */
#include "xgpio.h"		/* axi gpio interface */

#define INPUT 0x1							/* setting GPIO direction to output */
#define CHANNEL1 1							/* channel 1 of the GPIO port */

/* hidden private state */
static XGpio btnport;	       /* btn GPIO port instance */
static int pushes=0;	       /* variable used to count interrupts */
//static bool btn_down = false;

/*
 * controll is passed to this function when a button is pushed
 *
 * devicep -- ptr to the device that caused the interrupt
 */
void btn_handler(void *devicep) {
	/* coerce the generic pointer into a gpio */
	XGpio *dev = (XGpio*)devicep;

	u32 btnId = XGpio_DiscreteRead(dev, CHANNEL1);

//	printf("[STATUS %ld]", btnId);

//	if (!btn_down && btnId == 1) {
//		pushes++;
//		printf(".");
//		fflush(stdout);
//		btn_down = true;
//	} else if (btnId == 0) {
//		btn_down = false;
//	}

	if (btnId == 0x1) {
		pushes++;
		printf(".");
		fflush(stdout);
	}



	XGpio_InterruptClear(dev, XGPIO_IR_CH1_MASK);
}


int main() {
  init_platform();

  /* initialize the gic (c.f. gic.h) */
  gic_init();

  /* initialize btnport (c.f. module 1) and immediately dissable interrupts */
  XGpio_Initialize(&btnport, XPAR_AXI_GPIO_1_DEVICE_ID);
  XGpio_SetDataDirection(&btnport, CHANNEL1, INPUT);
  XGpio_InterruptDisable(&btnport, XGPIO_IR_CH1_MASK);

  /* connect handler to the gic (c.f. gic.h) */
  gic_connect(XPAR_FABRIC_GPIO_1_VEC_ID, btn_handler, &btnport);

  /* enable interrupts on channel (c.f. table 2.1) */
  XGpio_InterruptEnable(&btnport, XGPIO_IR_CH1_MASK);

  /* enable interrupt to processor (c.f. table 2.1) */
  XGpio_InterruptGlobalEnable(&btnport);

  printf("[hello]\n"); /* so we are know its alive */
  pushes=0;
  while(pushes<10) /* do nothing and handle interrups */
	  ;

  printf("\n[done]\n");

  /* disconnect the interrupts (c.f. gic.h) */
  gic_disconnect(XPAR_FABRIC_GPIO_1_VEC_ID);

  /* close the gic (c.f. gic.h) */
  gic_close();

  cleanup_platform();					/* cleanup the hardware platform */
  return 0;
}

