/*
 * io.c -- switch and button module
 *
 */

#include "io.h"
#include "gic.h"		/* interrupt controller interface */

#define INPUT 0x1
#define CHANNEL1 1							/* channel 1 of the GPIO port */

static XGpio btnport;	       /* btn GPIO port instance */
static XGpio swport;	       /* sw GPIO port instance */

static void (*btn_saved_callback)(u32 v);

static void (*sw_saved_callback)(u32 v);

static void get_btnId(void *devicep) {
	/* coerce the generic pointer into a gpio */
	XGpio *dev = (XGpio*)devicep;

	u32 btn = XGpio_DiscreteRead(dev, CHANNEL1);
	btn_saved_callback(btn);

	//clear it
	XGpio_InterruptClear(dev, XGPIO_IR_CH1_MASK);
}

static void get_swId(void *devicep) {
	/* coerce the generic pointer into a gpio */
	XGpio *dev = (XGpio*)devicep;

	u32 sw = XGpio_DiscreteRead(dev, CHANNEL1);
	sw_saved_callback(sw);

	//clear it
	XGpio_InterruptClear(dev, XGPIO_IR_CH1_MASK);
}

/*
 * initialize the btns providing a callback
 */
void io_btn_init(void (*btn_callback)(u32 btn)) {
	//save callback function
	btn_saved_callback = btn_callback;

	/* initialize the gic (c.f. gic.h) */
	gic_init();

	/* initialize btnport (c.f. module 1) and immediately dissable interrupts */
	XGpio_Initialize(&btnport, XPAR_AXI_GPIO_1_DEVICE_ID);
	XGpio_SetDataDirection(&btnport, CHANNEL1, INPUT);	    /* set tristate buffer to output */
	XGpio_InterruptDisable(&btnport, XGPIO_IR_CH1_MASK);

	/* connect handler to the gic (c.f. gic.h) */
	gic_connect(XPAR_FABRIC_GPIO_1_VEC_ID, get_btnId, &btnport);

	/* enable interrupts on channel (c.f. table 2.1) */
	XGpio_InterruptEnable(&btnport, XGPIO_IR_CH1_MASK);

	/* enable interrupt to processor (c.f. table 2.1) */
	XGpio_InterruptGlobalEnable(&btnport);
}

/*
 * close the btns
 */
void io_btn_close(void) {
	/* disconnect the interrupts (c.f. gic.h) */
	gic_disconnect(XPAR_FABRIC_GPIO_1_VEC_ID);

	/* close the gic (c.f. gic.h)   */
	gic_close();
}


/*
 * initialize the switches providing a callback
 */
void io_sw_init(void (*sw_callback)(u32 sw)) {
	//save callback function
	sw_saved_callback = sw_callback;

	/* initialize the gic (c.f. gic.h) */
	gic_init();

	/* initialize btnport (c.f. module 1) and immediately dissable interrupts */
	XGpio_Initialize(&swport, XPAR_AXI_GPIO_2_DEVICE_ID);
	XGpio_SetDataDirection(&swport, CHANNEL1, INPUT);	    /* set tristate buffer to output */
	XGpio_InterruptDisable(&swport, XGPIO_IR_CH1_MASK);

	/* connect handler to the gic (c.f. gic.h) */
	gic_connect(XPAR_FABRIC_GPIO_2_VEC_ID, get_swId, &swport);

	/* enable interrupts on channel (c.f. table 2.1) */
	XGpio_InterruptEnable(&swport, XGPIO_IR_CH1_MASK);

	/* enable interrupt to processor (c.f. table 2.1) */
	XGpio_InterruptGlobalEnable(&swport);
}

/*
 * close the switches
 */
void io_sw_close(void) {
	/* disconnect the interrupts (c.f. gic.h) */
	gic_disconnect(XPAR_FABRIC_GPIO_2_VEC_ID);

	/* close the gic (c.f. gic.h)   */
	gic_close();
}
