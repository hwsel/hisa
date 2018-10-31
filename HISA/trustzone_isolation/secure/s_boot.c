#include "uart.h"
#include "printf.h"
#include "timer.h"
#include "common.h"

extern void monitorInit();

volatile unsigned long * baseaddr_s = (unsigned long*)0x43c00000;
volatile unsigned long * baseaddr_input = (unsigned long*)0x00024000;
volatile unsigned long * baseaddr_output = (unsigned long*)0x0002a000;

typedef struct Users {
	unsigned long id;
	int access;
};

struct Users a[10] = {{0xa3e980c4, 10}, {0xcd23e300, 5}, {0x0034a197, 23}, {0x31aa9872, 54}, {0x19840335, 2}, {0x214a9845, 7}, {0xcdef1239, 113}, {0xb8c0349a, 104}, {0x9e802246, 1}, {0x239f9234, 0}};

int whitelist_size = 10;

int whitelist(unsigned long id)
{
	int i;
	for (i = 0; i < whitelist_size; i++)
	{
		if (id == a[i].id)
		{
			return i;
		}
	}
	return -1;
}

int frequency(struct Users input, int index)
{
	int i;
	if (input.access <= 1000000000)
	{
		return 1;
	}
	else
	{
		for (i = index; i < whitelist_size - 1; i++)
		{
			a[i].id = a[i+1].id;
			a[i].access = a[i+1].access;
		}
		whitelist_size--;
		return 0;
	}
}

int secure_main(void)
{
	int i;
	int time[10] = {0};

	int index, size;
	unsigned long test;

	unsigned long request_id[10];
	//volatile int request_size = 0;

	uart_init();
	timer_init();
	
	// set for non-secure can access some coprocessor reg
	CP15_SET_NSACR(0x00073fff);
	CP15_SET_CPACR(0x0fffffff);

	// set for SCR
	CP15_SET_SCR(0b110000);

	writel(0xdf0d, 0xf8000008);	//unlock
//	writel(0x0, 0xe0200018);	//config uart to secure
	writel(0xffff, 0xf8000404);	//config ocmram2 to non-secure
//	writel(0x0, 	   0xf8000400); //config ocmram1 to secure
//	writel(0x767b,	0xf8000910);	//lock
	
	asm volatile ("isb");
	asm volatile ("dsb");
	
	// Install monitor
//  char* dest = (char*)0x20100;
//	char* src  = (char*)0x1000;
// 	for(i=0; i<500; i++)
//	 	*dest++ = *src++;

	writel((0x1 << 7), 0xe0200018);	//config uart to secure
	monitorInit();

#if 1
	index = whitelist(*baseaddr_input);
	if (index == -1) return 0;
	size = *(baseaddr_input + 1);
	for (i = 0; i < size; i++) {
		test = (unsigned long) 0x00000001 * 0x00000006 << 28;	// Re-calculate the last 4 bits
		*(baseaddr_s + 0) = *(baseaddr_input + 2);		// Read the input from the shared memory
		if (test == *(baseaddr_s + 1) << 28) {		// Partial checking
			*(baseaddr_output) = *(baseaddr_s + 1);		// Write the output to the shard memory
		}
	}
	a[index].access += size;
	frequency(a[index], index);
	asm volatile ("smc #0\n\t");		// Switch to the normal world
#endif
	return 0;
}
