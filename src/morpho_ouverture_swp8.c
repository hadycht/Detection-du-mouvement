/* ------------------------------- */
/* --- morpho_ouverture_swp8.c --- */
/* ------------------------------- */

/*
 * CopTright (c) 2004 - 2013, Lionel Lacassagne, All rights reserved
 * UniversitT of Paris Sud, Laboratoire de Recherche en Informatique
 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "nrtype.h"
#include "nrdef.h"
#include "nrutil.h"
//#include "nrutil_ext.h" // printfM8

//#include "sequence.h"

#include "swp.h"  // left right
#include "morpho.h"

#include "morpho_erosion_swp8.h"
#include "morpho_dilatation_swp8.h"

//#define LOCAL_DEBUG

#ifdef LOCAL_DEBUG
    #define disp(x) printf("%s = %2x ", #x, x)
    #define CR putchar('\n')
    #define PUTS(x) puts(x)
#else
    #define disp(x)
    #define CR
    #define PUTS(x)
#endif
// ------------------------------------------------------------------------------------
void line_ouverture3_ui8matrix_swp8_fusion_basic(uint8 **X, int i, int j0, int j1, uint8 **Y)
// ------------------------------------------------------------------------------------
{
}
// -----------------------------------------------------------------------------------------
void line_ouverture3_ui8matrix_swp8_fusion_red1(uint8 **X, int i, int j0, int j1, uint8 **Y)
// -----------------------------------------------------------------------------------------
{
    // TODO
}
// -----------------------------------------------------------------------------------------
void line_ouverture3_ui8matrix_swp8_fusion_red2(uint8 **X, int i, int j0, int j1, uint8 **Y)
// -----------------------------------------------------------------------------------------
{
    // TODO
}
// ---------------------------------------------------------------------------------------------
void line_ouverture3_ui8matrix_swp8_fusion_ilu3_red(uint8 **X, int i, int j0, int j1, uint8 **Y)
// ---------------------------------------------------------------------------------------------
{
    // TODO
}
// ---------------------------------------------------------------------------------------------
void line_ouverture3_ui8matrix_swp8_fusion_elu2_red(uint8 **X, int i, int j0, int j1, uint8 **Y)
// ---------------------------------------------------------------------------------------------
{
    // TODO
}

// --------------------------------------------------------------------------------------------------
void line_ouverture3_ui8matrix_swp8_fusion_ilu3_elu2_red(uint8 **X, int i, int j0, int j1, uint8 **Y)
// --------------------------------------------------------------------------------------------------
{
    // TODO
}

// ---------------------------------------------------------------------------------------------------------
void line_ouverture3_ui8matrix_swp8_fusion_ilu3_elu2_red_factor(uint8 **X, int i, int j0, int j1, uint8 **Y)
// ---------------------------------------------------------------------------------------------------------
{
    // TODO
 }
// --------------------------------------------------------------------------------------------------
void ouverture3_ui8matrix_swp8_basic(uint8 **X, int i0, int i1, int j0, int j1, uint8 **T, uint8 **Y)
// --------------------------------------------------------------------------------------------------
{
    erosion3_ui8matrix_swp8_basic(X, i0-1, i1+1, j0, j1, T);
    dilatation3_ui8matrix_swp8_basic(T, i0,   i1,   j0,   j1,   Y);
}
// ----------------------------------------------------------------------------------------
void ouverture3_ui8matrix_swp8_fusion_basic(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// ----------------------------------------------------------------------------------------
{
    for(int i=i0; i<=i1; i++) {
        line_ouverture3_ui8matrix_swp8_fusion_basic(X, i, j0, j1, Y);
    }
}
// ---------------------------------------------------------------------------------------------
void ouverture3_ui8matrix_swp8_fusion_red(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// ---------------------------------------------------------------------------------------------
{
    // TODO
}
// ---------------------------------------------------------------------------------------------
void ouverture3_ui8matrix_swp8_fusion_red2(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// ---------------------------------------------------------------------------------------------
{
    // TODO
}
// -------------------------------------------------------------------------------------------------
void ouverture3_ui8matrix_swp8_fusion_ilu3_red(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// -------------------------------------------------------------------------------------------------
{
    // TODO
}
// -------------------------------------------------------------------------------------------------
void ouverture3_ui8matrix_swp8_fusion_elu2_red(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// -------------------------------------------------------------------------------------------------
{
    // TODO
}
// ------------------------------------------------------------------------------------------------------
void ouverture3_ui8matrix_swp8_fusion_ilu3_elu2_red(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// ------------------------------------------------------------------------------------------------------
{
    // TODO
}
// -------------------------------------------------------------------------------------------------------------
void ouverture3_ui8matrix_swp8_fusion_ilu3_elu2_red_factor(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// -------------------------------------------------------------------------------------------------------------
{
    // TODO
}
// -----------------------------------------------------------------------------------------------------------
void ouverture3_ui8matrix_swp8_pipeline_basic(uint8 **X, int i0, int i1, int j0, int j1, uint8 **T, uint8 **Y)
// ----------------------------------------------------------------------------------------------------------
{
    // PROLOGUE

    line_erosion3_ui8matrix_swp8_basic(X, i0, j0, j1, T);

    for(int i=i0; i<i1; i++) {
        line_erosion3_ui8matrix_swp8_basic(X, i+1, j0, j1, T);
        line_dilatation3_ui8matrix_swp8_basic(T, i, j0, j1, Y);
    }

    //EPILOGUE
    line_erosion3_ui8matrix_swp8_basic(X, i1, j0, j1, T);
    line_dilatation3_ui8matrix_swp8_basic(T, i1, j0, j1, Y);
    // erosion3_ui8matrix_swp8_basic(X, i0, i1, j0, j1, T);
    // //display_ui8matrix(T, i0, i1, j0, j1, "%d", "X après l'érosion");
    // dilatation3_ui8matrix_swp8_basic(T, i0, i1, j0, j1, Y);
}
// ---------------------------------------------------------------------------------------------------------
void ouverture3_ui8matrix_swp8_pipeline_red(uint8 **X, int i0, int i1, int j0, int j1, uint8 **T, uint8 **Y)
// ---------------------------------------------------------------------------------------------------------
{
    // PROLOGUE

    line_erosion3_ui8matrix_swp8_red(X, i0, j0, j1, T);
   
    for(int i=i0; i<i1; i++) {
        line_erosion3_ui8matrix_swp8_red(X, i+1, j0, j1, T);
        line_dilatation3_ui8matrix_swp8_red(T, i, j0, j1, Y);
    }

    // EPILOGUE
    line_erosion3_ui8matrix_swp8_red(X, i1, j0, j1, T);
    line_dilatation3_ui8matrix_swp8_red(T, i1, j0, j1, Y);
}
// --------------------------------------------------------------------------------------------------------------
void ouverture3_ui8matrix_swp8_pipeline_ilu3_red(uint8 **X, int i0, int i1, int j0, int j1, uint8 **T, uint8 **Y)
// --------------------------------------------------------------------------------------------------------------
{
    // PROLOGUE
    line_erosion3_ui8matrix_swp8_ilu3_red(X, i0-1, j0, j1, T);
    line_erosion3_ui8matrix_swp8_ilu3_red(X, i0, j0, j1, T);
    
    for(int i=i0; i<i1; i++) {
        line_erosion3_ui8matrix_swp8_ilu3_red(X, i+1, j0, j1, T);
        line_dilatation3_ui8matrix_swp8_ilu3_red(T, i, j0, j1, Y);
    }

    // EPILOGUE 
    line_erosion3_ui8matrix_swp8_ilu3_red(X, i1+1, j0, j1, T);
    line_dilatation3_ui8matrix_swp8_ilu3_red(T, i1, j0, j1, Y);
}
// --------------------------------------------------------------------------------------------------------------
void ouverture3_ui8matrix_swp8_pipeline_elu2_red(uint8 **X, int i0, int i1, int j0, int j1, uint8 **T, uint8 **Y)
// --------------------------------------------------------------------------------------------------------------
{
    int r = (i1 - i0 + 1) % 2;
    //PROLOGUE
    line_erosion3_ui8matrix_swp8_elu2_red(X, i0, j0, j1, T);
    
    for(int i=i0; i<=i1-r; i+=2) {
        line_erosion3_ui8matrix_swp8_elu2_red(X, i+1, j0, j1, T);
        line_dilatation3_ui8matrix_swp8_elu2_red(T, i, j0, j1, Y);
    }

    if (r) {
        line_erosion3_ui8matrix_swp8_elu2_red(X, i1, j0, j1, T);
        line_dilatation3_ui8matrix_swp8_elu2_red(T, i1-1, j0, j1, Y);
    }
}
// ---------------------------------------------------------------------------------------------------------------------
void ouverture3_ui8matrix_swp8_pipeline_elu2_red_factor(uint8 **X, int i0, int i1, int j0, int j1, uint8 **T, uint8 **Y)
// ---------------------------------------------------------------------------------------------------------------------
{
    int r = (i1 - i0 + 1) % 2;
    //PROLOGUE
    line_erosion3_ui8matrix_swp8_elu2_red_factor(X, i0, j0, j1, T);
    
    for(int i=i0; i<=i1-r; i+=2) {
        line_erosion3_ui8matrix_swp8_elu2_red_factor(X, i+1, j0, j1, T);
        line_dilatation3_ui8matrix_swp8_elu2_red_factor(T, i, j0, j1, Y);
    }

    if (r) {
        line_erosion3_ui8matrix_swp8_elu2_red_factor(X, i1, j0, j1, T);
        line_dilatation3_ui8matrix_swp8_elu2_red_factor(T, i1-1, j0, j1, Y);
    }
}
// -------------------------------------------------------------------------------------------------------------------
void ouverture3_ui8matrix_swp8_pipeline_ilu3_elu2_red(uint8 **X, int i0, int i1, int j0, int j1, uint8 **T, uint8 **Y)
// -------------------------------------------------------------------------------------------------------------------
{
    int r = (i1 - i0 + 1) % 2;
    //PROLOGUE
    line_erosion3_ui8matrix_swp8_ilu3_elu2_red(X, i0, j0, j1, T);
    for(int i=i0; i<i1-r; i+=2) {
        line_erosion3_ui8matrix_swp8_ilu3_elu2_red(X, i+1, j0, j1, T);
        line_dilatation3_ui8matrix_swp8_ilu3_elu2_red(T, i, j0, j1, Y);
    }
   
    if (r) {
        line_erosion3_ui8matrix_swp8_ilu3_elu2_red(X, i1, j0, j1, T);
        line_dilatation3_ui8matrix_swp8_ilu3_elu2_red(T, i1-1, j0, j1, Y);
    } else { // en soit c'est l'epilogue
        line_erosion3_ui8matrix_swp8_basic(X, i1, j0, j1, T);
        line_dilatation3_ui8matrix_swp8_basic(T, i1, j0, j1, Y);
    }
}
// --------------------------------------------------------------------------------------------------------------------------
void ouverture3_ui8matrix_swp8_pipeline_ilu3_elu2_red_factor(uint8 **X, int i0, int i1, int j0, int j1, uint8 **T, uint8 **Y)
// --------------------------------------------------------------------------------------------------------------------------
{
    int r = (i1 - i0 + 1) % 2;
    //PROLOGUE
    line_erosion3_ui8matrix_swp8_ilu3_elu2_red(X, i0, j0, j1, T);
    for(int i=i0; i<i1-r; i+=2) {
        line_erosion3_ui8matrix_swp8_ilu3_elu2_red(X, i+1, j0, j1, T);
        line_dilatation3_ui8matrix_swp8_ilu3_elu2_red(T, i, j0, j1, Y);
    }
   
    if (r) {
        line_erosion3_ui8matrix_swp8_ilu3_elu2_red(X, i1, j0, j1, T);
        line_dilatation3_ui8matrix_swp8_ilu3_elu2_red(T, i1-1, j0, j1, Y);
    } else { // en soit c'est l'epilogue
        line_erosion3_ui8matrix_swp8_basic(X, i1, j0, j1, T);
        line_dilatation3_ui8matrix_swp8_basic(T, i1, j0, j1, Y);
    }
}
