/* -------------------------------- */
/* --- morpho_ouverture_swp32.h --- */
/* -------------------------------- */

/*
* CopTright (c) 2020 - 2021, Lionel Lacassagne, All rights reserved
* Sorbonne University, LIP6, CNRS
*/

// 32-bit binary opening

#ifndef __MORPHO_OUVERTURE_SWP32_H__
#define __MORPHO_OUVERTURE_SWP32_H__

#ifdef __cplusplus
#ifdef PRAGMA_VERBOSE
#pragma message ("C++")
#endif
extern "C" {
#endif

// ======================
// == fonctions lignes ==
// ======================

// fusion d'operateurs
void line_ouverture3_ui32matrix_swp32_fusion_basic               (uint32 **X, int i, int j0, int j1, uint32 **T);
void line_ouverture3_ui32matrix_swp32_fusion_red                 (uint32 **X, int i, int j0, int j1, uint32 **T);
void line_ouverture3_ui32matrix_swp32_fusion_ilu3_red            (uint32 **X, int i, int j0, int j1, uint32 **T);
void line_ouverture3_ui32matrix_swp32_fusion_ilu3_elu2_red       (uint32 **X, int i, int j0, int j1, uint32 **T);
void line_ouverture3_ui32matrix_swp32_fusion_ilu3_elu2_red_factor(uint32 **X, int i, int j0, int j1, uint32 **T);

// =========================
// == fonctions completes ==
// =========================

// composition d'operateurs
void ouverture3_ui32matrix_swp32_basic                        (uint32 **X, int i0, int i1, int j0, int j1, uint32 **T, uint32 **Y);

// fusion d'operateurs
void ouverture3_ui32matrix_swp32_fusion_basic                 (uint32 **X, int i0, int i1, int j0, int j1, uint32 **T);
void ouverture3_ui32matrix_swp32_fusion_red                   (uint32 **X, int i0, int i1, int j0, int j1, uint32 **T);
void ouverture3_ui32matrix_swp32_fusion_ilu3_red              (uint32 **X, int i0, int i1, int j0, int j1, uint32 **T);
void ouverture3_ui32matrix_swp32_fusion_elu2_red              (uint32 **X, int i0, int i1, int j0, int j1, uint32 **T);
void ouverture3_ui32matrix_swp32_fusion_ilu3_elu2_red         (uint32 **X, int i0, int i1, int j0, int j1, uint32 **T);
void ouverture3_ui32matrix_swp32_fusion_ilu3_elu2_red_factor  (uint32 **X, int i0, int i1, int j0, int j1, uint32 **T);

// pipeline d'operateurs
void ouverture3_ui32matrix_swp32_pipeline_basic               (uint32 **X, int i0, int i1, int j0, int j1, uint32 **T, uint32 **Y);
void ouverture3_ui32matrix_swp32_pipeline_red                 (uint32 **X, int i0, int i1, int j0, int j1, uint32 **T, uint32 **Y);
void ouverture3_ui32matrix_swp32_pipeline_ilu3_red            (uint32 **X, int i0, int i1, int j0, int j1, uint32 **T, uint32 **Y);
void ouverture3_ui32matrix_swp32_pipeline_elu2_red            (uint32 **X, int i0, int i1, int j0, int j1, uint32 **T, uint32 **Y);
void ouverture3_ui32matrix_swp32_pipeline_elu2_red_factor     (uint32 **X, int i0, int i1, int j0, int j1, uint32 **T, uint32 **Y);
void ouverture3_ui32matrix_swp32_pipeline_ilu3_elu2_red_factor(uint32 **X, int i0, int i1, int j0, int j1, uint32 **T, uint32 **Y);

#ifdef __cplusplus
}
#endif

#endif // __MORPHO_OUVERTURE_SWP32_H__
