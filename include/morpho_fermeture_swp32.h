/* -------------------------------- */
/* --- morpho_fermeture_swp32.h --- */
/* -------------------------------- */

/*
* Copyright (c) 2020 - 2021, Lionel Lacassagne, All rights reserved
* Sorbonne University, LIP6, CNRS
*/

// 32-bit binary opening

#ifndef __MORPHO_FERMETURE_SWP32_H__
#define __MORPHO_FERMETURE_SWP32_H__

#ifdef __cplusplus
#ifdef PRAGMA_VERBOSE
#pragma message ("C++")
#endif
extern "C" {
#endif

void line_fermeture3_ui32matrix_swp32_fusion_basic               (uint32 **X, int i, int j0, int j1, uint32 **Y);
void line_fermeture3_ui32matrix_swp32_fusion_red1                (uint32 **X, int i, int j0, int j1, uint32 **Y);
void line_fermeture3_ui32matrix_swp32_fusion_red2                (uint32 **X, int i, int j0, int j1, uint32 **Y);
void line_fermeture3_ui32matrix_swp32_fusion_ilu3_red            (uint32 **X, int i, int j0, int j1, uint32 **Y);
void line_fermeture3_ui32matrix_swp32_fusion_ilu3_elu2_red       (uint32 **X, int i, int j0, int j1, uint32 **Y);
void line_fermeture3_ui32matrix_swp32_fusion_ilu3_elu2_red_factor(uint32 **X, int i, int j0, int j1, uint32 **Y);

void fermeture3_ui32matrix_swp32_basic                      (uint32 **X, int i0, int i1, int j0, int j1, uint32 **Y, uint32 **Z);

void fermeture3_ui32matrix_swp32_fusion_basic               (uint32 **X, int i0, int i1, int j0, int j1, uint32 **Y);
void fermeture3_ui32matrix_swp32_fusion_red1                (uint32 **X, int i0, int i1, int j0, int j1, uint32 **Y);
void fermeture3_ui32matrix_swp32_fusion_red2                (uint32 **X, int i0, int i1, int j0, int j1, uint32 **Y);
void fermeture3_ui32matrix_swp32_fusion_ilu3_red            (uint32 **X, int i0, int i1, int j0, int j1, uint32 **Y);
void fermeture3_ui32matrix_swp32_fusion_ilu3_elu2_red       (uint32 **X, int i0, int i1, int j0, int j1, uint32 **Y);
void fermeture3_ui32matrix_swp32_fusion_ilu3_elu2_red_factor(uint32 **X, int i0, int i1, int j0, int j1, uint32 **Y);

void fermeture3_ui32matrix_swp32_pipeline_basic               (uint32 **X, int i0, int i1, int j0, int j1, uint32 **Y, uint32 **Z);
void fermeture3_ui32matrix_swp32_pipeline_ilu3_red            (uint32 **X, int i0, int i1, int j0, int j1, uint32 **Y, uint32 **Z);
void fermeture3_ui32matrix_swp32_pipeline_elu2_red_factor     (uint32 **X, int i0, int i1, int j0, int j1, uint32 **Y, uint32 **Z);
void fermeture3_ui32matrix_swp32_pipeline_ilu3_elu2_red_factor(uint32 **X, int i0, int i1, int j0, int j1, uint32 **Y, uint32 **Z);

#ifdef __cplusplus
}
#endif

#endif // __MORPHO_FERMETURE_SWP32_H__
