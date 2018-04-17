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
#include "dsplib_ll_api.h"
#include "dsp_transform_tables.h"

// Produce instance with: MAC16x16; pIn - X; 

#pragma Code("dsplib_dsp_dct2_postprocess_init_q15_cfX")

/**
 *  DCT Type II postprocessing initialization function for q15 data type
 *
 * \param inst - pointer to instance structure
 * \param length - transform size
 * \param offs_twd - XY memory offset to twiddle table. length: 3/4*length q31_data
 */

dsp_status dsp_dct2_postprocess_init_q15_cfX(dct2_q15_xy_t *inst, uint32_t length, uint32_t offs_twd) 
{
    uint32_t order = 30-dspfx_norm_q31(length);
    inst->length = length;

    inst->xyptr_twd  = offs_twd;
    inst->xyptr_twd4 = offs_twd + (length/4);

    _BurstSMemToXMem(
        inst->xyptr_twd, 
        ((int32_t*)(void*)(dsp_cifft_q15_twd_tab[order - DSP_MIN_TRANSFORM_ORDER]))+1, 
        (length/4));
    _BurstSMemToXMem(
        inst->xyptr_twd4, 
        (int32_t*)(void*)(dsp_dct23_q15_twd_tab[order - DSP_MIN_TRANSFORM_ORDER]),
        (length/2));

    return 0;
};
// Produce instance with: MAC16x16; pIn - Y; 

#pragma Code("dsplib_dsp_dct2_postprocess_init_q15_cfY")

/**
 *  DCT Type II postprocessing initialization function for q15 data type
 *
 * \param inst - pointer to instance structure
 * \param length - transform size
 * \param offs_twd - XY memory offset to twiddle table. length: 3/4*length q31_data
 */

dsp_status dsp_dct2_postprocess_init_q15_cfY(dct2_q15_xy_t *inst, uint32_t length, uint32_t offs_twd) 
{
    uint32_t order = 30-dspfx_norm_q31(length);
    inst->length = length;

    inst->xyptr_twd  = offs_twd;
    inst->xyptr_twd4 = offs_twd + (length/4);

    _BurstSMemToYMem(
        inst->xyptr_twd, 
        ((int32_t*)(void*)(dsp_cifft_q15_twd_tab[order - DSP_MIN_TRANSFORM_ORDER]))+1, 
        (length/4));
    _BurstSMemToYMem(
        inst->xyptr_twd4, 
        (int32_t*)(void*)(dsp_dct23_q15_twd_tab[order - DSP_MIN_TRANSFORM_ORDER]),
        (length/2));

    return 0;
};



// Produce instance with: MAC16x16; pIn - X; 

#pragma Code("dsplib_dsp_dct2_preprocess_q15_XtoY")

/**
 *  DCT Type II preprocessing function for q15 data type
 *
 * \param offs_input[in] - XY memory offset to input vector
 * \param offs_output[out] - XY memory offset to output vector
 * \param length - transform size
 */

void  dsp_dct2_preprocess_q15_XtoY(uint32_t offs_input, uint32_t offs_output, uint32_t length) 
{
    int32_t N = length/2;
    int32_t i;


    _set_ax0(offs_input);
    _set_mx00(AM_16BIT|1);
    _set_ay0(offs_output);
    _set_my00(AM_16BIT|1);
    _set_ay1(offs_output + 2*N - 1);
    _set_my10(AM_16BIT|AM_OFFS(-1));

    for (i = 0; i < N >> 1; i++) 
    {
        _set_y0_u0( _x0_u0());
        _set_y1_u0( _x0_u0());
        _set_y0_u0( _x0_u0());
        _set_y1_u0( _x0_u0());
    }
}
// Produce instance with: MAC16x16; pIn - Y; 

#pragma Code("dsplib_dsp_dct2_preprocess_q15_YtoX")

/**
 *  DCT Type II preprocessing function for q15 data type
 *
 * \param offs_input[in] - XY memory offset to input vector
 * \param offs_output[out] - XY memory offset to output vector
 * \param length - transform size
 */

