/* -------------------------- */
/* --- morpho_fermeture.h --- */
/* -------------------------- */

/*
* Copyright (c) 2020 - 2021, Lionel Lacassagne, All rights reserved
* Sorbonne University, LIP6, CNRS
*/

#ifndef __MORPHO_FERMETURE_H__
#define __MORPHO_FERMETURE_H__

#ifdef __cplusplus
#ifdef PRAGMA_VERBOSE
#pragma message ("C++")
#endif
extern "C" {
#endif


void line_fermeture3_ui8matrix_fusion_basic               (uint8 **X, int i, int j0, int j1, uint8 **Y);
void line_fermeture3_ui8matrix_fusion_ilu5_red            (uint8 **X, int i, int j0, int j1, uint8 **Y);
void line_fermeture3_ui8matrix_fusion_ilu5_elu2_red       (uint8 **X, int i, int j0, int j1, uint8 **Y);
void line_fermeture3_ui8matrix_fusion_ilu5_elu2_red_factor(uint8 **X, int i, int j0, int j1, uint8 **Y);
void line_fermeture3_ui8matrix_fusion_ilu15_red           (uint8 **X, int i, int j0, int j1, uint8 **Y);

void fermeture3_ui8matrix_basic                      (uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y, uint8 **Z);

void fermeture3_ui8matrix_fusion_basic               (uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y);
void fermeture3_ui8matrix_fusion_ilu5_red            (uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y);
void fermeture3_ui8matrix_fusion_ilu5_elu2_red       (uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y);
void fermeture3_ui8matrix_fusion_ilu5_elu2_red_factor(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y);
void fermeture3_ui8matrix_fusion_ilu15_red           (uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y);

void fermeture3_ui8matrix_pipeline_basic          (uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y, uint8 **Z);
void fermeture3_ui8matrix_pipeline_ilu3_red       (uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y, uint8 **Z);
void fermeture3_ui8matrix_pipeline_elu2_red_factor(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y, uint8 **Z);

#ifdef __cplusplus
}
#endif

#endif // __MORPHO_FERMETURE_H__
