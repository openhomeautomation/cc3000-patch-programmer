/*****************************************************************************
*
*  PatchProgrammer.c - CC3000 Slim Driver Implementation.
*  Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*    Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the   
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*****************************************************************************/


#include "wlan.h" 
#include "evnt_handler.h"    
#include "nvmem.h"
#include "socket.h"
#include "netapp.h"
#include "spi.h"
#include "board.h"
#include <string.h>
#include <msp430.h>


//Service Pack version P1.11.7.12.13 - FW patches



const unsigned char fw_patch[4616] = { 0x00, 0x01, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x04, 0x15, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0xB8, 0xF1, 0x90, 0x0F, 0xA4, 0x16, 0x00, 0x00, 0xC8, 0x00, 0x00, 0x00, 0x66, 0xE0, 0x04, 0xA8, 0x35, 0x1D, 0x21, 0xF0, 0x99, 0xFC, 0x68, 0x46, 0x23, 0xF0, 0x28, 0xFB, 0x9A, 0xF8, 0x00, 0x00, 0x01, 0x28, 0x07, 0xD1, 0x28, 0x46, 0x05, 0xF0, 0xC3, 0xFE, 0x01, 0x46, 0x01, 0xAA, 0x00, 0x20, 0x21, 0xF0, 0x50, 0xF9, 0x28, 0x46, 0x05, 0xF0, 0xBB, 0xFE, 0x01, 0xA9, 0x21, 0xF0, 0x2A, 0xFA, 0xE9, 0x79, 0x4F, 0xEA, 0xE0, 0x00, 0x40, 0xB2, 0x11, 0xB1, 0x00, 0xF1, 0x06, 0x00, 0x40, 0xB2, 0xA8, 0x71, 0x1F, 0x38, 0x40, 0x00, 0xE8, 0x71, 0x30, 0x46, 0x01, 0xF0, 0x0D, 0xFF, 0x10, 0xF1, 0x00, 0x09, 0x4F, 0xF0, 0x00, 0x01, 0x09, 0xD0, 0x28, 0x68, 0x40, 0x0C, 0x09, 0xD3, 0xE8, 0x68, 0xC0, 0x0B, 0x03, 0xD2, 0x48, 0x46, 0xFF, 0xF7, 0xDD, 0xFE, 0x01, 0x21, 0x28, 0x68, 0x40, 0x0C, 0x0A, 0xD2 ,
0x38, 0x68, 0x40, 0x1C, 0x38, 0x60, 0x20, 0x68, 0x6F, 0xF3, 0x0F, 0x00, 0x20, 0x60, 0x22, 0x68, 0x38, 0x68, 0x10, 0x43, 0x20, 0x60, 0xE8, 0x68, 0xC0, 0x0B, 0x0F, 0xD3, 0xD8, 0xF8, 0x00, 0x00, 0x00, 0xF1, 0x01, 0x00, 0xC8, 0xF8, 0x00, 0x00, 0x20, 0x68, 0x6F, 0xF3, 0x1F, 0x40, 0x20, 0x60, 0xD8, 0xF8, 0x00, 0x20, 0x20, 0x68, 0x40, 0xEA, 0x02, 0x40, 0x20, 0x60, 0x49, 0xB9, 0xB9, 0xF1, 0x00, 0x0F, 0x03, 0xD1, 0x30, 0x46, 0x07, 0xF0, 0xBE, 0xF9, 0x02, 0xE0, 0x48, 0x46, 0x07, 0xF0, 0x06, 0xFA, 0x6C, 0x17, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x68, 0x46, 0x0E, 0xF0, 0x85, 0xFB, 0x00, 0x9E, 0x00, 0x2E, 0x96, 0xD1, 0x05, 0xB0, 0xBD, 0xE8, 0xF0, 0x87, 0xC0, 0x46, 0x9C, 0x20, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x23, 0xF0, 0x7A, 0xFD, 0x74, 0x47, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x0B, 0x21, 0x21, 0xF0, 0xCB, 0xF9, 0x20, 0x68, 0x18, 0x4B, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x21, 0xF0, 0x6C, 0xF8 ,
0x0A, 0xE0, 0x20, 0x68, 0x00, 0x68, 0x0C, 0x21, 0x40, 0xF0, 0x20, 0x00, 0x21, 0xF0, 0x64, 0xF8, 0x04, 0x62, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x20, 0x79, 0x1F, 0xF0, 0xA5, 0xFC, 0x69, 0xE0, 0x34, 0xAD, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x09, 0x68, 0xFF, 0x29, 0x98, 0xAD, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x10, 0xD4, 0x21, 0xDC, 0xD3, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0xB5, 0x18, 0xF0, 0xFB, 0xFB, 0x00, 0xF0, 0x03, 0xF8, 0x00, 0xBD, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xDF, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x17, 0xF0, 0x3C, 0xFD, 0xC8, 0xF7, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x31, 0x2E, 0x31, 0x39, 0x2E, 0x33, 0x31, 0x34, 0x5F, 0x4E, 0x65, 0x77, 0x5F, 0x46, 0x54, 0x43, 0x5F, 0x6C, 0x65, 0x6E, 0x67, 0x74, 0x68, 0x5F, 0x6D, 0x6F, 0x64, 0x75, 0x6C, 0x61, 0x74, 0x69, 0x80, 0x7E, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00, 0xDF, 0xF8, 0x58, 0x82, 0xF4, 0x7E, 0x01, 0x00, 0x04, 0x00 ,
0x00, 0x00, 0x7A, 0x48, 0x6B, 0x49, 0x9C, 0x7F, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x70, 0x51, 0x4D, 0xC4, 0x7F, 0x01, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x40, 0x49, 0x00, 0xF1, 0x52, 0x00, 0x3A, 0x4A, 0xA8, 0x81, 0x28, 0x83, 0xE8, 0x83, 0xE8, 0x84, 0xA8, 0x85, 0x2A, 0x60, 0x39, 0x48, 0x41, 0xF2, 0x11, 0x12, 0x2A, 0x85, 0x18, 0x90, 0x19, 0x91, 0x39, 0x49, 0x1A, 0x91, 0x39, 0x49, 0x1B, 0x91, 0x39, 0x49, 0x20, 0x46, 0xDF, 0xF8, 0xC4, 0x90, 0x1C, 0x91, 0x38, 0x49, 0xDF, 0xF8, 0xC0, 0xB0, 0x31, 0x4E, 0x1D, 0x91, 0x48, 0x80, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00, 0x0D, 0xF0, 0x7C, 0xFA, 0xB4, 0x80, 0x01, 0x00, 0x34, 0x00, 0x00, 0x00, 0x34, 0x12, 0x66, 0x09, 0x84, 0x6B, 0x00, 0x20, 0x94, 0x70, 0x00, 0x20, 0xB4, 0x70, 0x00, 0x20, 0xC4, 0x78, 0x00, 0x20, 0x50, 0x7A, 0x00, 0x20, 0xFE, 0xFF, 0x03, 0x00, 0xA4, 0x70, 0x00, 0x20, 0xB0, 0x70, 0x00, 0x20, 0xB8, 0x70, 0x00, 0x20, 0x60, 0x55, 0x30, 0x80 ,
0x3C, 0x5C, 0x00, 0x20, 0x04, 0x74, 0x00, 0x20, 0x04, 0x4D, 0x02, 0x00, 0xC8, 0x00, 0x00, 0x00, 0x2D, 0xE9, 0xF0, 0x4F, 0xAD, 0xF1, 0x8C, 0x0D, 0x04, 0x68, 0x0F, 0x30, 0x03, 0x94, 0xFE, 0xF7, 0xDB, 0xFA, 0x04, 0xF1, 0x14, 0x00, 0x04, 0x90, 0x21, 0x7D, 0x00, 0x20, 0x21, 0xF0, 0x73, 0x01, 0x88, 0x29, 0x08, 0xBF, 0x01, 0x20, 0x05, 0x90, 0x03, 0x98, 0x00, 0x1D, 0x06, 0x90, 0x5D, 0x48, 0x00, 0x68, 0x01, 0x28, 0x40, 0xF0, 0xFA, 0x83, 0x5B, 0x4D, 0x5C, 0x48, 0xDF, 0xF8, 0x70, 0x91, 0x29, 0x68, 0xDF, 0xF8, 0x70, 0x81, 0xDF, 0xF8, 0x74, 0xA1, 0x88, 0x42, 0x21, 0xD1, 0x10, 0x22, 0x00, 0x21, 0x48, 0x46, 0x4F, 0x46, 0xFB, 0xF7, 0xB7, 0xFE, 0x10, 0x22, 0x00, 0x21, 0x40, 0x46, 0xFB, 0xF7, 0xB2, 0xFE, 0x54, 0x49, 0x08, 0x68, 0x40, 0xF4, 0x80, 0x10, 0x08, 0x60, 0x01, 0x20, 0x51, 0x46, 0x08, 0x60, 0x00, 0x21, 0x29, 0x60, 0x39, 0x78, 0x46, 0x46, 0x00, 0x91, 0x31, 0x78, 0x41, 0xF2, 0x11, 0x13, 0x04, 0x22 ,
0x01, 0x91, 0x07, 0x21, 0x02, 0x90, 0x03, 0x20, 0xF3, 0xF7, 0x41, 0xFC, 0x51, 0x46, 0x07, 0x91, 0x08, 0x68, 0x56, 0x49, 0xDF, 0xF8, 0x5C, 0xB1, 0x01, 0x28, 0x08, 0x91, 0x56, 0x49, 0xDF, 0xF8, 0x48, 0xA1, 0x09, 0x91, 0x55, 0x49, 0x0A, 0x91, 0x44, 0x49, 0x0B, 0x91, 0x44, 0x49, 0x0C, 0x91, 0x44, 0x49, 0x0D, 0x91, 0x44, 0x49, 0x0E, 0x91, 0x44, 0x49, 0x0F, 0x91, 0x44, 0x49, 0x10, 0x91, 0x44, 0x49, 0xCC, 0x4D, 0x02, 0x00, 0xC8, 0x00, 0x00, 0x00, 0x11, 0x91, 0x44, 0x49, 0x12, 0x91, 0x44, 0x49, 0x13, 0x91, 0x44, 0x49, 0x14, 0x91, 0x44, 0x49, 0x15, 0x91, 0x44, 0x49, 0x16, 0x91, 0x9F, 0x49, 0x17, 0x91, 0x9F, 0x49, 0x18, 0x91, 0x9F, 0x49, 0x19, 0x91, 0x9F, 0x49, 0x1A, 0x91, 0x9F, 0x49, 0x1B, 0x91, 0x9F, 0x49, 0x1C, 0x91, 0x9F, 0x49, 0x1D, 0x91, 0x9F, 0x49, 0x1E, 0x91, 0x9F, 0x49, 0x1F, 0x91, 0x40, 0xF0, 0x00, 0x81, 0x04, 0x98, 0x40, 0x78, 0x00, 0xF0, 0x03, 0x01, 0x01, 0x29, 0x40, 0xF0, 0xF9, 0x80 ,
0x04, 0x99, 0x09, 0x78, 0x01, 0xF0, 0x0C, 0x01, 0x08, 0x29, 0x40, 0xF0, 0xF2, 0x80, 0x06, 0x99, 0x09, 0x88, 0xA1, 0xF1, 0x3C, 0x01, 0x0E, 0xB2, 0x05, 0x99, 0x11, 0xB1, 0xA6, 0xF1, 0x02, 0x06, 0x36, 0xB2, 0xC0, 0x09, 0x4F, 0xF0, 0x00, 0x07, 0x15, 0xD3, 0x08, 0x3E, 0x36, 0xB2, 0x03, 0x2E, 0x10, 0xD0, 0x17, 0x2E, 0x0E, 0xD0, 0x08, 0x3E, 0x36, 0xB2, 0x03, 0x2E, 0x08, 0xD0, 0x17, 0x2E, 0x06, 0xD0, 0x36, 0x1F, 0x36, 0xB2, 0x03, 0x2E, 0x14, 0xBF, 0x17, 0x2E, 0x02, 0x27, 0x02, 0xE0, 0x03, 0x27, 0x00, 0xE0, 0x01, 0x27, 0x03, 0x2E, 0x18, 0xBF, 0x17, 0x2E, 0x04, 0x9A, 0x40, 0xF0, 0xC8, 0x80, 0x0A, 0x32, 0x12, 0xF8, 0x01, 0x1B, 0x05, 0x24, 0x12, 0xF8, 0x01, 0x3B, 0x4B, 0x40, 0x64, 0x1E, 0xD9, 0xB2, 0xF9, 0xD1, 0x81, 0xEA, 0x21, 0x11, 0x03, 0x2E, 0x94, 0x4E, 0x02, 0x00, 0xC8, 0x00, 0x00, 0x00, 0x01, 0xF0, 0x0F, 0x01, 0x48, 0x46, 0x35, 0xD0, 0x43, 0x46, 0xCA, 0x5C, 0x52, 0x1C, 0xCA, 0x54, 0x0C, 0x18 ,
0x33, 0xE0, 0x48, 0x57, 0x02, 0x00, 0x04, 0x74, 0x00, 0x20, 0x34, 0x12, 0x66, 0x09, 0x58, 0x57, 0x02, 0x00, 0x68, 0x57, 0x02, 0x00, 0xC8, 0x48, 0x30, 0x80, 0x44, 0x57, 0x02, 0x00, 0x80, 0x57, 0x02, 0x00, 0x78, 0x57, 0x02, 0x00, 0x10, 0x58, 0x02, 0x00, 0x18, 0x58, 0x02, 0x00, 0x20, 0x58, 0x02, 0x00, 0x28, 0x58, 0x02, 0x00, 0x30, 0x58, 0x02, 0x00, 0x38, 0x58, 0x02, 0x00, 0xDC, 0x58, 0x02, 0x00, 0xDA, 0x58, 0x02, 0x00, 0x0C, 0x58, 0x02, 0x00, 0xE7, 0x58, 0x02, 0x00, 0xD8, 0x58, 0x02, 0x00, 0xE6, 0x58, 0x02, 0x00, 0x54, 0x57, 0x02, 0x00, 0xDE, 0x58, 0x02, 0x00, 0xE0, 0x58, 0x02, 0x00, 0x0C, 0x18, 0x20, 0x78, 0x40, 0x1C, 0x20, 0x70, 0x20, 0x78, 0x43, 0x46, 0x03, 0xEB, 0x01, 0x08, 0x0A, 0x28, 0x76, 0xDB, 0x98, 0xF8, 0x00, 0x00, 0x0A, 0x28, 0x72, 0xDB, 0x94, 0x48, 0x00, 0x25, 0x01, 0x60, 0x04, 0x98, 0x00, 0xF1, 0x0A, 0x09, 0x40, 0x78, 0x41, 0x08, 0x24, 0xBF, 0x04, 0x99, 0x09, 0x1D, 0x04, 0xD2 ,
0x80, 0x08, 0x2E, 0xBF, 0x49, 0x46, 0x04, 0x99, 0x10, 0x31, 0x0B, 0x98, 0x06, 0x22, 0xFA, 0xF7, 0xB9, 0xFC, 0x0C, 0x98, 0x06, 0x22, 0x49, 0x46, 0xFA, 0xF7, 0xB4, 0xFC, 0x9B, 0x48, 0x9A, 0x49, 0x5C, 0x4F, 0x02, 0x00, 0xC8, 0x00, 0x00, 0x00, 0x00, 0x78, 0x08, 0x22, 0x08, 0x60, 0x58, 0x46, 0x07, 0x60, 0x0D, 0x98, 0x29, 0x46, 0xFB, 0xF7, 0xAF, 0xFD, 0x08, 0x22, 0x0E, 0x98, 0x29, 0x46, 0xFB, 0xF7, 0xAA, 0xFD, 0x08, 0x22, 0x0F, 0x98, 0x29, 0x46, 0xFB, 0xF7, 0xA5, 0xFD, 0x08, 0x22, 0x10, 0x98, 0x29, 0x46, 0xFB, 0xF7, 0xA0, 0xFD, 0x08, 0x22, 0x11, 0x98, 0x29, 0x46, 0xFB, 0xF7, 0x9B, 0xFD, 0x08, 0x22, 0x12, 0x98, 0x29, 0x46, 0xFB, 0xF7, 0x96, 0xFD, 0x07, 0x99, 0x02, 0x20, 0x08, 0x60, 0x13, 0x99, 0x28, 0x46, 0x08, 0x80, 0x08, 0x99, 0x08, 0x70, 0x14, 0x99, 0x08, 0x70, 0x15, 0x99, 0x08, 0x60, 0x16, 0x99, 0x08, 0x70, 0x17, 0x99, 0x08, 0x60, 0x18, 0x99, 0x08, 0x60, 0x19, 0x99, 0x08, 0x60, 0x09, 0x99 ,
0x08, 0x80, 0x0A, 0x99, 0x08, 0x80, 0x1A, 0x99, 0x08, 0x80, 0x7D, 0x49, 0x0D, 0x60, 0x1B, 0x99, 0x08, 0x70, 0x1C, 0x99, 0x08, 0x70, 0x1D, 0x99, 0x08, 0x70, 0x1E, 0x99, 0x08, 0x80, 0x1F, 0x99, 0x08, 0x80, 0x51, 0x46, 0x08, 0x80, 0x20, 0x78, 0x00, 0x96, 0x04, 0x22, 0x01, 0x90, 0x98, 0xF8, 0x00, 0x00, 0x41, 0xF2, 0x14, 0x13, 0x07, 0x21, 0x02, 0x90, 0x03, 0x20, 0xF3, 0xF7, 0x07, 0xFB, 0x07, 0x98, 0xA2, 0x49, 0x00, 0x68, 0x20, 0x91, 0xA2, 0x49, 0x05, 0x28, 0x21, 0x91, 0x9E, 0x49, 0x22, 0x91, 0x40, 0xF0, 0x37, 0x82, 0x04, 0x98, 0x40, 0x78, 0x24, 0x50, 0x02, 0x00, 0xC8, 0x00, 0x00, 0x00, 0x41, 0x08, 0x24, 0xBF, 0x04, 0x99, 0x09, 0x1D, 0x05, 0xD2, 0x80, 0x08, 0x27, 0xBF, 0x04, 0x99, 0x0A, 0x31, 0x04, 0x99, 0x10, 0x31, 0x0B, 0x98, 0xF8, 0xF7, 0x2D, 0xFC, 0x01, 0x28, 0x04, 0x98, 0x40, 0xF0, 0x23, 0x82, 0x40, 0x78, 0x00, 0xF0, 0x03, 0x00, 0x03, 0x28, 0x18, 0xD0, 0x02, 0x28, 0x07, 0xBF, 0x04, 0x99 ,
0x10, 0x31, 0x04, 0x99, 0x0A, 0x31, 0x13, 0xE0, 0x40, 0x58, 0x02, 0x00, 0x44, 0x58, 0x02, 0x00, 0x48, 0x58, 0x02, 0x00, 0xE8, 0x58, 0x02, 0x00, 0xEA, 0x58, 0x02, 0x00, 0xEB, 0x58, 0x02, 0x00, 0xEC, 0x58, 0x02, 0x00, 0xE2, 0x58, 0x02, 0x00, 0xE4, 0x58, 0x02, 0x00, 0x04, 0x99, 0x18, 0x31, 0x0C, 0x98, 0xF8, 0xF7, 0x06, 0xFC, 0x04, 0x99, 0x01, 0x28, 0x40, 0xF0, 0xFC, 0x81, 0x49, 0x78, 0x01, 0xF0, 0x03, 0x01, 0x01, 0x29, 0x40, 0xF0, 0xF6, 0x81, 0x59, 0x46, 0x09, 0x68, 0x04, 0x9A, 0x8E, 0x46, 0x51, 0x46, 0xD2, 0x8A, 0x0B, 0x88, 0x12, 0x11, 0x93, 0x42, 0x00, 0xF0, 0x37, 0x82, 0x0A, 0x80, 0x06, 0x99, 0x09, 0x88, 0x3C, 0x39, 0x0B, 0xB2, 0x71, 0x46, 0x49, 0x1E, 0x09, 0xD0, 0x49, 0x1E, 0x04, 0xD0, 0x49, 0x1E, 0x08, 0xD1, 0xA3, 0xF1, 0x10, 0x03, 0x04, 0xE0, 0xA3, 0xF1, 0x14, 0x03, 0x01, 0xE0, 0xA3, 0xF1, 0x08, 0x03, 0x1B, 0xB2, 0x05, 0x98, 0x10, 0xB1, 0xA3, 0xF1, 0x02, 0x03, 0x1B, 0xB2, 0xEC, 0x50 ,
0x02, 0x00, 0xC8, 0x00, 0x00, 0x00, 0x00, 0x2B, 0x00, 0xF1, 0xCE, 0x81, 0x16, 0x98, 0x40, 0xF2, 0x77, 0x51, 0x99, 0x42, 0x0B, 0x90, 0x14, 0x98, 0x05, 0x90, 0x00, 0xF0, 0xA3, 0x81, 0x08, 0x46, 0x00, 0xF1, 0x3C, 0x00, 0x98, 0x42, 0x05, 0x9A, 0x00, 0xF0, 0x9A, 0x81, 0xDF, 0xF8, 0x48, 0x82, 0x14, 0x78, 0x0B, 0x9A, 0xBB, 0x2C, 0x12, 0x78, 0x0C, 0x92, 0x63, 0xD1, 0xDD, 0xF8, 0x68, 0xC0, 0xBC, 0xF9, 0x00, 0x20, 0x91, 0x42, 0x18, 0xBF, 0x90, 0x42, 0x5B, 0xD1, 0x1C, 0x2B, 0x4F, 0xF0, 0x00, 0x00, 0xC0, 0xF2, 0x82, 0x81, 0x3D, 0x2B, 0x13, 0x9D, 0x80, 0xF2, 0x7E, 0x81, 0xA3, 0xF1, 0x1C, 0x01, 0x09, 0xB2, 0x29, 0x80, 0x1E, 0x9A, 0xDD, 0xF8, 0x30, 0x90, 0xB2, 0xF9, 0x00, 0x60, 0xB9, 0xF1, 0x01, 0x0F, 0x15, 0xD1, 0xDD, 0xF8, 0x24, 0x90, 0x03, 0x2E, 0x0D, 0xDA, 0xB9, 0xF9, 0x00, 0x70, 0xB9, 0x42, 0x07, 0xD0, 0x01, 0x26, 0x16, 0x80, 0x4A, 0x46, 0x0E, 0x46, 0x03, 0xE0, 0xC0, 0x46, 0x88, 0x57, 0x02, 0x00 ,
0x76, 0x1C, 0x16, 0x80, 0x1B, 0x9A, 0x16, 0x78, 0x1D, 0x9A, 0x16, 0x70, 0x0C, 0x9A, 0x1F, 0x9E, 0x02, 0x2A, 0x28, 0xD1, 0xB6, 0xF9, 0x00, 0x20, 0xBE, 0xF1, 0x00, 0x0F, 0x1A, 0xD0, 0x03, 0x2A, 0x35, 0x46, 0x0B, 0xDA, 0x0A, 0x9E, 0xB6, 0xF9, 0x00, 0x70, 0xB9, 0x42, 0x04, 0xD0, 0x01, 0x22, 0x2A, 0x80, 0x0A, 0x46, 0x35, 0x46, 0x00, 0xE0, 0xB4, 0x51, 0x02, 0x00, 0xC8, 0x00, 0x00, 0x00, 0x52, 0x1C, 0x2A, 0x80, 0x1C, 0x99, 0x0A, 0x78, 0x1D, 0x99, 0x0A, 0x70, 0x0F, 0xE0, 0xC0, 0x46, 0x50, 0x57, 0x02, 0x00, 0xA6, 0x64, 0x00, 0x20, 0x4C, 0x57, 0x02, 0x00, 0x28, 0x80, 0x05, 0x99, 0x08, 0x70, 0x08, 0x9A, 0x11, 0x78, 0x41, 0xF0, 0x04, 0x01, 0x11, 0x70, 0x04, 0x46, 0x45, 0x46, 0xAC, 0xF8, 0x00, 0x00, 0x28, 0x70, 0xCC, 0x2C, 0x09, 0x9A, 0x40, 0xF0, 0x00, 0x81, 0xDD, 0xF8, 0x7C, 0xC0, 0x0A, 0x99, 0x1E, 0x9F, 0x45, 0x46, 0xB1, 0xF9, 0x00, 0x60, 0xB2, 0xF9, 0x00, 0x90, 0x28, 0x78, 0xCD, 0xF8, 0x50, 0xC0 ,
0x16, 0x91, 0x0A, 0x97, 0x13, 0x92, 0x09, 0x95, 0x0C, 0x99, 0x01, 0x29, 0x05, 0xD1, 0x05, 0x9A, 0xB0, 0xEB, 0x49, 0x0F, 0x01, 0xD1, 0x00, 0x21, 0x11, 0x70, 0x0C, 0x99, 0x02, 0x29, 0x05, 0xD1, 0x05, 0x9A, 0xB0, 0xEB, 0x46, 0x0F, 0x01, 0xD1, 0x00, 0x21, 0x11, 0x70, 0xB3, 0xF5, 0x14, 0x7F, 0x40, 0xF3, 0xBA, 0x80, 0x40, 0xF2, 0x51, 0x31, 0x99, 0x42, 0x08, 0x99, 0x40, 0xF3, 0xB4, 0x80, 0x09, 0x78, 0x06, 0x29, 0x00, 0xF0, 0xB0, 0x80, 0xA3, 0xF2, 0x51, 0x21, 0x0B, 0xB2, 0x4F, 0xEA, 0xE3, 0x01, 0x03, 0xEB, 0x11, 0x71, 0x4F, 0xEA, 0x21, 0x11, 0xCA, 0xB2, 0x4F, 0xEA, 0xE3, 0x01, 0x03, 0xEB, 0x11, 0x71, 0x15, 0x9D, 0x21, 0xF0, 0x0F, 0x01, 0xA3, 0xEB, 0x01, 0x03, 0x1D, 0x99, 0x2E, 0x68, 0x7C, 0x52, 0x02, 0x00, 0xC8, 0x00, 0x00, 0x00, 0x91, 0xF8, 0x00, 0xE0, 0x5F, 0xFA, 0x83, 0xF8, 0x60, 0xB1, 0x31, 0x18, 0x11, 0xF8, 0x01, 0x1C, 0x51, 0x40, 0xCB, 0xB2, 0x07, 0xE0, 0xC0, 0x46, 0x50, 0x58, 0x02, 0x00 ,
0x4C, 0x58, 0x02, 0x00, 0xD4, 0x58, 0x02, 0x00, 0x13, 0x46, 0x19, 0x99, 0xDD, 0xF8, 0x6C, 0xA0, 0xDD, 0xF8, 0x70, 0xB0, 0xD1, 0xF8, 0x00, 0xC0, 0x00, 0xF0, 0x0F, 0x01, 0x99, 0x42, 0x22, 0xD0, 0x0A, 0x9B, 0x86, 0x45, 0x9A, 0xF8, 0x00, 0x10, 0xB3, 0xF9, 0x00, 0x30, 0x9B, 0xF8, 0x00, 0x50, 0x15, 0xDD, 0x37, 0x5C, 0x44, 0x1C, 0x82, 0xEA, 0x07, 0x0C, 0x04, 0xF0, 0x0F, 0x07, 0x67, 0x45, 0x0B, 0xD0, 0xA6, 0x45, 0x06, 0x44, 0x0A, 0xDD, 0x76, 0x78, 0x72, 0x40, 0x86, 0x1C, 0x06, 0xF0, 0x0F, 0x06, 0x96, 0x42, 0x03, 0xD1, 0xC0, 0x1C, 0x4D, 0xE0, 0x80, 0x1C, 0x4B, 0xE0, 0x05, 0x9E, 0x00, 0x22, 0x32, 0x70, 0x4A, 0xE0, 0x00, 0xF0, 0x1F, 0x01, 0x01, 0x22, 0x8A, 0x40, 0x17, 0x99, 0xC4, 0x10, 0x0B, 0x68, 0x24, 0xF0, 0x03, 0x01, 0x5C, 0xF8, 0x01, 0x70, 0x3A, 0x42, 0x32, 0xD1, 0x5F, 0x58, 0x3A, 0x42, 0x06, 0x44, 0x1F, 0xD0, 0x18, 0x9B, 0x37, 0x78, 0x1B, 0x68, 0xB8, 0x45, 0x04, 0xD1, 0x58, 0x58, 0x02, 0x42 ,
0x19, 0xD0, 0x63, 0x46, 0x15, 0xE0, 0xA1, 0x10, 0x8C, 0x00, 0x19, 0x59, 0x91, 0x43, 0x19, 0x51, 0x00, 0x90, 0x30, 0x78, 0x01, 0x90, 0x44, 0x53, 0x02, 0x00, 0xC8, 0x00, 0x00, 0x00, 0x03, 0x20, 0x06, 0x21, 0x02, 0x46, 0x4D, 0xF6, 0x80, 0x53, 0xF3, 0xF7, 0x63, 0xF9, 0x09, 0x99, 0x28, 0x68, 0x0E, 0x78, 0x36, 0x18, 0x06, 0xE0, 0xDB, 0x58, 0x02, 0x00, 0xA0, 0x10, 0x81, 0x00, 0x58, 0x58, 0x02, 0x43, 0x5A, 0x50, 0x86, 0xF8, 0x00, 0x80, 0x0B, 0x98, 0x00, 0x78, 0x0C, 0x90, 0x13, 0x98, 0xB0, 0xF9, 0x00, 0x90, 0x09, 0x98, 0x00, 0x78, 0x0A, 0x9A, 0x40, 0x1C, 0x9B, 0xF8, 0x00, 0x50, 0x9A, 0xF8, 0x00, 0x10, 0xB2, 0xF9, 0x00, 0x30, 0x09, 0x9A, 0xC0, 0xB2, 0x10, 0x70, 0x0C, 0x9A, 0x01, 0x2A, 0x03, 0xD1, 0x88, 0x42, 0xC8, 0xBF, 0x8A, 0xF8, 0x00, 0x00, 0x0C, 0x99, 0x02, 0x29, 0x07, 0xD1, 0xA8, 0x42, 0xC8, 0xBF, 0x8B, 0xF8, 0x00, 0x00, 0x02, 0xE0, 0x0A, 0x98, 0xB0, 0xF9, 0x00, 0x30, 0xB9, 0xF1, 0x00, 0x0F ,
0x06, 0xDD, 0x03, 0x2B, 0x04, 0xD1, 0x11, 0x98, 0x01, 0x22, 0x49, 0x46, 0x00, 0xF0, 0x70, 0xFB, 0x16, 0x98, 0xB0, 0xF9, 0x00, 0x10, 0x14, 0x98, 0x00, 0x29, 0xB0, 0xF9, 0x00, 0x00, 0x05, 0xDD, 0x03, 0x28, 0x03, 0xD1, 0x12, 0x98, 0x02, 0x22, 0x00, 0xF0, 0x62, 0xFB, 0x05, 0x98, 0x04, 0x78, 0x0B, 0x98, 0x00, 0x78, 0x0C, 0x90, 0x08, 0x98, 0xBB, 0x2C, 0x01, 0x78, 0x02, 0xD0, 0xAA, 0x2C, 0x2F, 0xD0, 0x31, 0xE0, 0x0C, 0x98, 0x01, 0x28, 0x0B, 0xD1, 0x0D, 0x98, 0x17, 0x9A, 0x10, 0x60, 0x0C, 0x54, 0x02, 0x00, 0xC8, 0x00, 0x00, 0x00, 0x0F, 0x98, 0x18, 0x9A, 0x10, 0x60, 0x19, 0x9A, 0x11, 0x98, 0x10, 0x60, 0x47, 0x4A, 0x15, 0x98, 0x02, 0x60, 0x0C, 0x98, 0x02, 0x28, 0x0B, 0xD1, 0x0E, 0x98, 0x17, 0x9A, 0x10, 0x60, 0x10, 0x98, 0x18, 0x9A, 0x10, 0x60, 0x19, 0x9A, 0x12, 0x98, 0x10, 0x60, 0x41, 0x4A, 0x15, 0x98, 0x02, 0x60, 0xCC, 0x20, 0x0F, 0xE0, 0x08, 0x99, 0x09, 0x78, 0x0C, 0xE0, 0x02, 0x20, 0x00, 0xE0 ,
0x01, 0x20, 0x0B, 0x99, 0x08, 0x70, 0x1A, 0x99, 0x0B, 0x80, 0x13, 0x99, 0x00, 0x20, 0x08, 0x80, 0x08, 0x98, 0x01, 0x78, 0xBB, 0x20, 0x05, 0x9A, 0x10, 0x70, 0x06, 0x29, 0x07, 0x99, 0x11, 0xD1, 0x06, 0x20, 0x08, 0x60, 0x20, 0x98, 0x00, 0x25, 0x05, 0x70, 0x21, 0x98, 0x29, 0x46, 0x04, 0x22, 0xFB, 0xF7, 0x27, 0xFB, 0x01, 0x20, 0x00, 0xF0, 0x36, 0xFA, 0x02, 0x20, 0x00, 0xF0, 0x33, 0xFA, 0x22, 0x98, 0x05, 0x60, 0x07, 0x98, 0x04, 0x99, 0x00, 0x68, 0x07, 0x28, 0x44, 0xD1, 0x09, 0x78, 0x03, 0x9C, 0x80, 0x29, 0x40, 0xD1, 0x38, 0x34, 0x21, 0x78, 0x64, 0x1C, 0x22, 0x46, 0x00, 0x29, 0x3A, 0xD1, 0x20, 0x98, 0x90, 0xF8, 0x00, 0xA0, 0x22, 0x98, 0x05, 0x90, 0xD0, 0xF8, 0x00, 0x90, 0x51, 0x46, 0x48, 0x46, 0xFD, 0xF7, 0x79, 0xF9, 0x5F, 0xFA, 0x80, 0xF8, 0x21, 0x9F, 0x15, 0x78, 0x51, 0x46, 0x48, 0x46, 0x17, 0xF8, 0x08, 0x30, 0xFD, 0xF7, 0xD4, 0x54, 0x02, 0x00, 0xC8, 0x00, 0x00, 0x00, 0x6F, 0xF9, 0x1E, 0x46 ,
0x1B, 0x1D, 0xC0, 0xB2, 0x38, 0x5C, 0x00, 0x1D, 0xA8, 0x42, 0x28, 0x46, 0x07, 0xDD, 0x80, 0x18, 0x82, 0x1C, 0x10, 0x78, 0x2D, 0x18, 0xAD, 0x1C, 0xAD, 0xB2, 0xAB, 0x42, 0xF7, 0xDC, 0x04, 0xF8, 0x01, 0x6B, 0x11, 0x49, 0x32, 0x46, 0x20, 0x46, 0x01, 0xEB, 0x48, 0x11, 0xFA, 0xF7, 0xDC, 0xF9, 0xAD, 0x1B, 0xFF, 0x20, 0x34, 0x19, 0xAD, 0x1E, 0x04, 0xF8, 0x01, 0x0B, 0x25, 0x70, 0x05, 0x98, 0x01, 0x68, 0x49, 0x1C, 0x01, 0x60, 0x07, 0x98, 0x00, 0x68, 0x07, 0x28, 0x04, 0xD0, 0x06, 0x99, 0x08, 0x68, 0x20, 0xF4, 0x80, 0x30, 0x08, 0x60, 0x23, 0xB0, 0xBD, 0xE8, 0xF0, 0x8F, 0xC0, 0x46, 0x8C, 0x57, 0x02, 0x00, 0xCC, 0x57, 0x02, 0x00, 0x54, 0x58, 0x02, 0x00, 0x6D, 0x48, 0x2D, 0xE9, 0xF0, 0x47, 0x6D, 0x4D, 0x82, 0x89, 0x4A, 0xF6, 0x55, 0x21, 0xAD, 0xF5, 0xFE, 0x7D, 0x91, 0x42, 0x4F, 0xF0, 0x00, 0x01, 0xAD, 0xF1, 0x38, 0x0D, 0x10, 0xD1, 0x29, 0x60, 0x41, 0xF2, 0x10, 0x41, 0x81, 0x81, 0x4F, 0xF6, 0xFF, 0x70 ,
0x65, 0x49, 0x08, 0x60, 0x28, 0x68, 0x00, 0x90, 0x02, 0x22, 0x05, 0x21, 0x03, 0x20, 0x41, 0xF2, 0x34, 0x23, 0xF3, 0xF7, 0x4A, 0xF8, 0x29, 0x68, 0x60, 0x4C, 0x01, 0x29, 0x20, 0x68, 0x40, 0xF0, 0xAF, 0x80, 0x5E, 0x4D, 0x02, 0x28, 0x29, 0x68, 0x23, 0xD1, 0x5D, 0x48, 0x00, 0x68, 0x9C, 0x55, 0x02, 0x00, 0xC8, 0x00, 0x00, 0x00, 0x5E, 0x4A, 0x12, 0x5C, 0x00, 0x92, 0x01, 0x91, 0x5D, 0x49, 0x09, 0x68, 0x02, 0x91, 0x5A, 0x49, 0x0B, 0x5C, 0x03, 0x20, 0x07, 0x21, 0x04, 0x22, 0xF3, 0xF7, 0x30, 0xF8, 0x5A, 0x49, 0x4A, 0x79, 0x58, 0x48, 0x00, 0x92, 0x02, 0x79, 0x01, 0x92, 0x40, 0x79, 0x04, 0x22, 0x02, 0x90, 0x03, 0x20, 0x0B, 0x79, 0x07, 0x21, 0xF3, 0xF7, 0x22, 0xF8, 0x03, 0x20, 0x20, 0x60, 0xEC, 0xF7, 0x1C, 0xFF, 0x20, 0x68, 0x29, 0x68, 0x04, 0x28, 0x15, 0xD1, 0x0D, 0x20, 0x8D, 0xF8, 0x10, 0x00, 0x00, 0x25, 0x8D, 0xF8, 0x14, 0x50, 0x8D, 0xF8, 0x15, 0x10, 0x28, 0x46, 0x63, 0x21, 0x00, 0xF0, 0xFC, 0xFA ,
0x63, 0x21, 0x28, 0x46, 0x00, 0xF0, 0x86, 0xFA, 0x03, 0xA9, 0x04, 0xA8, 0xE0, 0xF7, 0xCC, 0xFC, 0x05, 0x20, 0x20, 0x60, 0x45, 0x4D, 0x05, 0x28, 0x29, 0x68, 0x01, 0xF1, 0x01, 0x01, 0x29, 0x60, 0x1D, 0xD1, 0x43, 0x4A, 0x8A, 0x42, 0x1A, 0xD1, 0x42, 0x48, 0x01, 0x68, 0x00, 0x91, 0x40, 0x68, 0x01, 0x90, 0x03, 0x20, 0x02, 0x46, 0x06, 0x21, 0x40, 0x4E, 0x33, 0x46, 0x22, 0x3B, 0xF2, 0xF7, 0xED, 0xFF, 0x3D, 0x48, 0x01, 0x68, 0x00, 0x91, 0x40, 0x68, 0x33, 0x46, 0x06, 0x21, 0x01, 0x90, 0x03, 0x20, 0x02, 0x46, 0xF2, 0xF7, 0xE2, 0xFF, 0x00, 0x20, 0x28, 0x60, 0x20, 0x68, 0x06, 0x28, 0x47, 0xD1, 0x4F, 0xF0, 0x07, 0x0A, 0xC4, 0xF8, 0x64, 0x56, 0x02, 0x00, 0xC8, 0x00, 0x00, 0x00, 0x00, 0xA0, 0x03, 0x20, 0x48, 0xF2, 0x70, 0x53, 0x01, 0x22, 0x04, 0x21, 0xF2, 0xF7, 0xD2, 0xFF, 0xDF, 0xF8, 0xC8, 0x80, 0x30, 0x4F, 0x48, 0xF2, 0x70, 0x59, 0x01, 0x25, 0x49, 0xEA, 0x05, 0x03, 0x01, 0x22, 0x04, 0x21, 0x03, 0x20 ,
0xF2, 0xF7, 0xC5, 0xFF, 0x00, 0x24, 0x17, 0xF8, 0x02, 0x0F, 0x20, 0xE0, 0x69, 0x01, 0x04, 0xEB, 0x08, 0x02, 0x88, 0x18, 0xC6, 0x1C, 0x43, 0x78, 0x00, 0x93, 0x80, 0x78, 0x01, 0x90, 0x30, 0x78, 0x02, 0x90, 0x53, 0x5C, 0x51, 0x46, 0x04, 0x22, 0x03, 0x20, 0xF2, 0xF7, 0xB0, 0xFF, 0x38, 0x78, 0x01, 0x1B, 0xB1, 0xF1, 0xFF, 0x3F, 0x08, 0xD1, 0xF0, 0x78, 0x00, 0x90, 0xB3, 0x78, 0x03, 0x20, 0x02, 0x22, 0x05, 0x21, 0xF2, 0xF7, 0xA3, 0xFF, 0x38, 0x78, 0x24, 0x1D, 0xE4, 0xB2, 0xA0, 0x42, 0xDC, 0xDC, 0xAD, 0x1C, 0xED, 0xB2, 0x04, 0x2D, 0xCD, 0xDB, 0x63, 0x21, 0x4F, 0xF4, 0x05, 0x70, 0xF1, 0xF7, 0xA5, 0xFC, 0xE1, 0xF7, 0x87, 0xFD, 0x7F, 0xB0, 0x0E, 0xB0, 0xBD, 0xE8, 0xF0, 0x87, 0xC0, 0x46, 0x04, 0x74, 0x00, 0x20, 0x48, 0x57, 0x02, 0x00, 0x40, 0x00, 0x3D, 0x80, 0x44, 0x57, 0x02, 0x00, 0x50, 0x57, 0x02, 0x00, 0x88, 0x57, 0x02, 0x00, 0x58, 0x57, 0x02, 0x00, 0x68, 0x57, 0x02, 0x00, 0x54, 0x57, 0x02, 0x00 ,
0x78, 0x57, 0x02, 0x00, 0x80, 0x57, 0x02, 0x00, 0x4C, 0x57, 0x02, 0x00, 0x2C, 0x57, 0x02, 0x00, 0xC8, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x03, 0x00, 0x30, 0x58, 0x02, 0x00, 0x38, 0x58, 0x02, 0x00, 0x66, 0x55, 0xDD, 0xEE, 0xD3, 0x58, 0x02, 0x00, 0x54, 0x58, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF4, 0x57, 0x02, 0x00, 0xC8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBC, 0x58, 0x02, 0x00, 0xC8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2D, 0xE9, 0xFE, 0x4F, 0x4A, 0x4A, 0x49, 0x4D, 0x00, 0x90, 0xB2, 0xF9, 0x00, 0x20, 0x43, 0xA3, 0x29, 0x78, 0x01, 0x93, 0x43, 0xA3, 0x01, 0x28, 0x02, 0x93, 0x47, 0x4E, 0xDF, 0xF8, 0x1C, 0x81, 0xDF, 0xF8, 0x1C, 0x91, 0x1E, 0xD0, 0x43, 0x48, 0x04, 0x78, 0x48, 0x01, 0x87, 0x19, 0x30, 0x18, 0x01, 0x99, 0x06, 0x22, 0xF9, 0xF7, 0xCC, 0xFF, 0x40, 0x46, 0x01, 0x78, 0x00, 0x68, 0x07, 0xF1, 0x06, 0x07, 0x07, 0xF8, 0x01, 0x1B, 0x07, 0xF8, 0x01, 0x4B, 0x4F, 0xF0, 0x00, 0x01, 0x07, 0xF8, 0x01, 0x1B, 0x20, 0xB9, 0x39, 0x70, 0x28, 0x78, 0x4E, 0x46, 0x0A, 0x21, 0x58, 0xE0, 0xDF, 0xF8, 0xE4, 0xB0, 0x0F, 0xE0, 0xD4, 0xB2, 0x48, 0x01, 0x87, 0x19, 0x30, 0x18, 0x02, 0x99, 0x07, 0x22 ,
0xF9, 0xF7, 0xAE, 0xFF, 0xFF, 0x1D, 0x00, 0x20, 0xDF, 0xF8, 0xCC, 0xB0, 0x07, 0xF8, 0x01, 0x4B, 0x07, 0xF8, 0x01, 0x0B, 0x16, 0x2C, 0xB2, 0x46, 0xAE, 0xBF, 0x14, 0x20, 0x60, 0x1E, 0xC0, 0xB2, 0x38, 0x70, 0x4E, 0x46, 0x84, 0x59, 0x02, 0x00, 0xC8, 0x00, 0x00, 0x00, 0x7F, 0x1C, 0x40, 0x1C, 0xC2, 0xB2, 0x28, 0x78, 0x5B, 0x46, 0x00, 0x21, 0x32, 0x54, 0x38, 0x46, 0x00, 0xF0, 0x10, 0xF9, 0x28, 0x78, 0x16, 0x2C, 0x31, 0x5C, 0x01, 0xF1, 0x0A, 0x01, 0x31, 0x54, 0x00, 0xF1, 0x01, 0x00, 0xC0, 0xB2, 0x28, 0x70, 0x2B, 0xDB, 0x00, 0x99, 0x0A, 0xEB, 0x40, 0x10, 0x07, 0x46, 0x01, 0x29, 0xA4, 0xF1, 0x01, 0x09, 0x09, 0xD0, 0x01, 0x99, 0x06, 0x22, 0xF9, 0xF7, 0x7D, 0xFF, 0x40, 0x46, 0x00, 0x78, 0xBF, 0x1D, 0x07, 0xF8, 0x01, 0x0B, 0x04, 0xE0, 0x02, 0x99, 0x07, 0x22, 0xF9, 0xF7, 0x73, 0xFF, 0xFF, 0x1D, 0x07, 0xF8, 0x01, 0x4B, 0x15, 0x3C, 0x15, 0x21, 0x28, 0x78, 0xE2, 0xB2, 0x5B, 0x46, 0x07, 0xF8, 0x01, 0x1B ,
0x32, 0x54, 0x07, 0xF8, 0x01, 0x9B, 0x38, 0x46, 0x00, 0xF0, 0xDF, 0xF8, 0x28, 0x78, 0x31, 0x5C, 0x0A, 0x31, 0x31, 0x54, 0x40, 0x1C, 0x28, 0x70, 0xBD, 0xE8, 0xFE, 0x8F, 0xC0, 0x46, 0x54, 0x54, 0x54, 0x4B, 0x45, 0x59, 0x00, 0xC0, 0x54, 0x54, 0x54, 0x53, 0x53, 0x49, 0x44, 0x00, 0x4C, 0x58, 0x02, 0x00, 0xDE, 0x58, 0x02, 0x00, 0xE0, 0x58, 0x02, 0x00, 0x54, 0x58, 0x02, 0x00, 0x54, 0x57, 0x02, 0x00, 0xD4, 0x58, 0x02, 0x00, 0xCC, 0x57, 0x02, 0x00, 0x8C, 0x57, 0x02, 0x00, 0x7E, 0xB5, 0x04, 0x46, 0x0D, 0x46, 0x94, 0xF8, 0x36, 0x10, 0x15, 0x4A, 0x07, 0x29, 0x18, 0xBF, 0x4C, 0x5A, 0x02, 0x00, 0xC8, 0x00, 0x00, 0x00, 0x11, 0x68, 0x0A, 0xD1, 0x01, 0x21, 0x11, 0x60, 0x12, 0x4B, 0x0D, 0x26, 0x04, 0xF1, 0x24, 0x02, 0x76, 0x1E, 0x42, 0xF8, 0x14, 0x3F, 0x53, 0x60, 0xFA, 0xD1, 0x94, 0xF8, 0x36, 0x00, 0x0E, 0x4E, 0x00, 0x90, 0x30, 0x68, 0x41, 0xF2, 0x12, 0x13, 0x04, 0x22, 0x01, 0x90, 0x02, 0x91, 0x07, 0x21 ,
0x03, 0x20, 0xF2, 0xF7, 0xCB, 0xFD, 0x29, 0x46, 0x20, 0x46, 0xE7, 0xF7, 0x81, 0xFC, 0x31, 0x68, 0x00, 0x22, 0x05, 0x29, 0x02, 0xD1, 0x05, 0x49, 0x08, 0x31, 0x0A, 0x60, 0x7E, 0xBD, 0xC0, 0x46, 0x48, 0x57, 0x02, 0x00, 0xA0, 0x86, 0x01, 0x00, 0x44, 0x57, 0x02, 0x00, 0x00, 0x7C, 0x00, 0x20, 0x70, 0xB5, 0x4D, 0x00, 0x20, 0x2D, 0x4F, 0xF0, 0x00, 0x01, 0x0C, 0x46, 0x4F, 0xEA, 0x55, 0x13, 0x09, 0xD3, 0x50, 0xF8, 0x21, 0x60, 0xB6, 0xF1, 0xFF, 0x3F, 0x04, 0xD1, 0x49, 0x1C, 0xC9, 0xB2, 0x20, 0x3D, 0x5B, 0x1E, 0xF5, 0xD1, 0x4D, 0xB1, 0x20, 0x2D, 0x09, 0xD2, 0x01, 0x23, 0xAB, 0x40, 0x5B, 0x1E, 0x50, 0xF8, 0x21, 0x00, 0x18, 0x40, 0x83, 0x42, 0x01, 0xD1, 0x4F, 0xF0, 0x01, 0x04, 0x07, 0x48, 0x03, 0x78, 0x2C, 0xB9, 0x01, 0x21, 0x91, 0x40, 0xC9, 0x43, 0xC9, 0xB2, 0x0B, 0x40, 0x03, 0xE0, 0x01, 0x21, 0x91, 0x40, 0xC9, 0xB2, 0x0B, 0x43, 0x03, 0x70, 0x70, 0xBD, 0xC0, 0x46, 0xE6, 0x58, 0x02, 0x00, 0x1C, 0xB5 ,
0x0F, 0x4C, 0x14, 0x5B, 0x02, 0x00, 0xC8, 0x00, 0x00, 0x00, 0x22, 0x68, 0x01, 0x2A, 0x15, 0xD1, 0x0C, 0x48, 0x00, 0x68, 0x07, 0x28, 0x14, 0xBF, 0x40, 0x20, 0x4F, 0xF4, 0x05, 0x70, 0x63, 0x21, 0xF1, 0xF7, 0x85, 0xFA, 0x20, 0x68, 0x00, 0x90, 0x43, 0xF2, 0x33, 0x33, 0x06, 0x21, 0x40, 0x20, 0x01, 0x90, 0x03, 0x20, 0x02, 0x46, 0xF2, 0xF7, 0x6A, 0xFD, 0x1C, 0xBD, 0xF1, 0xF7, 0x77, 0xFA, 0x1C, 0xBD, 0x44, 0x57, 0x02, 0x00, 0x48, 0x57, 0x02, 0x00, 0x1E, 0xB5, 0x04, 0x46, 0x0A, 0x49, 0x00, 0x20, 0x0A, 0x4A, 0x08, 0x60, 0x0A, 0x49, 0x00, 0x90, 0x01, 0x90, 0x11, 0x60, 0x09, 0x4A, 0x45, 0xF2, 0x55, 0x53, 0x02, 0x91, 0x10, 0x60, 0x07, 0x21, 0x04, 0x22, 0x03, 0x20, 0xF2, 0xF7, 0x4E, 0xFD, 0x20, 0x46, 0xE7, 0xF7, 0xCF, 0xFD, 0x1E, 0xBD, 0x48, 0x57, 0x02, 0x00, 0x04, 0x74, 0x00, 0x20, 0x34, 0x12, 0x66, 0x09, 0x44, 0x57, 0x02, 0x00, 0x30, 0xB5, 0x0D, 0x46, 0x84, 0x69, 0xDC, 0xF7, 0x47, 0xFC, 0x24, 0x1D ,
0x01, 0x28, 0x04, 0xEB, 0x45, 0x01, 0x06, 0xD1, 0x89, 0x8E, 0x04, 0xEB, 0x85, 0x04, 0x11, 0xB9, 0x21, 0x69, 0x01, 0xB9, 0x00, 0x20, 0x30, 0xBD, 0x30, 0xB5, 0x05, 0x46, 0x5A, 0xB1, 0x03, 0xEB, 0x41, 0x03, 0x1C, 0x78, 0x59, 0x78, 0x41, 0xEA, 0x04, 0x11, 0x52, 0x1E, 0x03, 0xF1, 0x02, 0x03, 0x05, 0xF8, 0x01, 0x1B, 0xF5, 0xD1, 0x30, 0xBD, 0x00, 0xB5, 0x00, 0x20, 0xDC, 0x5B, 0x02, 0x00, 0x30, 0x00, 0x00, 0x00, 0xE7, 0xF7, 0x86, 0xFF, 0x03, 0x48, 0x01, 0x68, 0x03, 0x29, 0x04, 0xBF, 0x04, 0x21, 0x01, 0x60, 0x00, 0xBD, 0xC0, 0x46, 0x44, 0x57, 0x02, 0x00, 0x03, 0x4A, 0x12, 0x68, 0x01, 0x2A, 0x04, 0xBF, 0x02, 0x48, 0x63, 0x21, 0xF1, 0xF7, 0x26, 0xBA, 0x48, 0x57, 0x02, 0x00, 0x02, 0x20, 0x01, 0x00 };