void  dsp_dct2_preprocess_q15_YtoX(uint32_t offs_input, uint32_t offs_output, uint32_t length) 
{
    int32_t N = length/2;
    int32_t i;


    _set_ay0(offs_input);
    _set_my00(AM_16BIT|1);
    _set_ax0(offs_output);
    _set_mx00(AM_16BIT|1);
    _set_ax1(offs_output + 2*N - 1);
    _set_mx10(AM_16BIT|AM_OFFS(-1));

    for (i = 0; i < N >> 1; i++) 
    {
        _set_x0_u0( _y0_u0());
        _set_x1_u0( _y0_u0());
        _set_x0_u0( _y0_u0());
        _set_x1_u0( _y0_u0());
    }
}

// Produce instance with: MAC16x16; pIn0 - X; 

#pragma Code("dsplib_dsp_dct2_postprocess_q15_XtoY_cfY_MAC16x16")

/**
 *  DCT Type II postprocessing function for q15 data type
 *
 * \param inst - pointer to instance structure
 * \param offs_input[in] - XY memory offset to input vector
 * \param offs_output[out] - XY memory offset to output vector
 */

void dsp_dct2_postprocess_q15_XtoY_cfY_MAC16x16(const dct2_q15_xy_t *inst, uint32_t offs_input, uint32_t offs_output) 
{
    int32_t i;
    int32_t N = inst->length /2;
    q31_t xCos, xSin;
    q31_t tmp_real, tmp_imag;
    int32_t acc;


    _set_ay0(inst->xyptr_twd*2);
    _set_my00(AM_16BIT|1);
    _set_ax0(offs_input);
    _set_mx00(AM_16BIT|1);
    _set_mx01(AM_16BIT|AM_OFFS(-1));
    _set_ax1(offs_input + 2*N-1);
    _set_mx10(AM_16BIT|AM_OFFS(-1));
    _set_mx11(AM_16BIT|1);

    _set_MACMODE((1UL<<15)+(1UL<<3)+(1UL<<2));

    tmp_real = _x0_u0();
    tmp_imag = _x0_u0();

    for (i = 1; i < N>>1; i++) {
        int32_t a1, a2, a3, a4, a5, a6;
        q31_t in2i,in2i1,in2Ni, in2Ni1;

        xCos = _y0_u0();
        xSin = _y0_u0();

        in2i   = _x0_u0();
        in2i1  = _x0_nu();
        in2Ni1 = _x1_u0();
        in2Ni  = _x1_nu();

        a1 = dspfx_add_q31(in2i,  in2Ni)>>1;
        a2 = dspfx_add_q31(in2i1, in2Ni1);
        a3 = dspfx_sub_q31(in2Ni, in2i);
        a4 = dspfx_sub_q31(in2i1, in2Ni1)>>1;

             _mulrdw(a2,xCos);
        a5 = _macrdw(a3,xSin)>>1;

             _mulrdw(a2,xSin);
        a6 = _macrdw(a3,dspfx_neg_q31(xCos))>>1;

        _set_x0_u1( dspfx_sub_q31(a6, a4));
        _set_x0_u0( dspfx_add_q31(a1, a5));
        _set_x1_u1( dspfx_sub_q31(a1, a5));
        _set_x1_u0( dspfx_add_q31(a6, a4));
        
        _add(_x0_u0(), _x1_u0());
    }

    _set_ax1(offs_input + 2);
    _set_mx10(AM_16BIT|1);
    _set_ay2(offs_output);
    _set_my20(AM_16BIT|1);
    _set_ay3(offs_output + 2*N -1);
    _set_my30(AM_16BIT|AM_OFFS(-1));
    _set_ay1(inst->xyptr_twd4*2);
    _set_my10(AM_16BIT|1);

    _set_y2_u0( dspfx_add_q31(tmp_real, tmp_imag));
    for (i = 1; i < N; i++) 
    {
        q31_t in2i = _x1_u0();
        q31_t in2i1= _x1_u0();
        xCos = _y1_u0();
        xSin = _y1_u0();
                            _mulrdw(in2i, xCos);
        _set_y2_u0(  _macrdw(in2i1, dspfx_neg_q31(xSin)));

                        _mulrdw(in2i1,  xCos);
        _set_y3_u0( _macrdw(in2i,  xSin));
    }
    xCos = _y1_u0();
    acc = dspfx_sub_q31(tmp_real, tmp_imag);
    _set_y2_u0( _mulrdw(acc, xCos));
}
// Produce instance with: MAC16x16; pIn0 - Y; 

