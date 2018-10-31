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
//                     Copyright(c) 2011 Avnet, Inc.
//                             All rights reserved.
//
//----------------------------------------------------------------
//
// Create Date:         Dec 09, 2011
// Design Name:         Video Detector
// Module Name:         video_detector.h
// Project Name:        FMC-IMAGEON
// Target Devices:      Zynq-7000
// Avnet Boards:        FMC-IMAGEON
//
// Tool versions:       ISE 14.3
//
// Description:         Video Timing Detector using:
//                      - Xilinx Video Timing Controller (VTC)
//
// Dependencies:
//
// Revision:            Dec 09, 2011: 1.00 Initial version
//                      Dec 05, 2012: 1.01 vdet_init resets VTC core
//
//----------------------------------------------------------------

#include "video_detector.h"
#include "video_resolution.h"

/*****************************************************************************/
/**
*
* vdet_init
* - initializes the VTC detector
*
* @param	VtcDeviceID is the device ID of the Video Timing Controller core.
*           pVtc is a pointer to a VTC instance

*
* @return	0 if all tests pass, 1 otherwise.
*
* @note		None.
*
******************************************************************************/
int vdet_init(XVtc *pVtc, u16 VtcDeviceID)
{
	XVtc_Config *VtcCfgPtr;
	int Status;

	/* Look for the device configuration info for the Video Timing
	 * Controller.
	 */
	VtcCfgPtr = XVtc_LookupConfig( VtcDeviceID );
	if (VtcCfgPtr == NULL) {
		return 1;
	}

	/* Initialize the Video Timing Controller instance */

	Status = XVtc_CfgInitialize(pVtc, VtcCfgPtr,
		VtcCfgPtr->BaseAddress);
	if (Status != XST_SUCCESS) {
		return 1;
	}

	return 0;
}

/*****************************************************************************/
/**
*
* vdet_reset
* - resets the VTC detector
*
* @param	pVtc is a pointer to a VTC instance

*
* @return	0 if all tests pass, 1 otherwise.
*
* @note		None.
*
******************************************************************************/
int vdet_reset(XVtc *pVtc)
{
	XVtc_Reset(pVtc);

	return 0;
}