unsigned short fw_length = 4616;

unsigned char ucStatus_FW, return_status = 0xFF;

// array to store RM parameters from EEPROM
unsigned char cRMParamsFromEeprom[128];

// array to store MAC address from EEPROM
unsigned char cMacFromEeprom[MAC_ADDR_LEN];

unsigned char 	is_allocated[NVMEM_RM_FILEID + 1];
unsigned char 	is_valid[NVMEM_RM_FILEID + 1];
unsigned char 	write_protected[NVMEM_RM_FILEID + 1];
unsigned short 	file_address[NVMEM_RM_FILEID + 1];
unsigned short 	file_length[NVMEM_RM_FILEID + 1];

//
// Smart Config Prefix
//
char aucCC3000_prefix[] = {'T', 'T', 'T'};

// 2 dim array to store address and length of new FAT
unsigned short aFATEntries[2][NVMEM_RM_FILEID + 1] = 
/*  address 	*/  {{0x50, 	0x1f0, 	0x390, 	0x1390, 	0x2390, 	0x4390, 	0x6390, 	0x63a0, 	0x63b0, 	0x63f0, 	0x6430, 	0x6830},
/*  length	*/	{0x1a0, 	0x1a0, 	0x1000, 	0x1000, 	0x2000, 	0x2000, 	0x10, 	0x10, 	0x40, 	0x40, 	0x400, 	0x200}};
	 /* 0. NVS */
	 /* 1. NVS Shadow */
	 /* 2. Wireless Conf */
	 /* 3. Wireless Conf Shadow */
	 /* 4. BT (WLAN driver) Patches */
	 /* 5. WiLink (Firmware) Patches */
	 /* 6. MAC addr */
	 /* 7. Frontend Vars */
	 /* 8. IP config */
	 /* 9. IP config Shadow */
	 /* 10. Bootloader Patches */
	 /* 11. Radio Module params */
	 /* 12. AES128 for smart config */
	 /* 13. user file */
	 /* 14. user file */
	 /* 15. user file */

//*****************************************************************************
//
//! sendDriverPatch
//!
//! \param  pointer to the length
//!
//! \return none
//!
//! \brief  The function returns a pointer to the driver patch: since there is no patch yet - 
//!				it returns 0
//
//*****************************************************************************
char *sendDriverPatch(unsigned long *Length)
{
	*Length = 0;
	return NULL;
}


//*****************************************************************************
//
//! sendBootLoaderPatch
//!
//! \param  pointer to the length
//!
//! \return none
//!
//! \brief  The function returns a pointer to the boot loader patch: since there is no patch yet -
//!				it returns 0
//
//*****************************************************************************
char *sendBootLoaderPatch(unsigned long *Length)
{
	*Length = 0;
	return NULL;
}

//*****************************************************************************
//
//! sendWLFWPatch
//!
//! \param  pointer to the length
//!
//! \return none
//!
//! \brief  The function returns a pointer to the FW patch: since there is no patch yet - it returns 0
//
//*****************************************************************************

char *sendWLFWPatch(unsigned long *Length)
{
	*Length = 0;
	return NULL;
}

//*****************************************************************************
//
//! CC3000_UsynchCallback
//!
//! \param  Event type
//!
//! \return none
//!
//! \brief  The function handles asynchronous events that come from CC3000 device 
//!		  and operates a LED4 to have an on-board indication
//
//*****************************************************************************

