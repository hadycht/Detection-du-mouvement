/* --------------------------------- */
/* --- morpho_dilatation_swp64.c --- */
/* --------------------------------- */

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
#include "morpho_dilatation_swp64.h"

// ----------------------------------------------------------------------------------------
void line_dilatation3_ui64matrix_swp64_basic(uint64 **X, int i, int j0, int j1, uint64 **Y)
// ----------------------------------------------------------------------------------------
{
    for (int j=j0; j<=j1; j++) {

        uint64 a1 = load2(X, i-1, j-1);
        uint64 a2 = load2(X, i-1, j);
        uint64 a3 = load2(X, i-1, j+1);

        uint64 b1 = load2(X, i, j-1);
        uint64 b2 = load2(X, i, j);
        uint64 b3 = load2(X, i, j+1);

        uint64 c1 = load2(X, i+1, j-1);
        uint64 c2 = load2(X, i+1, j);
        uint64 c3 = load2(X, i+1, j+1);

        uint64 y = ui64max9(ui64left1(a2, a1), a2, ui64right1(a2, a3),ui64left1(b2, b1), b2, ui64right1(b2, b3),ui64left1(c2, c1), c2, ui64right1(c2, c3));
        store2(Y, i, j, y);

    }
}
// --------------------------------------------------------------------------------------
void line_dilatation3_ui64matrix_swp64_reg(uint64 **X, int i, int j0, int j1, uint64 **Y)
// --------------------------------------------------------------------------------------
{
    for (int j=j0; j<=j1; j++) {
        
        uint64 a1 = load2(X, i-1, j-1);
        uint64 a2 = load2(X, i-1, j);
        uint64 a3 = load2(X, i-1, j+1);

        uint64 b1 = load2(X, i, j-1);
        uint64 b2 = load2(X, i, j);
        uint64 b3 = load2(X, i, j+1);

        uint64 c1 = load2(X, i+1, j-1);
        uint64 c2 = load2(X, i+1, j);
        uint64 c3 = load2(X, i+1, j+1);

        uint64 z = ui64left1(a2, a1);
        uint64 x = ui64right1(a2, a3);
        uint64 t = ui64left1(b2, b1);
        uint64 u = ui64right1(b2, b3);
        uint64 v = ui64left1(c2, c1);
        uint64 w = ui64right1(c2, c3); 
        

        uint64 y = ui64max9(z, a2, x, t, b2, u, v, c2, w );
        
        store2(Y, i, j, y);
    }
}
// --------------------------------------------------------------------------------------
void line_dilatation3_ui64matrix_swp64_rot(uint64 **X, int i, int j0, int j1, uint64 **Y)
// --------------------------------------------------------------------------------------
{
    uint64 a1, a2, a3, b1, b2, b3, c1, c2, c3, x1, x2, y1, y2, z1, z2; 
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

        x1 = ui64left1(a2, a1);
        x2 = ui64right1(a2, a3);

        y1 = ui64left1(b2, b1);
        y2 = ui64right1(b2, b3);

        z1 = ui64left1(c2, c1);
        z2 = ui64right1(c2, c3);
        
        store2(Y, i, j-1, ui64max9(a2,x1,x2,b2,y1,y2,c2,z1,z2));

        a1 = a2;
        b1 = b2;
        c1 = c2;

        a2 = a3;
        b2 = b3;
        c2 = c3;
    }
}
// --------------------------------------------------------------------------------------
void line_dilatation3_ui64matrix_swp64_red(uint64 **X, int i, int j0, int j1, uint64 **Y)
// --------------------------------------------------------------------------------------
{
    uint64 a1, a2, a3, b1, b2, b3, c1, c2, c3, x1, x2, y1, y2, z1, z2, res1, res2, res3, res; 
    
    a1 = load2(X, i-1, j0-1);
    b1 = load2(X, i, j0-1);
    c1 = load2(X, i+1, j0-1);
    
    a2 = load2(X, i-1, j0);
    b2 = load2(X, i, j0);
    c2 = load2(X, i+1,j0);

    x1 = ui64left1(a2, a1);

    y1 = ui64left1(b2, b1);

    z1 = ui64left1(c2, c1);

    res1 = ui64max3(x1, y1, z1);
    res2 = ui64max3(a2, b2, c2);

    for (int j=j0+1; j<=j1+1; j++) {

        a3 = load2(X, i-1, j);
        b3 = load2(X, i, j);
        c3 = load2(X, i+1, j); 

        x2 = ui64right1(a2, a3);
        y2 = ui64right1(b2, b3);
        z2 = ui64right1(c2, c3);

        res3 = ui64max3(x2, y2, z2);
        res = ui64max3(res1, res2, res3);

        store2(Y, i, j-1, res);

        res1 = ui64max3(ui64left1(a3, a2), ui64left1(b3, b2), ui64left1(c3, c2)) ;
        res2 = ui64max3(a3, b3, c3);

        a2 = a3;
        b2 = b3;
        c2 = c3;
    }
}
// ---------------------------------------------------------------------------------------
void line_dilatation3_ui64matrix_swp64_ilu3(uint64 **X, int i, int j0, int j1, uint64 **Y)
// ---------------------------------------------------------------------------------------
{
    uint64 a1, a2, a3, a4, a5, b1, b2, b3, b4, b5, c1, c2, c3, c4, c5, x1, x2, y1, y2, z1, z2, res; 

    int r = (j1 - j0 + 1) % 3;

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

        x1 = ui64left1(a2, a1);
        x2 = ui64right1(a2, a3);

        y1 = ui64left1(b2, b1);
        y2 = ui64right1(b2, b3);

        z1 = ui64left1(c2, c1);
        z2 = ui64right1(c2, c3);
        
        res = ui64max9(a2,x1,x2,b2,y1,y2,c2,z1,z2);
        store2(Y, i, j, res); 

        x1 = ui64left1(a3, a2);
        x2 = ui64right1(a3, a4);

        y1 = ui64left1(b3, b2);
        y2 = ui64right1(b3, b4);

        z1 = ui64left1(c3, c2);
        z2 = ui64right1(c3, c4);
        
        res = ui64max9(a3,x1,x2,b3,y1,y2,c3,z1,z2);
        store2(Y, i, j+1, res);

        x1 = ui64left1(a4, a3);
        x2 = ui64right1(a4, a5);

        y1 = ui64left1(b4, b3);
        y2 = ui64right1(b4, b5);

        z1 = ui64left1(c4, c3);
        z2 = ui64right1(c4, c5);
        
        res = ui64max9(a4,x1,x2,b4,y1,y2,c4,z1,z2);
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

            x1 = ui64left1(a2, a1);
            x2 = ui64right1(a2, a3);

            y1 = ui64left1(b2, b1);
            y2 = ui64right1(b2, b3);

            z1 = ui64left1(c2, c1);
            z2 = ui64right1(c2, c3);
            
            res = ui64max9(a2,x1,x2,b2,y1,y2,c2,z1,z2);
            store2(Y, i, j, res); 
        }
    }
}
// -------------------------------------------------------------------------------------------
void line_dilatation3_ui64matrix_swp64_ilu3_red(uint64 **X, int i, int j0, int j1, uint64 **Y)
// -------------------------------------------------------------------------------------------
{
    uint64 a, b, c, x, y, res1, res2, res3, res; 
    
    a = load2(X, i-1, j0-1);
    b = load2(X, i, j0-1);
    c = load2(X, i+1, j0-1);
    
    res1 = ui64max3(a, b, c);

    a = load2(X, i-1, j0);
    b = load2(X, i, j0);
    c = load2(X, i+1,j0);

    res2 = ui64max3(a, b, c);

    int r = (j1 - j0 + 1)%3;

    for (int j = j0; j<=j1-r-1; j+=3) {


        a = load2(X, i-1, j+1);
        b = load2(X, i, j+1);
        c = load2(X, i+1, j+1); 

        res3 = ui64max3(a, b, c);

        x = ui64left1(res2, res1);
        y = ui64right1(res2, res3);

        res = ui64max3(res2, x, y);

        store2(Y, i, j, res);

        res1 = res2;
        res2 = res3; 

    
        a = load2(X, i-1, j+2);
        b = load2(X, i, j+2);
        c = load2(X, i+1, j+2);

        res3 = ui64max3(a, b, c);

        x = ui64left1(res2, res1);
        y = ui64right1(res2, res3);

        res = ui64max3(res2, x, y);

        store2(Y, i, j+1, res);

        res1 = res2;
        res2 = res3; 

    
        a = load2(X, i-1, j+3);
        b = load2(X, i, j+3);
        c = load2(X, i+1, j+3);

        res3 = ui64max3(a, b, c);

        x = ui64left1(res2, res1);
        y = ui64right1(res2, res3);

        res = ui64max3(res2, x, y);

        store2(Y, i, j+2, res);

        res1 = res2;
        res2 = res3;

    }

    if (r) {
        for (int j=max(j0,j1-r); j <= j1; j++) {
            a = load2(X, i-1, j-1);
            b = load2(X, i, j-1);
            c = load2(X, i+1, j-1);

            res1 = ui64max3(a, b, c);

            a = load2(X, i-1, j);
            b = load2(X, i, j);
            c = load2(X, i+1, j);

            res2 = ui64max3(a, b, c);

            a = load2(X, i-1, j+1);
            b = load2(X, i, j+1);
            c = load2(X, i+1, j+1);

            res3 = ui64max3(a, b, c);

            x = ui64left1(res2, res1);
            y = ui64right1(res2, res3);
        
            res = ui64max3(x, res2, y);
            store2(Y, i, j, res); 
        }
    }
}
// -------------------------------------------------------------------------------------------
void line_dilatation3_ui64matrix_swp64_elu2_red(uint64 **X, int i, int j0, int j1, uint64 **Y)
// -------------------------------------------------------------------------------------------
{
    uint64 a, b, c, d, x, y, ca1, ca2, cb1, cb2, cc1, cc2, res; 
    
    for (int j=j0; j<=j1; j++) {

        a = load2(X, i-1, j-1);
        b = load2(X, i, j-1);
        c = load2(X, i+1, j-1);

        d = load2(X, i+2, j-1);
        
        ca1 = ui64max3(a, b, c);
        ca2 = ui64max3(b, c, d);

        a = load2(X, i-1, j);
        b = load2(X, i, j);
        c = load2(X, i+1,j);

        d = load2(X, i+2, j);

        cb1 = ui64max3(a, b, c);
        cb2 = ui64max3(b, c, d);

        a = load2(X, i-1, j+1);
        b = load2(X, i, j+1);
        c = load2(X, i+1, j+1);

        d = load2(X, i+2, j+1);

        cc1 = ui64max3(a, b, c);
        cc2 = ui64max3(b, c, d);

        x = ui64left1(cb1, ca1);
        y = ui64right1(cb1, cc1);
        
        res = ui64max3(x, cb1, y);
        store2(Y, i, j, res); 

        x = ui64left1(cb2, ca2);
        y = ui64right1(cb2, cc2);
        
        res = ui64max3(x, cb2, y);
        store2(Y, i+1, j, res);

        ca1 = cb1;
        ca2 = cb2; 

        cb1 = cc1;
        cb2 = cc2;

    }
}
// --------------------------------------------------------------------------------------------------
void line_dilatation3_ui64matrix_swp64_elu2_red_factor(uint64 **X, int i, int j0, int j1, uint64 **Y)
// --------------------------------------------------------------------------------------------------
{
    uint64 a, b, c, d, x, y, ca1, ca2, cb1, cb2, cc1, cc2, res; 
    
    a = load2(X, i-1, j0-1);
    b = load2(X, i, j0-1);
    c = load2(X, i+1, j0-1);

    d = load2(X, i+2, j0-1);
    
    ca1 = ui64max3(a, b, c);
    ca2 = ui64max3(b, c, d);

    a = load2(X, i-1, j0);
    b = load2(X, i, j0);
    c = load2(X, i+1,j0);

    d = load2(X, i+2, j0);

    cb1 = ui64max3(a, b, c);
    cb2 = ui64max3(b, c, d);

    for (int j=j0; j<=j1; j++) {

        a = load2(X, i-1, j+1);
        b = load2(X, i, j+1);
        c = load2(X, i+1, j+1);

        d = load2(X, i+2, j+1);

        cc1 = ui64max3(a, b, c);
        cc2 = ui64max3(b, c, d);

        x = ui64left1(cb1, ca1);
        y = ui64right1(cb1, cc1);
        
        res = ui64max3(x, cb1, y);
        store2(Y, i, j, res); 

        x = ui64left1(cb2, ca2);
        y = ui64right1(cb2, cc2);
        
        res = ui64max3(x, cb2, y);
        store2(Y, i+1, j, res);

        ca1 = cb1;
        ca2 = cb2; 

        cb1 = cc1;
        cb2 = cc2;
    }
}
// ------------------------------------------------------------------------------------------------
void line_dilatation3_ui64matrix_swp64_ilu3_elu2_red(uint64 **X, int i, int j0, int j1, uint64 **Y)
// ------------------------------------------------------------------------------------------------
{
    uint64 a, b, c, d, x, y, ca1, ca2, cb1, cb2, cc1, cc2, res; 

    int r = (j1 - j0 + 1)%3;
    for (int j=j0; j<=j1-r-1;j+=3) {

        a = load2(X, i-1, j-1);
        b = load2(X, i, j-1);
        c = load2(X, i+1, j-1);

        d = load2(X, i+2, j-1);
        
        ca1 = ui64max3(a, b, c);
        ca2 = ui64max3(b, c, d);

        a = load2(X, i-1, j);
        b = load2(X, i, j);
        c = load2(X, i+1,j);

        d = load2(X, i+2, j);

        cb1 = ui64max3(a, b, c);
        cb2 = ui64max3(b, c, d);
        
        a = load2(X, i-1, j+1);
        b = load2(X, i, j+1);
        c = load2(X, i+1, j+1);

        d = load2(X, i+2, j+1);

        cc1 = ui64max3(a, b, c);
        cc2 = ui64max3(b, c, d);

        x = ui64left1(cb1, ca1);
        y = ui64right1(cb1, cc1);
        
        res = ui64max3(x, cb1, y);
        store2(Y, i, j, res); 

        x = ui64left1(cb2, ca2);
        y = ui64right1(cb2, cc2);
        
        res = ui64max3(x, cb2, y);
        store2(Y, i+1, j, res);

        ca1 = cb1;
        ca2 = cb2; 

        cb1 = cc1;
        cb2 = cc2;

        a = load2(X, i-1, j+2);
        b = load2(X, i, j+2);
        c = load2(X, i+1, j+2);

        d = load2(X, i+2, j+2);

        cc1 = ui64max3(a, b, c);
        cc2 = ui64max3(b, c, d);

        x = ui64left1(cb1, ca1);
        y = ui64right1(cb1, cc1);
        
        res = ui64max3(x, cb1, y);
        store2(Y, i, j+1, res); 

        x = ui64left1(cb2, ca2);
        y = ui64right1(cb2, cc2);
        
        res = ui64max3(x, cb2, y);
        store2(Y, i+1, j+1, res);

        ca1 = cb1;
        ca2 = cb2; 

        cb1 = cc1;
        cb2 = cc2;

        a = load2(X, i-1, j+3);
        b = load2(X, i, j+3);
        c = load2(X, i+1, j+3);

        d = load2(X, i+2, j+3);

        cc1 = ui64max3(a, b, c);
        cc2 = ui64max3(b, c, d);

        x = ui64left1(cb1, ca1);
        y = ui64right1(cb1, cc1);
        
        res = ui64max3(x, cb1, y);
        store2(Y, i, j+2, res); 

        x = ui64left1(cb2, ca2);
        y = ui64right1(cb2, cc2);
        
        res = ui64max3(x, cb2, y);
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

            ca1 = ui64max3(a, b, c);
            ca2 = ui64max3(b, c, d);

            a = load2(X, i-1, j);
            b = load2(X, i, j);
            c = load2(X, i+1, j);

            d = load2(X, i+2, j);
            
            cb1 = ui64max3(a, b, c);
            cb2 = ui64max3(b, c, d);

            a = load2(X, i-1, j+1);
            b = load2(X, i, j+1);
            c = load2(X, i+1, j+1);

            d = load2(X, i+2, j+1);

            cc1 = ui64max3(a, b, c);
            cc2 = ui64max3(b, c, d);

            x = ui64left1(cb1, ca1);
            y = ui64right1(cb1, cc1);
        
            res = ui64max3(x, cb1, y);
            store2(Y, i, j, res); 

            x = ui64left1(cb2, ca2);
            y = ui64right1(cb2, cc2);
        
            res = ui64max3(x, cb2, y);
            store2(Y, i+1, j, res);
        }
    }
}

