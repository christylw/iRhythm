/*
//
//CONFIDENTIAL AND PROPRIETARY INFORMATION
//
//Copyright (c) 2015 Synopsys, Inc. All rights reserved.
//This software and documentation contain confidential and
//proprietary information that is the property of
//Synopsys, Inc. The software and documentation are
//furnished under a license agreement and may be used
//or copied only in accordance with the terms of the license
//agreement. No part of the software and documentation
//may be reproduced, transmitted, or translated, in any
//form or by any means, electronic, mechanical, manual,
//optical, or otherwise, without prior written permission
//of Synopsys, Inc., or as expressly provided by the license agreement.
//Reverse engineering is prohibited, and reproduction,
//disclosure or use without specific written authorization
//of Synopsys Inc. is strictly forbidden.
//
//
*/

#include "dsplib_primitives.h"

#if defined(_ARC_AS210_mac16)
#include "dsp_transform_tables.h"

// Produce instance with: MAC16x16; pIn0 - X; 

#pragma Code("dsplib_dsp_rifft_preprocess_q15_XtoY_cfX_MAC16x16")

/**
 *  Inverse Real FFT preprocessing function for q15 data type
 *
 * \param inst - pointer to instance structure
 * \param offs_input[in] - XY memory offset to input vector
 * \param offs_output[out] - XY memory offset to output vector
 */

void dsp_rifft_preprocess_q15_XtoY_cfX_MAC16x16(const rfft_q15_xy_t *inst, uint32_t offs_input , uint32_t offs_output) 
{
    uint32_t i;
    uint32_t length = inst->length;
  

    _set_MACMODE((1UL<<15)+(1UL<<3)+(1UL<<2));
    _set_ax2(inst->xyptr_twd*2);
    _set_mx20(AM_16BIT|1);

    _set_ax0(offs_input + 0);
    _set_mx00(AM_16BIT|1);
    _set_ax1(offs_input + length + 1);
    _set_mx10(AM_16BIT|AM_OFFS(-1));
    _set_ay0(offs_output + 0);
    _set_my00(AM_16BIT|1);


    for (i = 0; i<length / 2; i++)
    {
        q31_t xCos = ((q31_t)_x2_u0())>>1;
        q31_t xSin = ((q31_t)_x2_u0())>>1;
   
        int32_t Are = dspfx_sub_q31(0x3fffffff , xSin);
        int32_t Aim = xCos; 
        int32_t Bre = dspfx_add_q31(0x3fffffff , xSin);
        int32_t Bim = dspfx_neg_q31(xCos);

        int32_t input2i   = _x0_u0();
        int32_t input2i1  = _x0_u0();
        int32_t inputm2i1 = _x1_u0();
        int32_t inputm2i  = _x1_u0();

                    _mulrdw(input2i, Are);
                    _macrdw(input2i1, dspfx_neg_q31(Aim));
                    _macrdw(inputm2i, Bre);
        _set_y0_u0(_macrdw(inputm2i1, Bim));

                        _mulrdw(input2i1, Are);
                        _macrdw(input2i, Aim);
                        _macrdw(inputm2i, Bim);
        _set_y0_u0((_macrdw(inputm2i1, dspfx_neg_q31(Bre))));
    }

 
};
// Produce instance with: MAC16x16; pIn0 - Y; 

#pragma Code("dsplib_dsp_rifft_preprocess_q15_YtoX_cfY_MAC16x16")

/**
 *  Inverse Real FFT preprocessing function for q15 data type
 *
 * \param inst - pointer to instance structure
 * \param offs_input[in] - XY memory offset to input vector
 * \param offs_output[out] - XY memory offset to output vector
 */

void dsp_rifft_preprocess_q15_YtoX_cfY_MAC16x16(const rfft_q15_xy_t *inst, uint32_t offs_input , uint32_t offs_output) 
{
    uint32_t i;
    uint32_t length = inst->length;
  

    _set_MACMODE((1UL<<15)+(1UL<<3)+(1UL<<2));
    _set_ay2(inst->xyptr_twd*2);
    _set_my20(AM_16BIT|1);

    _set_ay0(offs_input + 0);
    _set_my00(AM_16BIT|1);
    _set_ay1(offs_input + length + 1);
    _set_my10(AM_16BIT|AM_OFFS(-1));
    _set_ax0(offs_output + 0);
    _set_mx00(AM_16BIT|1);


    for (i = 0; i<length / 2; i++)
    {
        q31_t xCos = ((q31_t)_y2_u0())>>1;
        q31_t xSin = ((q31_t)_y2_u0())>>1;
   
        int32_t Are = dspfx_sub_q31(0x3fffffff , xSin);
        int32_t Aim = xCos; 
        int32_t Bre = dspfx_add_q31(0x3fffffff , xSin);
        int32_t Bim = dspfx_neg_q31(xCos);

        int32_t input2i   = _y0_u0();
        int32_t input2i1  = _y0_u0();
        int32_t inputm2i1 = _y1_u0();
        int32_t inputm2i  = _y1_u0();

                    _mulrdw(input2i, Are);
                    _macrdw(input2i1, dspfx_neg_q31(Aim));
                    _macrdw(inputm2i, Bre);
        _set_x0_u0(_macrdw(inputm2i1, Bim));

                        _mulrdw(input2i1, Are);
                        _macrdw(input2i, Aim);
                        _macrdw(inputm2i, Bim);
        _set_x0_u0((_macrdw(inputm2i1, dspfx_neg_q31(Bre))));
    }

 
};


// Produce instance with: MAC16x16; pIn0 - X; 

#pragma Code("dsplib_dsp_rifft_preprocess_init_q15_cfX")

/**
 *  Inverse Real FFT preprocessing initialization function for q31 data type
 *
 * \param inst - pointer to instance structure
 * \param length - transform size
 * \param offs_twd - XY memory offset to cosine table
 */

dsp_status dsp_rifft_preprocess_init_q15_cfX(rfft_q15_xy_t *inst, uint32_t length, uint32_t offs_twd) 
{
    uint32_t order = 30-dspfx_norm_q31(length);

    inst->length = length;

    inst->xyptr_twd = offs_twd;

    _BurstSMemToXMem(
        inst->xyptr_twd,
        (int32_t *)(void*)dsp_cifft_q15_twd_tab[order - DSP_MIN_TRANSFORM_ORDER],
        length/2);

    return DSP_ERR_OK;
};
// Produce instance with: MAC16x16; pIn0 - Y; 

#pragma Code("dsplib_dsp_rifft_preprocess_init_q15_cfY")

/**
 *  Inverse Real FFT preprocessing initialization function for q31 data type
 *
 * \param inst - pointer to instance structure
 * \param length - transform size
 * \param offs_twd - XY memory offset to cosine table
 */

dsp_status dsp_rifft_preprocess_init_q15_cfY(rfft_q15_xy_t *inst, uint32_t length, uint32_t offs_twd) 
{
    uint32_t order = 30-dspfx_norm_q31(length);

    inst->length = length;

    inst->xyptr_twd = offs_twd;

    _BurstSMemToYMem(
        inst->xyptr_twd,
        (int32_t *)(void*)dsp_cifft_q15_twd_tab[order - DSP_MIN_TRANSFORM_ORDER],
        length/2);

    return DSP_ERR_OK;
};

#endif
