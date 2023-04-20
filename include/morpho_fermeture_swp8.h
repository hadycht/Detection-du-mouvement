/* ------------------------------- */
/* --- morpho_fermeture_swp8.h --- */
/* ------------------------------- */

/*
* Copyright (c) 2020 - 2021, Lionel Lacassagne, All rights reserved
* Sorbonne University, LIP6, CNRS
*/

// 32-bit binary opening

#ifndef __MORPHO_FERMETURE_SWP8_H__
#define __MORPHO_FERMETURE_SWP8_H__

#ifdef __cplusplus
#ifdef PRAGMA_VERBOSE
#pragma message ("C++")
#endif
extern "C" {
#endif

void line_fermeture3_ui32matrix_swp8_fusion_basic               (uint8 **X, int i, int j0, int j1, uint8 **Y);
void line_fermeture3_ui32matrix_swp8_fusion_red1                (uint8 **X, int i, int j0, int j1, uint8 **Y);
void line_fermeture3_ui32matrix_swp8_fusion_red2                (uint8 **X, int i, int j0, int j1, uint8 **Y);
void line_fermeture3_ui32matrix_swp8_fusion_ilu3_red            (uint8 **X, int i, int j0, int j1, uint8 **Y);
void line_fermeture3_ui32matrix_swp8_fusion_ilu3_elu2_red       (uint8 **X, int i, int j0, int j1, uint8 **Y);
void line_fermeture3_ui32matrix_swp8_fusion_ilu3_elu2_red_factor(uint8 **X, int i, int j0, int j1, uint8 **Y);

void fermeture3_ui8matrix_swp8_basic                        (uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y, uint8 **Z);

void fermeture3_ui8matrix_swp8_fusion_basic                 (uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y);
void fermeture3_ui8matrix_swp8_fusion_red1                  (uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y);
void fermeture3_ui8matrix_swp8_fusion_red2                  (uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y);
void fermeture3_ui8matrix_swp8_fusion_ilu3_red              (uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y);
void fermeture3_ui8matrix_swp8_fusion_ilu3_elu2_red         (uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y);
void fermeture3_ui8matrix_swp8_fusion_ilu3_elu2_red_factor  (uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y);
void fermeture3_ui8matrix_swp8_pipeline_basic               (uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y, uint8 **Z);
void fermeture3_ui8matrix_swp8_pipeline_ilu3_red            (uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y, uint8 **Z);
void fermeture3_ui8matrix_swp8_pipeline_elu2_red_factor     (uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y, uint8 **Z);
void fermeture3_ui8matrix_swp8_pipeline_ilu3_elu2_red_factor(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y, uint8 **Z);

#ifdef __cplusplus
}
#endif

#endif // __MORPHO_FERMETURE_SWP8_H__
