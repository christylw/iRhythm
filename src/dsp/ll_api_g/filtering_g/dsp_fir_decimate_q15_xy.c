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
// Produce instance with: MAC16x16; pOut - X; pIn - X; pCoeffs - X; 

#pragma Code("dsplib_dsp_fir_decimate_q15_XtoX_cfXstY_MAC16x16")

/**
 *  Q15 real decimation
 *
 * \param inst - pointer to instance structure
 * \param offs_input[in] - XY memory offset to input vector
 * \param offs_output[out] - XY memory offset to output vector
 * \param nSamples - number of samples to be processed on output
 */

void dsp_fir_decimate_q15_XtoX_cfXstY_MAC16x16(fir_intdec_q15_xy_t *inst, uint32_t offs_input, uint32_t offs_output, uint32_t nSamples)  
{
    int32_t j, n; // loop counters
    int32_t N; // Number of samples to process
    int32_t    T; // Filter order
    uint8_t factor; // Interpolation factor
    uint8_t tmp;
    
    _set_MACMODE((1UL<<15)+(1UL<<3)+(1UL<<2));
    
    
    T = inst->filter.numTaps;
    N = nSamples;
    factor = inst->factor;
    // Init input XY pointer
    _set_ax0(offs_input);
    _set_mx00((AM_16BIT | AM_OFFS(1)));
    // Init output XY pointer
    _set_ax1(offs_output);
    _set_mx10((AM_16BIT |AM_OFFS(1)));
    // Init XY pointer for filter coefficients (modulo T addressing)
    _set_ax2(inst->filter.xyptr_coeffs);
    _set_mx20((AM_16BIT | AM_OFFS(1)));
    _set_mx21((AM_16BIT | AM_OFFS(1 - T)));
    // Init XY pointer for filter delay line (modulo T addressing)
    _set_ay0(inst->filter.shift + inst->filter.xyptr_states);
    if (T < 2){
        _set_my00((AM_16BIT));
        _set_my01((AM_16BIT));
    }else{
        _set_my00((AM_16BIT | AM_MOD(T)|AM_OFFS(1)));
        _set_my01((AM_16BIT | AM_MOD(T)|AM_OFFS(-1)));
    }
    
    // Store input sample into delay line
    _set_y0_nu( _x0_u0());
    tmp = factor - 1;
    T--;
    for ( n = 0; n < N; n++ )
    {
        
        // Calculate convolution
        _mulrdw(0, 0);
        for (j = 0; j < T >> 1; j++){
            _macrdw(_y0_u0(), _x2_u0());
            _macrdw(_y0_u0(), _x2_u0());
        }
        if (T & 1) _macrdw(_y0_u0(), _x2_u0());
        // Move convolution result to output buffer
        _set_x1_u0( _macrdw(_y0_nu(), _x2_u1()));
        // Store input sample into delay line
        for (j = 0; j < tmp >> 1; j++){
            _set_y0_u1( _x0_u0());
            _set_y0_u1( _x0_u0());
        }
        if (tmp & 1) _set_y0_u1( _x0_u0());
        _set_y0_nu( _x0_u0());
    }
    // Save delay line offset
    inst->filter.shift = _ay0() - inst->filter.xyptr_states;
}
// Produce instance with: MAC16x16; pOut - X; pCoeffs - X; pIn - Y; 

#pragma Code("dsplib_dsp_fir_decimate_q15_YtoX_cfXstY_MAC16x16")

/**
 *  Q15 real decimation
 *
 * \param inst - pointer to instance structure
 * \param offs_input[in] - XY memory offset to input vector
 * \param offs_output[out] - XY memory offset to output vector
 * \param nSamples - number of samples to be processed on output
 */

