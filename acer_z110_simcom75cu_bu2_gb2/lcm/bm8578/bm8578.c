/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

#include <linux/string.h>

#include "lcm_drv.h"


// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH  (240)
#define FRAME_HEIGHT (400)
#define LCM_ID       (0x52)
// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    (lcm_util.set_reset_pin((v)))

#define UDELAY(n) (lcm_util.udelay(n))
#define MDELAY(n) (lcm_util.mdelay(n))


// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

static __inline unsigned int LOW_BYTE(unsigned int x)
{
    return (x & 0xFF);
}

static __inline unsigned int HIGH_BYTE(unsigned int x)
{
    return ((x >> 8) & 0xFF);
}

static __inline void send_ctrl_cmd(unsigned int cmd)
{
    lcm_util.send_cmd(cmd);
}

static __inline void send_data_cmd(unsigned int data)
{
    lcm_util.send_data(data);
}

static __inline unsigned int read_data_cmd(void)
{
    return lcm_util.read_data();
}

static __inline void set_lcm_register(unsigned int regIndex,
                                      unsigned int regData)
{
    send_ctrl_cmd(regIndex);
    send_data_cmd(regData);
}


static void init_lcm_registers(void)
{
    set_lcm_register(0x83, 0x02);    // TESTTM=1
    set_lcm_register(0x85, 0x03);    // VDC_SEL=011
    set_lcm_register(0x8c, 0x93);
    set_lcm_register(0x91, 0x01);    // pre:0x01
    set_lcm_register(0x83, 0x00);
    
    // Gamma Setting
    set_lcm_register(0x3E, 0xB0);
    set_lcm_register(0x3F, 0x03);
    set_lcm_register(0x40, 0x10);
    set_lcm_register(0x41, 0x56);
    set_lcm_register(0x42, 0x13);
    set_lcm_register(0x43, 0x46);
    set_lcm_register(0x44, 0x23);
    set_lcm_register(0x45, 0x76);
    set_lcm_register(0x46, 0x00);
    set_lcm_register(0x47, 0x5e);
    set_lcm_register(0x48, 0x4f);
    set_lcm_register(0x49, 0x40);
    
    // Power Supply Setting
    set_lcm_register(0x17, 0x91);    // RADJ = 1010(7.5MHz x 100%), OSC_EN = 1
    set_lcm_register(0x23, 0x01);    // TE ON, new added
    set_lcm_register(0x2B, 0xF9);    // N_DCDC = 0xF9
    MDELAY(10);
    
    set_lcm_register(0x1B, 0x16);    // BT = 0001, AP = 100
    set_lcm_register(0x1A, 0x11);    // VC3 = 001, VC1 = 001
    set_lcm_register(0x1C, 0x0D);    // VRH = 1101
    set_lcm_register(0x1F, 0x42);    // VCM = 100_0010
    MDELAY(20);
    
    set_lcm_register(0x19, 0x0A);    // GASENB=0,PON=0,DK=1,XDK=0,VLCD_TRI=1,STB=0
    set_lcm_register(0x19, 0x1A);    // GASENB=0,PON=1,DK=1,XDK=0,VLCD_TRI=1,STB=0
    MDELAY(40);
    set_lcm_register(0x19, 0x12);    // GASENB=0,PON=1,DK=0,XDK=0,VLCD_TRI=1,STB=0
    MDELAY(40);
    set_lcm_register(0x1E, 0x2C);    // VCOMG=1,VDV=0_1100
    MDELAY(100);
    set_lcm_register(0x3C, 0x60);
    set_lcm_register(0x3D, 0x40);
    set_lcm_register(0x34, 0x38);
    set_lcm_register(0x35, 0x38); 
    set_lcm_register(0x24, 0x38);
    MDELAY(40);
    set_lcm_register(0x24, 0x3C);
    set_lcm_register(0x16, 0x08);    // pre:C8
    set_lcm_register(0x01, 0x02);
    set_lcm_register(0x55, 0x00);
}


// ---------------------------------------------------------------------------
//  LCM Driver Implementations
// ---------------------------------------------------------------------------

static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
    memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}


static void lcm_get_params(LCM_PARAMS *params)
{
    memset(params, 0, sizeof(LCM_PARAMS));

    params->type   = LCM_TYPE_DBI;
    params->ctrl   = LCM_CTRL_PARALLEL_DBI;
    params->width  = FRAME_WIDTH;
    params->height = FRAME_HEIGHT;
    
    params->dbi.port                    = 0;
    params->dbi.clock_freq              = LCM_DBI_CLOCK_FREQ_52M;
    params->dbi.data_width              = LCM_DBI_DATA_WIDTH_18BITS;
    params->dbi.data_format.color_order = LCM_COLOR_ORDER_RGB;
    params->dbi.data_format.trans_seq   = LCM_DBI_TRANS_SEQ_LSB_FIRST;
    params->dbi.data_format.padding     = LCM_DBI_PADDING_ON_MSB;
    params->dbi.data_format.format      = LCM_DBI_FORMAT_RGB666;
    params->dbi.data_format.width       = LCM_DBI_DATA_WIDTH_18BITS;
    params->dbi.cpu_write_bits          = LCM_DBI_CPU_WRITE_32_BITS;
    params->dbi.io_driving_current      = 0;

    params->dbi.te_mode                 = LCM_DBI_TE_MODE_VSYNC_OR_HSYNC;
    params->dbi.te_edge_polarity        = LCM_POLARITY_RISING;
    params->dbi.te_hs_delay_cnt         = 50;
    params->dbi.te_vs_width_cnt         = 277;
    params->dbi.te_vs_width_cnt_div     = LCM_DBI_TE_VS_WIDTH_CNT_DIV_16;

    params->dbi.parallel.write_setup    = 2;
    params->dbi.parallel.write_hold     = 2;
    params->dbi.parallel.write_wait     = 4;
    params->dbi.parallel.read_setup     = 2;
    params->dbi.parallel.read_latency   = 9;
    params->dbi.parallel.wait_period    = 0;
}


