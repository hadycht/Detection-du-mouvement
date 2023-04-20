/* --------------------------------- */
/* --- morpho_dilatation_swp32.h --- */
/* --------------------------------- */

/*
* Copyright (c) 2020 - 2021, Lionel Lacassagne, All rights reserved
* Sorbonne University, LIP6, CNRS
*/

#ifndef __MORPHO_DILATATION_SWP32_H__
#define __MORPHO_DILATATION_SWP32_H__

#ifdef __cplusplus
#ifdef PRAGMA_VERBOSE
#pragma message ("C++")
#endif
extern "C" {
#endif

// line functions
void line_dilatation3_ui32matrix_swp32_basic               (uint32 **X, int i, int j0, int j1, uint32 **Y);
void line_dilatation3_ui32matrix_swp32_reg                 (uint32 **X, int i, int j0, int j1, uint32 **Y);
void line_dilatation3_ui32matrix_swp32_rot                 (uint32 **X, int i, int j0, int j1, uint32 **Y);
void line_dilatation3_ui32matrix_swp32_red                 (uint32 **X, int i, int j0, int j1, uint32 **Y);
void line_dilatation3_ui32matrix_swp32_ilu3                (uint32 **X, int i, int j0, int j1, uint32 **Y);
void line_dilatation3_ui32matrix_swp32_ilu3_red            (uint32 **X, int i, int j0, int j1, uint32 **Y);
void line_dilatation3_ui32matrix_swp32_elu2_red            (uint32 **X, int i, int j0, int j1, uint32 **Y);
void line_dilatation3_ui32matrix_swp32_elu2_red_factor     (uint32 **X, int i, int j0, int j1, uint32 **Y);
void line_dilatation3_ui32matrix_swp32_ilu3_elu2_red_factor(uint32 **X, int i, int j0, int j1, uint32 **Y);
    
// full-matrix functions
void dilatation3_ui32matrix_swp32_basic               (uint32 **X, int i0, int i1, int j0, int j1, uint32 **Y);
void dilatation3_ui32matrix_swp32_reg                 (uint32 **X, int i0, int i1, int j0, int j1, uint32 **Y);
void dilatation3_ui32matrix_swp32_rot                 (uint32 **X, int i0, int i1, int j0, int j1, uint32 **Y);
void dilatation3_ui32matrix_swp32_red                 (uint32 **X, int i0, int i1, int j0, int j1, uint32 **Y);
void dilatation3_ui32matrix_swp32_ilu3                (uint32 **X, int i0, int i1, int j0, int j1, uint32 **Y);
void dilatation3_ui32matrix_swp32_ilu3_red            (uint32 **X, int i0, int i1, int j0, int j1, uint32 **Y);
void dilatation3_ui32matrix_swp32_elu2_red            (uint32 **X, int i0, int i1, int j0, int j1, uint32 **Y);
void dilatation3_ui32matrix_swp32_elu2_red_factor     (uint32 **X, int i0, int i1, int j0, int j1, uint32 **Y);
void dilatation3_ui32matrix_swp32_ilu3_elu2_red_factor(uint32 **X, int i0, int i1, int j0, int j1, uint32 **Y);
    
#ifdef __cplusplus
}
#endif

#endif // __MORPHO_DILATATION_SWP32_H__