void dsp_fir_decimate_q15_YtoX_cfXstY_MAC16x16(fir_intdec_q15_xy_t *inst, uint32_t offs_input, uint32_t offs_output, uint32_t nSamples)  
{
    int32_t j, n; // loop counters
    int32_t N; // Number of samples to process
    int32_t    T; // Filter order
    uint8_t factor; // Interpolation factor
    uint8_t tmp;
    
    _set_MACMODE((1UL<<15)+(1UL<<3)+(1UL<<2));
    
    
    T = inst->filter.numTaps;
    N = nSamples;
    factor = inst->factor;
    // Init input XY pointer
    _set_ay0(offs_input);
    _set_my00((AM_16BIT | AM_OFFS(1)));
    // Init output XY pointer
    _set_ax0(offs_output);
    _set_mx00((AM_16BIT |AM_OFFS(1)));
    // Init XY pointer for filter coefficients (modulo T addressing)
    _set_ax1(inst->filter.xyptr_coeffs);
    _set_mx10((AM_16BIT | AM_OFFS(1)));
    _set_mx11((AM_16BIT | AM_OFFS(1 - T)));
    // Init XY pointer for filter delay line (modulo T addressing)
    _set_ay1(inst->filter.shift + inst->filter.xyptr_states);
    if (T < 2){
        _set_my10((AM_16BIT));
        _set_my11((AM_16BIT));
    }else{
        _set_my10((AM_16BIT | AM_MOD(T)|AM_OFFS(1)));
        _set_my11((AM_16BIT | AM_MOD(T)|AM_OFFS(-1)));
    }
    
    // Store input sample into delay line
    _set_y1_nu( _y0_u0());
    tmp = factor - 1;
    T--;
    for ( n = 0; n < N; n++ )
    {
        
        // Calculate convolution
        _mulrdw(0, 0);
        for (j = 0; j < T >> 1; j++){
            _macrdw(_y1_u0(), _x1_u0());
            _macrdw(_y1_u0(), _x1_u0());
        }
        if (T & 1) _macrdw(_y1_u0(), _x1_u0());
        // Move convolution result to output buffer
        _set_x0_u0( _macrdw(_y1_nu(), _x1_u1()));
        // Store input sample into delay line
        for (j = 0; j < tmp >> 1; j++){
            _set_y1_u1( _y0_u0());
            _set_y1_u1( _y0_u0());
        }
        if (tmp & 1) _set_y1_u1( _y0_u0());
        _set_y1_nu( _y0_u0());
    }
    // Save delay line offset
    inst->filter.shift = _ay1() - inst->filter.xyptr_states;
}
// Produce instance with: MAC16x16; pOut - X; pCoeffs - Y; pIn - X; 

#pragma Code("dsplib_dsp_fir_decimate_q15_XtoX_cfYstX_MAC16x16")

/**
 *  Q15 real decimation
 *
 * \param inst - pointer to instance structure
 * \param offs_input[in] - XY memory offset to input vector
 * \param offs_output[out] - XY memory offset to output vector
 * \param nSamples - number of samples to be processed on output
 */

void dsp_fir_decimate_q15_XtoX_cfYstX_MAC16x16(fir_intdec_q15_xy_t *inst, uint32_t offs_input, uint32_t offs_output, uint32_t nSamples)  
{
    int32_t j, n; // loop counters
    int32_t N; // Number of samples to process
    int32_t    T; // Filter order
    uint8_t factor; // Interpolation factor
    uint8_t tmp;
    
    _set_MACMODE((1UL<<15)+(1UL<<3)+(1UL<<2));
    
    
    T = inst->filter.numTaps;
    N = nSamples;
    factor = inst->factor;
    // Init input XY pointer
    _set_ax0(offs_input);
    _set_mx00((AM_16BIT | AM_OFFS(1)));
    // Init output XY pointer
    _set_ax1(offs_output);
    _set_mx10((AM_16BIT |AM_OFFS(1)));
    // Init XY pointer for filter coefficients (modulo T addressing)
    _set_ay0(inst->filter.xyptr_coeffs);
    _set_my00((AM_16BIT | AM_OFFS(1)));
    _set_my01((AM_16BIT | AM_OFFS(1 - T)));
    // Init XY pointer for filter delay line (modulo T addressing)
    _set_ax2(inst->filter.shift + inst->filter.xyptr_states);
    if (T < 2){
        _set_mx20((AM_16BIT));
        _set_mx21((AM_16BIT));
    }else{
        _set_mx20((AM_16BIT | AM_MOD(T)|AM_OFFS(1)));
        _set_mx21((AM_16BIT | AM_MOD(T)|AM_OFFS(-1)));
    }
    
    // Store input sample into delay line
    _set_x2_nu( _x0_u0());
    tmp = factor - 1;
    T--;
    for ( n = 0; n < N; n++ )
    {
        
        // Calculate convolution
        _mulrdw(0, 0);
        for (j = 0; j < T >> 1; j++){
            _macrdw(_x2_u0(), _y0_u0());
            _macrdw(_x2_u0(), _y0_u0());
        }
        if (T & 1) _macrdw(_x2_u0(), _y0_u0());
        // Move convolution result to output buffer
        _set_x1_u0( _macrdw(_x2_nu(), _y0_u1()));
        // Store input sample into delay line
        for (j = 0; j < tmp >> 1; j++){
            _set_x2_u1( _x0_u0());
            _set_x2_u1( _x0_u0());
        }
        if (tmp & 1) _set_x2_u1( _x0_u0());
        _set_x2_nu( _x0_u0());
    }
    // Save delay line offset
    inst->filter.shift = _ax2() - inst->filter.xyptr_states;
}
// Produce instance with: MAC16x16; pOut - X; pIn - Y; pCoeffs - Y; 