void CC3000_UsynchCallback(long lEventType, char * data, unsigned char length)
{
	
}

//*****************************************************************************
//
//! initDriver
//!
//!  \param[in] cRequestPatch 0 to load with EEPROM patches and 1 to load with no patches
//!
//!  \return none
//!
//!  \brief  The function initializes a CC3000 device and triggers it to start operation
//
//*****************************************************************************
int
initDriver(unsigned short cRequestPatch)
{
	

    //
	// Init GPIO's
	//
	pio_init();
	//
    //init all layers
    //
    init_spi();


	//
	// WLAN On API Implementation
	//
	wlan_init( CC3000_UsynchCallback, sendWLFWPatch, sendDriverPatch, sendBootLoaderPatch, ReadWlanInterruptPin, WlanInterruptEnable, WlanInterruptDisable, WriteWlanPin);
	//
	// Trigger a WLAN device
	//
	wlan_start(cRequestPatch);
  wlan_smart_config_set_prefix(aucCC3000_prefix);
	
	// Turn on the LED 5 to indicate that we are active and initiated WLAN successfully
     turnLedOn(5);

	//
	// Mask out all non-required events from CC3000
	//
	wlan_set_event_mask(HCI_EVNT_WLAN_KEEPALIVE|HCI_EVNT_WLAN_UNSOL_INIT|HCI_EVNT_WLAN_ASYNC_PING_REPORT);

        unsolicicted_events_timer_init();
    return(0);
}


