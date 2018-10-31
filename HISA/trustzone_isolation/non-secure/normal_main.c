#include "uart.h"
#include "printf.h"

volatile unsigned long * baseaddr_input = (unsigned long*)0x00024000;
volatile unsigned long * baseaddr_output = (unsigned long*)0x0002a000;

void normal_main(void)
{	
	int i = 0;
	uart_init();

	//while(1) {
	
	*(baseaddr_input) = 0x239f9234;

	*(baseaddr_input + 1) = SIZE;
	while(1)
	{
		for (i = 0; i < SIZE; i++) {
			*(baseaddr_input + 2) = 0x5c9180a6;
			//asm volatile ("smc #0\n\t");
		}
		//uart_puts("Normal World\n");
		asm volatile ("smc #0\n\t");
		//printf("normal output %08x\n", *(baseaddr_output));
	}
}