#pragma Code("dsplib_dsp_fir_decimate_q15_YtoX_cfYstX_MAC16x16")

/**
 *  Q15 real decimation
 *
 * \param inst - pointer to instance structure
 * \param offs_input[in] - XY memory offset to input vector
 * \param offs_output[out] - XY memory offset to output vector
 * \param nSamples - number of samples to be processed on output
 */

void dsp_fir_decimate_q15_YtoX_cfYstX_MAC16x16(fir_intdec_q15_xy_t *inst, uint32_t offs_input, uint32_t offs_output, uint32_t nSamples)  
{
    int32_t j, n; // loop counters
    int32_t N; // Number of samples to process
    int32_t    T; // Filter order
    uint8_t factor; // Interpolation factor
    uint8_t tmp;
    
    _set_MACMODE((1UL<<15)+(1UL<<3)+(1UL<<2));
    
    
    T = inst->filter.numTaps;
    N = nSamples;
    factor = inst->factor;
    // Init input XY pointer
    _set_ay0(offs_input);
    _set_my00((AM_16BIT | AM_OFFS(1)));
    // Init output XY pointer
    _set_ax0(offs_output);
    _set_mx00((AM_16BIT |AM_OFFS(1)));
    // Init XY pointer for filter coefficients (modulo T addressing)
    _set_ay1(inst->filter.xyptr_coeffs);
    _set_my10((AM_16BIT | AM_OFFS(1)));
    _set_my11((AM_16BIT | AM_OFFS(1 - T)));
    // Init XY pointer for filter delay line (modulo T addressing)
    _set_ax1(inst->filter.shift + inst->filter.xyptr_states);
    if (T < 2){
        _set_mx10((AM_16BIT));
        _set_mx11((AM_16BIT));
    }else{
        _set_mx10((AM_16BIT | AM_MOD(T)|AM_OFFS(1)));
        _set_mx11((AM_16BIT | AM_MOD(T)|AM_OFFS(-1)));
    }
    
    // Store input sample into delay line
    _set_x1_nu( _y0_u0());
    tmp = factor - 1;
    T--;
    for ( n = 0; n < N; n++ )
    {
        
        // Calculate convolution
        _mulrdw(0, 0);
        for (j = 0; j < T >> 1; j++){
            _macrdw(_x1_u0(), _y1_u0());
            _macrdw(_x1_u0(), _y1_u0());
        }
        if (T & 1) _macrdw(_x1_u0(), _y1_u0());
        // Move convolution result to output buffer
        _set_x0_u0( _macrdw(_x1_nu(), _y1_u1()));
        // Store input sample into delay line
        for (j = 0; j < tmp >> 1; j++){
            _set_x1_u1( _y0_u0());
            _set_x1_u1( _y0_u0());
        }
        if (tmp & 1) _set_x1_u1( _y0_u0());
        _set_x1_nu( _y0_u0());
    }
    // Save delay line offset
    inst->filter.shift = _ax1() - inst->filter.xyptr_states;
}
// Produce instance with: MAC16x16; pIn - X; pCoeffs - X; pOut - Y; 

#pragma Code("dsplib_dsp_fir_decimate_q15_XtoY_cfXstY_MAC16x16")

/**
 *  Q15 real decimation
 *
 * \param inst - pointer to instance structure
 * \param offs_input[in] - XY memory offset to input vector
 * \param offs_output[out] - XY memory offset to output vector
 * \param nSamples - number of samples to be processed on output
 */

void dsp_fir_decimate_q15_XtoY_cfXstY_MAC16x16(fir_intdec_q15_xy_t *inst, uint32_t offs_input, uint32_t offs_output, uint32_t nSamples)  
{
    int32_t j, n; // loop counters
    int32_t N; // Number of samples to process
    int32_t    T; // Filter order
    uint8_t factor; // Interpolation factor
    uint8_t tmp;
    
    _set_MACMODE((1UL<<15)+(1UL<<3)+(1UL<<2));
    
    
    T = inst->filter.numTaps;
    N = nSamples;
    factor = inst->factor;
    // Init input XY pointer
    _set_ax0(offs_input);
    _set_mx00((AM_16BIT | AM_OFFS(1)));
    // Init output XY pointer
    _set_ay0(offs_output);
    _set_my00((AM_16BIT |AM_OFFS(1)));
    // Init XY pointer for filter coefficients (modulo T addressing)
    _set_ax1(inst->filter.xyptr_coeffs);
    _set_mx10((AM_16BIT | AM_OFFS(1)));
    _set_mx11((AM_16BIT | AM_OFFS(1 - T)));
    // Init XY pointer for filter delay line (modulo T addressing)
    _set_ay1(inst->filter.shift + inst->filter.xyptr_states);
    if (T < 2){
        _set_my10((AM_16BIT));
        _set_my11((AM_16BIT));
    }else{
        _set_my10((AM_16BIT | AM_MOD(T)|AM_OFFS(1)));
        _set_my11((AM_16BIT | AM_MOD(T)|AM_OFFS(-1)));
    }
    
    // Store input sample into delay line
    _set_y1_nu( _x0_u0());
    tmp = factor - 1;
    T--;
    for ( n = 0; n < N; n++ )
    {
        
        // Calculate convolution
        _mulrdw(0, 0);
        for (j = 0; j < T >> 1; j++){
            _macrdw(_y1_u0(), _x1_u0());
            _macrdw(_y1_u0(), _x1_u0());
        }
        if (T & 1) _macrdw(_y1_u0(), _x1_u0());
        // Move convolution result to output buffer
        _set_y0_u0( _macrdw(_y1_nu(), _x1_u1()));
        // Store input sample into delay line
        for (j = 0; j < tmp >> 1; j++){
            _set_y1_u1( _x0_u0());
            _set_y1_u1( _x0_u0());
        }
        if (tmp & 1) _set_y1_u1( _x0_u0());
        _set_y1_nu( _x0_u0());
    }
    // Save delay line offset
    inst->filter.shift = _ay1() - inst->filter.xyptr_states;
}
// Produce instance with: MAC16x16; pOut - Y; pCoeffs - Y; pIn - X; 

#pragma Code("dsplib_dsp_fir_decimate_q15_XtoY_cfYstX_MAC16x16")

/**
 *  Q15 real decimation
 *
 * \param inst - pointer to instance structure
 * \param offs_input[in] - XY memory offset to input vector
 * \param offs_output[out] - XY memory offset to output vector
 * \param nSamples - number of samples to be processed on output
 */

void dsp_fir_decimate_q15_XtoY_cfYstX_MAC16x16(fir_intdec_q15_xy_t *inst, uint32_t offs_input, uint32_t offs_output, uint32_t nSamples)  
{
    int32_t j, n; // loop counters
    int32_t N; // Number of samples to process
    int32_t    T; // Filter order
    uint8_t factor; // Interpolation factor
    uint8_t tmp;
    
    _set_MACMODE((1UL<<15)+(1UL<<3)+(1UL<<2));
    
    
    T = inst->filter.numTaps;
    N = nSamples;
    factor = inst->factor;
    // Init input XY pointer
    _set_ax0(offs_input);
    _set_mx00((AM_16BIT | AM_OFFS(1)));
    // Init output XY pointer
    _set_ay0(offs_output);
    _set_my00((AM_16BIT |AM_OFFS(1)));
    // Init XY pointer for filter coefficients (modulo T addressing)
    _set_ay1(inst->filter.xyptr_coeffs);
    _set_my10((AM_16BIT | AM_OFFS(1)));
    _set_my11((AM_16BIT | AM_OFFS(1 - T)));
    // Init XY pointer for filter delay line (modulo T addressing)
    _set_ax1(inst->filter.shift + inst->filter.xyptr_states);
    if (T < 2){
        _set_mx10((AM_16BIT));
        _set_mx11((AM_16BIT));
    }else{
        _set_mx10((AM_16BIT | AM_MOD(T)|AM_OFFS(1)));
        _set_mx11((AM_16BIT | AM_MOD(T)|AM_OFFS(-1)));
    }
    
    // Store input sample into delay line
    _set_x1_nu( _x0_u0());
    tmp = factor - 1;
    T--;
    for ( n = 0; n < N; n++ )
    {
        
        // Calculate convolution
        _mulrdw(0, 0);
        for (j = 0; j < T >> 1; j++){
            _macrdw(_x1_u0(), _y1_u0());
            _macrdw(_x1_u0(), _y1_u0());
        }
        if (T & 1) _macrdw(_x1_u0(), _y1_u0());
        // Move convolution result to output buffer
        _set_y0_u0( _macrdw(_x1_nu(), _y1_u1()));
        // Store input sample into delay line
        for (j = 0; j < tmp >> 1; j++){
            _set_x1_u1( _x0_u0());
            _set_x1_u1( _x0_u0());
        }
        if (tmp & 1) _set_x1_u1( _x0_u0());
        _set_x1_nu( _x0_u0());
    }
    // Save delay line offset
    inst->filter.shift = _ax1() - inst->filter.xyptr_states;
}
// Produce instance with: MAC16x16; pOut - Y; pIn - Y; pCoeffs - X; 

