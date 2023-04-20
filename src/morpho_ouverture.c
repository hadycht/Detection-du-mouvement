/* -------------------------- */
/* --- morpho_ouverture.c --- */
/* -------------------------- */

/*
 * Copyright (c) 2020 - 2021, Lionel Lacassagne, All rights reserved
  
 
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
#include "morpho_min.h"
#include "morpho_max.h"

// -------------------------------------------------------------------------------------
void line_ouverture3_ui8matrix_fusion_basic(uint8 **X, int i, int j0, int j1, uint8 **Y)
// -------------------------------------------------------------------------------------
{
    uint8 x01, x02, x03, x04, x05, x11, x12, x13, x14, x15, x21, x22, x23, x24, x25, x31, x32, x33, x34, x35, x41, x42, x43, x44, x45;
    uint8 t11, t12, t13, t21, t22, t23, t31, t32, t33, res; 
    for (int j = j0; j<=j1; j++) {

        x01 = load2(X, i-2, j-2);
        x02 = load2(X, i-2, j-1);
        x03 = load2(X, i-2, j);
        x04 = load2(X, i-2, j+1);
        x05 = load2(X, i-2, j+2);

        x11 = load2(X, i-1, j-2);
        x12 = load2(X, i-1, j-1);
        x13 = load2(X, i-1, j);
        x14 = load2(X, i-1, j+1);
        x15 = load2(X, i-1, j+2);

        x21 = load2(X, i, j-2);
        x22 = load2(X, i, j-1);
        x23 = load2(X, i, j);
        x24 = load2(X, i, j+1);
        x25 = load2(X, i, j+2);

        x31 = load2(X, i+1, j-2);
        x32 = load2(X, i+1, j-1);
        x33 = load2(X, i+1, j);
        x34 = load2(X, i+1, j+1);
        x35 = load2(X, i+1, j+2);

        x41 = load2(X, i+2, j-2);
        x42 = load2(X, i+2, j-1);
        x43 = load2(X, i+2, j);
        x44 = load2(X, i+2, j+1);
        x45 = load2(X, i+2, j+2);

        t11 = ui8min9(x01, x02, x03, x11, x12, x13, x21, x22, x23);
        t12 = ui8min9(x02, x03, x04, x12, x13, x14, x22, x23, x24);
        t13 = ui8min9(x03, x04, x05, x13, x14, x15, x23, x24, x25);

        t21 = ui8min9(x11, x12, x13, x21, x22, x23, x31, x32, x33);
        t22 = ui8min9(x12, x13, x14, x22, x23, x24, x32, x33, x34);
        t23 = ui8min9(x13, x14, x15, x23, x24, x25, x33, x34, x35);

        t31 = ui8min9(x21, x22, x23, x31, x32, x33, x41, x42, x43);
        t32 = ui8min9(x22, x23, x24, x32, x33, x34, x42, x43, x44);
        t33 = ui8min9(x23, x24, x25, x33, x34, x35, x43, x44, x45);

        res = ui8max9(t11, t12, t13, t21, t22, t23, t31, t32, t33);

        store2(Y, i, j, res);
        
    }
}
// ------------------------------------------------------------------------------------
void line_ouverture3_ui8matrix_fusion_red0(uint8 **X, int i, int j0, int j1, uint8 **Y)
// ------------------------------------------------------------------------------------
{
    
    uint8 x01, x02, x03, x11, x12, x13, x21, x22, x23, x31, x32, x33, x41, x42, x43;
    uint8 a1, a2, a3, a4, a5, b1, b2, b3, b4, b5, c1, c2, c3, c4, c5; //variables qui correspondent aux colonnes
    uint8 t11, t12, t13, t21, t22, t23, t31, t32, t33, res; 

    x01 = load2(X, i-2, j0-2);
    x11 = load2(X, i-1, j0-2);
    x21 = load2(X, i, j0-2);
    x31 = load2(X, i+1, j0-2);
    x41 = load2(X, i+2, j0-2);

    a1 = ui8min3(x01, x11, x21);
    b1 = ui8min3(x11, x21, x31);
    c1 = ui8min3(x21, x31, x41);

    x02 = load2(X, i-2, j0-1);
    x12 = load2(X, i-1, j0-1);
    x22 = load2(X, i, j0-1);
    x32 = load2(X, i+1, j0-1);
    x42 = load2(X, i+2, j0-1);

    a2 = ui8min3(x02, x12, x22);
    b2 = ui8min3(x12, x22, x32);
    c2 = ui8min3(x22, x32, x42);

    x03 = load2(X, i-2, j0);
    x13 = load2(X, i-1, j0);
    x23 = load2(X, i, j0);
    x33 = load2(X, i+1, j0);
    x43 = load2(X, i+2, j0); 

    a3 = ui8min3(x03, x13, x23);
    b3 = ui8min3(x13, x23, x33);
    c3 = ui8min3(x23, x33, x43); 

    x03 = load2(X, i-2, j0+1);
    x13 = load2(X, i-1, j0+1);
    x23 = load2(X, i, j0+1);
    x33 = load2(X, i+1, j0+1);
    x43 = load2(X, i+2, j0+1); 

    a4 = ui8min3(x03, x13, x23);
    b4 = ui8min3(x13, x23, x33);
    c4 = ui8min3(x23, x33, x43); 

    t11 = ui8min3(a1, a2, a3);
    t21 = ui8min3(b1, b2, b3);
    t31 = ui8min3(c1, c2, c3);

    t12 = ui8min3(a2, a3, a4);
    t22 = ui8min3(b2, b3, b4);
    t32 = ui8min3(c2, c3, c4);

    for (int j=j0; j<=j1; j++) {

        x03 = load2(X, i-2, j+2);
        x13 = load2(X, i-1, j+2);
        x23 = load2(X, i, j+2);
        x33 = load2(X, i+1, j+2);
        x43 = load2(X, i+2, j+2); 

        a5 = ui8min3(x03, x13, x23);
        b5 = ui8min3(x13, x23, x33);
        c5 = ui8min3(x23, x33, x43); 

        t13 = ui8min3(a3, a4, a5);
        t23 = ui8min3(b3, b4, b5);
        t33 = ui8min3(c3, c4, c5);

        res = ui8max9(t11, t12, t13, t21, t22, t23, t31, t32, t33);

        store2(Y, i, j, res);

        //rotation des rréductions colonnes de la matrice  X 

        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;

        b1 = b2;
        b2 = b3;
        b3 = b4;
        b4 = b5;

        c1 = c2;
        c2 = c3;
        c3 = c4; 
        c4 = c5; 

        //rotation des rréductions colonnes de la matrice  X 

        t11 = t12;
        t12 = t13;

        t21 = t22;
        t22 = t23;

        t31 = t32;
        t32 = t33;

    }
    

}
// ------------------------------------------------------------------------------------
void line_ouverture3_ui8matrix_fusion_red1(uint8 **X, int i, int j0, int j1, uint8 **Y)
// ------------------------------------------------------------------------------------
{
    
}
// ------------------------------------------------------------------------------------
void line_ouverture3_ui8matrix_fusion_red2(uint8 **X, int i, int j0, int j1, uint8 **Y)
// ------------------------------------------------------------------------------------
{
    // TODO
}
// -----------------------------------------------------------------------------------------
void line_ouverture3_ui8matrix_fusion_ilu5_red2(uint8 **X, int i, int j0, int j1, uint8 **Y)
// -----------------------------------------------------------------------------------------
{
    // TODO
}
// -----------------------------------------------------------------------------------------
void line_ouverture3_ui8matrix_fusion_elu2_red0(uint8 **X, int i, int j0, int j1, uint8 **Y)
// -----------------------------------------------------------------------------------------
{
    // TODO
}
// -----------------------------------------------------------------------------------------
void line_ouverture3_ui8matrix_fusion_elu2_red2(uint8 **X, int i, int j0, int j1, uint8 **Y)
// -----------------------------------------------------------------------------------------
{
    // TODO
}

// ----------------------------------------------------------------------------------------------
void line_ouverture3_ui8matrix_fusion_ilu5_elu2_red2(uint8 **X, int i, int j0, int j1, uint8 **Y)
// ----------------------------------------------------------------------------------------------
{
    // TODO
}
// -----------------------------------------------------------------------------------------------------
void line_ouverture3_ui8matrix_fusion_ilu5_elu2_red2_factor(uint8 **X, int i, int j0, int j1, uint8 **Y)
// -----------------------------------------------------------------------------------------------------
{
    // TODO
}
// -----------------------------------------------------------------------------------------
void line_ouverture3_ui8matrix_fusion_ilu15_red(uint8 **X, int i, int j0, int j1, uint8 **Y)
// -----------------------------------------------------------------------------------------
{
    // TODO
}
// ---------------------------------------------------------------------------------------------
void ouverture3_ui8matrix_basic(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y, uint8 **Z)
// ---------------------------------------------------------------------------------------------
{
    erosion3_ui8matrix_basic   (X, i0-1, i1+1, j0-1, j1+1, Y);
    dilatation3_ui8matrix_basic(Y, i0,   i1,   j0,   j1,   Z);
}
// -----------------------------------------------------------------------------------------
void ouverture3_ui8matrix_fusion_basic(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// -----------------------------------------------------------------------------------------
{
    for(int i=i0; i<=i1; i++) {
        line_ouverture3_ui8matrix_fusion_basic(X, i, j0, j1, Y);
    }
}
// ---------------------------------------------------------------------------------------
void ouverture3_ui8matrix_fusion_red(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// ---------------------------------------------------------------------------------------
{
    
    for(int i=i0; i<=i1; i++) {
        line_ouverture3_ui8matrix_fusion_red0(X, i, j0, j1, Y);
    }
}
// --------------------------------------------------------------------------------------------
void ouverture3_ui8matrix_fusion_ilu5_red(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// --------------------------------------------------------------------------------------------
{
    // TODO
}
// --------------------------------------------------------------------------------------------
void ouverture3_ui8matrix_fusion_elu2_red(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// --------------------------------------------------------------------------------------------
{
    // TODO
}
// -------------------------------------------------------------------------------------------------
void ouverture3_ui8matrix_fusion_ilu5_elu2_red(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// -------------------------------------------------------------------------------------------------
{
    // TODO
}
// --------------------------------------------------------------------------------------------------------
void ouverture3_ui8matrix_fusion_ilu5_elu2_red_factor(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// --------------------------------------------------------------------------------------------------------
{
    // TODO
}
// ---------------------------------------------------------------------------------------------
void ouverture3_ui8matrix_fusion_ilu15_red(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// ---------------------------------------------------------------------------------------------
{
    // TODO
}
// ------------------------------------------------------------------------------------------------------
void ouverture3_ui8matrix_pipeline_basic(uint8 **X, int i0, int i1, int j0, int j1, uint8 **T, uint8 **Y)
// ------------------------------------------------------------------------------------------------------
{
    // PROLOGUE
    line_erosion3_ui8matrix_basic(X, i0, j0, j1, T);
   
    for(int i=i0; i<i1; i++) {
        line_erosion3_ui8matrix_basic(X, i+1, j0, j1, T);
        line_dilatation3_ui8matrix_basic(T, i, j0, j1, Y);
    }

    //EPILOGUE
    line_erosion3_ui8matrix_basic(X, i1, j0, j1, T);
    line_dilatation3_ui8matrix_basic(T, i1, j0, j1, Y);
    
}
// ----------------------------------------------------------------------------------------------------
void ouverture3_ui8matrix_pipeline_red(uint8 **X, int i0, int i1, int j0, int j1, uint8 **T, uint8 **Y)
// ----------------------------------------------------------------------------------------------------
{
    // PROLOGUE

    line_erosion3_ui8matrix_red(X, i0, j0, j1, T);
   
    for(int i=i0; i<i1; i++) {
        line_erosion3_ui8matrix_red(X, i+1, j0, j1, T);
        line_dilatation3_ui8matrix_red(T, i, j0, j1, Y);
    }

    // EPILOGUE
    line_erosion3_ui8matrix_red(X, i1, j0, j1, T);
    line_dilatation3_ui8matrix_red(T, i1, j0, j1, Y);

}
// ---------------------------------------------------------------------------------------------------------
void ouverture3_ui8matrix_pipeline_ilu3_red(uint8 **X, int i0, int i1, int j0, int j1, uint8 **T, uint8 **Y)
// ---------------------------------------------------------------------------------------------------------
{
    
    // PROLOGUE
    //line_erosion3_ui8matrix_ilu3_red(X, i0-1, j0, j1, T);
    line_erosion3_ui8matrix_ilu3_red(X, i0, j0, j1, T);
    

    for(int i=i0; i<i1; i++) {
        line_erosion3_ui8matrix_ilu3_red(X, i+1, j0, j1, T);
        line_dilatation3_ui8matrix_ilu3_red(T, i, j0, j1, Y);
    }

    // EPILOGUE 
    //line_erosion3_ui8matrix_ilu3_red(X, i1, j0, j1, T);
    line_dilatation3_ui8matrix_ilu3_red(T, i1, j0, j1, Y);
}
// ---------------------------------------------------------------------------------------------------------
void ouverture3_ui8matrix_pipeline_elu2_red(uint8 **X, int i0, int i1, int j0, int j1, uint8 **T, uint8 **Y)
// ---------------------------------------------------------------------------------------------------------
{
    int r = (i1 - i0 + 1) % 2;
    //PROLOGUE
    line_erosion3_ui8matrix_elu2_red(X, i0, j0, j1, T);
    
    for(int i=i0; i<=i1-r; i+=2) {
        line_erosion3_ui8matrix_elu2_red(X, i+1, j0, j1, T);
        line_dilatation3_ui8matrix_elu2_red(T, i, j0, j1, Y);
    }

    if (r) {
        line_erosion3_ui8matrix_elu2_red(X, i1, j0, j1, T);
        line_dilatation3_ui8matrix_elu2_red(T, i1-1, j0, j1, Y);
    }
}
// ----------------------------------------------------------------------------------------------------------------
void ouverture3_ui8matrix_pipeline_elu2_red_factor(uint8 **X, int i0, int i1, int j0, int j1, uint8 **T, uint8 **Y)
// ----------------------------------------------------------------------------------------------------------------
{
    int r = (i1 - i0 + 1) % 2;
    //PROLOGUE
    line_erosion3_ui8matrix_elu2_red_factor(X, i0, j0, j1, T);
    
    for(int i=i0; i<i1-r; i+=2) {
        line_erosion3_ui8matrix_elu2_red_factor(X, i+1, j0, j1, T);
        line_dilatation3_ui8matrix_elu2_red_factor(T, i, j0, j1, Y);
    }

    if (r) {
        line_erosion3_ui8matrix_elu2_red_factor(X, i1, j0, j1, T);
        line_dilatation3_ui8matrix_elu2_red_factor(T, i1-1, j0, j1, Y);
    }
}
// --------------------------------------------------------------------------------------------------------------
void ouverture3_ui8matrix_pipeline_ilu3_elu2_red(uint8 **X, int i0, int i1, int j0, int j1, uint8 **T, uint8 **Y)
// --------------------------------------------------------------------------------------------------------------
{
    int r = (i1 - i0 + 1) % 2;
    //PROLOGUE
    line_erosion3_ui8matrix_ilu3_elu2_red(X, i0, j0, j1, T);
    for(int i=i0; i<i1-r; i+=2) {
        line_erosion3_ui8matrix_ilu3_elu2_red(X, i+1, j0, j1, T);
        line_dilatation3_ui8matrix_ilu3_elu2_red(T, i, j0, j1, Y);
    }
   
    if (r) {
        line_erosion3_ui8matrix_ilu3_elu2_red(X, i1, j0, j1, T);
        line_dilatation3_ui8matrix_ilu3_elu2_red(T, i1-1, j0, j1, Y);
    } else { // en soit c'est l'epilogue
        line_erosion3_ui8matrix_basic(X, i1, j0, j1, T);
        line_dilatation3_ui8matrix_basic(T, i1, j0, j1, Y);
    }

}
// ---------------------------------------------------------------------------------------------------------------------
void ouverture3_ui8matrix_pipeline_ilu3_elu2_red_factor(uint8 **X, int i0, int i1, int j0, int j1, uint8 **T, uint8 **Y)
// ---------------------------------------------------------------------------------------------------------------------
{
    int r = (i1 - i0 + 1) % 2;
    //PROLOGUE
    line_erosion3_ui8matrix_ilu3_elu2_red_factor(X, i0, j0, j1, T);
    
    for(int i=i0; i<=i1-r; i+=2) {
        line_erosion3_ui8matrix_ilu3_elu2_red_factor(X, i+1, j0, j1, T);
        line_dilatation3_ui8matrix_ilu3_elu2_red_factor(T, i, j0, j1, Y);
    }

    if (r) {
        line_erosion3_ui8matrix_ilu3_elu2_red_factor(X, i1, j0, j1, T);
        line_dilatation3_ui8matrix_ilu3_elu2_red_factor(T, i1-1, j0, j1, Y);
    }
}
