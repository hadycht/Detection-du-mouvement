/* --------------------------------- */
/* --- morpho_dilatation_swp64.h --- */
/* --------------------------------- */

/*
* Copyright (c) 2020 - 2021, Lionel Lacassagne, All rights reserved
* Sorbonne University, LIP6, CNRS
*/

#ifndef __MORPHO_DILATATION_SWP64_H__
#define __MORPHO_DILATATION_SWP64_H__

#ifdef __cplusplus
#ifdef PRAGMA_VERBOSE
#pragma message ("C++")
#endif
extern "C" {
#endif

// line functions
void line_dilatation3_ui64matrix_swp64_basic               (uint64 **X, int i, int j0, int j1, uint64 **Y);
void line_dilatation3_ui64matrix_swp64_reg                 (uint64 **X, int i, int j0, int j1, uint64 **Y);
void line_dilatation3_ui64matrix_swp64_rot                 (uint64 **X, int i, int j0, int j1, uint64 **Y);
void line_dilatation3_ui64matrix_swp64_red                 (uint64 **X, int i, int j0, int j1, uint64 **Y);
void line_dilatation3_ui64matrix_swp64_ilu3                (uint64 **X, int i, int j0, int j1, uint64 **Y);
void line_dilatation3_ui64matrix_swp64_ilu3_red            (uint64 **X, int i, int j0, int j1, uint64 **Y);
void line_dilatation3_ui64matrix_swp64_elu2_red            (uint64 **X, int i, int j0, int j1, uint64 **Y);
void line_dilatation3_ui64matrix_swp64_elu2_red_factor     (uint64 **X, int i, int j0, int j1, uint64 **Y);
void line_dilatation3_ui64matrix_swp64_ilu3_elu2_red       (uint64 **X, int i, int j0, int j1, uint64 **Y);
void line_dilatation3_ui64matrix_swp64_ilu3_elu2_red_factor(uint64 **X, int i, int j0, int j1, uint64 **Y);
    
// full-matrix functions
void dilatation3_ui64matrix_swp64_basic               (uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y);
void dilatation3_ui64matrix_swp64_reg                 (uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y);
void dilatation3_ui64matrix_swp64_rot                 (uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y);
void dilatation3_ui64matrix_swp64_red                 (uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y);
void dilatation3_ui64matrix_swp64_ilu3                (uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y);
void dilatation3_ui64matrix_swp64_ilu3_red            (uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y);
void dilatation3_ui64matrix_swp64_elu2_red            (uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y);
void dilatation3_ui64matrix_swp64_elu2_red_factor     (uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y);
void dilatation3_ui64matrix_swp64_ilu3_elu2_red       (uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y);
void dilatation3_ui64matrix_swp64_ilu3_elu2_red_factor(uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y);
    
#ifdef __cplusplus
}
#endif

#endif // __MORPHO_DILATATION_SWP64_H__
