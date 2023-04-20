/* ------------------------- */
/* -- nralloc_mod_test_.c -- */
/* ------------------------- */


/*
 * Copyright (c) 2005-2010 Lionel Lacassagne, IEF, Universite Paris-Sud, CNRS
 * Copyright (c) 2010-2015 Lionel Lacassagne, LRI, Universite Paris-Sud, CNRS
 * Copyright (c) 2015-2018 Lionel Lacassagne, LIP6, UPMC, CNRS
 * Copyright (c) 2018-2020 Lionel Lacassagne, LIP6, Sorbonne University, CNRS
 */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "nrtype.h"
#include "nrdef.h"
#include "nrutil.h"

#include "nralloc_mod.h"

// -------------
void test1(void)
// -------------
{
    int h = 4;
    int w = 6;
    int b = 1;
    int m = 3;
    int len = m * (w + 2 * b);
    
    uint8 **X;
    uint8 *v;
    
    X = ui8matrix_mod(0-b, h-1+b, 0-b, w-1+b, m);
    init_ui8matrix_param(X, 0-b, 0-b+(m-1), 0-b, w-1+b, 1, 1, 10);
    
    display_ui8matrix(X, 0-b, 0-b+(m-1), 0-b, w-1+b, "%4d", "X mod 3");
    
    display_ui8matrix(X, 0-b, h-1+b, 0-b, w-1+b, "%4d", "X full");
    
    display_ui8vector((uint8*) &X[0-b][0-b], 0, len-1, "%4d", "v");
    
    free_ui8matrix_mod(X, 0-b, h-1+b, 0-b, w-1+b, m);
}
// -----------------------------
void test2_routine(int h, int w)
// -----------------------------
{
    int m3 = 3;
    int m4 = 4;
    int b = 1;
    
    uint8 **X3;
    uint8 **X4;
    
    X3 = ui8matrix_mod(0-b, h-1+b, 0-b, w-1+b, m3);
    X4 = ui8matrix_mod(0-b, h-1+b, 0-b, w-1+b, m4);
    
    init_ui8matrix_param(X3, 0-b, 0-b+(m3-1), 0-b, w-1+b, 1, 1, 10);
    init_ui8matrix_param(X4, 0-b, 0-b+(m4-1), 0-b, w-1+b, 1, 1, 10);
    
    //display_ui8matrix(X, 0-b, 0-b+(m-1), 0-b, w-1+b, "%4d", "X mod 3");
    
    display_ui8matrix(X3, 0-b, h-1+b, 0-b, w-1+b, "%4d", "X3");
    display_ui8matrix(X4, 0-b, h-1+b, 0-b, w-1+b, "%4d", "X4");
    putchar('\n');
    
    free_ui8matrix_mod(X3, 0-b, h-1+b, 0-b, w-1+b, m3);
    free_ui8matrix_mod(X4, 0-b, h-1+b, 0-b, w-1+b, m4);
}
// -------------
void test2(void)
// -------------
{
    int h0 = 8;
    int h1 = h0 + 4;
    int w0 = 4;
    int w1 = w0 + 4;
    int b = 1;
    
    puts("");
    
    for(int h = h0; h <= h1; h++) {
        for(int w = w0; w <= w1; w++) {
            printf("h = %d w = %d\n", h, w);
            test2_routine(h, w);
        }
    }
}
// -----------------------------------------
int test_nralloc_mod(int argc, char* argv[])
// -----------------------------------------
{
    puts("========================");
    puts("== [test_nralloc_mod] ==");
    puts("========================");
    
    test1();
    test2();
    return 0;
}
