/* -------------------------------- */
/* --- morpho_dilatation_swp8.h --- */
/* -------------------------------- */

/*
* Copyright (c) 2020 - 2021, Lionel Lacassagne, All rights reserved
* Sorbonne University, LIP6, CNRS
*/

#ifndef __MORPHO_DILATATION_SWP8_H__
#define __MORPHO_DILATATION_SWP8_H__
#define max(x, y) ((x) > (y) ? (x) : (y))  

#ifdef __cplusplus
#ifdef PRAGMA_VERBOSE
#pragma message ("C++")
#endif
extern "C" {
#endif

// line functions
void line_dilatation3_ui8matrix_swp8_basic               (uint8 **X, int i, int j0, int j1, uint8 **Y);
void line_dilatation3_ui8matrix_swp8_reg                 (uint8 **X, int i, int j0, int j1, uint8 **Y);
void line_dilatation3_ui8matrix_swp8_rot                 (uint8 **X, int i, int j0, int j1, uint8 **Y);
void line_dilatation3_ui8matrix_swp8_red                 (uint8 **X, int i, int j0, int j1, uint8 **Y);
void line_dilatation3_ui8matrix_swp8_ilu3                (uint8 **X, int i, int j0, int j1, uint8 **Y);
void line_dilatation3_ui8matrix_swp8_ilu3_red            (uint8 **X, int i, int j0, int j1, uint8 **Y);
void line_dilatation3_ui8matrix_swp8_elu2_red            (uint8 **X, int i, int j0, int j1, uint8 **Y);
void line_dilatation3_ui8matrix_swp8_elu2_red_factor     (uint8 **X, int i, int j0, int j1, uint8 **Y);
void line_dilatation3_ui8matrix_swp8_ilu3_elu2_red       (uint8 **X, int i, int j0, int j1, uint8 **Y);
void line_dilatation3_ui8matrix_swp8_ilu3_elu2_red_factor(uint8 **X, int i, int j0, int j1, uint8 **Y);
    
// full-matrix functions
void dilatation3_ui8matrix_swp8_basic               (uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y);
void dilatation3_ui8matrix_swp8_reg                 (uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y);
void dilatation3_ui8matrix_swp8_rot                 (uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y);
void dilatation3_ui8matrix_swp8_red                 (uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y);
void dilatation3_ui8matrix_swp8_ilu3                (uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y);
void dilatation3_ui8matrix_swp8_ilu3_red            (uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y);
void dilatation3_ui8matrix_swp8_elu2_red            (uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y);
void dilatation3_ui8matrix_swp8_elu2_red_factor     (uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y);
void dilatation3_ui8matrix_swp8_ilu3_elu2_red       (uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y);
void dilatation3_ui8matrix_swp8_ilu3_elu2_red_factor(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y);
    
#ifdef __cplusplus
}
#endif

#endif // __MORPHO_DILATATION_SWP8_H__