#pragma Code("dsplib_dsp_fir_decimate_q15_YtoY_cfXstY_MAC16x16")

/**
 *  Q15 real decimation
 *
 * \param inst - pointer to instance structure
 * \param offs_input[in] - XY memory offset to input vector
 * \param offs_output[out] - XY memory offset to output vector
 * \param nSamples - number of samples to be processed on output
 */

void dsp_fir_decimate_q15_YtoY_cfXstY_MAC16x16(fir_intdec_q15_xy_t *inst, uint32_t offs_input, uint32_t offs_output, uint32_t nSamples)  
{
    int32_t j, n; // loop counters
    int32_t N; // Number of samples to process
    int32_t    T; // Filter order
    uint8_t factor; // Interpolation factor
    uint8_t tmp;
    
    _set_MACMODE((1UL<<15)+(1UL<<3)+(1UL<<2));
    
    
    T = inst->filter.numTaps;
    N = nSamples;
    factor = inst->factor;
    // Init input XY pointer
    _set_ay0(offs_input);
    _set_my00((AM_16BIT | AM_OFFS(1)));
    // Init output XY pointer
    _set_ay1(offs_output);
    _set_my10((AM_16BIT |AM_OFFS(1)));
    // Init XY pointer for filter coefficients (modulo T addressing)
    _set_ax0(inst->filter.xyptr_coeffs);
    _set_mx00((AM_16BIT | AM_OFFS(1)));
    _set_mx01((AM_16BIT | AM_OFFS(1 - T)));
    // Init XY pointer for filter delay line (modulo T addressing)
    _set_ay2(inst->filter.shift + inst->filter.xyptr_states);
    if (T < 2){
        _set_my20((AM_16BIT));
        _set_my21((AM_16BIT));
    }else{
        _set_my20((AM_16BIT | AM_MOD(T)|AM_OFFS(1)));
        _set_my21((AM_16BIT | AM_MOD(T)|AM_OFFS(-1)));
    }
    
    // Store input sample into delay line
    _set_y2_nu( _y0_u0());
    tmp = factor - 1;
    T--;
    for ( n = 0; n < N; n++ )
    {
        
        // Calculate convolution
        _mulrdw(0, 0);
        for (j = 0; j < T >> 1; j++){
            _macrdw(_y2_u0(), _x0_u0());
            _macrdw(_y2_u0(), _x0_u0());
        }
        if (T & 1) _macrdw(_y2_u0(), _x0_u0());
        // Move convolution result to output buffer
        _set_y1_u0( _macrdw(_y2_nu(), _x0_u1()));
        // Store input sample into delay line
        for (j = 0; j < tmp >> 1; j++){
            _set_y2_u1( _y0_u0());
            _set_y2_u1( _y0_u0());
        }
        if (tmp & 1) _set_y2_u1( _y0_u0());
        _set_y2_nu( _y0_u0());
    }
    // Save delay line offset
    inst->filter.shift = _ay2() - inst->filter.xyptr_states;
}
// Produce instance with: MAC16x16; pOut - Y; pCoeffs - Y; pIn - Y; 

#pragma Code("dsplib_dsp_fir_decimate_q15_YtoY_cfYstX_MAC16x16")

/**
 *  Q15 real decimation
 *
 * \param inst - pointer to instance structure
 * \param offs_input[in] - XY memory offset to input vector
 * \param offs_output[out] - XY memory offset to output vector
 * \param nSamples - number of samples to be processed on output
 */

