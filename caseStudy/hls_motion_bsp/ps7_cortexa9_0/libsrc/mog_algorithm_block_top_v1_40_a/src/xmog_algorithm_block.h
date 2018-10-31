// ==============================================================
// File generated by Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC
// Version: 2013.2
// Copyright (C) 2013 Xilinx Inc. All rights reserved.
// 
// ==============================================================

#ifndef XMOG_ALGORITHM_BLOCK_H
#define XMOG_ALGORITHM_BLOCK_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#ifndef __linux__
#include "xil_types.h"
#include "xil_assert.h"
#include "xstatus.h"
#include "xil_io.h"
#else
#include <stdint.h>
#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stddef.h>
#endif
#include "xmog_algorithm_block_hw.h"

/**************************** Type Definitions ******************************/
#ifdef __linux__
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
#else
typedef struct {
    u16 DeviceId;
    u32 Hls_fgbg_periph_bus_BaseAddress;
} XMog_algorithm_block_Config;
#endif

typedef struct {
    u32 Hls_fgbg_periph_bus_BaseAddress;
    u32 IsReady;
} XMog_algorithm_block;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XMog_algorithm_block_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XMog_algorithm_block_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XMog_algorithm_block_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XMog_algorithm_block_ReadReg(BaseAddress, RegOffset) \
    *(volatile u32*)((BaseAddress) + (RegOffset))

#define Xil_AssertVoid(expr)    assert(expr)
#define Xil_AssertNonvoid(expr) assert(expr)

#define XST_SUCCESS             0
#define XST_DEVICE_NOT_FOUND    2
#define XST_OPEN_DEVICE_FAILED  3
#define XIL_COMPONENT_IS_READY  1
#endif

/************************** Function Prototypes *****************************/
#ifndef __linux__
int XMog_algorithm_block_Initialize(XMog_algorithm_block *InstancePtr, u16 DeviceId);
XMog_algorithm_block_Config* XMog_algorithm_block_LookupConfig(u16 DeviceId);
int XMog_algorithm_block_CfgInitialize(XMog_algorithm_block *InstancePtr, XMog_algorithm_block_Config *ConfigPtr);
#else
int XMog_algorithm_block_Initialize(XMog_algorithm_block *InstancePtr, const char* InstanceName);
int XMog_algorithm_block_Release(XMog_algorithm_block *InstancePtr);
#endif

void XMog_algorithm_block_Start(XMog_algorithm_block *InstancePtr);
u32 XMog_algorithm_block_IsDone(XMog_algorithm_block *InstancePtr);
u32 XMog_algorithm_block_IsIdle(XMog_algorithm_block *InstancePtr);
u32 XMog_algorithm_block_IsReady(XMog_algorithm_block *InstancePtr);
void XMog_algorithm_block_EnableAutoRestart(XMog_algorithm_block *InstancePtr);
void XMog_algorithm_block_DisableAutoRestart(XMog_algorithm_block *InstancePtr);

void XMog_algorithm_block_SetWidth(XMog_algorithm_block *InstancePtr, u32 Data);
u32 XMog_algorithm_block_GetWidth(XMog_algorithm_block *InstancePtr);
void XMog_algorithm_block_SetC_alpha(XMog_algorithm_block *InstancePtr, u32 Data);
u32 XMog_algorithm_block_GetC_alpha(XMog_algorithm_block *InstancePtr);
void XMog_algorithm_block_SetC_vinit(XMog_algorithm_block *InstancePtr, u32 Data);
u32 XMog_algorithm_block_GetC_vinit(XMog_algorithm_block *InstancePtr);
void XMog_algorithm_block_SetC_varth(XMog_algorithm_block *InstancePtr, u32 Data);
u32 XMog_algorithm_block_GetC_varth(XMog_algorithm_block *InstancePtr);
void XMog_algorithm_block_SetC_winit(XMog_algorithm_block *InstancePtr, u32 Data);
u32 XMog_algorithm_block_GetC_winit(XMog_algorithm_block *InstancePtr);
void XMog_algorithm_block_SetC_bgth(XMog_algorithm_block *InstancePtr, u32 Data);
u32 XMog_algorithm_block_GetC_bgth(XMog_algorithm_block *InstancePtr);
void XMog_algorithm_block_SetFlags(XMog_algorithm_block *InstancePtr, u32 Data);
u32 XMog_algorithm_block_GetFlags(XMog_algorithm_block *InstancePtr);

void XMog_algorithm_block_InterruptGlobalEnable(XMog_algorithm_block *InstancePtr);
void XMog_algorithm_block_InterruptGlobalDisable(XMog_algorithm_block *InstancePtr);
void XMog_algorithm_block_InterruptEnable(XMog_algorithm_block *InstancePtr, u32 Mask);
void XMog_algorithm_block_InterruptDisable(XMog_algorithm_block *InstancePtr, u32 Mask);
void XMog_algorithm_block_InterruptClear(XMog_algorithm_block *InstancePtr, u32 Mask);
u32 XMog_algorithm_block_InterruptGetEnabled(XMog_algorithm_block *InstancePtr);
u32 XMog_algorithm_block_InterruptGetStatus(XMog_algorithm_block *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif
