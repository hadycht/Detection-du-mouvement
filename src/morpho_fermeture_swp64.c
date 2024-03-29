/* -------------------------------- */
/* --- morpho_fermeture_swp64.c --- */
/* -------------------------------- */

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
//#include "nrutil_ext.h" // printfM8

//#include "sequence.h"

#include "swp.h"  // left right
#include "morpho.h"

#include "morpho_erosion_swp64.h"
#include "morpho_dilatation_swp64.h"

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

// ----------------------------------------------------------------------------------------------
void line_fermeture3_ui64matrix_swp64_fusion_basic(uint64 **X, int i, int j0, int j1, uint64 **Y)
// ----------------------------------------------------------------------------------------------
{
    // TODO
}
// ---------------------------------------------------------------------------------------------
void line_fermeture3_ui64matrix_swp64_fusion_red1(uint64 **X, int i, int j0, int j1, uint64 **Y)
// ---------------------------------------------------------------------------------------------
{
    // TODO
}
// ---------------------------------------------------------------------------------------------
void line_fermeture3_ui64matrix_swp64_fusion_red2(uint64 **X, int i, int j0, int j1, uint64 **Y)
// ---------------------------------------------------------------------------------------------
{
    // TODO
}
// -------------------------------------------------------------------------------------------------
void line_fermeture3_ui64matrix_swp64_fusion_ilu3_red(uint64 **X, int i, int j0, int j1, uint64 **Y)
// -------------------------------------------------------------------------------------------------
{
    // TODO
}
// ------------------------------------------------------------------------------------------------------
void line_fermeture3_ui64matrix_swp64_fusion_ilu3_elu2_red(uint64 **X, int i, int j0, int j1, uint64 **Y)
// ------------------------------------------------------------------------------------------------------
{
    // TODO
}

// -------------------------------------------------------------------------------------------------------------
void line_fermeture3_ui64matrix_swp64_fusion_ilu3_elu2_red_factor(uint64 **X, int i, int j0, int j1, uint64 **Y)
// -------------------------------------------------------------------------------------------------------------
{
    // TODO
}
// -------------------------------------------------------------------------------------------------------
void fermeture3_ui64matrix_swp64_basic(uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y, uint64 **Z)
// -------------------------------------------------------------------------------------------------------
{
    erosion3_ui64matrix_swp64_basic(X, i0-1, i1+1, j0-1, j1+1, Y);
    dilatation3_ui64matrix_swp64_basic(Y, i0,   i1,   j0,   j1,   Z);
}
// --------------------------------------------------------------------------------------------
void fermeture3_ui64matrix_swp64_fusion(uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y)
// --------------------------------------------------------------------------------------------
{
    for(int i=i0; i<=i1; i++) {
        line_fermeture3_ui64matrix_swp64_fusion_basic(X, i, j0, j1, Y);
    }
}
// -------------------------------------------------------------------------------------------------
void fermeture3_ui64matrix_swp64_fusion_red1(uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y)
// -------------------------------------------------------------------------------------------------
{
    // TODO
}
// -------------------------------------------------------------------------------------------------
void fermeture3_ui64matrix_swp64_fusion_red2(uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y)
// -------------------------------------------------------------------------------------------------
{
    // TODO
}
// -----------------------------------------------------------------------------------------------------
void fermeture3_ui64matrix_swp64_fusion_ilu3_red(uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y)
// -----------------------------------------------------------------------------------------------------
{
    // TODO
}
// ----------------------------------------------------------------------------------------------------------
void fermeture3_ui64matrix_swp64_fusion_ilu3_elu2_red(uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y)
// ----------------------------------------------------------------------------------------------------------
{
    // TODO
}
// -----------------------------------------------------------------------------------------------------------------
void fermeture3_ui64matrix_swp64_fusion_ilu3_elu2_red_factor(uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y)
// -----------------------------------------------------------------------------------------------------------------
{
    // TODO
}
// ----------------------------------------------------------------------------------------------------------------
void fermeture3_ui64matrix_swp64_pipeline_basic(uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y, uint64 **Z)
// ----------------------------------------------------------------------------------------------------------------
{
    // TODO
}
// -------------------------------------------------------------------------------------------------------------------
void fermeture3_ui64matrix_swp64_pipeline_ilu3_red(uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y, uint64 **Z)
// -------------------------------------------------------------------------------------------------------------------
{
    // TODO
}
// --------------------------------------------------------------------------------------------------------------------------
void fermeture3_ui64matrix_swp64_pipeline_elu2_red_factor(uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y, uint64 **Z)
// --------------------------------------------------------------------------------------------------------------------------
{
    // TODO
}
// -------------------------------------------------------------------------------------------------------------------------------
void fermeture3_ui64matrix_swp64_pipeline_ilu3_elu2_red_factor(uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y, uint64 **Z)
// -------------------------------------------------------------------------------------------------------------------------------
{
    // TODO
}
