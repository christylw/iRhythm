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
#if defined(_ARC_AS210_nomac)
// Produce instance with: MAC32x32; pIn - X; 

#pragma Code("dsplib_dsp_max_q15_X")

/**
 *  Finds the maximum element of a Q15 vector.
 *
 * \param[in] offs_Src - XY memory offset to input vector. Vector length: nSamples
 * \param nSamples - number of samples to be processed
 * \param[out] pResult
 * \param[out] pIndex
 */

void dsp_max_q15_X(int32_t offs_Src, uint32_t nSamples, q15_t *pResult, uint32_t *pIndex) 
{
    int32_t sample, sample_max = 0;
    q31_t outval;
    _set_ax0(offs_Src);
    _set_mx00(AM_16BIT | 1);
    _nop();
    _nop();
    outval = _x0_u0();
    for (sample = 1; sample < (int32_t)nSamples; sample++)
    {
        q31_t tmp = _x0_u0();
        if(outval < tmp) {
            outval = tmp;
            sample_max = sample;
        }
    }
    *pResult = (outval >> 16);
    *pIndex = sample_max;
}
// Produce instance with: MAC32x32; pIn - Y; 

#pragma Code("dsplib_dsp_max_q15_Y")

/**
 *  Finds the maximum element of a Q15 vector.
 *
 * \param[in] offs_Src - XY memory offset to input vector. Vector length: nSamples
 * \param nSamples - number of samples to be processed
 * \param[out] pResult
 * \param[out] pIndex
 */

void dsp_max_q15_Y(int32_t offs_Src, uint32_t nSamples, q15_t *pResult, uint32_t *pIndex) 
{
    int32_t sample, sample_max = 0;
    q31_t outval;
    _set_ay0(offs_Src);
    _set_my00(AM_16BIT | 1);
    _nop();
    _nop();
    outval = _y0_u0();
    for (sample = 1; sample < (int32_t)nSamples; sample++)
    {
        q31_t tmp = _y0_u0();
        if(outval < tmp) {
            outval = tmp;
            sample_max = sample;
        }
    }
    *pResult = (outval >> 16);
    *pIndex = sample_max;
}
#endif
