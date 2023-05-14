/* -------------------------------- */
/* --- morpho_ouverture_swp64.c --- */
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
//#include "nrutil_ext.h" // printfM64

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
void line_ouverture3_ui64matrix_swp64_fusion_basic(uint64 **X, int i, int j0, int j1, uint64 **Y)
// ----------------------------------------------------------------------------------------------
{
    uint64 x01, x02, x03, x04, x05, x11, x12, x13, x14, x15, x21, x22, x23, x24, x25, x31, x32, x33, x34, x35, x41, x42, x43, x44, x45;
    uint64 t11, t12, t13, t21, t22, t23, t31, t32, t33, res; 
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

        t11 = ui64min9(ui64left1(x02, x01), x02, ui64right1(x02,x03), ui64left1(x12, x11), x12, ui64right1(x12,x13), ui64left1(x22, x21), x22, ui64right1(x22,x23));

        t12 = ui64min9(ui64left1(x03, x02), x03, ui64right1(x03,x04), ui64left1(x13, x12), x13, ui64right1(x13,x14), ui64left1(x23, x22), x23, ui64right1(x23,x24));

        t13 = ui64min9(ui64left1(x04, x03), x04, ui64right1(x04,x05), ui64left1(x14, x13), x14, ui64right1(x14,x15), ui64left1(x24, x23), x24, ui64right1(x24,x25));

        t21 = ui64min9(ui64left1(x12, x11), x12, ui64right1(x12,x13), ui64left1(x22, x21), x22, ui64right1(x22,x23), ui64left1(x32, x31), x32, ui64right1(x32,x33));

        t22 = ui64min9(ui64left1(x13, x12), x13, ui64right1(x13,x14), ui64left1(x23, x22), x23, ui64right1(x23,x24), ui64left1(x33, x32), x33, ui64right1(x33,x34));

        t23 = ui64min9(ui64left1(x14, x13), x14, ui64right1(x14,x15), ui64left1(x24, x23), x24, ui64right1(x24,x25), ui64left1(x34, x33), x34, ui64right1(x34,x35));


        t31 = ui64min9(ui64left1(x22, x21), x22, ui64right1(x22,x23), ui64left1(x32, x31), x32, ui64right1(x32,x33), ui64left1(x42, x41), x42, ui64right1(x42,x43));

        t32 = ui64min9(ui64left1(x23, x22), x23, ui64right1(x23,x24), ui64left1(x33, x32), x33, ui64right1(x33,x34), ui64left1(x43, x42), x43, ui64right1(x43,x44));

        t33 = ui64min9(ui64left1(x24, x23), x24, ui64right1(x24,x25), ui64left1(x34, x33), x34, ui64right1(x34,x35), ui64left1(x44, x43), x44, ui64right1(x44,x45));

        res = ui64max9(ui64left1(t12, t11), t12, ui64right1(t12,t13), ui64left1(t22, t21), t22, ui64right1(t22,t23), ui64left1(t32, t31), t32, ui64right1(t32,t33));
        
        store2(Y, i, j, res);
        
    }
}
// ---------------------------------------------------------------------------------------------
void line_ouverture3_ui64matrix_swp64_fusion_red0a(uint64 **X, int i, int j0, int j1, uint64 **Y)
// ---------------------------------------------------------------------------------------------
{
    uint64 x0, x1, x2, x3, x4;
    uint64 a1, a2, a3, a4, a5, b1, b2, b3, b4, b5, c1, c2, c3, c4, c5; //variables qui correspondent aux colonnes
    uint64 t11, t12, t13, t21, t22, t23, t31, t32, t33, res; 
    uint64 tmp;

    x0 = load2(X, i-2, j0-2);
    x1 = load2(X, i-1, j0-2);
    x2 = load2(X, i, j0-2);
    x3 = load2(X, i+1, j0-2);
    x4 = load2(X, i+2, j0-2);

    tmp = ui64min2(x1, x2);

    a1 = ui64min2(x0, tmp);
    b1 = ui64min2(x3, tmp);
    c1 = ui64min3(x2, x3, x4);

    x0 = load2(X, i-2, j0-1);
    x1 = load2(X, i-1, j0-1);
    x2 = load2(X, i, j0-1);
    x3 = load2(X, i+1, j0-1);
    x4 = load2(X, i+2, j0-1);

    tmp = ui64min2(x1, x2);

    a2 = ui64min2(x0, tmp);
    b2 = ui64min2(tmp, x3);
    c2 = ui64min3(x2, x3, x4);

    x0 = load2(X, i-2, j0);
    x1 = load2(X, i-1, j0);
    x2 = load2(X, i, j0);
    x3 = load2(X, i+1, j0);
    x4 = load2(X, i+2, j0); 

    tmp = ui64min2(x1, x2);

    a3 = ui64min2(x0, tmp);
    b3 = ui64min2(tmp, x3);
    c3 = ui64min3(x2, x3, x4); 

    x0 = load2(X, i-2, j0+1);
    x1 = load2(X, i-1, j0+1);
    x2 = load2(X, i, j0+1);
    x3 = load2(X, i+1, j0+1);
    x4 = load2(X, i+2, j0+1); 

    tmp = ui64min2(x1, x2);

    a4 = ui64min2(x0, tmp);
    b4 = ui64min2(tmp, x3);
    c4 = ui64min3(x2, x3, x4);


    for (int j=j0; j<=j1; j++) {

        t11 = ui64min3(ui64left1(a2, a1), a2, ui64right1(a2,a3));
        t21 = ui64min3(ui64left1(b2, b1), b2, ui64right1(b2,b3));
        t31 = ui64min3(ui64left1(c2, c1), c2, ui64right1(c2,c3));

        t12 = ui64min3(ui64left1(a3, a2), a3, ui64right1(a3,a4));
        t22 = ui64min3(ui64left1(b3, b2), b3, ui64right1(b3,b4));
        t32 = ui64min3(ui64left1(c3, c2), c3, ui64right1(c3,c4));

        //Factorisation 
        x0 = load2(X, i-2, j+2);
        x1 = load2(X, i-1, j+2);
        x2 = load2(X, i, j+2);
        x3 = load2(X, i+1, j+2);
        x4 = load2(X, i+2, j+2); 

        tmp = ui64min2(x1, x2);

        a5 = ui64min2(x0, tmp);
        b5 = ui64min2(tmp, x3);
        c5 = ui64min3(x2, x3, x4);

        t13 = ui64min3(ui64left1(a4, a3), a4, ui64right1(a4,a5));
        t23 = ui64min3(ui64left1(b4, b3), b4, ui64right1(b4,b5));
        t33 = ui64min3(ui64left1(c4, c3), c4, ui64right1(c4,c5));

        res = ui64max9(ui64left1(t12, t11), t12, ui64right1(t12,t13), ui64left1(t22, t21), t22, ui64right1(t22,t23), ui64left1(t32, t31), t32, ui64right1(t32,t33));

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
    }
}
// ---------------------------------------------------------------------------------------------
void line_ouverture3_ui64matrix_swp64_fusion_red0b(uint64 **X, int i, int j0, int j1, uint64 **Y)
// ---------------------------------------------------------------------------------------------
{
    // TODO
}
// ---------------------------------------------------------------------------------------------
void line_ouverture3_ui64matrix_swp64_fusion_red2(uint64 **X, int i, int j0, int j1, uint64 **Y)
// ---------------------------------------------------------------------------------------------
{
    // TODO
}
// -------------------------------------------------------------------------------------------------
void line_ouverture3_ui64matrix_swp64_fusion_ilu3_red1(uint64 **X, int i, int j0, int j1, uint64 **Y)
// -------------------------------------------------------------------------------------------------
{
    uint64 x01, x02, x03, x11, x12, x13, x21, x22, x23, x31, x32, x33, x41, x42, x43;
    uint64 a1, a2, a3, a4, a5, b1, b2, b3, b4, b5, c1, c2, c3, c4, c5; //variables qui correspondent aux colonnes
    uint64 t11, t12, t13, t21, t22, t23, t31, t32, t33, res; 

    x01 = load2(X, i-2, j0-2);
    x11 = load2(X, i-1, j0-2);
    x21 = load2(X, i, j0-2);
    x31 = load2(X, i+1, j0-2);
    x41 = load2(X, i+2, j0-2);

    a1 = ui64min3(x01, x11, x21);
    b1 = ui64min3(x11, x21, x31);
    c1 = ui64min3(x21, x31, x41);

    x02 = load2(X, i-2, j0-1);
    x12 = load2(X, i-1, j0-1);
    x22 = load2(X, i, j0-1);
    x32 = load2(X, i+1, j0-1);
    x42 = load2(X, i+2, j0-1);

    a2 = ui64min3(x02, x12, x22);
    b2 = ui64min3(x12, x22, x32);
    c2 = ui64min3(x22, x32, x42);

    x03 = load2(X, i-2, j0);
    x13 = load2(X, i-1, j0);
    x23 = load2(X, i, j0);
    x33 = load2(X, i+1, j0);
    x43 = load2(X, i+2, j0); 

    a3 = ui64min3(x03, x13, x23);
    b3 = ui64min3(x13, x23, x33);
    c3 = ui64min3(x23, x33, x43); 

    x03 = load2(X, i-2, j0+1);
    x13 = load2(X, i-1, j0+1);
    x23 = load2(X, i, j0+1);
    x33 = load2(X, i+1, j0+1);
    x43 = load2(X, i+2, j0+1); 

    a4 = ui64min3(x03, x13, x23);
    b4 = ui64min3(x13, x23, x33);
    c4 = ui64min3(x23, x33, x43); 


    int r = (j1 - j0 + 1) % 3;
    for (int j=j0; j<=j1-r-1; j+=3) {

        t11 = ui64min3(ui64left1(a2, a1), a2, ui64right1(a2,a3));
        t21 = ui64min3(ui64left1(b2, b1), b2, ui64right1(b2,b3));
        t31 = ui64min3(ui64left1(c2, c1), c2, ui64right1(c2,c3));

        t12 = ui64min3(ui64left1(a3, a2), a3, ui64right1(a3,a4));
        t22 = ui64min3(ui64left1(b3, b2), b3, ui64right1(b3,b4));
        t32 = ui64min3(ui64left1(c3, c2), c3, ui64right1(c3,c4));

        //Factorisation 
        x03 = load2(X, i-2, j+2);
        x13 = load2(X, i-1, j+2);
        x23 = load2(X, i, j+2);
        x33 = load2(X, i+1, j+2);
        x43 = load2(X, i+2, j+2); 

        a5 = ui64min3(x03, x13, x23);
        b5 = ui64min3(x13, x23, x33);
        c5 = ui64min3(x23, x33, x43); 

        t13 = ui64min3(ui64left1(a4, a3), a4, ui64right1(a4,a5));
        t23 = ui64min3(ui64left1(b4, b3), b4, ui64right1(b4,b5));
        t33 = ui64min3(ui64left1(c4, c3), c4, ui64right1(c4,c5));

        res = ui64max9(ui64left1(t12, t11), t12, ui64right1(t12,t13), ui64left1(t22, t21), t22, ui64right1(t22,t23), ui64left1(t32, t31), t32, ui64right1(t32,t33));

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

        t11 = ui64min3(ui64left1(a2, a1), a2, ui64right1(a2,a3));
        t21 = ui64min3(ui64left1(b2, b1), b2, ui64right1(b2,b3));
        t31 = ui64min3(ui64left1(c2, c1), c2, ui64right1(c2,c3));

        t12 = ui64min3(ui64left1(a3, a2), a3, ui64right1(a3,a4));
        t22 = ui64min3(ui64left1(b3, b2), b3, ui64right1(b3,b4));
        t32 = ui64min3(ui64left1(c3, c2), c3, ui64right1(c3,c4));

        x03 = load2(X, i-2, j+3);
        x13 = load2(X, i-1, j+3);
        x23 = load2(X, i, j+3);
        x33 = load2(X, i+1, j+3);
        x43 = load2(X, i+2, j+3); 

        a5 = ui64min3(x03, x13, x23);
        b5 = ui64min3(x13, x23, x33);
        c5 = ui64min3(x23, x33, x43); 

        t13 = ui64min3(ui64left1(a4, a3), a4, ui64right1(a4,a5));
        t23 = ui64min3(ui64left1(b4, b3), b4, ui64right1(b4,b5));
        t33 = ui64min3(ui64left1(c4, c3), c4, ui64right1(c4,c5));

        res = ui64max9(ui64left1(t12, t11), t12, ui64right1(t12,t13), ui64left1(t22, t21), t22, ui64right1(t22,t23), ui64left1(t32, t31), t32, ui64right1(t32,t33));

        store2(Y, i, j+1, res);

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

        t11 = ui64min3(ui64left1(a2, a1), a2, ui64right1(a2,a3));
        t21 = ui64min3(ui64left1(b2, b1), b2, ui64right1(b2,b3));
        t31 = ui64min3(ui64left1(c2, c1), c2, ui64right1(c2,c3));

        t12 = ui64min3(ui64left1(a3, a2), a3, ui64right1(a3,a4));
        t22 = ui64min3(ui64left1(b3, b2), b3, ui64right1(b3,b4));
        t32 = ui64min3(ui64left1(c3, c2), c3, ui64right1(c3,c4));

        x03 = load2(X, i-2, j+4);
        x13 = load2(X, i-1, j+4);
        x23 = load2(X, i, j+4);
        x33 = load2(X, i+1, j+4);
        x43 = load2(X, i+2, j+4); 

        a5 = ui64min3(x03, x13, x23);
        b5 = ui64min3(x13, x23, x33);
        c5 = ui64min3(x23, x33, x43); 

        t13 = ui64min3(ui64left1(a4, a3), a4, ui64right1(a4,a5));
        t23 = ui64min3(ui64left1(b4, b3), b4, ui64right1(b4,b5));
        t33 = ui64min3(ui64left1(c4, c3), c4, ui64right1(c4,c5));

        res = ui64max9(ui64left1(t12, t11), t12, ui64right1(t12,t13), ui64left1(t22, t21), t22, ui64right1(t22,t23), ui64left1(t32, t31), t32, ui64right1(t32,t33));

        store2(Y, i, j+2, res);

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

    }
    if (r) {
        uint64 x04, x05, x14, x15, x24, x25, x34, x35, x44, x45; 
        for (int j=j1-r-1; j <= j1; j++) {

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

            t11 = ui64min9(ui64left1(x02, x01), x02, ui64right1(x02,x03), ui64left1(x12, x11), x12, ui64right1(x12,x13), ui64left1(x22, x21), x22, ui64right1(x22,x23));
            t12 = ui64min9(ui64left1(x03, x02), x03, ui64right1(x03,x04), ui64left1(x13, x12), x13, ui64right1(x13,x14), ui64left1(x23, x22), x23, ui64right1(x23,x24));
            t13 = ui64min9(ui64left1(x04, x03), x04, ui64right1(x04,x05), ui64left1(x14, x13), x14, ui64right1(x14,x15), ui64left1(x24, x23), x24, ui64right1(x24,x25));

            t21 = ui64min9(ui64left1(x12, x11), x12, ui64right1(x12,x13), ui64left1(x22, x21), x22, ui64right1(x22,x23), ui64left1(x32, x31), x32, ui64right1(x32,x33));
            t22 = ui64min9(ui64left1(x13, x12), x13, ui64right1(x13,x14), ui64left1(x23, x22), x23, ui64right1(x23,x24), ui64left1(x33, x32), x33, ui64right1(x33,x34));
            t23 = ui64min9(ui64left1(x14, x13), x14, ui64right1(x14,x15), ui64left1(x24, x23), x24, ui64right1(x24,x25), ui64left1(x34, x33), x34, ui64right1(x34,x35));

            t31 = ui64min9(ui64left1(x22, x21), x22, ui64right1(x22,x23), ui64left1(x32, x31), x32, ui64right1(x32,x33), ui64left1(x42, x41), x42, ui64right1(x42,x43));
            t32 = ui64min9(ui64left1(x23, x22), x23, ui64right1(x23,x24), ui64left1(x33, x32), x33, ui64right1(x33,x34), ui64left1(x43, x42), x43, ui64right1(x43,x44));
            t33 = ui64min9(ui64left1(x24, x23), x24, ui64right1(x24,x25), ui64left1(x34, x33), x34, ui64right1(x34,x35), ui64left1(x44, x43), x44, ui64right1(x44,x45));

            res = ui64max9(ui64left1(t12, t11), t12, ui64right1(t12,t13), ui64left1(t22, t21), t22, ui64right1(t22,t23), ui64left1(t32, t31), t32, ui64right1(t32,t33));
            
            store2(Y, i, j, res); 
        }
    }
}
// -------------------------------------------------------------------------------------------------
void line_ouverture3_ui64matrix_swp64_fusion_elu2_red0(uint64 **X, int i, int j0, int j1, uint64 **Y)
// -------------------------------------------------------------------------------------------------
{
    // TODO
}
// -------------------------------------------------------------------------------------------------------
void line_ouverture3_ui64matrix_swp64_fusion_ilu3_elu2_red1(uint64 **X, int i, int j0, int j1, uint64 **Y)
// -------------------------------------------------------------------------------------------------------
{
    // TODO
}

