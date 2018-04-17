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

#if !defined(EXCLUDE_DSP_SHIFT_Q7)

#include "dsplib_primitives.h"

#pragma Code("dsplib_dsp_shift_q7")

#if defined _ARCv2EM_XY_AGU_SMALL && defined __FXAPI__

void _DSPLIB_XYCC dsp_shift_q7 (const __xy q7_t* __restrict pSrc, int8_t  shiftBits, __xy q7_t* __restrict pDst, uint32_t nSamples)
{
    int32_t i, N  = nSamples >> 1;
    v2i16_t v2i16_shiftBits = fx_create_v2i16(shiftBits, shiftBits);
	_DSPLIB_store_regs(2, 2, 0);

    _set_agu_ap0(pSrc); // Set AP registers for pSrcA
    _set_agu_ap1(pDst); // Set AP registers for pSrcB

    _set_agu_mod0(0, AGU_DT_v2q15_cast_v2q7, AGU_AD_INC_q7(2));
    _set_agu_mod1(1, AGU_DT_v2q7, AGU_AD_INC_q7(2));

    for (i = 0; i < N; i++) {
        _st_agu_u1(_vpack2hblf(fx_q31_cast_v2q15(fx_asl_v2q15(fx_v2q15_cast_q31(_agu_u0()), v2i16_shiftBits))));
    }
    if (nSamples & 1) {
        _set_agu_mod0(0, AGU_DT_q15_cast_q7, AGU_AD_INC_q7(1));
        _set_agu_mod1(1, AGU_DT_q7, AGU_AD_INC_q7(1));
        _st_agu_u1(_vpack2hblf(fx_q31_cast_v2q15(fx_asl_v2q15(fx_v2q15_cast_q31(_agu_u0()), v2i16_shiftBits))));
    }

	_DSPLIB_restore_regs();
}

#else //defined _ARCv2EM_XY_AGU_MEDIUM && defined __FXAPI__

/**
 * Shifts the elements of a Q7 vector by a specified
 * number of bits. The algorithm used is:
 * pDst[n] = pSrc[n] << shift,   0 <= n < nSamples. 
 *
 * \param[in] pSrc - pointer to input vector. Vector length: nSamples
 * \param[out] pDst - pointer to output vector. Vector length: nSamples
 * \param shiftBits
 * \param nSamples - number of samples to be processed
 */

void dsp_shift_q7 (const q7_t* __restrict pSrc, int8_t  shiftBits, q7_t* __restrict pDst, uint32_t nSamples)
{
    int32_t sample;

#if (!defined(ASL_VEC4_Q7))

#if defined(__Xdsp) && defined(__FXAPI__) && defined(_ARC)
    if ( ((uint32_t)pDst & 0x3) 
        || ((uint32_t)pSrc & 0x3) 
        || ((uint32_t)nSamples & 0x3) )
    {
        for (sample = 0; sample < (int32_t)nSamples; sample++){
            pDst[sample] = dspfx_asl_q7(pSrc[sample], shiftBits);
        }
    }
    else
    {
        v2i16_t shiftBits_v2i16 = fx_create_v2i16((shiftBits), (shiftBits));
        for (sample = 0; sample < (int32_t)(nSamples/4); sample++) {
            int ta02;
            int ta13 = ((int *)pSrc)[sample];
            ta02 = (ta13 << 8) & 0xff00ff00;
            ta13 = ta13 & 0xff00ff00;

            ta02 = fx_q31_cast_v2q15(fx_asl_v2q15(fx_v2q15_cast_q31(ta02), shiftBits_v2i16));
            ta13 = fx_q31_cast_v2q15(fx_asl_v2q15(fx_v2q15_cast_q31(ta13), shiftBits_v2i16));

            ((int *)pDst)[sample] = (ta13 & 0xff00ff00) | ((ta02 >> 8) & 0x00ff00ff);
        }
    }
#else
    for (sample = 0; sample < (int32_t)nSamples; sample++){
        pDst[sample] = dspfx_asl_q7(pSrc[sample], shiftBits);
    }
#endif

#else
    int32_t startSample = ((uint32_t)pSrc) & 0x3;

    //input and ouput are not 'equally' (un)aligned -> use 1d cycle
    if ((startSample != ((uint32_t)pDst & 0x3 )) )
    {
        for (sample = 0; sample < nSamples; sample++)
            pDst[sample] = dspfx_asl_q7(pSrc[sample], shiftBits);
    }
    else //use vector instruction
    {
        q31_t* pDst_32;
        q31_t* pSrc_32;
        int32_t N;

        if (startSample) {
            startSample = 4 - startSample;
            for (sample=0; (sample < startSample) && (sample < nSamples); sample++)
                    pDst[sample] = dspfx_asl_q7(pSrc[sample], shiftBits);
        }

        pDst_32 = (int32_t*)(void*)(&pDst[startSample]);
        pSrc_32 = (int32_t*)(void*)(&pSrc[startSample]);

        N = ((int32_t)nSamples - startSample) >> 2;
        for (sample = 0; sample < N; sample++) 
                *pDst_32++ = ASL_VEC4_Q7(*pSrc_32++, shiftBits);

        for (sample = (N << 2) + startSample; sample<nSamples;sample++) //'tail'
                        pDst[sample] =  dspfx_asl_q7(pSrc[sample], shiftBits);
    }
#endif
}
#endif //defined _ARCv2EM_XY_AGU_MEDIUM && defined __FXAPI__

#endif //!defined(EXCLUDE_DSP_SHIFT_Q7)
