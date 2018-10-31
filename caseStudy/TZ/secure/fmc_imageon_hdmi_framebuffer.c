//----------------------------------------------------------------
//      _____
//     /     \
//    /____   \____
//   / \===\   \==/
//  /___\===\___\/  AVNET
//       \======/
//        \====/
//---------------------------------------------------------------
//
// This design is the property of Avnet.  Publication of this
// design is not authorized without written consent from Avnet.
//
// Please direct any questions to:  technical.support@avnet.com
//
// Disclaimer:
//    Avnet, Inc. makes no warranty for the use of this code or design.
//    This code is provided  "As Is". Avnet, Inc assumes no responsibility for
//    any errors, which may appear in this code, nor does it make a commitment
//    to update the information contained herein. Avnet, Inc specifically
//    disclaims any implied warranties of fitness for a particular purpose.
//                     Copyright(c) 2012 Avnet, Inc.
//                             All rights reserved.
//
//----------------------------------------------------------------
//
// Create Date:         Oct 24, 2012
// Design Name:         FMC-IMAGEON HDMI Video Frame Buffer
// Module Name:         fmc_imageon_hdmi_framebuffer.c
// Project Name:        FMC-IMAGEON HDMI Video Frame Buffer Program
// Target Devices:      Spartan-6, Virtex-6, Kintex-6
// Hardware Boards:     FMC-IMAGEON
// 
//
// Tool versions:       ISE 14.3
//
// Description:         FMC-IMAGEON HDMI Demo Program
//                      This application will configure the FMC-IMAGEON module
//                      for the simple case of an HDMI passthrough:
//                      - HDMI Input
//                         - ADV7611 configured for 16 bit YCbCr 4:2:2 mode
//                           with embedded syncs
//                      - HDMI Output
//                         - ADV7511 configured for 16 bit YCbCr 4:2:2 mode
//                           with embedded syncs
//                      If present, it will also reset the Video Timing Controller
//                      that is used for timing in the AXI4-Stream bridges
//
// Dependencies:
//
// Revision:            Oct 24, 2012: 1.00 Initial version
//                      Nov 01, 2012: 1.02 Update EDID content
//
//----------------------------------------------------------------

//#include <stdio.h>
#include "timer.h"
#include "fmc_imageon_hdmi_framebuffer.h"
#define USHLS 0            //1-> HLS 0->soft
//==========================these below is changed by Feng=======================================================
#define offset 0x01000000
//+==============================================================================================================

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>HLS motion detection<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//#include <stdio.h>
#include "platform.h"
//#include "assert.h"
// Add BSP header files
//#include <stdlib.h>  // Standard C functions, e.g. exit()
//#include <stdbool.h> // Provides a Boolean data type for ANSI/ISO-C
#include "xparameters.h" // Parameter definitions for processor periperals
#include "xscugic.h"     // Processor interrupt controller device driver
#include "xhls_macc.h"   // Device driver for HLS HW block
#include "xil_cache.h"
//#include "xil_assert.h"
void* memcpy_cus(Xuint8* destination, Xuint8* source, Xuint32 num)
{
	Xuint32 i;
	Xuint8* d = destination;
	Xuint8* s = source;
	for (i = 0; i < num; i++) {
		d[i] = s[i];
	}
	return destination;
}

/*typedef struct {
    u16 DeviceId;
    u32 Axi4lites_BaseAddress;
} XHls_macc_Config;*/

//XHls_macc_Config* XHls_macc_LookupConfig(u16 DeviceId);

XHls_macc_Config XHls_macc_ConfigTable[] =
{
	{
		XPAR_HLS_MACC_0_DEVICE_ID,
		XPAR_HLS_MACC_0_S_AXI_AXI4LITES_BASEADDR
	}
};

