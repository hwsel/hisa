#include "uart.h"
#include "printf.h"

void normal_main(void)
{	
	uart_init();
	
	while(1) {
//uart_puts("normal\n");
		asm volatile ("smc #0\n\t");
	}
}