// --------------------------------------------------------------------------------------------------------------
void line_ouverture3_ui64matrix_swp64_fusion_ilu3_elu2_red1_factor(uint64 **X, int i, int j0, int j1, uint64 **Y)
// --------------------------------------------------------------------------------------------------------------
{
    // TODO
}
// -------------------------------------------------------------------------------------------------------
void ouverture3_ui64matrix_swp64_basic(uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y, uint64 **Z)
// -------------------------------------------------------------------------------------------------------
{
    
    //printf("i0 = %d\n", i0);
    erosion3_ui64matrix_swp64_basic   (X, i0-1, i1+1 , j0, j1, Y);
    dilatation3_ui64matrix_swp64_basic(Y, i0,   i1,   j0,   j1,   Z);
}
// --------------------------------------------------------------------------------------------------
void ouverture3_ui64matrix_swp64_fusion_basic(uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y)
// --------------------------------------------------------------------------------------------------
{
    for(int i=i0; i<=i1; i++) {
        line_ouverture3_ui64matrix_swp64_fusion_basic(X, i, j0, j1, Y);
    }
}
// -------------------------------------------------------------------------------------------------
void ouverture3_ui64matrix_swp64_fusion_red(uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y)
// -------------------------------------------------------------------------------------------------
{
    for(int i=i0; i<=i1; i++) {
        line_ouverture3_ui64matrix_swp64_fusion_red0a(X, i, j0, j1, Y);
    }
}
// -----------------------------------------------------------------------------------------------------
void ouverture3_ui64matrix_swp64_fusion_ilu3_red(uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y)
// -----------------------------------------------------------------------------------------------------
{
    // for(int i=i0; i<=i1; i++) {
    //     line_ouverture3_ui64matrix_swp64_fusion_ilu3_red1(X, i, j0, j1, Y);
    // }
}
// -----------------------------------------------------------------------------------------------------
void ouverture3_ui64matrix_swp64_fusion_elu2_red(uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y)
// -----------------------------------------------------------------------------------------------------
{
    // TODO
}
// ----------------------------------------------------------------------------------------------------------
void ouverture3_ui64matrix_swp64_fusion_ilu3_elu2_red(uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y)
// ----------------------------------------------------------------------------------------------------------
{
    // TODO
}
// -----------------------------------------------------------------------------------------------------------------
void ouverture3_ui64matrix_swp64_fusion_ilu3_elu2_red_factor(uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y)
// -----------------------------------------------------------------------------------------------------------------
{
    // TODO
}
// ----------------------------------------------------------------------------------------------------------------
void ouverture3_ui64matrix_swp64_pipeline_basic(uint64 **X, int i0, int i1, int j0, int j1, uint64 **T, uint64 **Y)
// ----------------------------------------------------------------------------------------------------------------
{
    // PROLOGUE

    line_erosion3_ui64matrix_swp64_basic(X, i0, j0, j1, T);

    for(int i=i0; i<i1; i++) {
        line_erosion3_ui64matrix_swp64_basic(X, i+1, j0, j1, T);
        line_dilatation3_ui64matrix_swp64_basic(T, i, j0, j1, Y);
    }

    //EPILOGUE
    line_erosion3_ui64matrix_swp64_basic(X, i1, j0, j1, T);
    line_dilatation3_ui64matrix_swp64_basic(T, i1, j0, j1, Y);
}
// --------------------------------------------------------------------------------------------------------------
void ouverture3_ui64matrix_swp64_pipeline_red(uint64 **X, int i0, int i1, int j0, int j1, uint64 **T, uint64 **Y)
// --------------------------------------------------------------------------------------------------------------
{
    // PROLOGUE

    line_erosion3_ui64matrix_swp64_red(X, i0, j0, j1, T);
   
    for(int i=i0; i<i1; i++) {
        line_erosion3_ui64matrix_swp64_red(X, i+1, j0, j1, T);
        line_dilatation3_ui64matrix_swp64_red(T, i, j0, j1, Y);
    }

    // EPILOGUE
    line_erosion3_ui64matrix_swp64_red(X, i1, j0, j1, T);
    line_dilatation3_ui64matrix_swp64_red(T, i1, j0, j1, Y);
}
// -------------------------------------------------------------------------------------------------------------------
void ouverture3_ui64matrix_swp64_pipeline_ilu3_red(uint64 **X, int i0, int i1, int j0, int j1, uint64 **T, uint64 **Y)
// -------------------------------------------------------------------------------------------------------------------
{
    // PROLOGUE
    line_erosion3_ui64matrix_swp64_ilu3_red(X, i0-1, j0, j1, T);
    line_erosion3_ui64matrix_swp64_ilu3_red(X, i0, j0, j1, T);
    for(int i=i0; i<i1; i++) {
        line_erosion3_ui64matrix_swp64_ilu3_red(X, i+1, j0, j1, T);
        line_dilatation3_ui64matrix_swp64_ilu3_red(T, i, j0, j1, Y);
    }

    // EPILOGUE 
    line_erosion3_ui64matrix_swp64_ilu3_red(X, i1+1, j0, j1, T);
    line_dilatation3_ui64matrix_swp64_ilu3_red(T, i1, j0, j1, Y);
}
// -------------------------------------------------------------------------------------------------------------------
void ouverture3_ui64matrix_swp64_pipeline_elu2_red(uint64 **X, int i0, int i1, int j0, int j1, uint64 **T, uint64 **Y)
// -------------------------------------------------------------------------------------------------------------------
{
    int r = (i1 - i0 + 1) % 2;
    //PROLOGUE
    line_erosion3_ui64matrix_swp64_elu2_red(X, i0, j0, j1, T);
    
    for(int i=i0; i<=i1-r; i+=2) {
        line_erosion3_ui64matrix_swp64_elu2_red(X, i+1, j0, j1, T);
        line_dilatation3_ui64matrix_swp64_elu2_red(T, i, j0, j1, Y);
    }

    if (r) {
        line_erosion3_ui64matrix_swp64_elu2_red(X, i1, j0, j1, T);
        line_dilatation3_ui64matrix_swp64_elu2_red(T, i1-1, j0, j1, Y);
    }
}
// --------------------------------------------------------------------------------------------------------------------------
void ouverture3_ui64matrix_swp64_pipeline_elu2_red_factor(uint64 **X, int i0, int i1, int j0, int j1, uint64 **T, uint64 **Y)
// --------------------------------------------------------------------------------------------------------------------------
{
    int r = (i1 - i0 + 1) % 2;
    //PROLOGUE
    line_erosion3_ui64matrix_swp64_elu2_red_factor(X, i0, j0, j1, T);
    
    for(int i=i0; i<=i1-r; i+=2) {
        line_erosion3_ui64matrix_swp64_elu2_red_factor(X, i+1, j0, j1, T);
        line_dilatation3_ui64matrix_swp64_elu2_red_factor(T, i, j0, j1, Y);
    }

    if (r) {
        line_erosion3_ui64matrix_swp64_elu2_red_factor(X, i1, j0, j1, T);
        line_dilatation3_ui64matrix_swp64_elu2_red_factor(T, i1-1, j0, j1, Y);
    }
}
// ------------------------------------------------------------------------------------------------------------------------
void ouverture3_ui64matrix_swp64_pipeline_ilu3_elu2_red(uint64 **X, int i0, int i1, int j0, int j1, uint64 **T, uint64 **Y)
// ------------------------------------------------------------------------------------------------------------------------
{
    int r = (i1 - i0 + 1) % 2;
    //PROLOGUE
    line_erosion3_ui64matrix_swp64_ilu3_elu2_red(X, i0, j0, j1, T);
    for(int i=i0; i<i1-r; i+=2) {
        line_erosion3_ui64matrix_swp64_ilu3_elu2_red(X, i+1, j0, j1, T);
        line_dilatation3_ui64matrix_swp64_ilu3_elu2_red(T, i, j0, j1, Y);
    }
   
    if (r) {
        line_erosion3_ui64matrix_swp64_ilu3_elu2_red(X, i1, j0, j1, T);
        line_dilatation3_ui64matrix_swp64_ilu3_elu2_red(T, i1-1, j0, j1, Y);
    } else { // en soit c'est l'epilogue
        line_erosion3_ui64matrix_swp64_basic(X, i1, j0, j1, T);
        line_dilatation3_ui64matrix_swp64_basic(T, i1, j0, j1, Y);
    }
}
// -------------------------------------------------------------------------------------------------------------------------------
void ouverture3_ui64matrix_swp64_pipeline_ilu3_elu2_red_factor(uint64 **X, int i0, int i1, int j0, int j1, uint64 **T, uint64 **Y)
// -------------------------------------------------------------------------------------------------------------------------------
{
    int r = (i1 - i0 + 1) % 2;
    //PROLOGUE
    line_erosion3_ui64matrix_swp64_ilu3_elu2_red(X, i0, j0, j1, T);
    for(int i=i0; i<i1-r; i+=2) {
        line_erosion3_ui64matrix_swp64_ilu3_elu2_red(X, i+1, j0, j1, T);
        line_dilatation3_ui64matrix_swp64_ilu3_elu2_red(T, i, j0, j1, Y);
    }
   
    if (r) {
        line_erosion3_ui64matrix_swp64_ilu3_elu2_red(X, i1, j0, j1, T);
        line_dilatation3_ui64matrix_swp64_ilu3_elu2_red(T, i1-1, j0, j1, Y);
    } else { // en soit c'est l'epilogue
        line_erosion3_ui64matrix_swp64_basic(X, i1, j0, j1, T);
        line_dilatation3_ui64matrix_swp64_basic(T, i1, j0, j1, Y);
    }
}
