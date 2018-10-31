#include "uart.h"
#include "printf.h"
#include "timer.h"
#include "common.h"

//#include "stdio.h"
//#include "platform.h"
#include "fmc_imageon_hdmi_framebuffer.h"

extern void monitorInit();

int secure_main(void)
{
	int i;
	int storage_size;
	int a[10] = {0};
	uart_init();
	timer_init();
	
#if 1
	//init_platform();
fmc_imageon_hdmi_framebuffer_t demo;

    //uart_puts("=====>>>>>Hello VDMA World\n\r");

    demo.uBaseAddr_IIC_FmcImageon = XPAR_AXI_IIC_1_BASEADDR;
//uart_puts("demo.uBaseAddr_IIC_FmcImageonn\n\r");
    demo.uDeviceId_VTC_HdmiiDetector = XPAR_V_TC_1_DEVICE_ID;
//uart_puts("demo.uDeviceId_VTC_HdmiiDetector\n\r");
    demo.uDeviceId_VTC_HdmioGenerator = XPAR_V_TC_2_DEVICE_ID;
//uart_puts("demo.uDeviceId_VTC_HdmioGenerator\n\r");
    demo.uDeviceId_VDMA_HdmiFrameBuffer = XPAR_AXI_VDMA_0_DEVICE_ID;
//uart_puts("demo.uDeviceId_VDMA_HdmiFrameBuffer\n\r");
    demo.uBaseAddr_MEM_HdmiFrameBuffer = XPAR_DDR_MEM_BASEADDR + 0x10000000;  			// at 256MB
//uart_puts("demo.uBaseAddr_MEM_HdmiFrameBuffer\n\r");
//    demo.uBaseAddr_MEM_FgBgCtxData = XPAR_DDR_MEM_BASEADDR + 0x10000000 + 0x1000000; 	// at 3/4 mem
    demo.uBaseAddr_MEM_FgBgCtxData = XPAR_DDR_MEM_BASEADDR + 0x20000000; 	// at 512MB
//uart_puts("demo.uBaseAddr_MEM_FgBgCtxData\n\r");
    demo.uNumFrames_HdmiFrameBuffer = XPAR_AXI_VDMA_0_NUM_FSTORES;
//uart_puts("demo.uNumFrames_HdmiFrameBuffer\n\r");
//uart_puts("***************Feng test***********************,\n\r");
//volatile Xuint8 *pMem = (Xuint8 *)demo.uBaseAddr_MEM_HdmiFrameBuffer;  //
//volatile Xuint8 *filter = (Xuint8 *)(demo.uBaseAddr_MEM_HdmiFrameBuffer+0x01000000);
    fmc_imageon_hdmi_framebuffer_init( &demo);

//==================memory test ===========================================
	/*pMem=0x10000000;
	storage_size = 1024;
	for ( i = 0; i < storage_size; i += 2 )
   {	
	printf("demo.uNumFrames_HdmiFrameBuffer%u\n\r",*pMem);
	pMem++;
   }*/


    //uart_puts("Goodbye VDMA World\n\r");

   // cleanup_platform();
#endif
	return 0;
}
