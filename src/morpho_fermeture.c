/* -------------------------- */
/* --- morpho_fermeture.c --- */
/* -------------------------- */

/*
 * Copyright (c) 2004 - 2013, Lionel Lacassagne, All rights reserved
 * University of Paris Sud, Laboratoire de Recherche en Informatique 
 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "nrtype.h"
#include "nrdef.h"
#include "nrutil.h"
//#include "sequence.h"

#include "swp.h"

#include "morpho.h"
#include "morpho_erosion.h"
#include "morpho_dilatation.h"

#include "morpho_fermeture.h"

// -------------------------------------------------------------------------------
void line_fermeture3_ui8matrix_fusion(uint8 **X, int i, int j0, int j1, uint8 **Y)
// -------------------------------------------------------------------------------
{
    // TODO
}
// ----------------------------------------------------------------------------------------
void line_fermeture3_ui8matrix_fusion_ilu5_red(uint8 **X, int i, int j0, int j1, uint8 **Y)
// ----------------------------------------------------------------------------------------
{
    // TODO
}
// ---------------------------------------------------------------------------------------------
void line_fermeture3_ui8matrix_fusion_ilu5_elu2_red(uint8 **X, int i, int j0, int j1, uint8 **Y)
// ---------------------------------------------------------------------------------------------
{
    // TODO
}
// ----------------------------------------------------------------------------------------------------
void line_fermeture3_ui8matrix_fusion_ilu5_elu2_red_factor(uint8 **X, int i, int j0, int j1, uint8 **Y)
// ----------------------------------------------------------------------------------------------------
{
    // TODO
}
// -----------------------------------------------------------------------------------------
void line_fermeture3_ui8matrix_fusion_ilu15_red(uint8 **X, int i, int j0, int j1, uint8 **Y)
// -----------------------------------------------------------------------------------------
{
    // TODO
}
// ---------------------------------------------------------------------------------------------
void fermeture3_ui8matrix_basic(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y, uint8 **Z)
// ---------------------------------------------------------------------------------------------
{
    dilatation3_ui8matrix_basic(X, i0-1, i1+1, j0-1, j1+1, Y);
    erosion3_ui8matrix_basic(Y, i0,   i1,   j0,   j1,   Z);
}
// -----------------------------------------------------------------------------------
void fermeture3_ui8matrix_fusion(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// -----------------------------------------------------------------------------------
{
    for(int i=i0; i<=i1; i++) {
        line_fermeture3_ui8matrix_fusion(X, i, j0, j1, Y);
    }
}
// --------------------------------------------------------------------------------------------
void fermeture3_ui8matrix_fusion_ilu5_red(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// --------------------------------------------------------------------------------------------
{
    // TODO
}
// -------------------------------------------------------------------------------------------------
void fermeture3_ui8matrix_fusion_ilu5_elu2_red(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// -------------------------------------------------------------------------------------------------
{
    // TODO
}
// --------------------------------------------------------------------------------------------------------
void fermeture3_ui8matrix_fusion_ilu5_elu2_red_factor(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// --------------------------------------------------------------------------------------------------------
{
    // TODO
}
// --------------------------------------------------------------------------------------------
void fermeture3_ui8matrix_fusion_ilu15_red(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// --------------------------------------------------------------------------------------------
{
    // TODO
}
// ------------------------------------------------------------------------------------------------------
void fermeture3_ui8matrix_pipeline_basic(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y, uint8 **Z)
// ------------------------------------------------------------------------------------------------------
{
    // PROLOGUE
    //line_dilatation3_ui8matrix_basic(X, i0-1, j0, j1, Z);
    line_dilatation3_ui8matrix_basic(X, i0, j0, j1, Z);
   
    for(int i=i0; i<i1; i++) {
        line_dilatation3_ui8matrix_basic(X, i+1, j0, j1, Z);
        line_erosion3_ui8matrix_basic(Z, i, j0, j1, Y);
    }
    // EPILOGUE
    line_dilatation3_ui8matrix_basic(X, i1, j0, j1, Z);
    line_erosion3_ui8matrix_basic(Z, i1, j0, j1, Y);

    // erosion3_ui8matrix_basic(X, i0, i1, j0, j1, Z);
    // dilatation3_ui8matrix_basic(Z, i0, i1, j0, j1, Y);
}
// ---------------------------------------------------------------------------------------------------------
void fermeture3_ui8matrix_pipeline_ilu3_red(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y, uint8 **Z)
// ---------------------------------------------------------------------------------------------------------
{
    //line_dilatation3_ui8matrix_ilu3_red(X, i0-1, j0, j1, Z);
    // PROLOGUE 
    line_dilatation3_ui8matrix_ilu3_red(X, i0, j0, j1, Z);
    
    for(int i=i0; i<i1; i++) {
        line_dilatation3_ui8matrix_ilu3_red(X, i+1, j0, j1, Z);
        line_erosion3_ui8matrix_ilu3_red(Z, i, j0, j1, Y);
    }

    // EPILOGUE
    line_dilatation3_ui8matrix_ilu3_red(X, i1, j0, j1, Z);
    line_erosion3_ui8matrix_ilu3_red(Z, i1, j0, j1, Y);

}
// ----------------------------------------------------------------------------------------------------------------
void fermeture3_ui8matrix_pipeline_elu2_red_factor(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y, uint8 **Z)
// ----------------------------------------------------------------------------------------------------------------
{
    int r = (i1 - i0 + 1) % 2;
    //PROLOGUE
    line_dilatation3_ui8matrix_elu2_red_factor(X, i0, j0, j1, Z);
    
    for(int i=i0; i<i1-r-1; i+=2) {
        line_dilatation3_ui8matrix_elu2_red_factor(X, i+1, j0, j1, Z);
        line_erosion3_ui8matrix_elu2_red_factor(Z, i, j0, j1, Y);
    }

    if (r) {
        line_dilatation3_ui8matrix_elu2_red_factor(X, i1, j0, j1, Z);
        line_erosion3_ui8matrix_elu2_red_factor(Z, i1-1, j0, j1, Y);
    }
}