//*****************************************************************************
//
//! fat_read_content
//!
//! \param[out] is_allocated  array of is_allocated in FAT table:\n
//!						 an allocated entry implies the address and length of the file are valid.
//!  		    			 0: not allocated; 1: allocated.
//! \param[out] is_valid  array of is_valid in FAT table:\n
//!						 a valid entry implies the content of the file is relevant.
//!  		    			 0: not valid; 1: valid.
//! \param[out] write_protected  array of write_protected in FAT table:\n
//!						 a write protected entry implies it is not possible to write into this entry.
//!  		    			 0: not protected; 1: protected.
//! \param[out] file_address  array of file address in FAT table:\n
//!						 this is the absolute address of the file in the EEPROM.
//! \param[out] file_length  array of file length in FAT table:\n
//!						 this is the upper limit of the file size in the EEPROM.
//!
//! \return on succes 0, error otherwise
//!
//! \brief  parse the FAT table from eeprom 
//
//*****************************************************************************
unsigned char fat_read_content(unsigned char *is_allocated,
                               unsigned char *is_valid,
                               unsigned char *write_protected,
                               unsigned short *file_address,
                               unsigned short *file_length)
{
        unsigned short  index;
        unsigned char   ucStatus;
        unsigned char   fatTable[48];
        unsigned char*  fatTablePtr = fatTable;
        
        // read in 6 parts to work with tiny driver
        for (index = 0; index < 6; index++)
        {
          ucStatus = nvmem_read(16, 8, 4 + 8*index, fatTablePtr); 
          fatTablePtr += 8;
        }
        
        fatTablePtr = fatTable;
        
        for (index = 0; index <= NVMEM_RM_FILEID; index++)
        {
            *is_allocated++ = (*fatTablePtr) & BIT0;
            *is_valid++ = ((*fatTablePtr) & BIT1) >> 1;
            *write_protected++ = ((*fatTablePtr) & BIT2) >> 2;
            *file_address++ = (*(fatTablePtr+1)<<8) | (*fatTablePtr) & (BIT4|BIT5|BIT6|BIT7);
            *file_length++ = (*(fatTablePtr+3)<<8) | (*(fatTablePtr+2)) & (BIT4|BIT5|BIT6|BIT7);
            
            fatTablePtr += 4;  // move to next file ID
        }
        
        return ucStatus;
}

