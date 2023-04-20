/* -------------------------------- */
/* --- morpho_fermeture_swp64.h --- */
/* -------------------------------- */

/*
* Copyright (c) 2020 - 2021, Lionel Lacassagne, All rights reserved
* Sorbonne University, LIP6, CNRS
*/

// 64-bit binary opening

#ifndef __MORPHO_FERMETURE_SWP64_H__
#define __MORPHO_FERMETURE_SWP64_H__

#ifdef __cplusplus
#ifdef PRAGMA_VERBOSE
#pragma message ("C++")
#endif
extern "C" {
#endif

void line_fermeture3_ui64matrix_swp64_fusion_basic               (uint64 **X, int i, int j0, int j1, uint64 **Y);
void line_fermeture3_ui64matrix_swp64_fusion_red1                (uint64 **X, int i, int j0, int j1, uint64 **Y);
void line_fermeture3_ui64matrix_swp64_fusion_red2                (uint64 **X, int i, int j0, int j1, uint64 **Y);
void line_fermeture3_ui64matrix_swp64_fusion_ilu3_red            (uint64 **X, int i, int j0, int j1, uint64 **Y);
void line_fermeture3_ui64matrix_swp64_fusion_ilu3_elu2_red       (uint64 **X, int i, int j0, int j1, uint64 **Y);
void line_fermeture3_ui64matrix_swp64_fusion_ilu3_elu2_red_factor(uint64 **X, int i, int j0, int j1, uint64 **Y);

void fermeture3_ui64matrix_swp64_basic                      (uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y, uint64 **Z);

void fermeture3_ui64matrix_swp64_fusion_basic               (uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y);
void fermeture3_ui64matrix_swp64_fusion_red1                (uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y);
void fermeture3_ui64matrix_swp64_fusion_red2                (uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y);
void fermeture3_ui64matrix_swp64_fusion_ilu3_red            (uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y);
void fermeture3_ui64matrix_swp64_fusion_ilu3_elu2_red       (uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y);
void fermeture3_ui64matrix_swp64_fusion_ilu3_elu2_red_factor(uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y);

void fermeture3_ui64matrix_swp64_pipeline_basic               (uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y, uint64 **Z);
void fermeture3_ui64matrix_swp64_pipeline_ilu3_red            (uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y, uint64 **Z);
void fermeture3_ui64matrix_swp64_pipeline_elu2_red_factor     (uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y, uint64 **Z);
void fermeture3_ui64matrix_swp64_pipeline_ilu3_elu2_red_factor(uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y, uint64 **Z);

#ifdef __cplusplus
}
#endif

#endif // __MORPHO_FERMETURE_SWP64_H__