#pragma Code("dsplib_dsp_dct2_postprocess_q15_YtoX_cfX_MAC16x16")

/**
 *  DCT Type II postprocessing function for q15 data type
 *
 * \param inst - pointer to instance structure
 * \param offs_input[in] - XY memory offset to input vector
 * \param offs_output[out] - XY memory offset to output vector
 */

void dsp_dct2_postprocess_q15_YtoX_cfX_MAC16x16(const dct2_q15_xy_t *inst, uint32_t offs_input, uint32_t offs_output) 
{
    int32_t i;
    int32_t N = inst->length /2;
    q31_t xCos, xSin;
    q31_t tmp_real, tmp_imag;
    int32_t acc;


    _set_ax0(inst->xyptr_twd*2);
    _set_mx00(AM_16BIT|1);
    _set_ay0(offs_input);
    _set_my00(AM_16BIT|1);
    _set_my01(AM_16BIT|AM_OFFS(-1));
    _set_ay1(offs_input + 2*N-1);
    _set_my10(AM_16BIT|AM_OFFS(-1));
    _set_my11(AM_16BIT|1);

    _set_MACMODE((1UL<<15)+(1UL<<3)+(1UL<<2));

    tmp_real = _y0_u0();
    tmp_imag = _y0_u0();

    for (i = 1; i < N>>1; i++) {
        int32_t a1, a2, a3, a4, a5, a6;
        q31_t in2i,in2i1,in2Ni, in2Ni1;

        xCos = _x0_u0();
        xSin = _x0_u0();

        in2i   = _y0_u0();
        in2i1  = _y0_nu();
        in2Ni1 = _y1_u0();
        in2Ni  = _y1_nu();

        a1 = dspfx_add_q31(in2i,  in2Ni)>>1;
        a2 = dspfx_add_q31(in2i1, in2Ni1);
        a3 = dspfx_sub_q31(in2Ni, in2i);
        a4 = dspfx_sub_q31(in2i1, in2Ni1)>>1;

             _mulrdw(a2,xCos);
        a5 = _macrdw(a3,xSin)>>1;

             _mulrdw(a2,xSin);
        a6 = _macrdw(a3,dspfx_neg_q31(xCos))>>1;

        _set_y0_u1( dspfx_sub_q31(a6, a4));
        _set_y0_u0( dspfx_add_q31(a1, a5));
        _set_y1_u1( dspfx_sub_q31(a1, a5));
        _set_y1_u0( dspfx_add_q31(a6, a4));
        
        _add(_y0_u0(), _y1_u0());
    }

    _set_ay1(offs_input + 2);
    _set_my10(AM_16BIT|1);
    _set_ax2(offs_output);
    _set_mx20(AM_16BIT|1);
    _set_ax3(offs_output + 2*N -1);
    _set_mx30(AM_16BIT|AM_OFFS(-1));
    _set_ax1(inst->xyptr_twd4*2);
    _set_mx10(AM_16BIT|1);

    _set_x2_u0( dspfx_add_q31(tmp_real, tmp_imag));
    for (i = 1; i < N; i++) 
    {
        q31_t in2i = _y1_u0();
        q31_t in2i1= _y1_u0();
        xCos = _x1_u0();
        xSin = _x1_u0();
                            _mulrdw(in2i, xCos);
        _set_x2_u0(  _macrdw(in2i1, dspfx_neg_q31(xSin)));

                        _mulrdw(in2i1,  xCos);
        _set_x3_u0( _macrdw(in2i,  xSin));
    }
    xCos = _x1_u0();
    acc = dspfx_sub_q31(tmp_real, tmp_imag);
    _set_x2_u0( _mulrdw(acc, xCos));
}

#endif