XHls_macc_Config *XHls_macc_LookupConfig(u16 DeviceId) {
	XHls_macc_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XHLS_MACC_NUM_INSTANCES; Index++) {
		if (XHls_macc_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XHls_macc_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

//int XHls_macc_CfgInitialize(XHls_macc *InstancePtr, XHls_macc_Config *ConfigPtr);

/*int XHls_macc_CfgInitialize(XHls_macc *InstancePtr, XHls_macc_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Axi4lites_BaseAddress = ConfigPtr->Axi4lites_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}*/

// HLS macc HW instance
XHls_macc HlsMacc;
//Interrupt Controller Instance
XScuGic ScuGic;
// Global variable definitions - used by ISR
volatile static int RunHlsMacc = 0;
volatile static int ResultAvailHlsMacc = 0;

// Setup and helper functions
int setup_interrupt();
int hls_macc_init(XHls_macc *hls_maccPtr);
void hls_macc_start(void *InstancePtr);
// The ISR prototype
void hls_macc_isr(void *InstancePtr);

int hls_macc_init(XHls_macc *hls_maccPtr)
{
   XHls_macc_Config *cfgPtr;
   int status;

   cfgPtr = XHls_macc_LookupConfig(XPAR_XHLS_MACC_0_DEVICE_ID);
   if (!cfgPtr) {
      uart_puts("ERROR: Lookup of acclerator configuration failed.\n\r");
      return XST_FAILURE;
   }
   status = XHls_macc_CfgInitialize(hls_maccPtr, cfgPtr);
   if (status != XST_SUCCESS) {
      uart_puts("ERROR: Could not initialize accelerator.\n\r");
      return XST_FAILURE;
   }
   return status;
}

void hls_macc_start(void *InstancePtr){
   XHls_macc *pAccelerator = (XHls_macc *)InstancePtr;
   XHls_macc_InterruptEnable(pAccelerator,1);
   XHls_macc_InterruptGlobalEnable(pAccelerator);
   XHls_macc_Start(pAccelerator);
}
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>HLS motion detection_end<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

Xuint8 fmc_imageon_hdmii_edid_content[256] =
{
		0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00,
		0x06, 0xD4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x16, 0x01, 0x03, 0x81, 0x46, 0x27, 0x78,
		0x0A, 0x32, 0x30, 0xA1, 0x54, 0x52, 0x9E, 0x26,
		0x0A, 0x49, 0x4B, 0xA3, 0x08, 0x00, 0x81, 0xC0,
		0x81, 0x00, 0x81, 0x0F, 0x81, 0x40, 0x81, 0x80,
		0x95, 0x00, 0xB3, 0x00, 0x01, 0x01, 0x02, 0x3A,
		0x80, 0x18, 0x71, 0x38, 0x2D, 0x40, 0x58, 0x2C,
		0x45, 0x00, 0xC4, 0x8E, 0x21, 0x00, 0x00, 0x1E,
		0xA9, 0x1A, 0x00, 0xA0, 0x50, 0x00, 0x16, 0x30,
		0x30, 0x20, 0x37, 0x00, 0xC4, 0x8E, 0x21, 0x00,
		0x00, 0x1A, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x46,
		0x4D, 0x43, 0x2D, 0x49, 0x4D, 0x41, 0x47, 0x45,
		0x4F, 0x4E, 0x0A, 0x20, 0x00, 0x00, 0x00, 0xFD,
		0x00, 0x38, 0x4B, 0x20, 0x44, 0x11, 0x00, 0x0A,
		0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0x54,
		0x02, 0x03, 0x1F, 0x71, 0x4B, 0x90, 0x03, 0x04,
		0x05, 0x12, 0x13, 0x14, 0x1F, 0x20, 0x07, 0x16,
		0x26, 0x15, 0x07, 0x50, 0x09, 0x07, 0x01, 0x67,
		0x03, 0x0C, 0x00, 0x10, 0x00, 0x00, 0x1E, 0x01,
		0x1D, 0x00, 0x72, 0x51, 0xD0, 0x1E, 0x20, 0x6E,
		0x28, 0x55, 0x00, 0xC4, 0x8E, 0x21, 0x00, 0x00,
		0x1E, 0x01, 0x1D, 0x80, 0x18, 0x71, 0x1C, 0x16,
		0x20, 0x58, 0x2C, 0x25, 0x00, 0xC4, 0x8E, 0x21,
		0x00, 0x00, 0x9E, 0x8C, 0x0A, 0xD0, 0x8A, 0x20,
		0xE0, 0x2D, 0x10, 0x10, 0x3E, 0x96, 0x00, 0xC4,
		0x8E, 0x21, 0x00, 0x00, 0x18, 0x01, 0x1D, 0x80,
		0x3E, 0x73, 0x38, 0x2D, 0x40, 0x7E, 0x2C, 0x45,
		0x80, 0xC4, 0x8E, 0x21, 0x00, 0x00, 0x1E, 0x1A,
		0x36, 0x80, 0xA0, 0x70, 0x38, 0x1F, 0x40, 0x30,
		0x20, 0x25, 0x00, 0xC4, 0x8E, 0x21, 0x00, 0x00,
		0x1A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
};

int fmc_imageon_hdmi_framebuffer_init( fmc_imageon_hdmi_framebuffer_t *pDemo )
{
   int timing = 0;

int count_num = 0;

unsigned long * done = (unsigned long*)0x0002f000;

   int ret;
   Xuint32 timeout = 100;
   Xuint32 iterations = 0;

   //uart_puts("\n\r");
   //uart_puts("------------------------------------------------------\n\r");
   //uart_puts("--|||||||||||     FMC-IMAGEON HDMI Video Frame Buffer        --\n\r");
   //uart_puts("------------------------------------------------------\n\r");
   //uart_puts("\n\r");

   //uart_puts( "FMC-IMAGEON Initialization ...\n\r" );

   ret = fmc_iic_xps_init(&(pDemo->fmc_imageon_iic),"FMC-IMAGEON I2C Controller", pDemo->uBaseAddr_IIC_FmcImageon );
   if ( !ret )
   {
      uart_puts( "ERROR : Failed to open FMC-IIC driver\n\r" );
      //exit(0);
   }

   fmc_imageon_init(&(pDemo->fmc_imageon), "FMC-IMAGEON", &(pDemo->fmc_imageon_iic));
   pDemo->fmc_imageon.bVerbose = pDemo->bVerbose;

   // Configure Video Clock Synthesizer
   fmc_imageon_vclk_init( &(pDemo->fmc_imageon) );
   fmc_imageon_vclk_config( &(pDemo->fmc_imageon), FMC_IMAGEON_VCLK_FREQ_74_250_000);

   // Initialize HDMI Input (including EDID content)
   //uart_puts( "HDMI Input Initialization ...\n\r" );
   ret = fmc_imageon_hdmii_init( &(pDemo->fmc_imageon),
                                 1, // hdmiiEnable = 1
                                 1, // editInit = 1
                                 fmc_imageon_hdmii_edid_content
                                 );
   if ( !ret )
   {
      uart_puts( "ERROR : Failed to init HDMI Input Interface\n\r" );
      //exit(0);
   }

   // Configure Video Clock Synthesizer
   //uart_puts( "Video Clock Synthesizer Configuration ...\n\r" );
   fmc_imageon_vclk_config( &(pDemo->fmc_imageon), FMC_IMAGEON_VCLK_FREQ_74_250_000);
   sleep(1);

   // Set HDMI output to 720P60 resolution
   pDemo->hdmio_resolution = VIDEO_RESOLUTION_720P;
   pDemo->hdmio_width  = 1280;
   pDemo->hdmio_height = 720;

   //pDemo->hdmio_timing.IsHDMI        = 1; // HDMI Mode
   pDemo->hdmio_timing.IsHDMI        = 0; // DVI Mode
   pDemo->hdmio_timing.IsEncrypted   = 0;
   pDemo->hdmio_timing.IsInterlaced  = 0;
   pDemo->hdmio_timing.ColorDepth    = 8;

   pDemo->hdmio_timing.HActiveVideo  = 1280;
   pDemo->hdmio_timing.HFrontPorch   =  110;
   pDemo->hdmio_timing.HSyncWidth    =   40;
   pDemo->hdmio_timing.HSyncPolarity =    1;
   pDemo->hdmio_timing.HBackPorch    =  220;

   pDemo->hdmio_timing.VActiveVideo  =  720;
   pDemo->hdmio_timing.VFrontPorch   =    5;
   pDemo->hdmio_timing.VSyncWidth    =    5;
   pDemo->hdmio_timing.VSyncPolarity =    1;
   pDemo->hdmio_timing.VBackPorch    =   20;

   //uart_puts( "HDMI Output Initialization ...\n\r" );
   ret = fmc_imageon_hdmio_init( &(pDemo->fmc_imageon),
 	                             1,                      // hdmioEnable = 1
 	                             &(pDemo->hdmio_timing), // pTiming
 	                             0                       // waitHPD = 0
 	                             );
   if ( !ret )
   {
      uart_puts( "ERROR : Failed to init HDMI Output Interface\n\r" );
      //exit(0);
   }

   // Clear frame stores
   Xuint32 i;
   Xuint32 storage_size = pDemo->uNumFrames_HdmiFrameBuffer * ((1280*720)<<1);
   volatile Xuint8 *pStorageMem = (Xuint8 *)pDemo->uBaseAddr_MEM_HdmiFrameBuffer;
   for ( i = 0; i < storage_size; i += 2 )
   {
      // Black Pixel
      *pStorageMem++ = 0x80; // CbCr (chroma)
      *pStorageMem++ = 0x00; // Y (luma)
   }
   //>>>>>>>>>>>>>>>>>>>>>add this <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
      volatile Xuint8 *filter = (Xuint8 *)(pDemo->uBaseAddr_MEM_HdmiFrameBuffer+offset);
            for ( i = 0; i < storage_size; i += 2 )
            {
               // Black Pixel
 //           	xil_printf("--------------------------------------------------%x--\n\r",filter);
               *filter++ = 0x40; // CbCr (chroma)
 //           	 *filter = 0x40; filter=filter+1;
   //            xil_printf("--------------------------------------------------%x--\n\r",filter);
               *filter++ = 0x20; // Y (luma)
//               *filter = 0x40; filter=filter+1;
            }
            //===============================================================================================================================


   // Initialize Output Side of AXI VDMA
   //uart_puts( "Video DMA (Output Side) Initialization new...\n\r" );
   vfb_common_init(
      pDemo->uDeviceId_VDMA_HdmiFrameBuffer, // uDeviceId
      &(pDemo->vdma_hdmi)                    // pAxiVdma
      );
   vfb_tx_init(
      &(pDemo->vdma_hdmi),                   // pAxiVdma
      &(pDemo->vdmacfg_hdmi_read),           // pReadCfg
      pDemo->hdmio_resolution,               // uVideoResolution
      pDemo->hdmio_resolution,               // uStorageResolution
//      pDemo->uBaseAddr_MEM_HdmiFrameBuffer,  // uMemAddr                 //>>>>>>>>>>>>>>>>>common this<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
      (pDemo->uBaseAddr_MEM_HdmiFrameBuffer+offset),
      pDemo->uNumFrames_HdmiFrameBuffer      // uNumFrames
      );

   // Configure VTC on output data path
   //uart_puts( "Video Timing Controller (generator) Initialization ...\n\r" );
   vgen_init( &(pDemo->vtc_hdmio_generator), pDemo->uDeviceId_VTC_HdmioGenerator );
   vgen_config( &(pDemo->vtc_hdmio_generator), pDemo->hdmio_resolution, 1 );
   //===========================add motion work======================================================
    //  Xil_DCacheDisable();
 //     volatile Xuint8 *pStorageMemOld = (Xuint8 *)pDemo->uBaseAddr_MEM_HdmiFrameBuffer;
 //     volatile Xuint8 *pStorageMemNew = (Xuint8 *)pDemo->uBaseAddr_MEM_FgBgCtxData;
//      volatile Xuint8 *pStorageMembg = 0x10000000+storage_size;
      int count_hls=0;
      int status;
      u32 input_frame;
      u32 background_frame;
      u32 output_frame;
     u32 len_frame;

     //Setup the matrix mult
     status = hls_macc_init(&HlsMacc);
     if(status != XST_SUCCESS){
   	 uart_puts("HLS peripheral setup failed\n\r");
   	 //exit(-1);
     }

  while (1)
  {
 /*  if ( iterations > 0 )
   {
      xil_printf( "\n\rPress ENTER to re-start ...\n\r" );
      getchar();
   }
*/   iterations++;

 //  xil_printf( "Waiting for ADV7611 to locked on incoming video ...\n\r" );
   pDemo->hdmii_locked = 0;
   timeout = 100;
   while ( !(pDemo->hdmii_locked) && timeout-- )
   {
      pDemo->hdmii_locked = fmc_imageon_hdmii_get_lock( &(pDemo->fmc_imageon) );
      usleep(100000); // wait 100msec ...   orignal 100000 now 10000
   }
   if ( !(pDemo->hdmii_locked) )
   {
      uart_puts( "\tERROR : ADV7611 has NOT locked on incoming video, aborting !\n\r" );
      //return -1;
      continue;
   }


   // Get Video Input information
   fmc_imageon_hdmii_get_timing( &(pDemo->fmc_imageon), &(pDemo->hdmii_timing) );
   pDemo->hdmii_width  = pDemo->hdmii_timing.HActiveVideo;
   pDemo->hdmii_height = pDemo->hdmii_timing.VActiveVideo;
   pDemo->hdmii_resolution = vres_detect( pDemo->hdmii_width, pDemo->hdmii_height );


   if ( (pDemo->hdmii_resolution) == -1 )
   {
      uart_puts( "\tERROR : Invalid resolution, aborting !\n\r" );
      //return -1;
      continue;
   }

   // Reset VTC on input data path
   vdet_init( &(pDemo->vtc_hdmii_detector), pDemo->uDeviceId_VTC_HdmiiDetector );
   vdet_reset( &(pDemo->vtc_hdmii_detector) );

//   xil_printf( "Video DMA (Input Side) Initialization ...\n\r" );

   // Stop Input Side of AXI_VDMA (from previous iteration)
   vfb_rx_stop(
      &(pDemo->vdma_hdmi)                    // pAxiVdma
      );

   // Clear frame stores
   Xuint32 i;
   Xuint32 storage_size = pDemo->uNumFrames_HdmiFrameBuffer * ((1280*720)<<1);
   volatile Xuint8 *pStorageMem = (Xuint8 *)pDemo->uBaseAddr_MEM_HdmiFrameBuffer;
   volatile Xuint8 *FirstFrame = (Xuint8 *)(pDemo->uBaseAddr_MEM_HdmiFrameBuffer+offset+offset);
   if(iterations ==10)memcpy_cus(FirstFrame, pStorageMem, storage_size/*new_storage_size*/);
   //>>>>>>>>>>>>>>>>>>>>>add this <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

//	FirstFrameP=FirstFrame;
   volatile Xuint8 *FirstFrameP=FirstFrame;
   volatile Xuint8 *filter = (Xuint8 *)(pDemo->uBaseAddr_MEM_HdmiFrameBuffer+offset);
   background_frame=FirstFrame;
   input_frame=pStorageMem;
   output_frame=filter;
   len_frame=storage_size;
           for ( i = 0; i < storage_size; i += 2 )
         {
            // Black Pixel
        	 if((*pStorageMem-*FirstFrameP)>10 || (*FirstFrameP-*pStorageMem)>10)
//        	   if(*pStorageMem==*FirstFrameP)
        	 {
        		 *filter++ = 0xffff; // CbCr (chroma)
        		 *filter++ = 0xffff; // Y (luma)
        	 }
        	 else
        	 {
        		 *filter++ = 0x00; // CbCr (chroma)
        		 *filter++ = 0x00; // Y (luma)
       	 	 }
        	   FirstFrameP=FirstFrameP+2;pStorageMem=pStorageMem+2;

 //           *filter++ = 0xff; // CbCr (chroma)
  //          *filter++ = 0xff; // Y (luma)
 //       	 *filter++=*pStorageMem++;
//        	 *filter++=*pStorageMem++;
         }

 //        memcpy(filter, FirstFrame, storage_size/*new_storage_size*/);
         //===============================================================================================================================

   // Initialize Input Side of AXI VDMA
   vfb_rx_init(
      &(pDemo->vdma_hdmi),                   // pAxiVdma
      &(pDemo->vdmacfg_hdmi_write),          // pWriteCfg
      pDemo->hdmii_resolution,               // uVideoResolution
      pDemo->hdmio_resolution,               // uStorageResolution
      pDemo->uBaseAddr_MEM_HdmiFrameBuffer,  // uMemAddr                 //>>>>>>>>>>>>>>>>>common this<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 //    (pDemo->uBaseAddr_MEM_HdmiFrameBuffer+offset),
      pDemo->uNumFrames_HdmiFrameBuffer      // uNumFrames
      );

   ret = fmc_imageon_hdmio_init( &(pDemo->fmc_imageon),
 	                             1,                      // hdmioEnable = 1
 	                             &(pDemo->hdmio_timing), // pTiming
 	                             0                       // waitHPD = 0
 	                             );
   if ( !ret )
   {
      uart_puts( "ERROR : Failed to init HDMI Output Interface\n\r" );
      //exit(0);
   }
if(iterations>11)
{
	//int count_hls=0;
	//int version=3;
	while(1)
	{
		volatile Xuint8 *pStorageMem = (Xuint8 *)pDemo->uBaseAddr_MEM_HdmiFrameBuffer;
		   volatile Xuint8 *FirstFrame = (Xuint8 *)(pDemo->uBaseAddr_MEM_HdmiFrameBuffer+offset+offset);
		   if(iterations ==10)memcpy_cus(FirstFrame, pStorageMem, storage_size/*new_storage_size*/);
		   //>>>>>>>>>>>>>>>>>>>>>add this <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#if USHLS
		//	FirstFrameP=FirstFrame;
		   volatile Xuint8 *FirstFrameP=FirstFrame;
		   volatile Xuint8 *filter = (Xuint8 *)(pDemo->uBaseAddr_MEM_HdmiFrameBuffer+offset);
		           for ( i = 0; i < storage_size; i += 2 )
		         {
		            // Black Pixel
		        	 if((*pStorageMem-*FirstFrameP)>35 || (*FirstFrameP-*pStorageMem)>35)
		//        	   if(*pStorageMem==*FirstFrameP)
		        	 {
		        		 *filter++ = 0xffff; // CbCr (chroma)
		        		 *filter++ = 0xffff; // Y (luma)
		        	 }
		        	 else
		        	 {
		        		 *filter++ = 0x00; // CbCr (chroma)
		        		 *filter++ = 0x00; // Y (luma)
		       	 	 }
		        	   FirstFrameP=FirstFrameP+2;pStorageMem=pStorageMem+2;

/*		        	   *(filter+1)=*(pStorageMem+1);
		        	   *(filter)=*(pStorageMem);
		        	   pStorageMem=pStorageMem+2;filter=filter+2;
*/
		 //           *filter++ = 0xff; // CbCr (chroma)
		  //          *filter++ = 0xff; // Y (luma)
		 //       	 *filter++=*pStorageMem++;
		//        	 *filter++=*pStorageMem++;
		         }
#else
   //=======================hls work===========================================
int timer1;
//int timer2;
int t_check;
//int frame_count;
//u32 threshold = 0x00ffff, frame_value = 0;
//u32 * frame_output = output_frame;
timer_init();
timer1 = timer_get_us();
		           while (!XHls_macc_IsReady(&HlsMacc));
		           	  XHls_macc_SetA(&HlsMacc, input_frame);
		           	     XHls_macc_SetB(&HlsMacc, background_frame);
		           	     XHls_macc_SetByte_wroffset(&HlsMacc,output_frame);
		           	     XHls_macc_SetFrame_len(&HlsMacc,len_frame);
		           	     XHls_macc_SetAccum_clr(&HlsMacc, 1);
		           	     while (!XHls_macc_IsReady(&HlsMacc));
		           	     XHls_macc_Start(&HlsMacc);
		           	    while (!XHls_macc_IsReady(&HlsMacc));
					/*for (frame_count = 0; frame_count < len_frame; frame_count++)
					{
						frame_value += *frame_output;
						frame_output +=2;
					}
					printf("0x%08x\n", frame_value);*/
					t_check = timer_get_us() - timer1;
					//printf("t_check %d\n", t_check);
					if (t_check < 250000)
					{
						return 0;
					}
					asm volatile ("smc #0\n\t");
//timer2 = timer_get_us();
//timing = (timer2 - timer1);
//printf("%d %d\n", timing, count_num);
count_num++;
//printf("count_num %d\n", count_num);
if (count_num == 200)
{
	//return 0;
	*done = 1;
}
		           	 //printf( " ==============one frame==============%d\n\r",count_hls );
		   //==================check the HLS IP version=====================
		           	//XHls_macc_SetAccum_clr(&HlsMacc, 0);
		           	//XHls_macc_Start(&HlsMacc);
		           	//while (!XHls_macc_IsReady(&HlsMacc));
		           	//version = XHls_macc_GetFlag(&HlsMacc);
   //======================hls work end==========================================

#endif
		           	//count_hls++;
		           	//printf( " ==============one frame==============%d  %d\n\r",count_hls,version );
	}
}

  }

   return 0;
}