// -------------------------------------------------------------------------------------------------------
void line_dilatation3_ui64matrix_swp64_ilu3_elu2_red_factor(uint64 **X, int i, int j0, int j1, uint64 **Y)
// -------------------------------------------------------------------------------------------------------
{
    uint64 a, b, c, d, x, y, ca1, ca2, cb1, cb2, cc1, cc2, res; 

    a = load2(X, i-1, j0-1);
    b = load2(X, i, j0-1);
    c = load2(X, i+1, j0-1);

    d = load2(X, i+2, j0-1);
    
    ca1 = ui64max3(a, b, c);
    ca2 = ui64max3(b, c, d);

    a = load2(X, i-1, j0);
    b = load2(X, i, j0);
    c = load2(X, i+1,j0);

    d = load2(X, i+2, j0);

    cb1 = ui64max3(a, b, c);
    cb2 = ui64max3(b, c, d);

    int r = (j1 - j0 + 1)%3;
    for (int j=j0; j<=j1-r-1;j+=3) {
        
        a = load2(X, i-1, j+1);
        b = load2(X, i, j+1);
        c = load2(X, i+1, j+1);

        d = load2(X, i+2, j+1);

        cc1 = ui64max3(a, b, c);
        cc2 = ui64max3(b, c, d);

        x = ui64left1(cb1, ca1);
        y = ui64right1(cb1, cc1);
        
        res = ui64max3(x, cb1, y);
        store2(Y, i, j, res); 

        x = ui64left1(cb2, ca2);
        y = ui64right1(cb2, cc2);
        
        res = ui64max3(x, cb2, y);
        store2(Y, i+1, j, res);

        ca1 = cb1;
        ca2 = cb2; 

        cb1 = cc1;
        cb2 = cc2;

        a = load2(X, i-1, j+2);
        b = load2(X, i, j+2);
        c = load2(X, i+1, j+2);

        d = load2(X, i+2, j+2);

        cc1 = ui64max3(a, b, c);
        cc2 = ui64max3(b, c, d);

        x = ui64left1(cb1, ca1);
        y = ui64right1(cb1, cc1);
        
        res = ui64max3(x, cb1, y);
        store2(Y, i, j+1, res); 

        x = ui64left1(cb2, ca2);
        y = ui64right1(cb2, cc2);
        
        res = ui64max3(x, cb2, y);
        store2(Y, i+1, j+1, res);

        ca1 = cb1;
        ca2 = cb2; 

        cb1 = cc1;
        cb2 = cc2;

        a = load2(X, i-1, j+3);
        b = load2(X, i, j+3);
        c = load2(X, i+1, j+3);

        d = load2(X, i+2, j+3);

        cc1 = ui64max3(a, b, c);
        cc2 = ui64max3(b, c, d);

        x = ui64left1(cb1, ca1);
        y = ui64right1(cb1, cc1);
        
        res = ui64max3(x, cb1, y);
        store2(Y, i, j+2, res); 

        x = ui64left1(cb2, ca2);
        y = ui64right1(cb2, cc2);
        
        res = ui64max3(x, cb2, y);
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

            ca1 = ui64max3(a, b, c);
            ca2 = ui64max3(b, c, d);

            a = load2(X, i-1, j);
            b = load2(X, i, j);
            c = load2(X, i+1, j);

            d = load2(X, i+2, j);
            
            cb1 = ui64max3(a, b, c);
            cb2 = ui64max3(b, c, d);

            a = load2(X, i-1, j+1);
            b = load2(X, i, j+1);
            c = load2(X, i+1, j+1);

            d = load2(X, i+2, j+1);

            cc1 = ui64max3(a, b, c);
            cc2 = ui64max3(b, c, d);

            x = ui64left1(cb1, ca1);
            y = ui64right1(cb1, cc1);
        
            res = ui64max3(x, cb1, y);
            store2(Y, i, j, res); 

            x = ui64left1(cb2, ca2);
            y = ui64right1(cb2, cc2);
        
            res = ui64max3(x, cb2, y);
            store2(Y, i+1, j, res);
        }
    }
}
// --------------------------------------------------------------------------------------------
void dilatation3_ui64matrix_swp64_basic(uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y)
// --------------------------------------------------------------------------------------------
{
    for(int i=i0; i<=i1; i++) {
        line_dilatation3_ui64matrix_swp64_basic(X, i, j0, j1, Y);
    }
}
// ------------------------------------------------------------------------------------------
void dilatation3_ui64matrix_swp64_reg(uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y)
// ------------------------------------------------------------------------------------------
{
    for(int i=i0; i<=i1; i++) {
        line_dilatation3_ui64matrix_swp64_reg(X, i, j0, j1, Y);
    }
}
// ------------------------------------------------------------------------------------------
void dilatation3_ui64matrix_swp64_rot(uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y)
// ------------------------------------------------------------------------------------------
{
    for(int i=i0; i<=i1; i++) {
        line_dilatation3_ui64matrix_swp64_rot(X, i, j0, j1, Y);
    }
}
// ------------------------------------------------------------------------------------------
void dilatation3_ui64matrix_swp64_red(uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y)
// ------------------------------------------------------------------------------------------
{
    for(int i=i0; i<=i1; i++) {
        line_dilatation3_ui64matrix_swp64_red(X, i, j0, j1, Y);
    }
}
// -------------------------------------------------------------------------------------------
void dilatation3_ui64matrix_swp64_ilu3(uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y)
// -------------------------------------------------------------------------------------------
{
    for(int i=i0; i<=i1; i++) {
        line_dilatation3_ui64matrix_swp64_ilu3(X, i, j0, j1, Y);
    }
}
// -----------------------------------------------------------------------------------------------
void dilatation3_ui64matrix_swp64_ilu3_red(uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y)
// -----------------------------------------------------------------------------------------------
{
    for(int i=i0; i<=i1; i++) {
        line_dilatation3_ui64matrix_swp64_ilu3_red(X, i, j0, j1, Y);
    }
}
// -----------------------------------------------------------------------------------------------
void dilatation3_ui64matrix_swp64_elu2_red(uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y)
// -----------------------------------------------------------------------------------------------
{
    int r = (i1 - i0 + 1) % 2;
    int i;
    for(i = i0; i<= i1-r; i+=2) {
        line_dilatation3_ui64matrix_swp64_elu2_red(X, i, j0, j1, Y);
    } 
    if (r) {
        line_dilatation3_ui64matrix_swp64_elu2_red(X, i-1, j0, j1, Y);
    }
}
// ------------------------------------------------------------------------------------------------------
void dilatation3_ui64matrix_swp64_elu2_red_factor(uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y)
// ------------------------------------------------------------------------------------------------------
{
    int r = (i1 - i0 + 1) % 2;
    int i;
    for(i = i0; i<= i1-r; i+=2) {
        line_dilatation3_ui64matrix_swp64_elu2_red_factor(X, i, j0, j1, Y);
    } 
    if (r) {
        line_dilatation3_ui64matrix_swp64_elu2_red_factor(X, i-1, j0, j1, Y);
    }
}
// ----------------------------------------------------------------------------------------------------
void dilatation3_ui64matrix_swp64_ilu3_elu2_red(uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y)
// ----------------------------------------------------------------------------------------------------
{
    int r = (i1 - i0 + 1) % 2;
    int i;
    for(i = i0; i<= i1-r; i+=2) {
        line_dilatation3_ui64matrix_swp64_ilu3_elu2_red(X, i, j0, j1, Y);
    } 
    if (r) {
        line_dilatation3_ui64matrix_swp64_ilu3_elu2_red(X, i-1, j0, j1, Y);
    }
}
// -----------------------------------------------------------------------------------------------------------
void dilatation3_ui64matrix_swp64_ilu3_elu2_red_factor(uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y)
// -----------------------------------------------------------------------------------------------------------
{
    int r = (i1 - i0 + 1) % 2;
    int i;
    for(i = i0; i<= i1-r; i+=2) {
        line_dilatation3_ui64matrix_swp64_ilu3_elu2_red_factor(X, i, j0, j1, Y);
    } 
    if (r) {
        line_dilatation3_ui64matrix_swp64_ilu3_elu2_red_factor(X, i-1, j0, j1, Y);
    }
}