//*****************************************************************************
//
//! fat_write_content
//!
//! \param[in] file_address  array of file address in FAT table:\n
//!						 this is the absolute address of the file in the EEPROM.
//! \param[in] file_length  array of file length in FAT table:\n
//!						 this is the upper limit of the file size in the EEPROM.
//!
//! \return on succes 0, error otherwise
//!
//! \brief  parse the FAT table from eeprom 
//
//*****************************************************************************
unsigned char fat_write_content(
                               unsigned short const *file_address,
                               unsigned short const *file_length)
{
        unsigned short  index = 0;
        unsigned char   ucStatus;
        unsigned char   fatTable[48];
        unsigned char*  fatTablePtr = fatTable;

	// first, write the magic number
        ucStatus = nvmem_write(16, 2, 0, "LS"); 
        
        for (; index <= NVMEM_RM_FILEID; index++)
        {
		// write address low char and mark as allocated
		*fatTablePtr++ = (unsigned char)(file_address[index] & 0xff) | BIT0;
		
		// write address high char
		*fatTablePtr++ = (unsigned char)((file_address[index]>>8) & 0xff);

		// write length low char
		*fatTablePtr++ = (unsigned char)(file_length[index] & 0xff);
		
		// write length high char
		*fatTablePtr++ = (unsigned char)((file_length[index]>>8) & 0xff);		
        }

	// second, write the FAT
        // write in two parts to work with tiny driver
        ucStatus = nvmem_write(16, 24, 4, fatTable); 
        ucStatus = nvmem_write(16, 24, 24+4, &fatTable[24]); 

	// third, we want to erase any user files
	memset(fatTable, 0, sizeof(fatTable));
	 ucStatus = nvmem_write(16, 16, 52, fatTable); 
        
        return ucStatus;
}




//*****************************************************************************
//
//! main
//!
//!  \param  None
//!
//!  \return none
//!
//!  \brief  The main loop is executed here
//
//*****************************************************************************

void main(void)
{
	WDTCTL = WDTPW + WDTHOLD;
        
		
	// init board and request to load with no patches.
	// this is in order to overwrite restrictions to write to specific places in EEPROM
	initDriver(0);

	ucStatus_FW = 1;
	
	
	while (ucStatus_FW)
	{
			//writing FW patch to EAPRROM  - PROTABLE CODE
    	//Note that the array itself is changing between the different Service Packs   
    	ucStatus_FW = nvmem_write_patch(NVMEM_WLAN_FW_SP_FILEID, fw_length, fw_patch);
	}
    
  	turnLedOn(6);

	// init board and request to load with patches.
	initDriver(0);
        
  	turnLedOn(8);


}
