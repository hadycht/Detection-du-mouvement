/* -------------------------------- */
/* --- morpho_ouverture_swp64.h --- */
/* -------------------------------- */

/*
* CopTright (c) 2020 - 2021, Lionel Lacassagne, All rights reserved
* Sorbonne University, LIP6, CNRS
*/

// 64-bit binary opening

#ifndef __MORPHO_OUVERTURE_SWP64_H__
#define __MORPHO_OUVERTURE_SWP64_H__

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
void line_ouverture3_ui64matrix_swp64_fusion_basic               (uint64 **X, int i, int j0, int j1, uint64 **T);
void line_ouverture3_ui64matrix_swp64_fusion_red                 (uint64 **X, int i, int j0, int j1, uint64 **T);
void line_ouverture3_ui64matrix_swp64_fusion_ilu3_red            (uint64 **X, int i, int j0, int j1, uint64 **T);
void line_ouverture3_ui64matrix_swp64_fusion_elu2_red            (uint64 **X, int i, int j0, int j1, uint64 **T);
void line_ouverture3_ui64matrix_swp64_fusion_ilu3_elu2_red       (uint64 **X, int i, int j0, int j1, uint64 **T);
void line_ouverture3_ui64matrix_swp64_fusion_ilu3_elu2_red_factor(uint64 **X, int i, int j0, int j1, uint64 **T);

// =========================
// == fonctions completes ==
// =========================

// composition d'operateurs
void ouverture3_ui64matrix_swp64_basic                        (uint64 **X, int i0, int i1, int j0, int j1, uint64 **T, uint64 **Y);

// fusion d'operateurs
void ouverture3_ui64matrix_swp64_fusion_basic                 (uint64 **X, int i0, int i1, int j0, int j1, uint64 **T);
void ouverture3_ui64matrix_swp64_fusion_red                   (uint64 **X, int i0, int i1, int j0, int j1, uint64 **T);
void ouverture3_ui64matrix_swp64_fusion_ilu3_red              (uint64 **X, int i0, int i1, int j0, int j1, uint64 **T);
void ouverture3_ui64matrix_swp64_fusion_elu2_red              (uint64 **X, int i0, int i1, int j0, int j1, uint64 **T);
void ouverture3_ui64matrix_swp64_fusion_ilu3_elu2_red         (uint64 **X, int i0, int i1, int j0, int j1, uint64 **T);
void ouverture3_ui64matrix_swp64_fusion_ilu3_elu2_red_factor  (uint64 **X, int i0, int i1, int j0, int j1, uint64 **T);

// pipeline d'operateurs
void ouverture3_ui64matrix_swp64_pipeline_basic               (uint64 **X, int i0, int i1, int j0, int j1, uint64 **T, uint64 **Y);
void ouverture3_ui64matrix_swp64_pipeline_red                 (uint64 **X, int i0, int i1, int j0, int j1, uint64 **T, uint64 **Y);
void ouverture3_ui64matrix_swp64_pipeline_ilu3_red            (uint64 **X, int i0, int i1, int j0, int j1, uint64 **T, uint64 **Y);
void ouverture3_ui64matrix_swp64_pipeline_elu2_red            (uint64 **X, int i0, int i1, int j0, int j1, uint64 **T, uint64 **Y);
void ouverture3_ui64matrix_swp64_pipeline_elu2_red_factor     (uint64 **X, int i0, int i1, int j0, int j1, uint64 **T, uint64 **Y);
void ouverture3_ui64matrix_swp64_pipeline_ilu3_elu2_red       (uint64 **X, int i0, int i1, int j0, int j1, uint64 **T, uint64 **Y);
void ouverture3_ui64matrix_swp64_pipeline_ilu3_elu2_red_factor(uint64 **X, int i0, int i1, int j0, int j1, uint64 **T, uint64 **Y);
#ifdef __cplusplus
}
#endif

#endif // __MORPHO_OUVERTURE_SWP64_H__