static void lcm_init(void)
{
    SET_RESET_PIN(0);
    MDELAY(40);
    SET_RESET_PIN(1);
    MDELAY(250);

    init_lcm_registers();

    // Enable tearing control signal
    //
    set_lcm_register(0x23, 0x03);   // enable TEMODE = 1
}


static void lcm_suspend(void)
{
    set_lcm_register(0x24, 0x38); // PT=00,GON=1, DTE=1, D=11
    MDELAY(40);
    set_lcm_register(0x24, 0x28); // PT=00,GON=1, DTE=1, D=11
    MDELAY(40);
    set_lcm_register(0x24, 0x20); // PT=00,GON=1, DTE=1, D=11
    set_lcm_register(0x3C, 0x00); // N_SAP=0000 0000
    set_lcm_register(0x1B, 0x10); // BT=0001, AP=000
    set_lcm_register(0x19, 0x0A); // PON=0, DK=1
    set_lcm_register(0x1E, 0x00); // VCOMG=1,
    set_lcm_register(0x19, 0x01); // STB=1
    set_lcm_register(0x17, 0x00); // OSC_EN=0
}


static void lcm_resume(void)
{
    set_lcm_register(0x17, 0x91); // RADJ=1010(7.5MHz x 100%), OSC_EN=1
    set_lcm_register(0x2B, 0xF9); // N_DCDC=0xF9.
    MDELAY(10);
    set_lcm_register(0x1B, 0x14); // BT=0001, AP=100
    set_lcm_register(0x1A, 0x11); // VC3=001, VC1=001
    set_lcm_register(0x1C, 0x0D); // VRH=1101
    set_lcm_register(0x1F, 0x42); // VCM=100_0010
    MDELAY(20);
    set_lcm_register(0x19, 0x0A); // GASENB=0, PON=0, DK=1, XDK=0,VLCD_TRI=1, STB=0
    set_lcm_register(0x19, 0x1A); // GASENB=0, PON=1, DK=1, XDK=0,VLCD_TRI=1, STB=0
    MDELAY(40);
    set_lcm_register(0x19, 0x12); // GASENB=0, PON=1, DK=0, XDK=0,VLCD_TRI=1, STB=0
    MDELAY(40);
    set_lcm_register(0x1E, 0x2C); // VCOMG=1, VDV=0_1100
    MDELAY(100);

    // Display ON Setting
    set_lcm_register(0x3C, 0x60); // N_SAP=0110 0000
    set_lcm_register(0x3D, 0x40); // I_SAP =0100 0000
    set_lcm_register(0x34, 0x38); // EQS=0011 1000
    set_lcm_register(0x35, 0x38); // EQP=0011 1000
    set_lcm_register(0x24, 0x38); // PT=00,GON=1, DTE=1, D=10
    MDELAY(40);
    set_lcm_register(0x24, 0x3C); // PT=00,GON=1, DTE=1, D=11
    set_lcm_register(0x16, 0x08); // MY=1,MX=1,MV=0,GS=0,BGR=1,SS=0,SRL_EN=0,SM=0
    set_lcm_register(0x01, 0x02); // IDMON=0,INVON=0,NORNO=1,PTLON=0
    set_lcm_register(0x55, 0x00); // SM_PANEL=0,SS_PANEL=0,GS_PANEL=0,REV_PANEL=0,BGR_PANEL=0

    // Enable tearing control signal
    //
    set_lcm_register(0x23, 0x03);   // enable TEMODE = 1
}


static void lcm_update(unsigned int x, unsigned int y,
                       unsigned int width, unsigned int height)
{
    unsigned int x0 = x;
    unsigned int y0 = y;
    unsigned int x1 = x0 + width - 1;
    unsigned int y1 = y0 + height - 1;

	set_lcm_register(0x02, HIGH_BYTE(x0));
	set_lcm_register(0x03, LOW_BYTE(x0));
	set_lcm_register(0x04, HIGH_BYTE(x1));
	set_lcm_register(0x05, LOW_BYTE(x1));
	set_lcm_register(0x06, HIGH_BYTE(y0));
	set_lcm_register(0x07, LOW_BYTE(y0));
	set_lcm_register(0x08, HIGH_BYTE(y1));
	set_lcm_register(0x09, LOW_BYTE(y1));

    send_ctrl_cmd(0x22);
}

static unsigned int lcm_compare_id(void)
{
	send_ctrl_cmd(0x00);
    return (LCM_ID == read_data_cmd())?1:0;
}


// ---------------------------------------------------------------------------
//  Get LCM Driver Hooks
// ---------------------------------------------------------------------------

LCM_DRIVER bm8578_lcm_drv = 
{
    .name			= "bm8578",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
	.update         = lcm_update,
	.compare_id     = lcm_compare_id
};
