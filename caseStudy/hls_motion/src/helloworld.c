/*
 * Copyright (c) 2009-2012 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "fmc_imageon_hdmi_framebuffer.h"


fmc_imageon_hdmi_framebuffer_t demo;



int main()
{
    init_platform();

    print("Hello VDMA World\n\r");

    demo.uBaseAddr_IIC_FmcImageon = XPAR_AXI_IIC_1_BASEADDR;
    demo.uDeviceId_VTC_HdmiiDetector = XPAR_V_TC_1_DEVICE_ID;
    demo.uDeviceId_VTC_HdmioGenerator = XPAR_V_TC_2_DEVICE_ID;
    demo.uDeviceId_VDMA_HdmiFrameBuffer = XPAR_AXI_VDMA_0_DEVICE_ID;
    demo.uBaseAddr_MEM_HdmiFrameBuffer = XPAR_DDR_MEM_BASEADDR + 0x10000000;  			// at 256MB
//    demo.uBaseAddr_MEM_FgBgCtxData = XPAR_DDR_MEM_BASEADDR + 0x10000000 + 0x1000000; 	// at 3/4 mem
    demo.uBaseAddr_MEM_FgBgCtxData = XPAR_DDR_MEM_BASEADDR + 0x20000000; 	// at 512MB
    demo.uNumFrames_HdmiFrameBuffer = XPAR_AXI_VDMA_0_NUM_FSTORES;

    fmc_imageon_hdmi_framebuffer_init( &demo );

    print("Goodbye VDMA World\n\r");

    cleanup_platform();
    return 0;
}
