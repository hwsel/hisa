// ==============================================================
// File generated by Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC
// Version: 2013.3
// Copyright (C) 2013 Xilinx Inc. All rights reserved.
// 
// ==============================================================

// AXI4LiteS
// 0x00 : Control signals
//        bit 0  - ap_start (Read/Write/COH)
//        bit 1  - ap_done (Read/COR)
//        bit 2  - ap_idle (Read)
//        bit 3  - ap_ready (Read)
//        bit 7  - auto_restart (Read/Write)
//        others - reserved
// 0x04 : Global Interrupt Enable Register
//        bit 0  - Global Interrupt Enable (Read/Write)
//        others - reserved
// 0x08 : IP Interrupt Enable Register (Read/Write)
//        bit 0  - Channel 0 (ap_done)
//        bit 1  - Channel 1 (ap_ready)
//        others - reserved
// 0x0c : IP Interrupt Status Register (Read/TOW)
//        bit 0  - Channel 0 (ap_done)
//        bit 1  - Channel 1 (ap_ready)
//        others - reserved
// 0x10 : reserved
// 0x14 : Data signal of a
//        bit 31~0 - a[31:0] (Read/Write)
// 0x18 : reserved
// 0x1c : Data signal of b
//        bit 31~0 - b[31:0] (Read/Write)
// 0x20 : reserved
// 0x24 : Data signal of accum
//        bit 31~0 - accum[31:0] (Read)
// 0x28 : reserved
// 0x2c : Data signal of flag
//        bit 31~0 - flag[31:0] (Read)
// 0x30 : reserved
// 0x34 : Data signal of accum_clr
//        bit 0  - accum_clr[0] (Read/Write)
//        others - reserved
// 0x38 : reserved
// 0x3c : Data signal of byte_wroffset
//        bit 31~0 - byte_wroffset[31:0] (Read/Write)
// 0x40 : reserved
// 0x44 : Data signal of frame_len
//        bit 31~0 - frame_len[31:0] (Read/Write)
// (SC = Self Clear, COR = Clear on Read, TOW = Toggle on Write, COH = Clear on Handshake)

#define XHLS_MACC_AXI4LITES_ADDR_AP_CTRL            0x00
#define XHLS_MACC_AXI4LITES_ADDR_GIE                0x04
#define XHLS_MACC_AXI4LITES_ADDR_IER                0x08
#define XHLS_MACC_AXI4LITES_ADDR_ISR                0x0c
#define XHLS_MACC_AXI4LITES_ADDR_A_DATA             0x14
#define XHLS_MACC_AXI4LITES_BITS_A_DATA             32
#define XHLS_MACC_AXI4LITES_ADDR_B_DATA             0x1c
#define XHLS_MACC_AXI4LITES_BITS_B_DATA             32
#define XHLS_MACC_AXI4LITES_ADDR_ACCUM_DATA         0x24
#define XHLS_MACC_AXI4LITES_BITS_ACCUM_DATA         32
#define XHLS_MACC_AXI4LITES_ADDR_FLAG_DATA          0x2c
#define XHLS_MACC_AXI4LITES_BITS_FLAG_DATA          32
#define XHLS_MACC_AXI4LITES_ADDR_ACCUM_CLR_DATA     0x34
#define XHLS_MACC_AXI4LITES_BITS_ACCUM_CLR_DATA     1
#define XHLS_MACC_AXI4LITES_ADDR_BYTE_WROFFSET_DATA 0x3c
#define XHLS_MACC_AXI4LITES_BITS_BYTE_WROFFSET_DATA 32
#define XHLS_MACC_AXI4LITES_ADDR_FRAME_LEN_DATA     0x44
#define XHLS_MACC_AXI4LITES_BITS_FRAME_LEN_DATA     32

