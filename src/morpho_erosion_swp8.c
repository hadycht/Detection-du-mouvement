/* ----------------------------- */
/* --- morpho_erosion_swp8.c --- */
/* ----------------------------- */

/*
 * Copyright1 (c) 2020 - 2021, Lionel Lacassagne, All right1s reserved
 * LIP6 - Sorbonne University
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
#include "morpho_erosion_swp8.h"

// ---------------------------------------------------------------------------------
void line_erosion3_ui8matrix_swp8_basic(uint8 **X, int i, int j0, int j1, uint8 **Y)
// ---------------------------------------------------------------------------------
{
    
    for (int j=j0; j<=j1; j++) {
        
        uint8 a1 = load2(X, i-1, j-1);
        uint8 a2 = load2(X, i-1, j);
        uint8 a3 = load2(X, i-1, j+1);

        uint8 b1 = load2(X, i, j-1);
        uint8 b2 = load2(X, i, j);
        uint8 b3 = load2(X, i, j+1);

        uint8 c1 = load2(X, i+1, j-1);
        uint8 c2 = load2(X, i+1, j);
        uint8 c3 = load2(X, i+1, j+1);

        uint8 y = ui8min9(ui8left1(a2, a1), a2, ui8right1(a2, a3),ui8left1(b2, b1), b2, ui8right1(b2, b3),ui8left1(c2, c1), c2, ui8right1(c2, c3));
        
        store2(Y, i, j, y);
    }
    //printf("\n");
}
// -------------------------------------------------------------------------------
void line_erosion3_ui8matrix_swp8_reg(uint8 **X, int i, int j0, int j1, uint8 **Y)
// -------------------------------------------------------------------------------
{
    for (int j=j0; j<=j1; j++) {
        
        uint8 a1 = load2(X, i-1, j-1);
        uint8 a2 = load2(X, i-1, j);
        uint8 a3 = load2(X, i-1, j+1);

        uint8 b1 = load2(X, i, j-1);
        uint8 b2 = load2(X, i, j);
        uint8 b3 = load2(X, i, j+1);

        uint8 c1 = load2(X, i+1, j-1);
        uint8 c2 = load2(X, i+1, j);
        uint8 c3 = load2(X, i+1, j+1);

        uint8 z = ui8left1(a2, a1);
        uint8 x = ui8right1(a2, a3);
        uint8 t = ui8left1(b2, b1);
        uint8 u = ui8right1(b2, b3);
        uint8 v = ui8left1(c2, c1);
        uint8 w = ui8right1(c2, c3); 
        

        uint8 y = ui8min9(z, a2, x, t, b2, u, v, c2, w );
        
        store2(Y, i, j, y);
    }
}
// -------------------------------------------------------------------------------
void line_erosion3_ui8matrix_swp8_rot(uint8 **X, int i, int j0, int j1, uint8 **Y)
// -------------------------------------------------------------------------------
{
    uint8 a1, a2, a3, b1, b2, b3, c1, c2, c3, x1, x2, y1, y2, z1, z2, res; 

    a1 = load2(X, i-1, j0-1);
    b1 = load2(X, i, j0-1);
    c1 = load2(X, i+1, j0-1);
    
    a2 = load2(X, i-1, j0);
    b2 = load2(X, i, j0);
    c2 = load2(X, i+1,j0); 

    for (int j=j0+1; j<=j1+1; j++) {

        a3 = load2(X, i-1, j);
        b3 = load2(X, i, j);
        c3 = load2(X, i+1, j);

        x1 = ui8left1(a2, a1);
        x2 = ui8right1(a2, a3);

        y1 = ui8left1(b2, b1);
        y2 = ui8right1(b2, b3);

        z1 = ui8left1(c2, c1);
        z2 = ui8right1(c2, c3);
        
        res = ui8min9(a2,x1,x2,b2,y1,y2,c2,z1,z2);
        store2(Y, i, j-1, res);

        a1 = a2;
        b1 = b2;
        c1 = c2;

        a2 = a3;
        b2 = b3;
        c2 = c3;
    }


}
// -------------------------------------------------------------------------------
void line_erosion3_ui8matrix_swp8_red(uint8 **X, int i, int j0, int j1, uint8 **Y)
// -------------------------------------------------------------------------------
{
    uint8 a1, a2, a3, b1, b2, b3, c1, c2, c3, x1, x2, y1, y2, z1, z2, res1, res2, res3, res; 
    
    a1 = load2(X, i-1, j0-1);
    b1 = load2(X, i, j0-1);
    c1 = load2(X, i+1, j0-1);
    
    a2 = load2(X, i-1, j0);
    b2 = load2(X, i, j0);
    c2 = load2(X, i+1,j0);

    res1 = ui8min3(a1, b1, c1);
    res2 = ui8min3(a2, b2, c2);


    for (int j=j0+1; j<=j1+1; j++) {

        x1 = ui8left1(res2, res1);

        a3 = load2(X, i-1, j);
        b3 = load2(X, i, j);
        c3 = load2(X, i+1, j); 

        res3 = ui8min3(a3, b3, c3);

        y1 = ui8right1(res2, res3);

        res = ui8min3(res2, x1, y1);

        store2(Y, i, j-1, res);

        res1 = res2;
        res2 = res3;

    }

    // x1 = ui8left1(a2, a1);

    // y1 = ui8left1(b2, b1);

    // z1 = ui8left1(c2, c1);

    // res1 = ui8min3(x1, y1, z1);
    // res2 = ui8min3(a2, b2, c2);

    // for (int j=j0+1; j<=j1+1; j++) {

    //     a3 = load2(X, i-1, j);
    //     b3 = load2(X, i, j);
    //     c3 = load2(X, i+1, j); 

    //     x2 = ui8right1(a2, a3);
    //     y2 = ui8right1(b2, b3);
    //     z2 = ui8right1(c2, c3);

    //     res3 = ui8min3(x2, y2, z2);
    //     res = ui8min3(res1, res2, res3);

    //     store2(Y, i, j-1, res);

    //     res1 = ui8min3(ui8left1(a3, a2), ui8left1(b3, b2), ui8left1(c3, c2)) ;
    //     res2 = ui8min3(a3, b3, c3);

    //     a2 = a3;
    //     b2 = b3;
    //     c2 = c3;
    // }


}
// --------------------------------------------------------------------------------
void line_erosion3_ui8matrix_swp8_ilu3(uint8 **X, int i, int j0, int j1, uint8 **Y)
// --------------------------------------------------------------------------------
{
    uint8 a1, a2, a3, a4, a5, b1, b2, b3, b4, b5, c1, c2, c3, c4, c5, x1, x2, y1, y2, z1, z2, res; 

    int r = (j1 - j0 + 1)%3;
    for (int j = j0; j<=j1-r-1; j+=3) {

        a1 = load2(X, i-1, j-1);
        a2 = load2(X, i-1, j);
        a3 = load2(X, i-1, j+1);
        a4 = load2(X, i-1, j+2);
        a5 = load2(X, i-1, j+3);
        
        b1 = load2(X, i, j-1);
        b2 = load2(X, i, j);
        b3 = load2(X, i, j+1);
        b4 = load2(X, i, j+2);
        b5 = load2(X, i, j+3);

        c1 = load2(X, i+1, j-1);
        c2 = load2(X, i+1, j);
        c3 = load2(X, i+1, j+1);
        c4 = load2(X, i+1, j+2);
        c5 = load2(X, i+1, j+3);

        x1 = ui8left1(a2, a1);
        x2 = ui8right1(a2, a3);

        y1 = ui8left1(b2, b1);
        y2 = ui8right1(b2, b3);

        z1 = ui8left1(c2, c1);
        z2 = ui8right1(c2, c3);
        
        res = ui8min9(a2,x1,x2,b2,y1,y2,c2,z1,z2);
        store2(Y, i, j, res); 

        x1 = ui8left1(a3, a2);
        x2 = ui8right1(a3, a4);

        y1 = ui8left1(b3, b2);
        y2 = ui8right1(b3, b4);

        z1 = ui8left1(c3, c2);
        z2 = ui8right1(c3, c4);
        
        res = ui8min9(a3,x1,x2,b3,y1,y2,c3,z1,z2);
        store2(Y, i, j+1, res);

        x1 = ui8left1(a4, a3);
        x2 = ui8right1(a4, a5);

        y1 = ui8left1(b4, b3);
        y2 = ui8right1(b4, b5);

        z1 = ui8left1(c4, c3);
        z2 = ui8right1(c4, c5);
        
        res = ui8min9(a4,x1,x2,b4,y1,y2,c4,z1,z2);
        store2(Y, i, j+2, res);
    }
    if (r) {
        for (int j=max(j0,j1-r); j <= j1; j++) {
            a1 = load2(X, i-1, j-1);
            a2 = load2(X, i-1, j);
            a3 = load2(X, i-1, j+1);

            b1 = load2(X, i, j-1);
            b2 = load2(X, i, j);
            b3 = load2(X, i, j+1);

            c1 = load2(X, i+1, j-1);
            c2 = load2(X, i+1, j);
            c3 = load2(X, i+1, j+1);

            x1 = ui8left1(a2, a1);
            x2 = ui8right1(a2, a3);

            y1 = ui8left1(b2, b1);
            y2 = ui8right1(b2, b3);

            z1 = ui8left1(c2, c1);
            z2 = ui8right1(c2, c3);
            
            res = ui8min9(a2,x1,x2,b2,y1,y2,c2,z1,z2);
            store2(Y, i, j, res); 
        }
    }
}
// ------------------------------------------------------------------------------------
void line_erosion3_ui8matrix_swp8_ilu3_red(uint8 **X, int i, int j0, int j1, uint8 **Y)
// ------------------------------------------------------------------------------------
{
    uint8 a, b, c, x, y, res1, res2, res3, res; 
    
    a = load2(X, i-1, j0-1);
    b = load2(X, i, j0-1);
    c = load2(X, i+1, j0-1);
    
    res1 = ui8min3(a, b, c);

    a = load2(X, i-1, j0);
    b = load2(X, i, j0);
    c = load2(X, i+1,j0);

    res2 = ui8min3(a, b, c);

    int r = (j1 - j0 + 1)%3;

    for (int j = j0; j<=j1-r-1; j+=3) {

        x = ui8left1(res2, res1);

        a = load2(X, i-1, j+1);
        b = load2(X, i, j+1);
        c = load2(X, i+1, j+1); 

        res3 = ui8min3(a, b, c);

        y = ui8right1(res2, res3);

        res = ui8min3(res2, x, y);

        store2(Y, i, j, res);

        res1 = res2;
        res2 = res3; 

        x = ui8left1(res2, res1);
    
        a = load2(X, i-1, j+2);
        b = load2(X, i, j+2);
        c = load2(X, i+1, j+2);

        res3 = ui8min3(a, b, c);

        y = ui8right1(res2, res3);

        res = ui8min3(res2, x, y);

        store2(Y, i, j+1, res);

        res1 = res2;
        res2 = res3; 

        x = ui8left1(res2, res1);
    
        a = load2(X, i-1, j+3);
        b = load2(X, i, j+3);
        c = load2(X, i+1, j+3);

        res3 = ui8min3(a, b, c);

        y = ui8right1(res2, res3);

        res = ui8min3(res2, x, y);

        store2(Y, i, j+2, res);

        res1 = res2;
        res2 = res3;

    }

    if (r) {
        for (int j=max(j0,j1-r); j <= j1; j++) {
            a = load2(X, i-1, j-1);
            b = load2(X, i, j-1);
            c = load2(X, i+1, j-1);

            res1 = ui8min3(a, b, c);

            a = load2(X, i-1, j);
            b = load2(X, i, j);
            c = load2(X, i+1, j);

            res2 = ui8min3(a, b, c);

            a = load2(X, i-1, j+1);
            b = load2(X, i, j+1);
            c = load2(X, i+1, j+1);

            res3 = ui8min3(a, b, c);

            x = ui8left1(res2, res1);
            y = ui8right1(res2, res3);
        
            res = ui8min3(x, res2, y);
            store2(Y, i, j, res); 
        }
    }
}
// ------------------------------------------------------------------------------------
void line_erosion3_ui8matrix_swp8_elu2_red(uint8 **X, int i, int j0, int j1, uint8 **Y)
// ------------------------------------------------------------------------------------
{
    uint8 a, b, c, d, x, y, ca1, ca2, cb1, cb2, cc1, cc2, res; 
    
    a = load2(X, i-1, j0-1);
    b = load2(X, i, j0-1);
    c = load2(X, i+1, j0-1);

    d = load2(X, i+2, j0-1);
    
    ca1 = ui8min3(a, b, c);
    ca2 = ui8min3(b, c, d);

    a = load2(X, i-1, j0);
    b = load2(X, i, j0);
    c = load2(X, i+1,j0);

    d = load2(X, i+2, j0);

    cb1 = ui8min3(a, b, c);
    cb2 = ui8min3(b, c, d);

    for (int j=j0; j<=j1; j++) {

        a = load2(X, i-1, j+1);
        b = load2(X, i, j+1);
        c = load2(X, i+1, j+1);

        d = load2(X, i+2, j+1);

        cc1 = ui8min3(a, b, c);
        cc2 = ui8min3(b, c, d);

        x = ui8left1(cb1, ca1);
        y = ui8right1(cb1, cc1);
        
        res = ui8min3(x, cb1, y);
        store2(Y, i, j, res); 

        x = ui8left1(cb2, ca2);
        y = ui8right1(cb2, cc2);
        
        res = ui8min3(x, cb2, y);
        store2(Y, i+1, j, res);

        ca1 = cb1;
        ca2 = cb2; 

        cb1 = cc1;
        cb2 = cc2;

    }
}
// -------------------------------------------------------------------------------------------
void line_erosion3_ui8matrix_swp8_elu2_red_factor(uint8 **X, int i, int j0, int j1, uint8 **Y)
// -------------------------------------------------------------------------------------------
{
    uint8 a, b, c, d, x, y, ca1, ca2, cb1, cb2, cc1, cc2, res; 
    
    a = load2(X, i-1, j0-1);
    b = load2(X, i, j0-1);
    c = load2(X, i+1, j0-1);

    d = load2(X, i+2, j0-1);
    
    ca1 = ui8min3(a, b, c);
    ca2 = ui8min3(b, c, d);

    a = load2(X, i-1, j0);
    b = load2(X, i, j0);
    c = load2(X, i+1,j0);

    d = load2(X, i+2, j0);

    cb1 = ui8min3(a, b, c);
    cb2 = ui8min3(b, c, d);

    for (int j=j0; j<=j1; j++) {

        a = load2(X, i-1, j+1);
        b = load2(X, i, j+1);
        c = load2(X, i+1, j+1);

        d = load2(X, i+2, j+1);

        cc1 = ui8min3(a, b, c);
        cc2 = ui8min3(b, c, d);

        x = ui8left1(cb1, ca1);
        y = ui8right1(cb1, cc1);
        
        res = ui8min3(x, cb1, y);
        store2(Y, i, j, res); 

        x = ui8left1(cb2, ca2);
        y = ui8right1(cb2, cc2);
        
        res = ui8min3(x, cb2, y);
        store2(Y, i+1, j, res);

        ca1 = cb1;
        ca2 = cb2; 

        cb1 = cc1;
        cb2 = cc2;

    }
}
// -----------------------------------------------------------------------------------------
void line_erosion3_ui8matrix_swp8_ilu3_elu2_red(uint8 **X, int i, int j0, int j1, uint8 **Y)
// -----------------------------------------------------------------------------------------
{
    uint8 a, b, c, d, x, y, ca1, ca2, cb1, cb2, cc1, cc2, res; 

    a = load2(X, i-1, j0-1);
    b = load2(X, i, j0-1);
    c = load2(X, i+1, j0-1);

    d = load2(X, i+2, j0-1);
    
    ca1 = ui8min3(a, b, c);
    ca2 = ui8min3(b, c, d);

    a = load2(X, i-1, j0);
    b = load2(X, i, j0);
    c = load2(X, i+1,j0);

    d = load2(X, i+2, j0);

    cb1 = ui8min3(a, b, c);
    cb2 = ui8min3(b, c, d);

    int r = (j1 - j0 + 1)%3;
    
    for (int j=j0; j<=j1-r-1;j+=3) {
        

        a = load2(X, i-1, j+1);
        b = load2(X, i, j+1);
        c = load2(X, i+1, j+1);

        d = load2(X, i+2, j+1);

        cc1 = ui8min3(a, b, c);
        cc2 = ui8min3(b, c, d);

        x = ui8left1(cb1, ca1);
        y = ui8right1(cb1, cc1);
        
        res = ui8min3(x, cb1, y);
        store2(Y, i, j, res); 

        x = ui8left1(cb2, ca2);
        y = ui8right1(cb2, cc2);
        
        res = ui8min3(x, cb2, y);
        store2(Y, i+1, j, res);

        ca1 = cb1;
        ca2 = cb2; 

        cb1 = cc1;
        cb2 = cc2;

        a = load2(X, i-1, j+2);
        b = load2(X, i, j+2);
        c = load2(X, i+1, j+2);

        d = load2(X, i+2, j+2);

        cc1 = ui8min3(a, b, c);
        cc2 = ui8min3(b, c, d);

        x = ui8left1(cb1, ca1);
        y = ui8right1(cb1, cc1);
        
        res = ui8min3(x, cb1, y);
        store2(Y, i, j+1, res); 

        x = ui8left1(cb2, ca2);
        y = ui8right1(cb2, cc2);
        
        res = ui8min3(x, cb2, y);
        store2(Y, i+1, j+1, res);

        ca1 = cb1;
        ca2 = cb2; 

        cb1 = cc1;
        cb2 = cc2;

        a = load2(X, i-1, j+3);
        b = load2(X, i, j+3);
        c = load2(X, i+1, j+3);

        d = load2(X, i+2, j+3);

        cc1 = ui8min3(a, b, c);
        cc2 = ui8min3(b, c, d);

        x = ui8left1(cb1, ca1);
        y = ui8right1(cb1, cc1);
        
        res = ui8min3(x, cb1, y);
        store2(Y, i, j+2, res); 

        x = ui8left1(cb2, ca2);
        y = ui8right1(cb2, cc2);
        
        res = ui8min3(x, cb2, y);
        store2(Y, i+1, j+2, res);

        ca1 = cb1;
        ca2 = cb2; 

        cb1 = cc1;
        cb2 = cc2;
    }

    if (r) {
        for (int j=max(j0,j1-r); j <= j1; j++) {
            a = load2(X, i-1, j-1);
            b = load2(X, i, j-1);
            c = load2(X, i+1, j-1); 

            d = load2(X, i+2, j-1);

            ca1 = ui8min3(a, b, c);
            ca2 = ui8min3(b, c, d);

            a = load2(X, i-1, j);
            b = load2(X, i, j);
            c = load2(X, i+1, j);

            d = load2(X, i+2, j);
            
            cb1 = ui8min3(a, b, c);
            cb2 = ui8min3(b, c, d);

            a = load2(X, i-1, j+1);
            b = load2(X, i, j+1);
            c = load2(X, i+1, j+1);

            d = load2(X, i+2, j+1);

            cc1 = ui8min3(a, b, c);
            cc2 = ui8min3(b, c, d);

            x = ui8left1(cb1, ca1);
            y = ui8right1(cb1, cc1);
        
            res = ui8min3(x, cb1, y);
            store2(Y, i, j, res); 

            x = ui8left1(cb2, ca2);
            y = ui8right1(cb2, cc2);
        
            res = ui8min3(x, cb2, y);
            store2(Y, i+1, j, res);
        }
    }
}
// ------------------------------------------------------------------------------------------------
void line_erosion3_ui8matrix_swp8_ilu3_elu2_red_factor(uint8 **X, int i, int j0, int j1, uint8 **Y)
// ------------------------------------------------------------------------------------------------
{
    // TODO
}

// --------------------------------------------------------------------------------------
void erosion3_ui8matrix_swp8_basic(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// --------------------------------------------------------------------------------------
{
    for(int i=i0; i<=i1; i++) {
        line_erosion3_ui8matrix_swp8_basic(X, i, j0, j1, Y);
    }
}
// ------------------------------------------------------------------------------------
void erosion3_ui8matrix_swp8_reg(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// ------------------------------------------------------------------------------------
{
    for(int i=i0; i<=i1; i++) {
        line_erosion3_ui8matrix_swp8_reg(X, i, j0, j1, Y);
    }
}
// ------------------------------------------------------------------------------------
void erosion3_ui8matrix_swp8_rot(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// ------------------------------------------------------------------------------------
{
    for(int i=i0; i<=i1; i++) {
        line_erosion3_ui8matrix_swp8_rot(X, i, j0, j1, Y);
    }
}
// ------------------------------------------------------------------------------------
void erosion3_ui8matrix_swp8_red(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// ------------------------------------------------------------------------------------
{
    for(int i=i0; i<=i1; i++) {
        line_erosion3_ui8matrix_swp8_red(X, i, j0, j1, Y);
    }
}
// -------------------------------------------------------------------------------------
void erosion3_ui8matrix_swp8_ilu3(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// -------------------------------------------------------------------------------------
{
    for(int i=i0; i<=i1; i++) {
        line_erosion3_ui8matrix_swp8_ilu3(X, i, j0, j1, Y);
    }
}
// -----------------------------------------------------------------------------------------
void erosion3_ui8matrix_swp8_ilu3_red(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// -----------------------------------------------------------------------------------------
{
    for(int i=i0; i<=i1; i++) {
        line_erosion3_ui8matrix_swp8_ilu3_red(X, i, j0, j1, Y);
    }
}
// -----------------------------------------------------------------------------------------
void erosion3_ui8matrix_swp8_elu2_red(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// -----------------------------------------------------------------------------------------
{
    int r = (i1 - i0 + 1) % 2;
    int i;
    for(i = i0; i<= i1-r; i+=2) {
        line_erosion3_ui8matrix_swp8_elu2_red(X, i, j0, j1, Y);
    } 
    if (r) {
        line_erosion3_ui8matrix_swp8_elu2_red(X, i-1, j0, j1, Y);
    }
}
// ------------------------------------------------------------------------------------------------
void erosion3_ui8matrix_swp8_elu2_red_factor(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// ------------------------------------------------------------------------------------------------
{
    int r = (i1 - i0 + 1) % 2;
    int i;
    for(i = i0; i<= i1-r; i+=2) {
        line_erosion3_ui8matrix_swp8_elu2_red_factor(X, i, j0, j1, Y);
    } 
    if (r) {
        line_erosion3_ui8matrix_swp8_elu2_red_factor(X, i-1, j0, j1, Y);
    }
}
// ----------------------------------------------------------------------------------------------
void erosion3_ui8matrix_swp8_ilu3_elu2_red(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// ----------------------------------------------------------------------------------------------
{
    int r = (i1 - i0 + 1) % 2;
    int i;
    for(i = i0; i<= i1-r; i+=2) {
        line_erosion3_ui8matrix_swp8_ilu3_elu2_red(X, i, j0, j1, Y);
    } 
    if (r) {
        line_erosion3_ui8matrix_swp8_ilu3_elu2_red(X, i-1, j0, j1, Y);
    }
}
// ----------------------------------------------------------------------------------------------------
void erosion3_ui8matrix_swp8_ilu3_elu2_red_factor(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// ----------------------------------------------------------------------------------------------------
{
    int r = (i1 - i0 + 1) % 2;
    int i;
    for(i = i0; i<= i1-r; i+=2) {
        line_erosion3_ui8matrix_swp8_ilu3_elu2_red(X, i, j0, j1, Y);
    } 
    if (r) {
        line_erosion3_ui8matrix_swp8_ilu3_elu2_red(X, i-1, j0, j1, Y);
    }
}