void dsp_fir_decimate_q15_YtoY_cfYstX_MAC16x16(fir_intdec_q15_xy_t *inst, uint32_t offs_input, uint32_t offs_output, uint32_t nSamples)  
{
    int32_t j, n; // loop counters
    int32_t N; // Number of samples to process
    int32_t    T; // Filter order
    uint8_t factor; // Interpolation factor
    uint8_t tmp;
    
    _set_MACMODE((1UL<<15)+(1UL<<3)+(1UL<<2));
    
    
    T = inst->filter.numTaps;
    N = nSamples;
    factor = inst->factor;
    // Init input XY pointer
    _set_ay0(offs_input);
    _set_my00((AM_16BIT | AM_OFFS(1)));
    // Init output XY pointer
    _set_ay1(offs_output);
    _set_my10((AM_16BIT |AM_OFFS(1)));
    // Init XY pointer for filter coefficients (modulo T addressing)
    _set_ay2(inst->filter.xyptr_coeffs);
    _set_my20((AM_16BIT | AM_OFFS(1)));
    _set_my21((AM_16BIT | AM_OFFS(1 - T)));
    // Init XY pointer for filter delay line (modulo T addressing)
    _set_ax0(inst->filter.shift + inst->filter.xyptr_states);
    if (T < 2){
        _set_mx00((AM_16BIT));
        _set_mx01((AM_16BIT));
    }else{
        _set_mx00((AM_16BIT | AM_MOD(T)|AM_OFFS(1)));
        _set_mx01((AM_16BIT | AM_MOD(T)|AM_OFFS(-1)));
    }
    
    // Store input sample into delay line
    _set_x0_nu( _y0_u0());
    tmp = factor - 1;
    T--;
    for ( n = 0; n < N; n++ )
    {
        
        // Calculate convolution
        _mulrdw(0, 0);
        for (j = 0; j < T >> 1; j++){
            _macrdw(_x0_u0(), _y2_u0());
            _macrdw(_x0_u0(), _y2_u0());
        }
        if (T & 1) _macrdw(_x0_u0(), _y2_u0());
        // Move convolution result to output buffer
        _set_y1_u0( _macrdw(_x0_nu(), _y2_u1()));
        // Store input sample into delay line
        for (j = 0; j < tmp >> 1; j++){
            _set_x0_u1( _y0_u0());
            _set_x0_u1( _y0_u0());
        }
        if (tmp & 1) _set_x0_u1( _y0_u0());
        _set_x0_nu( _y0_u0());
    }
    // Save delay line offset
    inst->filter.shift = _ax0() - inst->filter.xyptr_states;
}

// Produce instance with: MAC16x16; pCoeffs - X; 

#pragma Code("dsplib_dsp_fir_decimate_init_q15_cfXstY_MAC16x16")

/**
 *  initialization for Q15 real decimation
 *
 * \param inst - pointer to instance structure
 * \param nTaps - filter length
 * \param decFactor - decimation factor
 * \param offs_coeffs - XY memory offset to coefficients buffer
 * \param offs_states - XY memory offset to states vector buffer
 * \result dsp_status
 */

dsp_status dsp_fir_decimate_init_q15_cfXstY_MAC16x16(fir_intdec_q15_xy_t *inst, uint16_t nTaps, uint8_t decFactor,  uint32_t offs_coeffs, uint32_t offs_states)  
{
    inst->filter.numTaps = nTaps;
    inst->filter.shift = 0;
    inst->filter.xyptr_coeffs = offs_coeffs;
    inst->filter.xyptr_states = offs_states;
    inst->factor = decFactor;
    _MEM_FILL_Y_Q15(offs_states, 0, nTaps);
    return DSP_ERR_OK;
}
// Produce instance with: MAC16x16; pCoeffs - Y; 

#pragma Code("dsplib_dsp_fir_decimate_init_q15_cfYstX_MAC16x16")

/**
 *  initialization for Q15 real decimation
 *
 * \param inst - pointer to instance structure
 * \param nTaps - filter length
 * \param decFactor - decimation factor
 * \param offs_coeffs - XY memory offset to coefficients buffer
 * \param offs_states - XY memory offset to states vector buffer
 * \result dsp_status
 */

dsp_status dsp_fir_decimate_init_q15_cfYstX_MAC16x16(fir_intdec_q15_xy_t *inst, uint16_t nTaps, uint8_t decFactor,  uint32_t offs_coeffs, uint32_t offs_states)  
{
    inst->filter.numTaps = nTaps;
    inst->filter.shift = 0;
    inst->filter.xyptr_coeffs = offs_coeffs;
    inst->filter.xyptr_states = offs_states;
    inst->factor = decFactor;
    _MEM_FILL_X_Q15(offs_states, 0, nTaps);
    return DSP_ERR_OK;
}
#endif
