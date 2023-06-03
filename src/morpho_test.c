/* --------------------- */
/* --- morpho_test.c --- */
/* --------------------- */

/*
 * Copyright (c) 2004 - 2013, Lionel Lacassagne, All rights reserved
 * University of Paris Sud, Laboratoire de Recherche en Informatique 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include <inttypes.h> // printf 64 bits

#include "nrtype.h"
#include "nrdef.h"
#include "nrutil.h"
//#include "nrutil_ext.h" // set_ui8vector_str
//#include "nralloc_mod.h"

#include "macro_bench.h"
#include "x86intrin.h" // _rdtsc()
#include "swp.h"

#include "morpho.h"

#include "morpho_min.h"
#include "morpho_max.h"

#include "morpho_erosion.h"
#include "morpho_erosion_swp8.h"
//#include "morpho_erosion_swp32.h"

#include "morpho_dilatation.h"
#include "morpho_dilatation_swp8.h"
//#include "morpho_dilatation_swp32.h"

#include "morpho_ouverture.h"
#include "morpho_ouverture_swp8.h"
//#include "morpho_ouverture_swp32.h"
#include "morpho_ouverture_swp64.h"

#include "morpho_fermeture.h"
#include "morpho_fermeture_swp8.h"
//#include "morpho_fermeture_swp32.h"

#include "morpho_test.h"

#include "ue_l3_vision.h" // nom du binome

//#include "omp.h"
// versions de reference
// ------------------------------------------------------------------------------------------------------
int ref_ui8max9(uint8 x0, uint8 x1, uint8 x2, uint8 x3, uint8 x4, uint8 x5, uint8 x6, uint8 x7, uint8 x8)
// ------------------------------------------------------------------------------------------------------
{
    // version navive
    // fonctionne en niveau de gris et en binaire (1 bit par element)
    uint8 m = x0;
    if(x1 > m) m = x1;
    if(x2 > m) m = x2;
    if(x3 > m) m = x3;
    if(x4 > m) m = x4;
    if(x5 > m) m = x5;
    if(x6 > m) m = x6;
    if(x7 > m) m = x7;
    if(x8 > m) m = x8;
    return m;
}
// ------------------------------------------------------------------------------------------------------
int ref_ui8min9(uint8 x0, uint8 x1, uint8 x2, uint8 x3, uint8 x4, uint8 x5, uint8 x6, uint8 x7, uint8 x8)
// ------------------------------------------------------------------------------------------------------
{
    // version navive
    // fonctionne en niveau de gris et en binaire (1 bit par element)
    uint8 m = x0;
    if(x1 < m) m = x1;
    if(x2 < m) m = x2;
    if(x3 < m) m = x3;
    if(x4 < m) m = x4;
    if(x5 < m) m = x5;
    if(x6 < m) m = x6;
    if(x7 < m) m = x7;
    if(x8 < m) m = x8;
    return m;
}
// -----------------------------------------------------------------------------------
void ref_line_dilatation3_ui8matrix_basic(uint8 **X, int i, int j0, int j1, uint8 **Y)
// -----------------------------------------------------------------------------------
{
    for(int j=j0; j<=j1; j++) {
        
        Y[i][j] = ref_ui8max9(X[i-1][j-1], X[i-1][j], X[i-1][j+1],
                              X[i  ][j-1], X[i  ][j], X[i  ][j+1],
                              X[i+1][j-1], X[i+1][j], X[i+1][j+1]);
    }
}
// ---------------------------------------------------------------------------------------
void ref_dilatation3_ui8matrix_basic(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// ---------------------------------------------------------------------------------------
{
#ifdef _OMP
#pragma omp parallel for shared(X, Y) firstprivate(i0, i1, j0, j1) default(none)
#endif // _OMP
    for(int i=i0; i<=i1; i++) {
        ref_line_dilatation3_ui8matrix_basic(X, i, j0, j1, Y);
    }
}
// --------------------------------------------------------------------------------
void ref_line_erosion3_ui8matrix_basic(uint8 **X, int i, int j0, int j1, uint8 **Y)
// --------------------------------------------------------------------------------
{
    for(int j=j0; j<=j1; j++) {
        
        Y[i][j] = ref_ui8min9(X[i-1][j-1], X[i-1][j], X[i-1][j+1],
                              X[i  ][j-1], X[i  ][j], X[i  ][j+1],
                              X[i+1][j-1], X[i+1][j], X[i+1][j+1]);
    }
}
// ------------------------------------------------------------------------------------
void ref_erosion3_ui8matrix_basic(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// ------------------------------------------------------------------------------------
{
    for(int i=i0; i<=i1; i++) {
        ref_line_erosion3_ui8matrix_basic(X, i, j0, j1, Y);
    }
}
// -------------------------------------------------------------------------------------------------
void ref_ouverture3_ui8matrix_basic(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y, uint8 **Z)
// -------------------------------------------------------------------------------------------------
{
    
    // X[i0-2, i1+2][j0-2, j1+2]
    // Y[i0-1, i1+1][j0-1, j1+1]
    // Z[i0,   i1  ][j0,   j1  ]
    
    ref_erosion3_ui8matrix_basic   (X, i0-1, i1+1, j0-1, j1+1, Y);
    ref_dilatation3_ui8matrix_basic(Y, i0,   i1,   j0,   j1,   Z);
}

// -------------------------------------------------------------------------------
void rand_ui8matrix(uint8 **X, int i0, int i1, int j0, int j1, uint8 x0, uint8 x1)
// -------------------------------------------------------------------------------
{
    int d = x1 - x0 + 1; // range [x0, x1]
    
    for(int i=i0; i<=i1; i++) {
        for(int j=j0; j<=j1; j++) {
        
            int x = x0 + rand() % d;
            
            X[i][j] = (uint8) x;
        }
    }
}
// -------------------------------------------------------------------------
void rand1_ui8matrix(uint8 **X, int i0, int i1, int j0, int j1, int density)
// -------------------------------------------------------------------------
{
    // binary generator
    
    for(int i=i0; i<=i1; i++) {
        for(int j=j0; j<=j1; j++) {
        
            int r = rand() % 100;
            if(r <= density)
                X[i][j] = 1;
            else
                X[i][j] = 0;
        }
    }
}
// ------------------------------------------------------------------------------------
int compare_ui8matrix(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y, char* name)
// ------------------------------------------------------------------------------------
{
    int c = 0;
    
    //display_ui8matrix(Y, i0, i1, j0, j1, "%5d", name);
    
    for(int i=i0; i<=i1; i++) {
        for(int j=j0; j<=j1; j++) {
            if(X[i][j] != Y[i][j]) {
                //printf("%s [%3d][%3d] = %3d NEQ %3d\n", name, i, j, X[i][i], Y[i][i]);
                c++;
            }
        }
    }
    if(c) {
        printf("[compare]: %s %4d errors\n", name, c); //exit(-1);
    } else {
        printf("[compare]: %s is OK\n", name);
        //return c;
    }
    //puts("----------------------");
    if(c) return 0; else return 1;
}
// ---------------------------------------------------------------------------------------
int compare_ui64matrix(uint64 **X, int i0, int i1, int j0, int j1, uint64 **Y, char* name)
// ---------------------------------------------------------------------------------------
{
    int c = 0;
    
    //display_ui64matrix(Y, i0, i1, j0, j1, "%5d", name);
    
    for(int i=i0; i<=i1; i++) {
        for(int j=j0; j<=j1; j++) {
            if(X[i][j] != Y[i][j]) {
                //printf("%s [%3d][%3d] = " PRIx64 " NEQ " PRIx64 "\n", name, i, j, X[i][i], Y[i][i]);
                printf("%s [%3d][%3d] = ", name, i, j);
                printf("%" PRIx64, X[i][i]);
                printf(" NEQ ");
                printf("%" PRIx64, Y[i][i]);
                
                c++;
            }
        }
    }
    if(c) {
        printf("%d errors !\n", c); //exit(-1);
    } else {
        printf("[compare]: %s is OK\n", name);
        //return c;
    }
    //puts("----------------------");
    if(c) return 0; else return 1;
}
// -----------------------------------------------
void test_morpho_dilatation_routine(int h, int w0)
// -----------------------------------------------
{
    int b = 1; // bord

    int w8 = w0 / 8; if(w0 % 8) w8 = w8+1;
    int w1 = 8 * w8; // w1 >= w

    puts("--------------------------------------------------");
    printf("test_morpho_dilatation_routine h = %d w0 = %d w8 = %d, w1 = %d\n", h, w0, w8, w1);
    if(w1 > w0) puts("w1 > w0");
    
    int c; // error

    //puts("malloc");
    uint8 **X  = ui8matrix(0-b, h-1+b, 0-b, w1-1+b); zero_ui8matrix(X,  0-b, h-1+b, 0-b, w1-1+b);
    uint8 **X8 = ui8matrix(0-b, h-1+b, 0-b, w8-1+b); zero_ui8matrix(X8, 0-b, h-1+b, 0-b, w8-1+b);
    uint8 **X1 = ui8matrix(0-b, h-1+b, 0-b, w1-1+b); zero_ui8matrix(X1, 0-b, h-1+b, 0-b, w1-1+b);
    
    uint8 **Y_basic       = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_basic,       0, h-1, 0, w1-1);
    uint8 **Y_reg         = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_reg,         0, h-1, 0, w1-1);
    uint8 **Y_rot         = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_rot,         0, h-1, 0, w1-1);
    uint8 **Y_red         = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_red,         0, h-1, 0, w1-1);
    uint8 **Y_ilu3        = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_ilu3,        0, h-1, 0, w1-1);
    uint8 **Y_ilu3r       = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_ilu3r,       0, h-1, 0, w1-1);
    uint8 **Y_elu2r       = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_elu2r,       0, h-1, 0, w1-1);
    uint8 **Y_elu2rf      = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_elu2rf,      0, h-1, 0, w1-1);
    uint8 **Y_ilu3_elu2rf = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_ilu3_elu2rf, 0, h-1, 0, w1-1);
    
    uint8 **Y8_basic       = ui8matrix(0, h-1, 0, w8-1); zero_ui8matrix(Y8_basic,       0, h-1, 0, w8-1);
    uint8 **Y8_reg         = ui8matrix(0, h-1, 0, w8-1); zero_ui8matrix(Y8_reg,         0, h-1, 0, w8-1);
    uint8 **Y8_rot         = ui8matrix(0, h-1, 0, w8-1); zero_ui8matrix(Y8_rot,         0, h-1, 0, w8-1);
    uint8 **Y8_red         = ui8matrix(0, h-1, 0, w8-1); zero_ui8matrix(Y8_red,         0, h-1, 0, w8-1);
    uint8 **Y8_ilu3        = ui8matrix(0, h-1, 0, w8-1); zero_ui8matrix(Y8_ilu3,        0, h-1, 0, w8-1);
    uint8 **Y8_ilu3r       = ui8matrix(0, h-1, 0, w8-1); zero_ui8matrix(Y8_ilu3r,       0, h-1, 0, w8-1);
    uint8 **Y8_elu2r       = ui8matrix(0, h-1, 0, w8-1); zero_ui8matrix(Y8_elu2r,       0, h-1, 0, w8-1);
    uint8 **Y8_elu2rf      = ui8matrix(0, h-1, 0, w8-1); zero_ui8matrix(Y8_elu2rf,      0, h-1, 0, w8-1);
    uint8 **Y8_ilu3_elu2rf = ui8matrix(0, h-1, 0, w8-1); zero_ui8matrix(Y8_ilu3_elu2rf, 0, h-1, 0, w8-1);
    
    uint8 **Y1_basic       = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_basic,       0, h-1, 0, w1-1);
    uint8 **Y1_reg         = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_reg,         0, h-1, 0, w1-1);
    uint8 **Y1_rot         = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_rot,         0, h-1, 0, w1-1);
    uint8 **Y1_red         = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_red,         0, h-1, 0, w1-1);
    uint8 **Y1_ilu3        = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_ilu3,        0, h-1, 0, w1-1);
    uint8 **Y1_ilu3r       = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_ilu3r,       0, h-1, 0, w1-1);
    uint8 **Y1_elu2r       = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_elu2r,       0, h-1, 0, w1-1);
    uint8 **Y1_elu2rf      = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_elu2rf,      0, h-1, 0, w1-1);
    uint8 **Y1_ilu3_elu2rf = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_ilu3_elu2rf, 0, h-1, 0, w1-1);
        
    //puts("rand");
    //rand_ui8matrix(X, 0, h-1, 0, w0-1, 1, 255); // niveau de gris [1,255]
    rand1_ui8matrix(X, 0, h-1, 0, w0-1, 20); // binaire [0,1]
    //puts("pack/unpack");
    pack_ui8matrix  (X,  h, w1, X8); // bug ou pas bug si w1 ?
    unpack_ui8matrix(X8, h, w8, X1);
    
    //puts("display");
    //display_ui8matrix (X,  0, h-1, 0, w0-1, "%5d", "X0");
    //display_ui8matrix (X8, 0, h-1, 0, w8-1, "%5x", "X8");
    //displayM_ui8matrix(X8, 0, h-1, 0, w8-1,        "X8");
    //display_ui8matrix (X1, 0, h-1, 0, w0-1, "%5d", "X1");
    c = compare_ui8matrix(X, 0, h-1, 0, w0-1, X1, "X0=X1"); putchar('\n');
    
    //puts("\n-- max3 --");
    dilatation3_ui8matrix_basic               (X, 0, h-1, 0, w1-1, Y_basic);       // puts("basic               ");
    dilatation3_ui8matrix_reg                 (X, 0, h-1, 0, w1-1, Y_reg);         // puts("reg                 ");
    dilatation3_ui8matrix_rot                 (X, 0, h-1, 0, w1-1, Y_rot);         // puts("rot                 ");
    dilatation3_ui8matrix_red                 (X, 0, h-1, 0, w1-1, Y_red);         // puts("red                 ");
    dilatation3_ui8matrix_ilu3                (X, 0, h-1, 0, w1-1, Y_ilu3);        // puts("ilu3                ");
    dilatation3_ui8matrix_ilu3_red            (X, 0, h-1, 0, w1-1, Y_ilu3r);       // puts("ilu3_red            ");
    dilatation3_ui8matrix_elu2_red            (X, 0, h-1, 0, w1-1, Y_elu2r);       // puts("elu2_red            ");
    dilatation3_ui8matrix_elu2_red_factor     (X, 0, h-1, 0, w1-1, Y_elu2rf);      // puts("elu2_red_factor     ");
    dilatation3_ui8matrix_ilu3_elu2_red_factor(X, 0, h-1, 0, w1-1, Y_ilu3_elu2rf); // puts("ilu3_elu2_red_factor");
        
    //display_ui8matrix(Y_basic,       0, h-1, 0, w0-1, "%5d", "Y basic      ");
    //display_ui8matrix(Y_reg,         0, h-1, 0, w0-1, "%5d", "Y reg        ");
    //display_ui8matrix(Y_reg,         0, h-1, 0, w0-1, "%5d", "Y reg        ");
    //display_ui8matrix(Y_rot,         0, h-1, 0, w0-1, "%5d", "Y rot        ");
    //display_ui8matrix(Y_red,         0, h-1, 0, w0-1, "%5d", "Y red        ");
    //display_ui8matrix(Y_ilu3,        0, h-1, 0, w0-1, "%5d", "Y ilu3       ");
    //display_ui8matrix(Y_ilu3r,       0, h-1, 0, w0-1, "%5d", "Y ilu3r      ");
    //display_ui8matrix(Y_elu2r,       0, h-1, 0, w0-1, "%5d", "Y elu2r      ");
    //display_ui8matrix(Y_elu2rf,      0, h-1, 0, w0-1, "%5d", "Y elu2rf     ");
    //display_ui8matrix(Y_ilu3_elu2rf, 0, h-1, 0, w0-1, "%5d", "Y ilu3 elu2rf");
    
    puts("-- compare max --");
    c = compare_ui8matrix(Y_basic, 0, h-1, 0, w0-1, Y_reg        , "Y reg                       ");
    c = compare_ui8matrix(Y_basic, 0, h-1, 0, w0-1, Y_rot        , "Y rot                       ");
    c = compare_ui8matrix(Y_basic, 0, h-1, 0, w0-1, Y_red        , "Y red                       ");
    c = compare_ui8matrix(Y_basic, 0, h-1, 0, w0-1, Y_ilu3       , "Y ilu3                      ");
    c = compare_ui8matrix(Y_basic, 0, h-1, 0, w0-1, Y_ilu3r      , "Y ilu3 + red                ");
    c = compare_ui8matrix(Y_basic, 0, h-1, 0, w0-1, Y_elu2r      , "Y elu2 + red                ");
    c = compare_ui8matrix(Y_basic, 0, h-1, 0, w0-1, Y_elu2rf     , "Y ilu3 + red + factor       ");
    c = compare_ui8matrix(Y_basic, 0, h-1, 0, w0-1, Y_ilu3_elu2rf, "Y ilu3 + elu2 + red + factor");
    
    //puts("-- or3 (packed)--");
    dilatation3_ui8matrix_swp8_basic               (X8, 0, h-1, 0, w8-1, Y8_basic      ); // puts("basic               ");
    dilatation3_ui8matrix_swp8_reg                 (X8, 0, h-1, 0, w8-1, Y8_reg        ); // puts("reg                 ");
    dilatation3_ui8matrix_swp8_rot                 (X8, 0, h-1, 0, w8-1, Y8_rot        ); // puts("rot                 ");
    dilatation3_ui8matrix_swp8_red                 (X8, 0, h-1, 0, w8-1, Y8_red        ); // puts("red                 ");
    dilatation3_ui8matrix_swp8_ilu3                (X8, 0, h-1, 0, w8-1, Y8_ilu3       ); // puts("ilu3                ");
    dilatation3_ui8matrix_swp8_ilu3_red            (X8, 0, h-1, 0, w8-1, Y8_ilu3r      ); // puts("ilu3_red            ");
    dilatation3_ui8matrix_swp8_elu2_red            (X8, 0, h-1, 0, w8-1, Y8_elu2r      ); // puts("elu2_red            ");
    dilatation3_ui8matrix_swp8_elu2_red_factor     (X8, 0, h-1, 0, w8-1, Y8_elu2rf     ); // puts("elu2_red_factor     ");
    dilatation3_ui8matrix_swp8_ilu3_elu2_red_factor(X8, 0, h-1, 0, w8-1, Y8_ilu3_elu2rf); // puts("ilu3_elu2_red_factor");
    //puts("done\n");
    
    //puts("-- unpack --");
    unpack_ui8matrix(Y8_basic      , h, w8, Y1_basic      );
    unpack_ui8matrix(Y8_reg        , h, w8, Y1_reg        );
    unpack_ui8matrix(Y8_rot        , h, w8, Y1_rot        );
    unpack_ui8matrix(Y8_red        , h, w8, Y1_red        );
    unpack_ui8matrix(Y8_ilu3       , h, w8, Y1_ilu3       );
    unpack_ui8matrix(Y8_ilu3r      , h, w8, Y1_ilu3r      );
    unpack_ui8matrix(Y8_elu2r      , h, w8, Y1_elu2r      );
    unpack_ui8matrix(Y8_elu2rf     , h, w8, Y1_elu2rf     );
    unpack_ui8matrix(Y8_ilu3_elu2rf, h, w8, Y1_ilu3_elu2rf);
    //puts("done\n");
        
    //display_ui8matrix(Y8_basic,       0, h-1, 0, w8-1, "%5d", "Y8 basic      ");
    //display_ui8matrix(Y1_reg,         0, h-1, 0, w0-1, "%5d", "Y1 reg        ");
    //display_ui8matrix(Y1_rot,         0, h-1, 0, w0-1, "%5d", "Y1 rot        ");
    //display_ui8matrix(Y1_red,         0, h-1, 0, w0-1, "%5d", "Y1 red        ");
    //display_ui8matrix(Y1_ilu3,        0, h-1, 0, w0-1, "%5d", "Y1 ilu3       ");
    //display_ui8matrix(Y1_ilu3r,       0, h-1, 0, w0-1, "%5d", "Y1 ilu3r      ");
    //display_ui8matrix(Y1_elu2r,       0, h-1, 0, w0-1, "%5d", "Y1 elu2r      ");
    //display_ui8matrix(Y1_elu2rf,      0, h-1, 0, w0-1, "%5d", "Y1 elu2rf     ");
    //display_ui8matrix(Y1_ilu3_elu2rf, 0, h-1, 0, w0-1, "%5d", "Y1 ilu3_elu2rf");/**/
    
    // puts("\n-- SWP8 compare packed or --\n");
    // c = compare_ui8matrix(Y8_basic, 0, h-1, 0, w8-1, Y8_reg        , "Y8 reg                       ");
    // c = compare_ui8matrix(Y8_basic, 0, h-1, 0, w8-1, Y8_rot        , "Y8 rot                       ");
    // c = compare_ui8matrix(Y8_basic, 0, h-1, 0, w8-1, Y8_red        , "Y8 red                       ");
    // c = compare_ui8matrix(Y8_basic, 0, h-1, 0, w8-1, Y8_ilu3       , "Y8 ilu3                      ");
    // c = compare_ui8matrix(Y8_basic, 0, h-1, 0, w8-1, Y8_ilu3r      , "Y8 ilu3 + red                ");
    // c = compare_ui8matrix(Y8_basic, 0, h-1, 0, w8-1, Y8_elu2r      , "Y8 elu2 + red                ");
    // c = compare_ui8matrix(Y8_basic, 0, h-1, 0, w8-1, Y8_elu2rf     , "Y8 ilu3 + red + factor       ");
    // c = compare_ui8matrix(Y8_basic, 0, h-1, 0, w8-1, Y8_ilu3_elu2rf, "Y8 ilu3 + elu2 + red + factor");
    //puts("done\n");/**/
    
    puts("\n-- SWP8 compare unpacked results --\n");
    c = compare_ui8matrix(Y_basic, 0, h-1, 0, w0-1, Y1_basic      , "Y1 basic                     ");
    c = compare_ui8matrix(Y_basic, 0, h-1, 0, w0-1, Y1_reg        , "Y1 reg                       ");
    c = compare_ui8matrix(Y_basic, 0, h-1, 0, w0-1, Y1_rot        , "Y1 rot                       ");
    c = compare_ui8matrix(Y_basic, 0, h-1, 0, w0-1, Y1_red        , "Y1 red                       ");
    c = compare_ui8matrix(Y_basic, 0, h-1, 0, w0-1, Y1_ilu3       , "Y1 ilu3                      ");
    c = compare_ui8matrix(Y_basic, 0, h-1, 0, w0-1, Y1_ilu3r      , "Y1 ilu3 + red                ");
    c = compare_ui8matrix(Y_basic, 0, h-1, 0, w0-1, Y1_elu2r      , "Y1 elu2 + red                ");
    c = compare_ui8matrix(Y_basic, 0, h-1, 0, w0-1, Y1_elu2rf     , "Y1 elu2 + red + factor       ");
    c = compare_ui8matrix(Y_basic, 0, h-1, 0, w0-1, Y1_ilu3_elu2rf, "Y1 ilu3 + elu2 + red + factor");
    putchar('\n');
    
    free_ui8matrix(X ,            0-b, h-1+b, 0-b, w1-1+b);
    free_ui8matrix(X8,            0-b, h-1+b, 0-b, w8-1+b);
    free_ui8matrix(X1,            0-b, h-1+b, 0-b, w1-1+b);
    
    free_ui8matrix(Y_basic      , 0,   h-1,   0,   w1-1);
    free_ui8matrix(Y_reg        , 0,   h-1,   0,   w1-1);
    free_ui8matrix(Y_rot        , 0,   h-1,   0,   w1-1);
    free_ui8matrix(Y_red        , 0,   h-1,   0,   w1-1);
    free_ui8matrix(Y_ilu3       , 0,   h-1,   0,   w1-1);
    free_ui8matrix(Y_ilu3r      , 0,   h-1,   0,   w1-1);
    free_ui8matrix(Y_elu2r      , 0,   h-1,   0,   w1-1);
    free_ui8matrix(Y_elu2rf     , 0,   h-1,   0,   w1-1);
    free_ui8matrix(Y_ilu3_elu2rf, 0,   h-1,   0,   w1-1);
    
    free_ui8matrix(Y8_basic      , 0,   h-1,   0,   w8-1);
    free_ui8matrix(Y8_reg        , 0,   h-1,   0,   w8-1);
    free_ui8matrix(Y8_rot        , 0,   h-1,   0,   w8-1);
    free_ui8matrix(Y8_red        , 0,   h-1,   0,   w8-1);
    free_ui8matrix(Y8_ilu3       , 0,   h-1,   0,   w8-1);
    free_ui8matrix(Y8_ilu3r      , 0,   h-1,   0,   w8-1);
    free_ui8matrix(Y8_elu2r      , 0,   h-1,   0,   w8-1);
    free_ui8matrix(Y8_elu2rf     , 0,   h-1,   0,   w8-1);
    free_ui8matrix(Y8_ilu3_elu2rf, 0,   h-1,   0,   w8-1);
    
    free_ui8matrix(Y1_basic      , 0,   h-1,   0,   w1-1);
    free_ui8matrix(Y1_reg        , 0,   h-1,   0,   w1-1);
    free_ui8matrix(Y1_rot        , 0,   h-1,   0,   w1-1);
    free_ui8matrix(Y1_red        , 0,   h-1,   0,   w1-1);
    free_ui8matrix(Y1_ilu3       , 0,   h-1,   0,   w1-1);
    free_ui8matrix(Y1_ilu3r      , 0,   h-1,   0,   w1-1);
    free_ui8matrix(Y1_elu2r      , 0,   h-1,   0,   w1-1);
    free_ui8matrix(Y1_elu2rf     , 0,   h-1,   0,   w1-1);
    free_ui8matrix(Y1_ilu3_elu2rf, 0,   h-1,   0,   w1-1);
}
// --------------------------------------------
void test_morpho_erosion_routine(int h, int w0)
// --------------------------------------------
{
    int b = 1; // bord

    int w8 = w0 / 8; if(w0 % 8) w8 = w8+1;
    int w1 = 8 * w8; // w1 >= w

    puts("--------------------------------------------------");
    printf("test_morpho_min_routine h = %d w0 = %d w8 = %d, w1 = %d\n", h, w0, w8, w1);
    if(w1 > w0) puts("w1 > w0");
        
    int c; // error

    //puts("malloc");
    uint8 **X  = ui8matrix(0-b, h-1+b, 0-b, w1-1+b); zero_ui8matrix(X,  0-b, h-1+b, 0-b, w1-1+b);
    uint8 **X8 = ui8matrix(0-b, h-1+b, 0-b, w8-1+b); zero_ui8matrix(X8, 0-b, h-1+b, 0-b, w8-1+b);
    uint8 **X1 = ui8matrix(0-b, h-1+b, 0-b, w1-1+b); zero_ui8matrix(X1, 0-b, h-1+b, 0-b, w1-1+b);
    
    uint8 **Y_basic       = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_basic      , 0, h-1, 0, w1-1);
    uint8 **Y_reg         = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_reg        , 0, h-1, 0, w1-1);
    uint8 **Y_rot         = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_rot        , 0, h-1, 0, w1-1);
    uint8 **Y_red         = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_red        , 0, h-1, 0, w1-1);
    uint8 **Y_ilu3        = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_ilu3       , 0, h-1, 0, w1-1);
    uint8 **Y_ilu3r       = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_ilu3r      , 0, h-1, 0, w1-1);
    uint8 **Y_elu2r       = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_elu2r      , 0, h-1, 0, w1-1);
    uint8 **Y_elu2rf      = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_elu2rf     , 0, h-1, 0, w1-1);
    uint8 **Y_ilu3_elu2rf = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_ilu3_elu2rf, 0, h-1, 0, w1-1);
    
    uint8 **Y8_basic       = ui8matrix(0, h-1, 0, w8-1); zero_ui8matrix(Y8_basic      , 0, h-1, 0, w8-1);
    uint8 **Y8_reg         = ui8matrix(0, h-1, 0, w8-1); zero_ui8matrix(Y8_reg        , 0, h-1, 0, w8-1);
    uint8 **Y8_rot         = ui8matrix(0, h-1, 0, w8-1); zero_ui8matrix(Y8_rot        , 0, h-1, 0, w8-1);
    uint8 **Y8_red         = ui8matrix(0, h-1, 0, w8-1); zero_ui8matrix(Y8_red        , 0, h-1, 0, w8-1);
    uint8 **Y8_ilu3        = ui8matrix(0, h-1, 0, w8-1); zero_ui8matrix(Y8_ilu3       , 0, h-1, 0, w8-1);
    uint8 **Y8_ilu3r       = ui8matrix(0, h-1, 0, w8-1); zero_ui8matrix(Y8_ilu3r      , 0, h-1, 0, w8-1);
    uint8 **Y8_elu2r       = ui8matrix(0, h-1, 0, w8-1); zero_ui8matrix(Y8_elu2r      , 0, h-1, 0, w8-1);
    uint8 **Y8_elu2rf      = ui8matrix(0, h-1, 0, w8-1); zero_ui8matrix(Y8_elu2rf     , 0, h-1, 0, w8-1);
    uint8 **Y8_ilu3_elu2rf = ui8matrix(0, h-1, 0, w8-1); zero_ui8matrix(Y8_ilu3_elu2rf, 0, h-1, 0, w8-1);
    
    uint8 **Y1_basic       = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_basic      , 0, h-1, 0, w1-1);
    uint8 **Y1_reg         = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_reg        , 0, h-1, 0, w1-1);
    uint8 **Y1_rot         = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_rot        , 0, h-1, 0, w1-1);
    uint8 **Y1_red         = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_red        , 0, h-1, 0, w1-1);
    uint8 **Y1_ilu3        = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_ilu3       , 0, h-1, 0, w1-1);
    uint8 **Y1_ilu3r       = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_ilu3r      , 0, h-1, 0, w1-1);
    uint8 **Y1_elu2r       = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_elu2r      , 0, h-1, 0, w1-1);
    uint8 **Y1_elu2rf      = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_elu2rf     , 0, h-1, 0, w1-1);
    uint8 **Y1_ilu3_elu2rf = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_ilu3_elu2rf, 0, h-1, 0, w1-1);
    
    //puts("rand");
    //rand_ui8matrix(X, 0, h-1, 0, w0-1, 1, 255); // niveau de gris [1,255]
    //rand1_ui8matrix(X, 0, h-1, 0, w0-1, 85); // binaire [0,1]
    rand1_ui8matrix(X, 0, h-1, 0, w0-1, 90); // binaire [0,1]
    //puts("pack/unpack");
    pack_ui8matrix  (X,  h, w1, X8); // bug ou pas bug si w1 ?
    unpack_ui8matrix(X8, h, w8, X1); // bug si w
    
    //puts("display");
    //display_ui8matrix (X,  0, h-1, 0, w0-1, "%5d", "X0");
    //display_ui8matrix (X8, 0, h-1, 0, w8-1, "%5x", "X8");
    //displayM_ui8matrix(X8, 0, h-1, 0, w8-1,        "X8");
    //display_ui8matrix (X1, 0, h-1, 0, w0-1, "%5d", "X1");
    c = compare_ui8matrix(X, 0, h-1, 0, w0-1, X1, "X0=X1"); putchar('\n');
    
    //puts("\n-- max3 --");
    erosion3_ui8matrix_basic               (X, 0, h-1, 0, w1-1, Y_basic      ); // puts("bas");
    erosion3_ui8matrix_reg                 (X, 0, h-1, 0, w1-1, Y_reg        ); // puts("reg");
    erosion3_ui8matrix_rot                 (X, 0, h-1, 0, w1-1, Y_rot        ); // puts("rot");
    erosion3_ui8matrix_red                 (X, 0, h-1, 0, w1-1, Y_red        ); // puts("red");
    erosion3_ui8matrix_ilu3                (X, 0, h-1, 0, w1-1, Y_ilu3       ); // puts("ilu3");
    erosion3_ui8matrix_ilu3_red            (X, 0, h-1, 0, w1-1, Y_ilu3r      ); // puts("ilu3_red");
    erosion3_ui8matrix_elu2_red            (X, 0, h-1, 0, w1-1, Y_elu2r      ); // puts("elu2_red");
    erosion3_ui8matrix_elu2_red_factor     (X, 0, h-1, 0, w1-1, Y_elu2rf     ); // puts("elu2_red_factor");
    erosion3_ui8matrix_ilu3_elu2_red_factor(X, 0, h-1, 0, w1-1, Y_ilu3_elu2rf); // puts("ilu3_elu2_red_factor");
    //puts("done\n");
    
    //display_ui8matrix(Y_basic      , 0, h-1, 0, w1-1, "%5d", "Y basic      ");
    //display_ui8matrix(Y_reg        , 0, h-1, 0, w1-1, "%5d", "Y reg        ");
    //display_ui8matrix(Y_rot        , 0, h-1, 0, w1-1, "%5d", "Y rot        ");
    //display_ui8matrix(Y_red        , 0, h-1, 0, w1-1, "%5d", "Y red        ");
    //display_ui8matrix(Y_ilu3       , 0, h-1, 0, w1-1, "%5d", "Y ilu3       ");
    //display_ui8matrix(Y_ilu3r      , 0, h-1, 0, w1-1, "%5d", "Y ilu3r      ");
    //display_ui8matrix(Y_elu2r      , 0, h-1, 0, w1-1, "%5d", "Y elu2r      ");
    //display_ui8matrix(Y_elu2rf     , 0, h-1, 0, w1-1, "%5d", "Y elu2rf     ");
    //display_ui8matrix(Y_ilu3_elu2rf, 0, h-1, 0, w1-1, "%5d", "Y ilu3 elu2rf");
    
    //puts("-- compare max --");
    c = compare_ui8matrix(Y_basic, 0, h-1, 0, w1-1, Y_reg        , "Y reg                       ");
    c = compare_ui8matrix(Y_basic, 0, h-1, 0, w1-1, Y_rot        , "Y rot                       ");
    c = compare_ui8matrix(Y_basic, 0, h-1, 0, w1-1, Y_red        , "Y red                       ");
    c = compare_ui8matrix(Y_basic, 0, h-1, 0, w1-1, Y_ilu3       , "Y ilu3                      ");
    c = compare_ui8matrix(Y_basic, 0, h-1, 0, w1-1, Y_ilu3r      , "Y ilu3 + red                ");
    c = compare_ui8matrix(Y_basic, 0, h-1, 0, w1-1, Y_elu2r      , "Y elu2 + red                ");
    c = compare_ui8matrix(Y_basic, 0, h-1, 0, w1-1, Y_elu2rf     , "Y ilu3 + red + factor       ");
    c = compare_ui8matrix(Y_basic, 0, h-1, 0, w1-1, Y_ilu3_elu2rf, "Y ilu3 + elu2 + red + factor");
    puts("----------");
    //puts("-- or3 (packed)--");
    erosion3_ui8matrix_swp8_basic               (X8, 0, h-1, 0, w8-1, Y8_basic      ); // puts("bas");
    erosion3_ui8matrix_swp8_reg                 (X8, 0, h-1, 0, w8-1, Y8_reg        ); // puts("reg");
    erosion3_ui8matrix_swp8_rot                 (X8, 0, h-1, 0, w8-1, Y8_rot        ); // puts("rot");
    erosion3_ui8matrix_swp8_red                 (X8, 0, h-1, 0, w8-1, Y8_red        ); // puts("red");
    erosion3_ui8matrix_swp8_ilu3                (X8, 0, h-1, 0, w8-1, Y8_ilu3       ); // puts("ilu3");
    erosion3_ui8matrix_swp8_ilu3_red            (X8, 0, h-1, 0, w8-1, Y8_ilu3r      ); // puts("ilu3_red");
    erosion3_ui8matrix_swp8_elu2_red            (X8, 0, h-1, 0, w8-1, Y8_elu2r      ); // puts("elu2_red");
    erosion3_ui8matrix_swp8_elu2_red_factor     (X8, 0, h-1, 0, w8-1, Y8_elu2rf     ); // puts("elu2_red_factor");
    erosion3_ui8matrix_swp8_ilu3_elu2_red_factor(X8, 0, h-1, 0, w8-1, Y8_ilu3_elu2rf); // puts("ilu3_elu2_red_factor");
    //puts("done\n");
    
    //puts("-- unpack --");
    unpack_ui8matrix(Y8_basic      , h, w8, Y1_basic      );
    unpack_ui8matrix(Y8_reg        , h, w8, Y1_reg        );
    unpack_ui8matrix(Y8_rot        , h, w8, Y1_rot        );
    unpack_ui8matrix(Y8_red        , h, w8, Y1_red        );
    unpack_ui8matrix(Y8_ilu3       , h, w8, Y1_ilu3       );
    unpack_ui8matrix(Y8_ilu3r      , h, w8, Y1_ilu3r      );
    unpack_ui8matrix(Y8_elu2r      , h, w8, Y1_elu2r      );
    unpack_ui8matrix(Y8_elu2rf     , h, w8, Y1_elu2rf     );
    unpack_ui8matrix(Y8_ilu3_elu2rf, h, w8, Y1_ilu3_elu2rf);
    
    //display_ui8matrix(Y1_basic      , 0, h-1, 0, w1-1, "%5d", "Y1 basic      ");
    //display_ui8matrix(Y1_reg        , 0, h-1, 0, w1-1, "%5d", "Y1 reg        ");
    //display_ui8matrix(Y1_rot        , 0, h-1, 0, w1-1, "%5d", "Y1 rot        ");
    //display_ui8matrix(Y1_red        , 0, h-1, 0, w1-1, "%5d", "Y1 red        ");
    //display_ui8matrix(Y1_ilu3       , 0, h-1, 0, w1-1, "%5d", "Y1 ilu3       ");
    //display_ui8matrix(Y1_ilu3r      , 0, h-1, 0, w1-1, "%5d", "Y1 ilu3r      ");
    //display_ui8matrix(Y1_elu2r      , 0, h-1, 0, w1-1, "%5d", "Y1 elu2r      ");
    //display_ui8matrix(Y1_elu2rf     , 0, h-1, 0, w1-1, "%5d", "Y1 elu2rf     ");
    //display_ui8matrix(Y1_ilu3_elu2rf, 0, h-1, 0, w1-1, "%5d", "Y1 ilu3_elu2rf");/**/
    
    //puts("-- compare packed or --");
    // c = compare_ui8matrix(Y8_basic, 0, h-1, 0, w8-1, Y8_reg        , "Y8 reg                       ");
    // c = compare_ui8matrix(Y8_basic, 0, h-1, 0, w8-1, Y8_rot        , "Y8 rot                       ");
    // c = compare_ui8matrix(Y8_basic, 0, h-1, 0, w8-1, Y8_red        , "Y8 red                       ");
    // c = compare_ui8matrix(Y8_basic, 0, h-1, 0, w8-1, Y8_ilu3       , "Y8 ilu3                      ");
    // c = compare_ui8matrix(Y8_basic, 0, h-1, 0, w8-1, Y8_ilu3r      , "Y8 ilu3 + red                ");
    // c = compare_ui8matrix(Y8_basic, 0, h-1, 0, w8-1, Y8_elu2r      , "Y8 elu2 + red                ");
    // c = compare_ui8matrix(Y8_basic, 0, h-1, 0, w8-1, Y8_elu2rf     , "Y8 ilu3 + red + factor       ");
    // c = compare_ui8matrix(Y8_basic, 0, h-1, 0, w8-1, Y8_ilu3_elu2rf, "Y8 ilu3 + elu2 + red + factor");
    //puts("done\n");/**/
    
    puts("\n-- SWP 8 compare unpacked results --\n");
    c = compare_ui8matrix(Y_basic, 0, h-1, 0, w0-1, Y1_basic      , "Y1 basic                     ");
    c = compare_ui8matrix(Y_basic, 0, h-1, 0, w0-1, Y1_reg        , "Y1 reg                       ");
    c = compare_ui8matrix(Y_basic, 0, h-1, 0, w0-1, Y1_rot        , "Y1 rot                       ");
    c = compare_ui8matrix(Y_basic, 0, h-1, 0, w0-1, Y1_red        , "Y1 red                       ");
    c = compare_ui8matrix(Y_basic, 0, h-1, 0, w0-1, Y1_ilu3       , "Y1 ilu3                      ");
    c = compare_ui8matrix(Y_basic, 0, h-1, 0, w0-1, Y1_ilu3r      , "Y1 ilu3 + red                ");
    c = compare_ui8matrix(Y_basic, 0, h-1, 0, w0-1, Y1_elu2r      , "Y1 elu2 + red                ");
    c = compare_ui8matrix(Y_basic, 0, h-1, 0, w0-1, Y1_elu2rf     , "Y1 elu2 + red + factor       ");
    c = compare_ui8matrix(Y_basic, 0, h-1, 0, w0-1, Y1_ilu3_elu2rf, "Y1 ilu3 + elu2 + red + factor");
    puts("----------");
    
    free_ui8matrix(X,             0-b, h-1+b, 0-b, w1-1+b);
    free_ui8matrix(X8,            0-b, h-1+b, 0-b, w8-1+b);
    free_ui8matrix(X1,            0-b, h-1+b, 0-b, w1-1+b);
    
    free_ui8matrix(Y_basic      , 0, h-1, 0, w1-1);
    free_ui8matrix(Y_reg        , 0, h-1, 0, w1-1);
    free_ui8matrix(Y_rot        , 0, h-1, 0, w1-1);
    free_ui8matrix(Y_red        , 0, h-1, 0, w1-1);
    free_ui8matrix(Y_ilu3       , 0, h-1, 0, w1-1);
    free_ui8matrix(Y_ilu3r      , 0, h-1, 0, w1-1);
    free_ui8matrix(Y_elu2r      , 0, h-1, 0, w1-1);
    free_ui8matrix(Y_elu2rf     , 0, h-1, 0, w1-1);
    free_ui8matrix(Y_ilu3_elu2rf, 0, h-1, 0, w1-1);
    
    free_ui8matrix(Y8_basic      , 0, h-1, 0, w8-1);
    free_ui8matrix(Y8_reg        , 0, h-1, 0, w8-1);
    free_ui8matrix(Y8_rot        , 0, h-1, 0, w8-1);
    free_ui8matrix(Y8_red        , 0, h-1, 0, w8-1);
    free_ui8matrix(Y8_ilu3       , 0, h-1, 0, w8-1);
    free_ui8matrix(Y8_ilu3r      , 0, h-1, 0, w8-1);
    free_ui8matrix(Y8_elu2r      , 0, h-1, 0, w8-1);
    free_ui8matrix(Y8_elu2rf     , 0, h-1, 0, w8-1);
    free_ui8matrix(Y8_ilu3_elu2rf, 0, h-1, 0, w8-1);
    
    free_ui8matrix(Y1_basic      , 0, h-1, 0, w1-1);
    free_ui8matrix(Y1_reg        , 0, h-1, 0, w1-1);
    free_ui8matrix(Y1_rot        , 0, h-1, 0, w1-1);
    free_ui8matrix(Y1_red        , 0, h-1, 0, w1-1);
    free_ui8matrix(Y1_ilu3       , 0, h-1, 0, w1-1);
    free_ui8matrix(Y1_ilu3r      , 0, h-1, 0, w1-1);
    free_ui8matrix(Y1_elu2r      , 0, h-1, 0, w1-1);
    free_ui8matrix(Y1_elu2rf     , 0, h-1, 0, w1-1);
    free_ui8matrix(Y1_ilu3_elu2rf, 0, h-1, 0, w1-1);
}
// --------------------------------------------------
int test_morpho_ouverture_routine_SWP8(int h, int w0)
// --------------------------------------------------
{
    int r = 1; // rayon des operateurs
    int w8  = w0 / 8;  if(w0 %  8) w8  =  w8+1;
    int w1 = 8 * w8; // w1 >= w
    //int w64 = w0 / 64; if(w0 % 64) w64 = w64+1;
    //int w1 = 64 * w64; // w1 >= w

    puts("--------------------------------------------------");
    printf("test_morpho_ouverture_routine h = %d w0 = %d w8 = %d, w1 = %d\n", h, w0, w8, w1);
    if(w1 > w0) puts("w1 > w0");
    
    char *format1; // affichage 1 bit/pixel
    char *format8; // affichage 8 bits/pixel
    
    int c, s = 0; // error

    format8 = "%3x";
    format1 = "%4d";
    format1 = "%2d";
    //format1 = "%1d";
   format1 = "%2x ";
    // ------------------
    // -- malloc input --
    // ------------------
    //puts("-- malloc input --");
    // X: 2r border
    uint8  **X      = ui8matrix (0 - 2*r, h-1 + 2*r, 0 - 2*r,  w1-1 + 2*r); zero_ui8matrix (X  , 0 - 2*r, h-1 + 2*r, 0 - 2*r, w1 -1 + 2*r);
    uint8  **X8     = ui8matrix (0 - 2*r, h-1 + 2*r, 0 - 1*r,  w8-1 + 1*r); zero_ui8matrix (X8 , 0 - 2*r, h-1 + 2*r, 0 - 1*r, w8 -1 + 1*r);
    //uint64 **X64    = ui64matrix(0 - 2*r, h-1 + 2*r, 0 - 1*r, w64-1 + 1*r); zero_ui64matrix(X64, 0 - 2*r, h-1 + 2*r, 0 - 1*r, w64-1 + 1*r);
    
    uint8  **X1     = ui8matrix(0 - 2*r, h-1 + 2*r, 0 - 2*r, w1-1 + 2*r); zero_ui8matrix(X1, 0 - 2*r, h-1 + 2*r, 0 - 2*r, w1-1 + 2*r);
    
    // ----------------------
    // -- malloc reference --
    // ----------------------
    //puts("-- malloc reference --");
    uint8 **T_ref   = ui8matrix(0 - r, h-1 + r, 0 - r, w1-1 + r); zero_ui8matrix(T_ref, 0 - r, h-1 + r, 0 - r, w1-1 + r);
    uint8 **Y_ref   = ui8matrix(0    , h-1    , 0    , w1-1    ); zero_ui8matrix(Y_ref, 0    , h-1    , 0    , w1-1    );
    
    // -------------------
    // -- malloc fusion --
    // -------------------
    //puts("-- malloc fusion --");
    uint8 **Y_basic                       = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_basic                      , 0, h-1, 0, w1-1);
    uint8 **Y_fusion_basic                = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_fusion_basic               , 0, h-1, 0, w1-1);
    uint8 **Y_fusion_red                  = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_fusion_red                 , 0, h-1, 0, w1-1);
    uint8 **Y_fusion_elu2_red             = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_fusion_red                 , 0, h-1, 0, w1-1);
    uint8 **Y_fusion_ilu5_red             = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_fusion_ilu5_red            , 0, h-1, 0, w1-1);
    uint8 **Y_fusion_ilu5_elu2_red        = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_fusion_ilu5_elu2_red       , 0, h-1, 0, w1-1);
    uint8 **Y_fusion_ilu5_elu2_red_factor = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_fusion_ilu5_elu2_red_factor, 0, h-1, 0, w1-1);
    uint8 **Y_fusion_ilu15_red            = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_fusion_ilu15_red           , 0, h-1, 0, w1-1);
    
    // ---------------------
    // -- malloc pipeline --
    // ---------------------
    //puts("-- malloc pipeline --");
    // T: 1r border
    uint8 **T_basic                         = ui8matrix(0 - r, h-1 + r, 0 - r, w1-1 + r); zero_ui8matrix(T_basic                        , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    
    uint8 **T_pipeline_basic                = ui8matrix(0 - r, h-1 + r, 0 - r, w1-1 + r); zero_ui8matrix(T_pipeline_basic               , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    uint8 **T_pipeline_red                  = ui8matrix(0 - r, h-1 + r, 0 - r, w1-1 + r); zero_ui8matrix(T_pipeline_red                 , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    uint8 **T_pipeline_ilu3_red             = ui8matrix(0 - r, h-1 + r, 0 - r, w1-1 + r); zero_ui8matrix(T_pipeline_ilu3_red            , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    uint8 **T_pipeline_elu2_red             = ui8matrix(0 - r, h-1 + r, 0 - r, w1-1 + r); zero_ui8matrix(T_pipeline_elu2_red            , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    uint8 **T_pipeline_elu2_red_factor      = ui8matrix(0 - r, h-1 + r, 0 - r, w1-1 + r); zero_ui8matrix(T_pipeline_elu2_red_factor     , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    uint8 **T_pipeline_ilu3_elu2_red        = ui8matrix(0 - r, h-1 + r, 0 - r, w1-1 + r); zero_ui8matrix(T_pipeline_ilu3_elu2_red       , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    uint8 **T_pipeline_ilu3_elu2_red_factor = ui8matrix(0 - r, h-1 + r, 0 - r, w1-1 + r); zero_ui8matrix(T_pipeline_ilu3_elu2_red_factor, 0 - r, h-1 + r, 0 - r, w1-1 + r);
    
    //uint8 **T_pipeline_mod3 = ui8matrix_mod(0 - 1*r, h-1 + 1*r, 0 - 1*r, w1-1 + 1*r, 3); zero_ui8matrix(T_pipeline_mod3, 0-1*r, h-1+1*r, 0-1*r, w1-1+1*r);
    //uint8 **T_pipeline_mod4 = ui8matrix_mod(0 - 1*r, h-1 + 1*r, 0 - 1*r, w1-1 + 1*r, 4); zero_ui8matrix(T_pipeline_mod4, 0-1*r, h-1+1*r, 0-1*r, w1-1+1*r);
    
    uint8 **Y_pipeline_basic                = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_pipeline_basic               , 0, h-1, 0, w1-1);
    uint8 **Y_pipeline_red                  = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_pipeline_red                 , 0, h-1, 0, w1-1);
    uint8 **Y_pipeline_ilu3_red             = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_pipeline_ilu3_red            , 0, h-1, 0, w1-1);
    uint8 **Y_pipeline_elu2_red             = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_pipeline_elu2_red            , 0, h-1, 0, w1-1);
    uint8 **Y_pipeline_elu2_red_factor      = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_pipeline_elu2_red_factor     , 0, h-1, 0, w1-1);
    uint8 **Y_pipeline_ilu3_elu2_red        = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_pipeline_ilu3_elu2_red       , 0, h-1, 0, w1-1);
    uint8 **Y_pipeline_ilu3_elu2_red_factor = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_pipeline_ilu3_elu2_red_factor, 0, h-1, 0, w1-1);
    
    // ------------------------
    // -- malloc fusion SWP8 --
    // ------------------------
    //puts("-- malloc fusion SWP8 --");
    uint8 **Y8_basic                       = ui8matrix(0, h-1, 0, w8-1); zero_ui8matrix(Y8_basic                      , 0, h-1, 0, w8-1);
    uint8 **Y8_fusion_basic                = ui8matrix(0, h-1, 0, w8-1); zero_ui8matrix(Y8_fusion_basic               , 0, h-1, 0, w8-1);
    uint8 **Y8_fusion_red                  = ui8matrix(0, h-1, 0, w8-1); zero_ui8matrix(Y8_fusion_red                 , 0, h-1, 0, w8-1);
    uint8 **Y8_fusion_ilu3_red             = ui8matrix(0, h-1, 0, w8-1); zero_ui8matrix(Y8_fusion_ilu3_red            , 0, h-1, 0, w8-1);
    uint8 **Y8_fusion_ilu3_elu2_red        = ui8matrix(0, h-1, 0, w8-1); zero_ui8matrix(Y8_fusion_ilu3_elu2_red       , 0, h-1, 0, w8-1);
    uint8 **Y8_fusion_ilu3_elu2_red_factor = ui8matrix(0, h-1, 0, w8-1); zero_ui8matrix(Y8_fusion_ilu3_elu2_red_factor, 0, h-1, 0, w8-1);
    
    uint8 **Y1_basic                       = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_basic                      , 0, h-1, 0, w1-1);
    uint8 **Y1_fusion_basic                = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_fusion_basic               , 0, h-1, 0, w1-1);
    uint8 **Y1_fusion_red                  = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_fusion_red                 , 0, h-1, 0, w1-1);
    uint8 **Y1_fusion_ilu3_red             = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_fusion_ilu3_red            , 0, h-1, 0, w1-1);
    uint8 **Y1_fusion_ilu3_elu2_red        = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_fusion_ilu3_elu2_red       , 0, h-1, 0, w1-1);
    uint8 **Y1_fusion_ilu3_elu2_red_factor = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_fusion_ilu3_elu2_red_factor, 0, h-1, 0, w1-1);
    
    // --------------------------
    // -- malloc pipeline SWP8 --
    // --------------------------
    //puts("-- malloc pipeline SWP8 --");
    uint8 **Y8_pipeline_basic                = ui8matrix(0, h-1, 0, w8-1); zero_ui8matrix(Y8_pipeline_basic               , 0, h-1, 0, w8-1);
    uint8 **Y8_pipeline_red                  = ui8matrix(0, h-1, 0, w8-1); zero_ui8matrix(Y8_pipeline_red                 , 0, h-1, 0, w8-1);
    uint8 **Y8_pipeline_ilu3_red             = ui8matrix(0, h-1, 0, w8-1); zero_ui8matrix(Y8_pipeline_ilu3_red            , 0, h-1, 0, w8-1);
    uint8 **Y8_pipeline_elu2_red             = ui8matrix(0, h-1, 0, w8-1); zero_ui8matrix(Y8_pipeline_elu2_red            , 0, h-1, 0, w8-1);
    uint8 **Y8_pipeline_elu2_red_factor      = ui8matrix(0, h-1, 0, w8-1); zero_ui8matrix(Y8_pipeline_elu2_red_factor     , 0, h-1, 0, w8-1);
    uint8 **Y8_pipeline_ilu3_elu2_red        = ui8matrix(0, h-1, 0, w8-1); zero_ui8matrix(Y8_pipeline_ilu3_elu2_red       , 0, h-1, 0, w8-1);
    uint8 **Y8_pipeline_ilu3_elu2_red_factor = ui8matrix(0, h-1, 0, w8-1); zero_ui8matrix(Y8_pipeline_ilu3_elu2_red_factor, 0, h-1, 0, w8-1);
    
    uint8 **Y1_pipeline_basic                = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_pipeline_basic               , 0, h-1, 0, w1-1);
    uint8 **Y1_pipeline_red                  = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_pipeline_red                 , 0, h-1, 0, w1-1);
    uint8 **Y1_pipeline_ilu3_red             = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_pipeline_ilu3_red            , 0, h-1, 0, w1-1);
    uint8 **Y1_pipeline_elu2_red             = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_pipeline_elu2_red            , 0, h-1, 0, w1-1);
    uint8 **Y1_pipeline_elu2_red_factor      = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_pipeline_elu2_red_factor     , 0, h-1, 0, w1-1);
    uint8 **Y1_pipeline_ilu3_elu2_red        = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_pipeline_ilu3_elu2_red       , 0, h-1, 0, w1-1);
    uint8 **Y1_pipeline_ilu3_elu2_red_factor = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_pipeline_ilu3_elu2_red_factor, 0, h-1, 0, w1-1);
    
    uint8 **T8_basic                         = ui8matrix(0 - r, h-1 + r, 0 - r, w8-1 + r); zero_ui8matrix(T8_basic                        , 0 - r, h-1 + r, 0 - r, w8-1 + r);
    uint8 **T8_pipeline_basic                = ui8matrix(0 - r, h-1 + r, 0 - r, w8-1 + r); zero_ui8matrix(T8_pipeline_basic               , 0 - r, h-1 + r, 0 - r, w8-1 + r);
    uint8 **T8_pipeline_red                  = ui8matrix(0 - r, h-1 + r, 0 - r, w8-1 + r); zero_ui8matrix(T8_pipeline_red                 , 0 - r, h-1 + r, 0 - r, w8-1 + r);
    uint8 **T8_pipeline_ilu3_red             = ui8matrix(0 - r, h-1 + r, 0 - r, w8-1 + r); zero_ui8matrix(T8_pipeline_ilu3_red            , 0 - r, h-1 + r, 0 - r, w8-1 + r);
    uint8 **T8_pipeline_elu2_red             = ui8matrix(0 - r, h-1 + r, 0 - r, w8-1 + r); zero_ui8matrix(T8_pipeline_elu2_red            , 0 - r, h-1 + r, 0 - r, w8-1 + r);
    uint8 **T8_pipeline_elu2_red_factor      = ui8matrix(0 - r, h-1 + r, 0 - r, w8-1 + r); zero_ui8matrix(T8_pipeline_elu2_red_factor     , 0 - r, h-1 + r, 0 - r, w8-1 + r);
    uint8 **T8_pipeline_ilu3_elu2_red        = ui8matrix(0 - r, h-1 + r, 0 - r, w8-1 + r); zero_ui8matrix(T8_pipeline_ilu3_elu2_red       , 0 - r, h-1 + r, 0 - r, w8-1 + r);
    uint8 **T8_pipeline_ilu3_elu2_red_factor = ui8matrix(0 - r, h-1 + r, 0 - r, w8-1 + r); zero_ui8matrix(T8_pipeline_ilu3_elu2_red_factor, 0 - r, h-1 + r, 0 - r, w8-1 + r);
    
    uint8 **T1_pipeline_basic                = ui8matrix(0 - r, h-1 + r, 0 - r, w1-1 + r); zero_ui8matrix(T1_pipeline_basic               , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    uint8 **T1_pipeline_red                  = ui8matrix(0 - r, h-1 + r, 0 - r, w1-1 + r); zero_ui8matrix(T1_pipeline_red                 , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    uint8 **T1_pipeline_ilu3_red             = ui8matrix(0 - r, h-1 + r, 0 - r, w1-1 + r); zero_ui8matrix(T1_pipeline_ilu3_red            , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    uint8 **T1_pipeline_elu2_red             = ui8matrix(0 - r, h-1 + r, 0 - r, w1-1 + r); zero_ui8matrix(T1_pipeline_elu2_red            , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    uint8 **T1_pipeline_elu2_red_factor      = ui8matrix(0 - r, h-1 + r, 0 - r, w1-1 + r); zero_ui8matrix(T1_pipeline_elu2_red_factor     , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    uint8 **T1_pipeline_ilu3_elu2_red        = ui8matrix(0 - r, h-1 + r, 0 - r, w1-1 + r); zero_ui8matrix(T1_pipeline_ilu3_elu2_red       , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    uint8 **T1_pipeline_ilu3_elu2_red_factor = ui8matrix(0 - r, h-1 + r, 0 - r, w1-1 + r); zero_ui8matrix(T1_pipeline_ilu3_elu2_red_factor, 0 - r, h-1 + r, 0 - r, w1-1 + r);
    
    // -------------------------
    // -- malloc fusion SWP64 --
    // -------------------------
    //puts("-- malloc fusion SWP64 --");
    //uint64 **Y64_basic                       = ui64matrix(0, h-1, 0, w64-1); zero_ui64matrix(Y64_basic                      , 0, h-1, 0, w64-1);
    //
    //uint64 **Y64_fusion_basic                = ui64matrix(0, h-1, 0, w64-1); zero_ui64matrix(Y64_fusion_basic               , 0, h-1, 0, w64-1);
    //uint64 **Y64_fusion_red                  = ui64matrix(0, h-1, 0, w64-1); zero_ui64matrix(Y64_fusion_red                 , 0, h-1, 0, w64-1);
    //uint64 **Y64_fusion_ilu3_red             = ui64matrix(0, h-1, 0, w64-1); zero_ui64matrix(Y64_fusion_ilu3_red            , 0, h-1, 0, w64-1);
    //uint64 **Y64_fusion_elu2_red             = ui64matrix(0, h-1, 0, w64-1); zero_ui64matrix(Y64_fusion_elu2_red            , 0, h-1, 0, w64-1);
    //uint64 **Y64_fusion_ilu3_elu2_red        = ui64matrix(0, h-1, 0, w64-1); zero_ui64matrix(Y64_fusion_ilu3_elu2_red       , 0, h-1, 0, w64-1);
    //uint64 **Y64_fusion_ilu3_elu2_red_factor = ui64matrix(0, h-1, 0, w64-1); zero_ui64matrix(Y64_fusion_ilu3_elu2_red_factor, 0, h-1, 0, w64-1);
    //
    //uint8 **Y1_basic                       = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_basic                      , 0, h-1, 0, w1-1);
    //uint8 **Y1_fusion_basic                = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_fusion_basic               , 0, h-1, 0, w1-1);
    //uint8 **Y1_fusion_red                  = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_fusion_red                 , 0, h-1, 0, w1-1);
    //uint8 **Y1_fusion_ilu3_red             = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_fusion_ilu3_red            , 0, h-1, 0, w1-1);
    //uint8 **Y1_fusion_elu2_red             = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_fusion_elu2_red            , 0, h-1, 0, w1-1);
    //uint8 **Y1_fusion_ilu3_elu2_red        = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_fusion_ilu3_elu2_red       , 0, h-1, 0, w1-1);
    //uint8 **Y1_fusion_ilu3_elu2_red_factor = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_fusion_ilu3_elu2_red_factor, 0, h-1, 0, w1-1);
    
    // --------------------------
    // -- malloc pipeline SWP64 --
    // --------------------------
    //puts("-- malloc pipeline SWP64 --");
    //uint64 **Y64_pipeline_basic                = ui64matrix(0, h-1, 0, w64-1); zero_ui64matrix(Y64_pipeline_basic               , 0, h-1, 0, w64-1);
    //uint64 **Y64_pipeline_red                  = ui64matrix(0, h-1, 0, w64-1); zero_ui64matrix(Y64_pipeline_red                 , 0, h-1, 0, w64-1);
    //uint64 **Y64_pipeline_ilu3_red             = ui64matrix(0, h-1, 0, w64-1); zero_ui64matrix(Y64_pipeline_ilu3_red            , 0, h-1, 0, w64-1);
    //uint64 **Y64_pipeline_elu2_red             = ui64matrix(0, h-1, 0, w64-1); zero_ui64matrix(Y64_pipeline_elu2_red            , 0, h-1, 0, w64-1);
    //uint64 **Y64_pipeline_elu2_red_factor      = ui64matrix(0, h-1, 0, w64-1); zero_ui64matrix(Y64_pipeline_elu2_red_factor     , 0, h-1, 0, w64-1);
    //uint64 **Y64_pipeline_ilu3_elu2_red        = ui64matrix(0, h-1, 0, w64-1); zero_ui64matrix(Y64_pipeline_ilu3_elu2_red       , 0, h-1, 0, w64-1);
    //uint64 **Y64_pipeline_ilu3_elu2_red_factor = ui64matrix(0, h-1, 0, w64-1); zero_ui64matrix(Y64_pipeline_ilu3_elu2_red_factor, 0, h-1, 0, w64-1);
    //
    //uint8 **Y1_pipeline_basic                = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_pipeline_basic               , 0, h-1, 0, w1-1);
    //uint8 **Y1_pipeline_red                  = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_pipeline_red                 , 0, h-1, 0, w1-1);
    //uint8 **Y1_pipeline_ilu3_red             = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_pipeline_ilu3_red            , 0, h-1, 0, w1-1);
    //uint8 **Y1_pipeline_elu2_red             = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_pipeline_elu2_red            , 0, h-1, 0, w1-1);
    //uint8 **Y1_pipeline_elu2_red_factor      = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_pipeline_elu2_red_factor     , 0, h-1, 0, w1-1);
    //uint8 **Y1_pipeline_ilu3_elu2_red        = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_pipeline_ilu3_elu2_red       , 0, h-1, 0, w1-1);
    //uint8 **Y1_pipeline_ilu3_elu2_red_factor = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_pipeline_ilu3_elu2_red_factor, 0, h-1, 0, w1-1);
    //
    //uint64 **T64_basic                         = ui64matrix(0 - r, h-1 + r, 0 - r, w64-1 + r); zero_ui64matrix(T64_basic                        , 0 - r, h-1 + r, 0 - r, w64-1 + r);
    //uint64 **T64_pipeline_basic                = ui64matrix(0 - r, h-1 + r, 0 - r, w64-1 + r); zero_ui64matrix(T64_pipeline_basic               , 0 - r, h-1 + r, 0 - r, w64-1 + r);
    //uint64 **T64_pipeline_red                  = ui64matrix(0 - r, h-1 + r, 0 - r, w64-1 + r); zero_ui64matrix(T64_pipeline_red                 , 0 - r, h-1 + r, 0 - r, w64-1 + r);
    //uint64 **T64_pipeline_ilu3_red             = ui64matrix(0 - r, h-1 + r, 0 - r, w64-1 + r); zero_ui64matrix(T64_pipeline_ilu3_red            , 0 - r, h-1 + r, 0 - r, w64-1 + r);
    //uint64 **T64_pipeline_elu2_red             = ui64matrix(0 - r, h-1 + r, 0 - r, w64-1 + r); zero_ui64matrix(T64_pipeline_elu2_red            , 0 - r, h-1 + r, 0 - r, w64-1 + r);
    //uint64 **T64_pipeline_elu2_red_factor      = ui64matrix(0 - r, h-1 + r, 0 - r, w64-1 + r); zero_ui64matrix(T64_pipeline_elu2_red_factor     , 0 - r, h-1 + r, 0 - r, w64-1 + r);
    //uint64 **T64_pipeline_ilu3_elu2_red        = ui64matrix(0 - r, h-1 + r, 0 - r, w64-1 + r); zero_ui64matrix(T64_pipeline_ilu3_elu2_red       , 0 - r, h-1 + r, 0 - r, w64-1 + r);
    //uint64 **T64_pipeline_ilu3_elu2_red_factor = ui64matrix(0 - r, h-1 + r, 0 - r, w64-1 + r); zero_ui64matrix(T64_pipeline_ilu3_elu2_red_factor, 0 - r, h-1 + r, 0 - r, w64-1 + r);
    //
    //uint8 **T1_pipeline_basic                = ui8matrix(0 - r, h-1 + r, 0 - r, w1-1 + r); zero_ui8matrix(T1_pipeline_basic               , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    //uint8 **T1_pipeline_red                  = ui8matrix(0 - r, h-1 + r, 0 - r, w1-1 + r); zero_ui8matrix(T1_pipeline_red                 , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    //uint8 **T1_pipeline_ilu3_red             = ui8matrix(0 - r, h-1 + r, 0 - r, w1-1 + r); zero_ui8matrix(T1_pipeline_ilu3_red            , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    //uint8 **T1_pipeline_elu2_red             = ui8matrix(0 - r, h-1 + r, 0 - r, w1-1 + r); zero_ui8matrix(T1_pipeline_elu2_red            , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    //uint8 **T1_pipeline_elu2_red_factor      = ui8matrix(0 - r, h-1 + r, 0 - r, w1-1 + r); zero_ui8matrix(T1_pipeline_elu2_red_factor     , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    //uint8 **T1_pipeline_ilu3_elu2_red        = ui8matrix(0 - r, h-1 + r, 0 - r, w1-1 + r); zero_ui8matrix(T1_pipeline_ilu3_elu2_red       , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    //uint8 **T1_pipeline_ilu3_elu2_red_factor = ui8matrix(0 - r, h-1 + r, 0 - r, w1-1 + r); zero_ui8matrix(T1_pipeline_ilu3_elu2_red_factor, 0 - r, h-1 + r, 0 - r, w1-1 + r);
    
    // -------------------
    // -- random number --
    // -------------------
    
    //puts("rand");
    //rand_ui8matrix(X, 0, h-1, 0, w-1, 1, 255);
    //init_ui8matrix_param(X, 0, h-1, 0, w0-1, 1, 1, 2); // genere des oveflows tres rapidement
    //init_ui8matrix_param(X, 0, h-1, 0, w0-1, 1, 1, 0);
    //rand_ui8matrix(X, 0, h-1, 0, w0-1, 1, 255); // niveau de gris [1,255]
    //rand1_ui8matrix(X, 0, h-1, 0, w0-1, 20); // binaire [0,1]
        
    //rand1_ui8matrix(X, 0, h-1, 0, w0-1, 80); // binaire [0,1] (afin d'avoir des '1' apres traitement et pas que des '0');
    rand1_ui8matrix(X, 0, h-1, 0, w0-1, 90); // binaire [0,1] (afin d'avoir des '1' apres traitement et pas que des '0');
    
    puts("-----------------");
    puts("-- pack/unpack --");
    puts("-----------------");
    
    pack_ui8matrix  (X,  h, w1, X8);
    unpack_ui8matrix(X8, h, w8, X1);
    c = compare_ui8matrix(X, 0, h-1, 0, w0-1, X1, "pack/unpack: X -> X8 -> X1"); putchar('\n');
    
    //pack_ui64matrix  (X,  h, w1, X64);
    //unpack_ui64matrix(X64, h, w64, X1);
    //c = compare_ui8matrix(X, 0, h-1, 0, w0-1, X1, "pack/unpack (X -> X64 -> X1"); putchar('\n');
    
    //display_ui8matrix(X,  0, h-1, 0, w0-1, format1, "X");putchar('\n');
    //display_ui8matrix(X8, 0, h-1, 0, w8-1, format1, "X8");putchar('\n');
    
    puts("-----------------");
    puts("-- compute ref --");
    puts("-----------------");
    
    ref_ouverture3_ui8matrix_basic(X, 0, h-1, 0, w0-1, T_ref, Y_ref);
    //display_ui8matrix(Y_ref,  0, h-1, 0, w0-1, format1, "Y_ref");
    
    // goto pour ne tester que des sous ensembles
    //goto fusion_swp8;
    
    puts("------------");
    puts("-- fusion --");
    puts("------------");
    
    // --ouverture3_ui8matrix_basic = min puis max
    ouverture3_ui8matrix_basic                      (X, 0, h-1, 0, w0-1, T_basic, Y_basic                      );
    ouverture3_ui8matrix_fusion_basic               (X, 0, h-1, 0, w0-1,          Y_fusion_basic               );
    ouverture3_ui8matrix_fusion_red                 (X, 0, h-1, 0, w0-1,          Y_fusion_red                 );
    ouverture3_ui8matrix_fusion_elu2_red            (X, 0, h-1, 0, w0-1,          Y_fusion_elu2_red            );
    ouverture3_ui8matrix_fusion_ilu5_red            (X, 0, h-1, 0, w0-1,          Y_fusion_ilu5_red            );
    ouverture3_ui8matrix_fusion_ilu5_elu2_red       (X, 0, h-1, 0, w0-1,          Y_fusion_ilu5_elu2_red       );
    ouverture3_ui8matrix_fusion_ilu5_elu2_red_factor(X, 0, h-1, 0, w0-1,          Y_fusion_ilu5_elu2_red_factor);
    ouverture3_ui8matrix_fusion_ilu15_red           (X, 0, h-1, 0, w0-1,          Y_fusion_ilu15_red           );
    
    //display_ui8matrix(Y_basic                      ,  0, h-1, 0, w0-1, format1, "Y_basic                      ");
    //display_ui8matrix(Y_fusion_red                      ,  0, h-1, 0, w0-1, format1, "Y_fuction_red                     ");

    //display_ui8matrix(Y_fusion_basic               ,  0, h-1, 0, w0-1, format1, "Y_fusion_basic               ");
    //display_ui8matrix(Y_fusion_ilu5_red            ,  0, h-1, 0, w0-1, format1, "Y_fusion_ilu5_red            ");
    //display_ui8matrix(Y_fusion_ilu5_elu2_red       ,  0, h-1, 0, w0-1, format1, "Y_fusion_ilu5_elu2_red       ");
    //display_ui8matrix(Y_fusion_ilu5_elu2_red_factor,  0, h-1, 0, w0-1, format1, "Y_fusion_ilu5_elu2_red_factor");
    //display_ui8matrix(Y_fusion_ilu15_red           ,  0, h-1, 0, w0-1, format1, "Y_fusion_ilu15_red           ");
    
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y_basic                      , "Y_basic                      "); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y_fusion_basic               , "Y_fusion_basic               "); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y_fusion_red                 , "Y_fusion_red                 "); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y_fusion_elu2_red            , "Y_fusion_elu2_red            "); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y_fusion_ilu5_red            , "Y_fusion_ilu5_red            "); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y_fusion_ilu5_elu2_red       , "Y_fusion_ilu5_elu2_red       "); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y_fusion_ilu5_elu2_red_factor, "Y_fusion_ilu5_elu2_red_factor"); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y_fusion_ilu15_red           , "Y_fusion_ilu15_red           "); s += c;
    puts("----------");
    
    //goto pipeline;
    //goto pipeline_mod;
    //goto pipeline_swp8;
    
pipeline:
    puts("--------------");
    puts("-- pipeline --");
    puts("--------------");
    
    ouverture3_ui8matrix_pipeline_basic               (X, 0, h-1, 0, w0-1, T_pipeline_basic               , Y_pipeline_basic               );
    ouverture3_ui8matrix_pipeline_red                 (X, 0, h-1, 0, w0-1, T_pipeline_red                 , Y_pipeline_red                 );
    ouverture3_ui8matrix_pipeline_ilu3_red            (X, 0, h-1, 0, w0-1, T_pipeline_ilu3_red            , Y_pipeline_ilu3_red            );
    ouverture3_ui8matrix_pipeline_elu2_red            (X, 0, h-1, 0, w0-1, T_pipeline_elu2_red            , Y_pipeline_elu2_red            );
    ouverture3_ui8matrix_pipeline_elu2_red_factor     (X, 0, h-1, 0, w0-1, T_pipeline_elu2_red_factor     , Y_pipeline_elu2_red_factor     );
    ouverture3_ui8matrix_pipeline_ilu3_elu2_red       (X, 0, h-1, 0, w0-1, T_pipeline_ilu3_elu2_red       , Y_pipeline_ilu3_elu2_red       );
    ouverture3_ui8matrix_pipeline_ilu3_elu2_red_factor(X, 0, h-1, 0, w0-1, T_pipeline_ilu3_elu2_red_factor, Y_pipeline_ilu3_elu2_red_factor);
    
    //display_ui8matrix(T_basic                        , 0-r, h-1+r, 0-r, w0-1+r, format1, "T_basic                        ");
    //display_ui8matrix(T_pipeline_basic               , 0-r, h-1+r, 0-r, w0-1+r, format1, "T_pipeline_basic               ");
    //display_ui8matrix(T_pipeline_red                 , 0-r, h-1+r, 0-r, w0-1+r, format1, "T_pipeline_red                 ");
    //display_ui8matrix(T_pipeline_ilu3_red            , 0-r, h-1+r, 0-r, w0-1+r, format1, "T_pipeline_ilu3_red            ");
    //display_ui8matrix(T_pipeline_elu2_red            , 0-r, h-1+r, 0-r, w0-1+r, format1, "T_pipeline_elu2_red            ");
    //display_ui8matrix(T_pipeline_elu2_red_factor     , 0-r, h-1+r, 0-r, w0-1+r, format1, "T_pipeline_elu2_red_factor     ");
    //display_ui8matrix(T_pipeline_ilu3_elu2_red       , 0-r, h-1+r, 0-r, w0-1+r, format1, "T_pipeline_ilu3_elu2_red       ");
    //display_ui8matrix(T_pipeline_ilu3_elu2_red_factor, 0-r, h-1+r, 0-r, w0-1+r, format1, "T_pipeline_ilu3_elu2_red_factor");
        
    // c = compare_ui8matrix(T_basic, 0-r, h-1+r, 0-r, w0-1+r, T_pipeline_basic               , "T_pipeline_basic               ");
    // c = compare_ui8matrix(T_basic, 0-r, h-1+r, 0-r, w0-1+r, T_pipeline_red                 , "T_pipeline_red                 ");
    // c = compare_ui8matrix(T_basic, 0-r, h-1+r, 0-r, w0-1+r, T_pipeline_ilu3_red            , "T_pipeline_ilu3_red            ");
    // c = compare_ui8matrix(T_basic, 0-r, h-1+r, 0-r, w0-1+r, T_pipeline_elu2_red            , "T_pipeline_elu2_red            ");
    // c = compare_ui8matrix(T_basic, 0-r, h-1+r, 0-r, w0-1+r, T_pipeline_elu2_red_factor     , "T_pipeline_elu2_red_factor     ");
    // c = compare_ui8matrix(T_basic, 0-r, h-1+r, 0-r, w0-1+r, T_pipeline_ilu3_elu2_red       , "T_pipeline_ilu3_elu2_red       ");
    // c = compare_ui8matrix(T_basic, 0-r, h-1+r, 0-r, w0-1+r, T_pipeline_ilu3_elu2_red_factor, "T_pipeline_ilu3_elu2_red_factor");
    //putchar('\n');
    
    //display_ui8matrix(Y_basic                        , 0, h-1, 0, w0-1, format1, "Y_basic                        ");
    //display_ui8matrix(Y_pipeline_basic               , 0, h-1, 0, w0-1, format1, "Y_pipeline_basic               ");
    //display_ui8matrix(Y_pipeline_red                 , 0, h-1, 0, w0-1, format1, "Y_pipeline_red                 ");
    //display_ui8matrix(Y_pipeline_ilu3_red            , 0, h-1, 0, w0-1, format1, "Y_pipeline_ilu3_red            ");
    //display_ui8matrix(Y_pipeline_elu2_red            , 0, h-1, 0, w0-1, format1, "Y_pipeline_elu2_red            ");
    //display_ui8matrix(Y_pipeline_elu2_red_factor     , 0, h-1, 0, w0-1, format1, "Y_pipeline_elu2_red_factor     ");
    //display_ui8matrix(Y_pipeline_ilu3_elu2_red       , 0, h-1, 0, w0-1, format1, "Y_pipeline_ilu3_elu2_red       ");
    //display_ui8matrix(Y_pipeline_ilu3_elu2_red_factor, 0, h-1, 0, w0-1, format1, "Y_pipeline_ilu3_elu2_red_factor");
    
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y_pipeline_basic               , "Y_pipeline_basic               "); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y_pipeline_red                 , "Y_pipeline_red                 "); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y_pipeline_ilu3_red            , "Y_pipeline_ilu3_red            "); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y_pipeline_elu2_red            , "Y_pipeline_elu2_red            "); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y_pipeline_elu2_red_factor     , "Y_pipeline_elu2_red_factor     "); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y_pipeline_ilu3_elu2_red       , "Y_pipeline_ilu3_elu2_red       "); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y_pipeline_ilu3_elu2_red_factor, "Y_pipeline_ilu3_elu2_red_factor"); s += c;
    puts("----------");
    
    //goto free;
    
pipeline_mod:
    //puts("------------------");
    //puts("-- pipeline_mod --");
    //puts("------------------");

    //zero_ui8matrix(Y_pipeline_basic               , 0-0*r, h-1+0*r, 0-0*r, w1-1+0*r);
    //zero_ui8matrix(Y_pipeline_red                 , 0-0*r, h-1+0*r, 0-0*r, w1-1+0*r);
    //zero_ui8matrix(Y_pipeline_ilu3_red            , 0-0*r, h-1+0*r, 0-0*r, w1-1+0*r);
    //zero_ui8matrix(Y_pipeline_elu2_red            , 0-0*r, h-1+0*r, 0-0*r, w1-1+0*r);
    //zero_ui8matrix(Y_pipeline_elu2_red_factor     , 0-0*r, h-1+0*r, 0-0*r, w1-1+0*r);
    //zero_ui8matrix(Y_pipeline_ilu3_elu2_red       , 0-0*r, h-1+0*r, 0-0*r, w1-1+0*r);
    //zero_ui8matrix(Y_pipeline_ilu3_elu2_red_factor, 0-0*r, h-1+0*r, 0-0*r, w1-1+0*r);
    //
    // Y: modular-matrix
    //ouverture3_ui8matrix_pipeline_basic               (X, 0, h-1, 0, w0-1, T_pipeline_mod3, Y_pipeline_basic               ); zero_ui8matrix(T_pipeline_mod3, 0-1*r, h-1+1*r, 0-1*r, w1-1+1*r);
    //ouverture3_ui8matrix_pipeline_red                 (X, 0, h-1, 0, w0-1, T_pipeline_mod3, Y_pipeline_red                 ); zero_ui8matrix(T_pipeline_mod3, 0-1*r, h-1+1*r, 0-1*r, w1-1+1*r);
    //ouverture3_ui8matrix_pipeline_ilu3_red            (X, 0, h-1, 0, w0-1, T_pipeline_mod3, Y_pipeline_ilu3_red            ); zero_ui8matrix(T_pipeline_mod3, 0-1*r, h-1+1*r, 0-1*r, w1-1+1*r);
    //
    //ouverture3_ui8matrix_pipeline_elu2_red            (X, 0, h-1, 0, w0-1, T_pipeline_mod4, Y_pipeline_elu2_red            ); zero_ui8matrix(T_pipeline_mod4, 0-1*r, h-1+1*r, 0-1*r, w1-1+1*r);
    //ouverture3_ui8matrix_pipeline_elu2_red_factor     (X, 0, h-1, 0, w0-1, T_pipeline_mod4, Y_pipeline_elu2_red_factor     ); zero_ui8matrix(T_pipeline_mod4, 0-1*r, h-1+1*r, 0-1*r, w1-1+1*r);
    //ouverture3_ui8matrix_pipeline_ilu3_elu2_red       (X, 0, h-1, 0, w0-1, T_pipeline_mod4, Y_pipeline_ilu3_elu2_red       ); zero_ui8matrix(T_pipeline_mod4, 0-1*r, h-1+1*r, 0-1*r, w1-1+1*r);
    //ouverture3_ui8matrix_pipeline_ilu3_elu2_red_factor(X, 0, h-1, 0, w0-1, T_pipeline_mod4, Y_pipeline_ilu3_elu2_red_factor); zero_ui8matrix(T_pipeline_mod4, 0-1*r, h-1+1*r, 0-1*r, w1-1+1*r);
    //
    //c = compare_ui8matrix(Y_basic, 0, h-1, 0, w0-1, Y_pipeline_basic               , "Y_pipeline_basic               ");
    //c = compare_ui8matrix(Y_basic, 0, h-1, 0, w0-1, Y_pipeline_red                 , "Y_pipeline_red                 ");
    //c = compare_ui8matrix(Y_basic, 0, h-1, 0, w0-1, Y_pipeline_ilu3_red            , "Y_pipeline_ilu3_red            ");
    //c = compare_ui8matrix(Y_basic, 0, h-1, 0, w0-1, Y_pipeline_elu2_red            , "Y_pipeline_elu2_red            ");
    //c = compare_ui8matrix(Y_basic, 0, h-1, 0, w0-1, Y_pipeline_elu2_red_factor     , "Y_pipeline_elu2_red_factor     ");
    //c = compare_ui8matrix(Y_basic, 0, h-1, 0, w0-1, Y_pipeline_ilu3_elu2_red       , "Y_pipeline_ilu3_elu2_red       ");
    //c = compare_ui8matrix(Y_basic, 0, h-1, 0, w0-1, Y_pipeline_ilu3_elu2_red_factor, "Y_pipeline_ilu3_elu2_red_factor");
    //putchar('\n');
    //
    //goto free;
    //goto pipeline_swp8;

fusion_swp8:
    puts("-----------------");
    puts("-- fusion SWP8 --");
    puts("-----------------");
    
    ouverture3_ui8matrix_swp8_basic                      (X8, 0, h-1, 0, w8-1, T8_basic, Y8_basic                      );
    ouverture3_ui8matrix_swp8_fusion_basic               (X8, 0, h-1, 0, w8-1,          Y8_fusion_basic               );
    ouverture3_ui8matrix_swp8_fusion_red                 (X8, 0, h-1, 0, w8-1,          Y8_fusion_red                 );
    ouverture3_ui8matrix_swp8_fusion_ilu3_red            (X8, 0, h-1, 0, w8-1,          Y8_fusion_ilu3_red            );
    ouverture3_ui8matrix_swp8_fusion_ilu3_elu2_red       (X8, 0, h-1, 0, w8-1,          Y8_fusion_ilu3_elu2_red       );
    ouverture3_ui8matrix_swp8_fusion_ilu3_elu2_red_factor(X8, 0, h-1, 0, w8-1,          Y8_fusion_ilu3_elu2_red_factor);
    
    //display_ui8matrix(Y8_basic                      ,  0, h-1, 0, w8-1, format8, "Y8_basic                      ");
    //display_ui8matrix(Y8_fusion_basic               ,  0, h-1, 0, w8-1, format8, "Y8_fusion_basic               ");
    //display_ui8matrix(Y8_fusion_red                 ,  0, h-1, 0, w8-1, format8, "Y8_fusion_red                 ");
    //display_ui8matrix(Y8_fusion_ilu3_red            ,  0, h-1, 0, w8-1, format8, "Y8_fusion_ilu3_red            ");
    //display_ui8matrix(Y8_fusion_ilu3_elu2_red       ,  0, h-1, 0, w8-1, format8, "Y8_fusion_ilu3_elu2_red       ");
    //display_ui8matrix(Y8_fusion_ilu3_elu2_red_factor,  0, h-1, 0, w8-1, format8, "Y8_fusion_ilu3_elu2_red_factor");
    
    //c = compare_ui8matrix(Y8_basic, 0, h-1, 0, w8-1, Y8_fusion_basic               , "Y8_fusion_basic               ");
    //c = compare_ui8matrix(Y8_basic, 0, h-1, 0, w8-1, Y8_fusion_red                 , "Y8_fusion_red                 ");
    //c = compare_ui8matrix(Y8_basic, 0, h-1, 0, w8-1, Y8_fusion_ilu3_red            , "Y8_fusion_ilu3_red            ");
    //c = compare_ui8matrix(Y8_basic, 0, h-1, 0, w8-1, Y8_fusion_ilu3_elu2_red       , "Y8_fusion_ilu3_elu2_red       ");
    //c = compare_ui8matrix(Y8_basic, 0, h-1, 0, w8-1, Y8_fusion_ilu3_elu2_red_factor, "Y8_fusion_ilu3_elu2_red_factor"); putchar('\n');
    
    unpack_ui8matrix(Y8_basic                      , h, w8, Y1_basic                      );
    unpack_ui8matrix(Y8_fusion_basic               , h, w8, Y1_fusion_basic               );
    unpack_ui8matrix(Y8_fusion_red                 , h, w8, Y1_fusion_red                 );
    unpack_ui8matrix(Y8_fusion_ilu3_red            , h, w8, Y1_fusion_ilu3_red            );
    unpack_ui8matrix(Y8_fusion_ilu3_elu2_red       , h, w8, Y1_fusion_ilu3_elu2_red       );
    unpack_ui8matrix(Y8_fusion_ilu3_elu2_red_factor, h, w8, Y1_fusion_ilu3_elu2_red_factor);
    
    //display_ui8matrix(Y1_basic                      ,  0, h-1, 0, w0-1, format1, "Y1_basic                      ");
    //display_ui8matrix(Y1_fusion_basic               ,  0, h-1, 0, w0-1, format1, "Y1_fusion_basic               ");
    //display_ui8matrix(Y1_fusion_red                 ,  0, h-1, 0, w0-1, format1, "Y1_fusion_red                 ");
    //display_ui8matrix(Y1_fusion_ilu3_red            ,  0, h-1, 0, w0-1, format1, "Y1_fusion_ilu3_red            ");
    //display_ui8matrix(Y1_fusion_ilu3_elu2_red       ,  0, h-1, 0, w0-1, format1, "Y1_fusion_ilu3_elu2_red       ");
    //display_ui8matrix(Y1_fusion_ilu3_elu2_red_factor,  0, h-1, 0, w0-1, format1, "Y1_fusion_ilu3_elu2_red_factor");
    
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y1_basic                      , "Y1_basic                      "); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y1_fusion_basic               , "Y1_fusion_basic               "); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y1_fusion_red                 , "Y1_fusion_red                 "); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y1_fusion_ilu3_red            , "Y1_fusion_ilu3_red            "); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y1_fusion_ilu3_elu2_red       , "Y1_fusion_ilu3_elu2_red       "); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y1_fusion_ilu3_elu2_red_factor, "Y1_fusion_ilu3_elu2_red_factor"); s += c;
    puts("----------");
    
    //goto free;

pipeline_swp8:
    puts("-------------------");
    puts("-- pipeline SWP8 --");
    puts("-------------------");
    
    ouverture3_ui8matrix_swp8_pipeline_basic               (X8, 0, h-1, 0, w8-1, T8_pipeline_basic               , Y8_pipeline_basic               );
    ouverture3_ui8matrix_swp8_pipeline_red                 (X8, 0, h-1, 0, w8-1, T8_pipeline_red                 , Y8_pipeline_red                 );
    ouverture3_ui8matrix_swp8_pipeline_ilu3_red            (X8, 0, h-1, 0, w8-1, T8_pipeline_ilu3_red            , Y8_pipeline_ilu3_red            );
    ouverture3_ui8matrix_swp8_pipeline_elu2_red            (X8, 0, h-1, 0, w8-1, T8_pipeline_elu2_red            , Y8_pipeline_elu2_red            );
    ouverture3_ui8matrix_swp8_pipeline_elu2_red_factor     (X8, 0, h-1, 0, w8-1, T8_pipeline_elu2_red_factor     , Y8_pipeline_elu2_red_factor     );
    ouverture3_ui8matrix_swp8_pipeline_ilu3_elu2_red       (X8, 0, h-1, 0, w8-1, T8_pipeline_ilu3_elu2_red       , Y8_pipeline_ilu3_elu2_red       );
    ouverture3_ui8matrix_swp8_pipeline_ilu3_elu2_red_factor(X8, 0, h-1, 0, w8-1, T8_pipeline_ilu3_elu2_red_factor, Y8_pipeline_ilu3_elu2_red_factor);
    
    //unpack_ui8matrix(T8_pipeline_basic               , h, w8, T1_pipeline_basic               );
    //unpack_ui8matrix(T8_pipeline_red                 , h, w8, T1_pipeline_red                 );
    //unpack_ui8matrix(T8_pipeline_ilu3_red            , h, w8, T1_pipeline_ilu3_red            );
    //unpack_ui8matrix(T8_pipeline_elu2_red            , h, w8, T1_pipeline_elu2_red            );
    //unpack_ui8matrix(T8_pipeline_elu2_red_factor     , h, w8, T1_pipeline_elu2_red_factor     );
    //unpack_ui8matrix(T8_pipeline_ilu3_elu2_red       , h, w8, T1_pipeline_ilu3_elu2_red       );
    //unpack_ui8matrix(T8_pipeline_ilu3_elu2_red_factor, h, w8, T1_pipeline_ilu3_elu2_red_factor);
        
    //display_ui8matrix(T1_pipeline_basic               , 0, h-1, 0, w1-1, format1, "T1_pipeline_basic               ");
    //display_ui8matrix(T1_pipeline_red                 , 0, h-1, 0, w1-1, format1, "T1_pipeline_red                 ");
    //display_ui8matrix(T1_pipeline_ilu3_red            , 0, h-1, 0, w1-1, format1, "T1_pipeline_ilu3_red            ");
    //display_ui8matrix(T1_pipeline_elu2_red            , 0, h-1, 0, w1-1, format1, "T1_pipeline_elu2_red            ");
    //display_ui8matrix(T1_pipeline_elu2_red_factor     , 0, h-1, 0, w1-1, format1, "T1_pipeline_elu2_red_factor     ");
    //display_ui8matrix(T1_pipeline_ilu3_elu2_red       , 0, h-1, 0, w1-1, format1, "T1_pipeline_ilu3_elu2_red       ");
    //display_ui8matrix(T1_pipeline_ilu3_elu2_red_factor, 0, h-1, 0, w1-1, format1, "T1_pipeline_ilu3_elu2_red_factor");
    
    unpack_ui8matrix(Y8_pipeline_basic               , h, w8, Y1_pipeline_basic               );
    unpack_ui8matrix(Y8_pipeline_red                 , h, w8, Y1_pipeline_red                 );
    unpack_ui8matrix(Y8_pipeline_ilu3_red            , h, w8, Y1_pipeline_ilu3_red            );
    unpack_ui8matrix(Y8_pipeline_elu2_red            , h, w8, Y1_pipeline_elu2_red            );
    unpack_ui8matrix(Y8_pipeline_elu2_red_factor     , h, w8, Y1_pipeline_elu2_red_factor     );
    unpack_ui8matrix(Y8_pipeline_ilu3_elu2_red       , h, w8, Y1_pipeline_ilu3_elu2_red       );
    unpack_ui8matrix(Y8_pipeline_ilu3_elu2_red_factor, h, w8, Y1_pipeline_ilu3_elu2_red_factor);
    
    //display_ui8matrix(Y_basic                         , 0, h-1, 0, w1-1, format1, "Y_basic                         ");
    //display_ui8matrix(Y1_pipeline_basic               , 0, h-1, 0, w1-1, format1, "Y1_pipeline_basic               ");
    //display_ui8matrix(Y1_pipeline_red                 , 0, h-1, 0, w1-1, format1, "Y1_pipeline_red                 ");
    //display_ui8matrix(Y1_pipeline_elu2_red            , 0, h-1, 0, w1-1, format1, "Y1_pipeline_elu2_red            ");
    //display_ui8matrix(Y1_pipeline_elu2_red_factor     , 0, h-1, 0, w1-1, format1, "Y1_pipeline_elu2_red_factor     ");
    //display_ui8matrix(Y1_pipeline_ilu3_elu2_red       , 0, h-1, 0, w1-1, format1, "Y1_pipeline_ilu3_elu2_red       ");
    //display_ui8matrix(Y1_pipeline_ilu3_elu2_red_factor, 0, h-1, 0, w1-1, format1, "Y1_pipeline_ilu3_elu2_red_factor");
    
    //display_ui8matrix(Y1_pipeline_ilu3_red            , 0, h-1, 0, w1-1, format1, "Y1_pipeline_ilu3_red            ");
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y1_pipeline_basic               , "Y1_pipeline_basic               "); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y1_pipeline_red                 , "Y1_pipeline_red                 "); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y1_pipeline_ilu3_red            , "Y1_pipeline_ilu3_red            "); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y1_pipeline_elu2_red            , "Y1_pipeline_elu2_red            "); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y1_pipeline_elu2_red_factor     , "Y1_pipeline_elu2_red_factor     "); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y1_pipeline_ilu3_elu2_red       , "Y1_pipeline_ilu3_elu2_red       "); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y1_pipeline_ilu3_elu2_red_factor, "Y1_pipeline_ilu3_elu2_red_factor"); s += c;
    puts("----------");

    fusion_swp64:
    
    //puts("------------------");
    //puts("-- fusion SWP64 --");
    //puts("------------------");
    
    // 1 fonction naive
    //ouverture3_ui64matrix_swp64_basic                      (X64, 0, h-1, 0, w64-1, T64_basic, Y64_basic                      );
    
    // 6 fonction fusion
    //ouverture3_ui64matrix_swp64_fusion_basic               (X64, 0, h-1, 0, w64-1,            Y64_fusion_basic               );
    //ouverture3_ui64matrix_swp64_fusion_red                 (X64, 0, h-1, 0, w64-1,            Y64_fusion_red                 );
    //ouverture3_ui64matrix_swp64_fusion_ilu3_red            (X64, 0, h-1, 0, w64-1,            Y64_fusion_ilu3_red            );
    //ouverture3_ui64matrix_swp64_fusion_elu2_red            (X64, 0, h-1, 0, w64-1,            Y64_fusion_elu2_red            );
    //ouverture3_ui64matrix_swp64_fusion_ilu3_elu2_red       (X64, 0, h-1, 0, w64-1,            Y64_fusion_ilu3_elu2_red       );
    //ouverture3_ui64matrix_swp64_fusion_ilu3_elu2_red_factor(X64, 0, h-1, 0, w64-1,            Y64_fusion_ilu3_elu2_red_factor);
    
    //display_ui64matrix(Y64_basic                      ,  0, h-1, 0, w64-1, format8, "Y64_basic                      ");
    //display_ui64matrix(Y64_fusion_basic               ,  0, h-1, 0, w64-1, format8, "Y64_fusion_basic               ");
    //display_ui64matrix(Y64_fusion_red                 ,  0, h-1, 0, w64-1, format8, "Y64_fusion_red                 ");
    //display_ui64matrix(Y64_fusion_ilu3_red            ,  0, h-1, 0, w64-1, format8, "Y64_fusion_ilu3_red            ");
    //display_ui64matrix(Y64_fusion_ilu3_elu2_red       ,  0, h-1, 0, w64-1, format8, "Y64_fusion_ilu3_elu2_red       ");
    //display_ui64matrix(Y64_fusion_ilu3_elu2_red_factor,  0, h-1, 0, w64-1, format8, "Y64_fusion_ilu3_elu2_red_factor");
    
    //c = compare_ui64matrix(Y64_basic, 0, h-1, 0, w64-1, Y64_fusion_basic               , "Y64_fusion_basic               ");
    //c = compare_ui64matrix(Y64_basic, 0, h-1, 0, w64-1, Y64_fusion_red                 , "Y64_fusion_red                 ");
    //c = compare_ui64matrix(Y64_basic, 0, h-1, 0, w64-1, Y64_fusion_ilu3_red            , "Y64_fusion_ilu3_red            ");
    //c = compare_ui64matrix(Y64_basic, 0, h-1, 0, w64-1, Y64_fusion_ilu3_elu2_red       , "Y64_fusion_ilu3_elu2_red       ");
    //c = compare_ui64matrix(Y64_basic, 0, h-1, 0, w64-1, Y64_fusion_ilu3_elu2_red_factor, "Y64_fusion_ilu3_elu2_red_factor"); putchar('\n');
    
    //unpack_ui64matrix(Y64_basic                      , h, w64, Y1_basic                      );
    //unpack_ui64matrix(Y64_fusion_basic               , h, w64, Y1_fusion_basic               );
    //unpack_ui64matrix(Y64_fusion_red                 , h, w64, Y1_fusion_red                 );
    //unpack_ui64matrix(Y64_fusion_ilu3_red            , h, w64, Y1_fusion_ilu3_red            );
    //unpack_ui64matrix(Y64_fusion_ilu3_elu2_red       , h, w64, Y1_fusion_ilu3_elu2_red       );
    //unpack_ui64matrix(Y64_fusion_ilu3_elu2_red_factor, h, w64, Y1_fusion_ilu3_elu2_red_factor);
    
    //display_ui8matrix(Y1_basic                      ,  0, h-1, 0, w0-1, format1, "Y1_basic                      ");
    //display_ui8matrix(Y1_fusion_basic               ,  0, h-1, 0, w0-1, format1, "Y1_fusion_basic               ");
    //display_ui8matrix(Y1_fusion_red                 ,  0, h-1, 0, w0-1, format1, "Y1_fusion_red                 ");
    //display_ui8matrix(Y1_fusion_ilu3_red            ,  0, h-1, 0, w0-1, format1, "Y1_fusion_ilu3_red            ");
    //display_ui8matrix(Y1_fusion_ilu3_elu2_red       ,  0, h-1, 0, w0-1, format1, "Y1_fusion_ilu3_elu2_red       ");
    //display_ui8matrix(Y1_fusion_ilu3_elu2_red_factor,  0, h-1, 0, w0-1, format1, "Y1_fusion_ilu3_elu2_red_factor");
    
    //c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y1_basic                      , "Y1_basic                      "); s += c;
    //c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y1_fusion_basic               , "Y1_fusion_basic               "); s += c;
    //c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y1_fusion_red                 , "Y1_fusion_red                 "); s += c;
    //c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y1_fusion_ilu3_red            , "Y1_fusion_ilu3_red            "); s += c;
    //c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y1_fusion_ilu3_elu2_red       , "Y1_fusion_ilu3_elu2_red       "); s += c;
    //c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y1_fusion_ilu3_elu2_red_factor, "Y1_fusion_ilu3_elu2_red_factor"); s += c;
    //puts("----------");
    
    //goto free;

    pipeline_swp64:
    //puts("--------------------");
    //puts("-- pipeline SWP64 --");
    //puts("--------------------");
    
    // 7 fonctions pipeline
    //ouverture3_ui64matrix_swp64_pipeline_basic               (X64, 0, h-1, 0, w8-1, T64_pipeline_basic               , Y64_pipeline_basic               );
    //ouverture3_ui64matrix_swp64_pipeline_red                 (X64, 0, h-1, 0, w8-1, T64_pipeline_red                 , Y64_pipeline_red                 );
    //ouverture3_ui64matrix_swp64_pipeline_ilu3_red            (X64, 0, h-1, 0, w8-1, T64_pipeline_ilu3_red            , Y64_pipeline_ilu3_red            );
    //ouverture3_ui64matrix_swp64_pipeline_elu2_red            (X64, 0, h-1, 0, w8-1, T64_pipeline_elu2_red            , Y64_pipeline_elu2_red            );
    //ouverture3_ui64matrix_swp64_pipeline_elu2_red_factor     (X64, 0, h-1, 0, w8-1, T64_pipeline_elu2_red_factor     , Y64_pipeline_elu2_red_factor     );
    //ouverture3_ui64matrix_swp64_pipeline_ilu3_elu2_red       (X64, 0, h-1, 0, w8-1, T64_pipeline_ilu3_elu2_red       , Y64_pipeline_ilu3_elu2_red       );
    //ouverture3_ui64matrix_swp64_pipeline_ilu3_elu2_red_factor(X64, 0, h-1, 0, w8-1, T64_pipeline_ilu3_elu2_red_factor, Y64_pipeline_ilu3_elu2_red_factor);
    
    //unpack_ui64matrix(T64_pipeline_basic               , h, w64, T1_pipeline_basic               );
    //unpack_ui64matrix(T64_pipeline_red                 , h, w64, T1_pipeline_red                 );
    //unpack_ui64matrix(T64_pipeline_ilu3_red            , h, w64, T1_pipeline_ilu3_red            );
    //unpack_ui64matrix(T64_pipeline_elu2_red            , h, w64, T1_pipeline_elu2_red            );
    //unpack_ui64matrix(T64_pipeline_elu2_red_factor     , h, w64, T1_pipeline_elu2_red_factor     );
    //unpack_ui64matrix(T64_pipeline_ilu3_elu2_red       , h, w64, T1_pipeline_ilu3_elu2_red       );
    //unpack_ui64matrix(T64_pipeline_ilu3_elu2_red_factor, h, w64, T1_pipeline_ilu3_elu2_red_factor);
        
    //display_ui8matrix(T1_pipeline_basic               , 0, h-1, 0, w1-1, format1, "T1_pipeline_basic               ");
    //display_ui8matrix(T1_pipeline_red                 , 0, h-1, 0, w1-1, format1, "T1_pipeline_red                 ");
    //display_ui8matrix(T1_pipeline_ilu3_red            , 0, h-1, 0, w1-1, format1, "T1_pipeline_ilu3_red            ");
    //display_ui8matrix(T1_pipeline_elu2_red            , 0, h-1, 0, w1-1, format1, "T1_pipeline_elu2_red            ");
    //display_ui8matrix(T1_pipeline_elu2_red_factor     , 0, h-1, 0, w1-1, format1, "T1_pipeline_elu2_red_factor     ");
    //display_ui8matrix(T1_pipeline_ilu3_elu2_red       , 0, h-1, 0, w1-1, format1, "T1_pipeline_ilu3_elu2_red       ");
    //display_ui8matrix(T1_pipeline_ilu3_elu2_red_factor, 0, h-1, 0, w1-1, format1, "T1_pipeline_ilu3_elu2_red_factor");
    
    //unpack_ui64matrix(Y64_pipeline_basic               , h, w64, Y1_pipeline_basic               );
    //unpack_ui64matrix(Y64_pipeline_red                 , h, w64, Y1_pipeline_red                 );
    //unpack_ui64matrix(Y64_pipeline_ilu3_red            , h, w64, Y1_pipeline_ilu3_red            );
    //unpack_ui64matrix(Y64_pipeline_elu2_red            , h, w64, Y1_pipeline_elu2_red            );
    //unpack_ui64matrix(Y64_pipeline_elu2_red_factor     , h, w64, Y1_pipeline_elu2_red_factor     );
    //unpack_ui64matrix(Y64_pipeline_ilu3_elu2_red       , h, w64, Y1_pipeline_ilu3_elu2_red       );
    //unpack_ui64matrix(Y64_pipeline_ilu3_elu2_red_factor, h, w64, Y1_pipeline_ilu3_elu2_red_factor);
    
    //display_ui8matrix(Y_basic                         , 0, h-1, 0, w1-1, format1, "Y_basic                         ");
    //display_ui8matrix(Y1_pipeline_basic               , 0, h-1, 0, w1-1, format1, "Y1_pipeline_basic               ");
    //display_ui8matrix(Y1_pipeline_red                 , 0, h-1, 0, w1-1, format1, "Y1_pipeline_red                 ");
    //display_ui8matrix(Y1_pipeline_ilu3_red            , 0, h-1, 0, w1-1, format1, "Y1_pipeline_ilu3_red            ");
    //display_ui8matrix(Y1_pipeline_elu2_red            , 0, h-1, 0, w1-1, format1, "Y1_pipeline_elu2_red            ");
    //display_ui8matrix(Y1_pipeline_elu2_red_factor     , 0, h-1, 0, w1-1, format1, "Y1_pipeline_elu2_red_factor     ");
    //display_ui8matrix(Y1_pipeline_ilu3_elu2_red       , 0, h-1, 0, w1-1, format1, "Y1_pipeline_ilu3_elu2_red       ");
    //display_ui8matrix(Y1_pipeline_ilu3_elu2_red_factor, 0, h-1, 0, w1-1, format1, "Y1_pipeline_ilu3_elu2_red_factor");
    
    //c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y1_pipeline_basic               , "Y1_pipeline_basic               "); s += c;
    //c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y1_pipeline_red                 , "Y1_pipeline_red                 "); s += c;
    //c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y1_pipeline_ilu3_red            , "Y1_pipeline_ilu3_red            "); s += c;
    //c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y1_pipeline_elu2_red            , "Y1_pipeline_elu2_red            "); s += c;
    //c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y1_pipeline_elu2_red_factor     , "Y1_pipeline_elu2_red_factor     "); s += c;
    //c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y1_pipeline_ilu3_elu2_red       , "Y1_pipeline_ilu3_elu2_red       "); s += c;
    //c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y1_pipeline_ilu3_elu2_red_factor, "Y1_pipeline_ilu3_elu2_red_factor"); s += c;
    //puts("----------");

free:
    
    // free volontairement en commentaire ...
    
    
    // ----------------
    // -- free input --
    // ----------------
    
    // X: 2r border
    free_ui8matrix(X , 0 - 2*r, h-1 + 2*r, 0 - 2*r, w1-1 + 2*r);
    free_ui8matrix(X8, 0 - 2*r, h-1 + 2*r, 0 - 1*r, w8-1 + 1*r);
    free_ui8matrix(X1, 0 - 2*r, h-1 + 2*r, 0 - 2*r, w1-1 + 2*r);
    
    // --------------------
    // -- free reference --
    // --------------------
    
    free_ui8matrix(T_ref, 0 - r, h-1 + r, 0 - r, w1-1 + r);
    free_ui8matrix(Y_ref, 0    , h-1    , 0    , w1-1    );
    
    // -------------------
    // -- free fusion --
    // -------------------
    
    free_ui8matrix(Y_basic                      , 0, h-1, 0, w1-1);
    free_ui8matrix(Y_fusion_basic               , 0, h-1, 0, w1-1);
    free_ui8matrix(Y_fusion_red                 , 0, h-1, 0, w1-1);
    free_ui8matrix(Y_fusion_elu2_red            , 0, h-1, 0, w1-1);
    free_ui8matrix(Y_fusion_ilu5_red            , 0, h-1, 0, w1-1);
    free_ui8matrix(Y_fusion_ilu5_elu2_red       , 0, h-1, 0, w1-1);
    free_ui8matrix(Y_fusion_ilu5_elu2_red_factor, 0, h-1, 0, w1-1);
    free_ui8matrix(Y_fusion_ilu15_red           , 0, h-1, 0, w1-1);
    
    // -------------------
    // -- free pipeline --
    // -------------------
    
    // T: 1r border
    free_ui8matrix(T_basic                        , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    free_ui8matrix(T_pipeline_basic               , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    free_ui8matrix(T_pipeline_red                 , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    free_ui8matrix(T_pipeline_ilu3_red            , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    free_ui8matrix(T_pipeline_elu2_red            , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    free_ui8matrix(T_pipeline_elu2_red_factor     , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    free_ui8matrix(T_pipeline_ilu3_elu2_red       , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    free_ui8matrix(T_pipeline_ilu3_elu2_red_factor, 0 - r, h-1 + r, 0 - r, w1-1 + r);
    
    //free_ui8matrix(T_pipeline_mod3, 0 - r, h-1 + r, 0 - r, w1-1 + r);
    //free_ui8matrix(T_pipeline_mod4, 0 - r, h-1 + r, 0 - r, w1-1 + r);
    
    free_ui8matrix(Y_pipeline_basic               , 0, h-1, 0, w1-1);
    free_ui8matrix(Y_pipeline_red                 , 0, h-1, 0, w1-1);
    free_ui8matrix(Y_pipeline_ilu3_red            , 0, h-1, 0, w1-1);
    free_ui8matrix(Y_pipeline_elu2_red            , 0, h-1, 0, w1-1);
    free_ui8matrix(Y_pipeline_elu2_red_factor     , 0, h-1, 0, w1-1);
    free_ui8matrix(Y_pipeline_ilu3_elu2_red       , 0, h-1, 0, w1-1);
    free_ui8matrix(Y_pipeline_ilu3_elu2_red_factor, 0, h-1, 0, w1-1);
    
    // -------------------------------
    // -- free fusion SWP8 + unpack --
    // -------------------------------
    
    free_ui8matrix(Y8_basic                      , 0, h-1, 0, w8-1);
    free_ui8matrix(Y8_fusion_basic               , 0, h-1, 0, w8-1);
    free_ui8matrix(Y8_fusion_red                 , 0, h-1, 0, w8-1);
    free_ui8matrix(Y8_fusion_ilu3_red            , 0, h-1, 0, w8-1);
    free_ui8matrix(Y8_fusion_ilu3_elu2_red       , 0, h-1, 0, w8-1);
    free_ui8matrix(Y8_fusion_ilu3_elu2_red_factor, 0, h-1, 0, w8-1);
    
    free_ui8matrix(Y1_basic                      , 0, h-1, 0, w1-1);
    free_ui8matrix(Y1_fusion_basic               , 0, h-1, 0, w1-1);
    free_ui8matrix(Y1_fusion_red                 , 0, h-1, 0, w1-1);
    free_ui8matrix(Y1_fusion_ilu3_red            , 0, h-1, 0, w1-1);
    free_ui8matrix(Y1_fusion_ilu3_elu2_red       , 0, h-1, 0, w1-1);
    free_ui8matrix(Y1_fusion_ilu3_elu2_red_factor, 0, h-1, 0, w1-1);
    
    // -----------------------------------
    // -- malloc pipeline SWP8 + unpack --
    // -----------------------------------
    
    free_ui8matrix(Y8_pipeline_basic               , 0, h-1, 0, w8-1);
    free_ui8matrix(Y8_pipeline_red                 , 0, h-1, 0, w8-1);
    free_ui8matrix(Y8_pipeline_ilu3_red            , 0, h-1, 0, w8-1);
    free_ui8matrix(Y8_pipeline_elu2_red            , 0, h-1, 0, w8-1);
    free_ui8matrix(Y8_pipeline_elu2_red_factor     , 0, h-1, 0, w8-1);
    free_ui8matrix(Y8_pipeline_ilu3_elu2_red       , 0, h-1, 0, w8-1);
    free_ui8matrix(Y8_pipeline_ilu3_elu2_red_factor, 0, h-1, 0, w8-1);
    
    free_ui8matrix(Y1_pipeline_basic               , 0, h-1, 0, w1-1);
    free_ui8matrix(Y1_pipeline_red                 , 0, h-1, 0, w1-1);
    free_ui8matrix(Y1_pipeline_ilu3_red            , 0, h-1, 0, w1-1);
    free_ui8matrix(Y1_pipeline_elu2_red            , 0, h-1, 0, w1-1);
    free_ui8matrix(Y1_pipeline_elu2_red_factor     , 0, h-1, 0, w1-1);
    free_ui8matrix(Y1_pipeline_ilu3_elu2_red       , 0, h-1, 0, w1-1);
    free_ui8matrix(Y1_pipeline_ilu3_elu2_red_factor, 0, h-1, 0, w1-1);

    free_ui8matrix(T1_pipeline_basic               , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    free_ui8matrix(T1_pipeline_red                 , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    free_ui8matrix(T1_pipeline_ilu3_red            , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    free_ui8matrix(T1_pipeline_elu2_red            , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    free_ui8matrix(T1_pipeline_elu2_red_factor     , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    free_ui8matrix(T1_pipeline_ilu3_elu2_red       , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    free_ui8matrix(T1_pipeline_ilu3_elu2_red_factor, 0 - r, h-1 + r, 0 - r, w1-1 + r);


    free_ui8matrix(T8_basic                        , 0 - r, h-1 + r, 0 - r, w8-1 + r);
    free_ui8matrix(T8_pipeline_basic               , 0 - r, h-1 + r, 0 - r, w8-1 + r);
    free_ui8matrix(T8_pipeline_red                 , 0 - r, h-1 + r, 0 - r, w8-1 + r);
    free_ui8matrix(T8_pipeline_ilu3_red            , 0 - r, h-1 + r, 0 - r, w8-1 + r);
    free_ui8matrix(T8_pipeline_elu2_red            , 0 - r, h-1 + r, 0 - r, w8-1 + r);
    free_ui8matrix(T8_pipeline_elu2_red_factor     , 0 - r, h-1 + r, 0 - r, w8-1 + r);
    free_ui8matrix(T8_pipeline_ilu3_elu2_red       , 0 - r, h-1 + r, 0 - r, w8-1 + r);
    free_ui8matrix(T8_pipeline_ilu3_elu2_red_factor, 0 - r, h-1 + r, 0 - r, w8-1 + r);
    
    puts("free done");
    
    return s;
}
// ---------------------------------------------------
int test_morpho_ouverture_routine_SWP64(int h, int w0)
// ---------------------------------------------------
{
    int r = 1; // rayon des operateurs
    int w64 = w0 / 64; if(w0 % 64) w64 = w64+1;
    int w1 = 64 * w64; // w1 >= w

    puts("--------------------------------------------------");
    printf("test_morpho_ouverture_routine_SWP64 h = %d w0 = %d w8 = %d, w1 = %d\n", h, w0, w64, w1);
    if(w1 > w0) puts("w1 > w0");
    
    char *format1; // affichage 1 bit/pixel
    char *format8; // affichage 8 bits/pixel
    
    int c, s = 0; // error

    format8 = "%3x";
    format1 = "%4d";
    format1 = "%2d";
    //format1 = "%1d";
   format1 = "%2x ";
    // ------------------
    // -- malloc input --
    // ------------------
    // puts("-- malloc input --");
    // X: 2r border
    uint8  **X      = ui8matrix (0 - 2*r, h-1 + 2*r, 0 - 2*r,  w1-1 + 2*r); zero_ui8matrix (X  , 0 - 2*r, h-1 + 2*r, 0 - 2*r, w1 -1 + 2*r);
    uint64 **X64    = ui64matrix(0 - 2*r, h-1 + 2*r, 0 - 1*r, w64-1 + 1*r); zero_ui64matrix(X64, 0 - 2*r, h-1 + 2*r, 0 - 1*r, w64-1 + 1*r);
    uint8  **X1     = ui8matrix(0 - 2*r, h-1 + 2*r, 0 - 2*r, w1-1 + 2*r);   zero_ui8matrix (X1 , 0 - 2*r, h-1 + 2*r, 0 - 2*r, w1-1  + 2*r);
    
    // ----------------------
    // -- malloc reference --
    // ----------------------
    // puts("-- malloc reference --");
    uint8 **T_ref   = ui8matrix(0 - r, h-1 + r, 0 - r, w1-1 + r); zero_ui8matrix(T_ref, 0 - r, h-1 + r, 0 - r, w1-1 + r);
    uint8 **Y_ref   = ui8matrix(0    , h-1    , 0    , w1-1    ); zero_ui8matrix(Y_ref, 0    , h-1    , 0    , w1-1    );
    
    // -------------------
    // -- malloc fusion --
    // -------------------
    // puts("-- malloc fusion --");
    uint8 **Y_basic                       = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_basic                      , 0, h-1, 0, w1-1);
    uint8 **Y_fusion_basic                = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_fusion_basic               , 0, h-1, 0, w1-1);
    uint8 **Y_fusion_red                  = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_fusion_red                 , 0, h-1, 0, w1-1);
    uint8 **Y_fusion_elu2_red             = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_fusion_red                 , 0, h-1, 0, w1-1);
    uint8 **Y_fusion_ilu5_red             = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_fusion_ilu5_red            , 0, h-1, 0, w1-1);
    uint8 **Y_fusion_ilu5_elu2_red        = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_fusion_ilu5_elu2_red       , 0, h-1, 0, w1-1);
    uint8 **Y_fusion_ilu5_elu2_red_factor = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_fusion_ilu5_elu2_red_factor, 0, h-1, 0, w1-1);
    uint8 **Y_fusion_ilu15_red            = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_fusion_ilu15_red           , 0, h-1, 0, w1-1);
    
    // ---------------------
    // -- malloc pipeline --
    // ---------------------
    // puts("-- malloc pipeline --");
    // T: 1r border
    uint8 **T_basic                         = ui8matrix(0 - r, h-1 + r, 0 - r, w1-1 + r); zero_ui8matrix(T_basic                        , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    
    uint8 **T_pipeline_basic                = ui8matrix(0 - r, h-1 + r, 0 - r, w1-1 + r); zero_ui8matrix(T_pipeline_basic               , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    uint8 **T_pipeline_red                  = ui8matrix(0 - r, h-1 + r, 0 - r, w1-1 + r); zero_ui8matrix(T_pipeline_red                 , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    uint8 **T_pipeline_ilu3_red             = ui8matrix(0 - r, h-1 + r, 0 - r, w1-1 + r); zero_ui8matrix(T_pipeline_ilu3_red            , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    uint8 **T_pipeline_elu2_red             = ui8matrix(0 - r, h-1 + r, 0 - r, w1-1 + r); zero_ui8matrix(T_pipeline_elu2_red            , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    uint8 **T_pipeline_elu2_red_factor      = ui8matrix(0 - r, h-1 + r, 0 - r, w1-1 + r); zero_ui8matrix(T_pipeline_elu2_red_factor     , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    uint8 **T_pipeline_ilu3_elu2_red        = ui8matrix(0 - r, h-1 + r, 0 - r, w1-1 + r); zero_ui8matrix(T_pipeline_ilu3_elu2_red       , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    uint8 **T_pipeline_ilu3_elu2_red_factor = ui8matrix(0 - r, h-1 + r, 0 - r, w1-1 + r); zero_ui8matrix(T_pipeline_ilu3_elu2_red_factor, 0 - r, h-1 + r, 0 - r, w1-1 + r);
    
    //uint8 **T_pipeline_mod3 = ui8matrix_mod(0 - 1*r, h-1 + 1*r, 0 - 1*r, w1-1 + 1*r, 3); zero_ui8matrix(T_pipeline_mod3, 0-1*r, h-1+1*r, 0-1*r, w1-1+1*r);
    //uint8 **T_pipeline_mod4 = ui8matrix_mod(0 - 1*r, h-1 + 1*r, 0 - 1*r, w1-1 + 1*r, 4); zero_ui8matrix(T_pipeline_mod4, 0-1*r, h-1+1*r, 0-1*r, w1-1+1*r);
    
    uint8 **Y_pipeline_basic                = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_pipeline_basic               , 0, h-1, 0, w1-1);
    uint8 **Y_pipeline_red                  = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_pipeline_red                 , 0, h-1, 0, w1-1);
    uint8 **Y_pipeline_ilu3_red             = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_pipeline_ilu3_red            , 0, h-1, 0, w1-1);
    uint8 **Y_pipeline_elu2_red             = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_pipeline_elu2_red            , 0, h-1, 0, w1-1);
    uint8 **Y_pipeline_elu2_red_factor      = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_pipeline_elu2_red_factor     , 0, h-1, 0, w1-1);
    uint8 **Y_pipeline_ilu3_elu2_red        = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_pipeline_ilu3_elu2_red       , 0, h-1, 0, w1-1);
    uint8 **Y_pipeline_ilu3_elu2_red_factor = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_pipeline_ilu3_elu2_red_factor, 0, h-1, 0, w1-1);
    
    // -------------------------
    // -- malloc fusion SWP64 --
    // -------------------------
    // puts("-- malloc fusion SWP64 --");
    uint64 **Y64_basic                       = ui64matrix(0, h-1, 0, w64-1); zero_ui64matrix(Y64_basic                      , 0, h-1, 0, w64-1);
    uint64 **Y64_fusion_basic                = ui64matrix(0, h-1, 0, w64-1); zero_ui64matrix(Y64_fusion_basic               , 0, h-1, 0, w64-1);
    uint64 **Y64_fusion_red                  = ui64matrix(0, h-1, 0, w64-1); zero_ui64matrix(Y64_fusion_red                 , 0, h-1, 0, w64-1);
    uint64 **Y64_fusion_ilu3_red             = ui64matrix(0, h-1, 0, w64-1); zero_ui64matrix(Y64_fusion_ilu3_red            , 0, h-1, 0, w64-1);
    uint64 **Y64_fusion_elu2_red             = ui64matrix(0, h-1, 0, w64-1); zero_ui64matrix(Y64_fusion_elu2_red            , 0, h-1, 0, w64-1);
    uint64 **Y64_fusion_ilu3_elu2_red        = ui64matrix(0, h-1, 0, w64-1); zero_ui64matrix(Y64_fusion_ilu3_elu2_red       , 0, h-1, 0, w64-1);
    uint64 **Y64_fusion_ilu3_elu2_red_factor = ui64matrix(0, h-1, 0, w64-1); zero_ui64matrix(Y64_fusion_ilu3_elu2_red_factor, 0, h-1, 0, w64-1);
    
    uint8 **Y1_basic                       = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_basic                      , 0, h-1, 0, w1-1);
    uint8 **Y1_fusion_basic                = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_fusion_basic               , 0, h-1, 0, w1-1);
    uint8 **Y1_fusion_red                  = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_fusion_red                 , 0, h-1, 0, w1-1);
    uint8 **Y1_fusion_ilu3_red             = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_fusion_ilu3_red            , 0, h-1, 0, w1-1);
    uint8 **Y1_fusion_elu2_red             = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_fusion_elu2_red            , 0, h-1, 0, w1-1);
    uint8 **Y1_fusion_ilu3_elu2_red        = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_fusion_ilu3_elu2_red       , 0, h-1, 0, w1-1);
    uint8 **Y1_fusion_ilu3_elu2_red_factor = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_fusion_ilu3_elu2_red_factor, 0, h-1, 0, w1-1);
    
    // --------------------------
    // -- malloc pipeline SWP64 --
    // --------------------------
    // puts("-- malloc pipeline SWP64 --");
    uint64 **Y64_pipeline_basic                = ui64matrix(0, h-1, 0, w64-1); zero_ui64matrix(Y64_pipeline_basic               , 0, h-1, 0, w64-1);
    uint64 **Y64_pipeline_red                  = ui64matrix(0, h-1, 0, w64-1); zero_ui64matrix(Y64_pipeline_red                 , 0, h-1, 0, w64-1);
    uint64 **Y64_pipeline_ilu3_red             = ui64matrix(0, h-1, 0, w64-1); zero_ui64matrix(Y64_pipeline_ilu3_red            , 0, h-1, 0, w64-1);
    uint64 **Y64_pipeline_elu2_red             = ui64matrix(0, h-1, 0, w64-1); zero_ui64matrix(Y64_pipeline_elu2_red            , 0, h-1, 0, w64-1);
    uint64 **Y64_pipeline_elu2_red_factor      = ui64matrix(0, h-1, 0, w64-1); zero_ui64matrix(Y64_pipeline_elu2_red_factor     , 0, h-1, 0, w64-1);
    uint64 **Y64_pipeline_ilu3_elu2_red        = ui64matrix(0, h-1, 0, w64-1); zero_ui64matrix(Y64_pipeline_ilu3_elu2_red       , 0, h-1, 0, w64-1);
    uint64 **Y64_pipeline_ilu3_elu2_red_factor = ui64matrix(0, h-1, 0, w64-1); zero_ui64matrix(Y64_pipeline_ilu3_elu2_red_factor, 0, h-1, 0, w64-1);
    
    uint8 **Y1_pipeline_basic                = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_pipeline_basic               , 0, h-1, 0, w1-1);
    uint8 **Y1_pipeline_red                  = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_pipeline_red                 , 0, h-1, 0, w1-1);
    uint8 **Y1_pipeline_ilu3_red             = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_pipeline_ilu3_red            , 0, h-1, 0, w1-1);
    uint8 **Y1_pipeline_elu2_red             = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_pipeline_elu2_red            , 0, h-1, 0, w1-1);
    uint8 **Y1_pipeline_elu2_red_factor      = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_pipeline_elu2_red_factor     , 0, h-1, 0, w1-1);
    uint8 **Y1_pipeline_ilu3_elu2_red        = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_pipeline_ilu3_elu2_red       , 0, h-1, 0, w1-1);
    uint8 **Y1_pipeline_ilu3_elu2_red_factor = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y1_pipeline_ilu3_elu2_red_factor, 0, h-1, 0, w1-1);
    
    uint64 **T64_basic                         = ui64matrix(0 - r, h-1 + r, 0 - r, w64-1 + r); zero_ui64matrix(T64_basic                        , 0 - r, h-1 + r, 0 - r, w64-1 + r);
    uint64 **T64_pipeline_basic                = ui64matrix(0 - r, h-1 + r, 0 - r, w64-1 + r); zero_ui64matrix(T64_pipeline_basic               , 0 - r, h-1 + r, 0 - r, w64-1 + r);
    uint64 **T64_pipeline_red                  = ui64matrix(0 - r, h-1 + r, 0 - r, w64-1 + r); zero_ui64matrix(T64_pipeline_red                 , 0 - r, h-1 + r, 0 - r, w64-1 + r);
    uint64 **T64_pipeline_ilu3_red             = ui64matrix(0 - r, h-1 + r, 0 - r, w64-1 + r); zero_ui64matrix(T64_pipeline_ilu3_red            , 0 - r, h-1 + r, 0 - r, w64-1 + r);
    uint64 **T64_pipeline_elu2_red             = ui64matrix(0 - r, h-1 + r, 0 - r, w64-1 + r); zero_ui64matrix(T64_pipeline_elu2_red            , 0 - r, h-1 + r, 0 - r, w64-1 + r);
    uint64 **T64_pipeline_elu2_red_factor      = ui64matrix(0 - r, h-1 + r, 0 - r, w64-1 + r); zero_ui64matrix(T64_pipeline_elu2_red_factor     , 0 - r, h-1 + r, 0 - r, w64-1 + r);
    uint64 **T64_pipeline_ilu3_elu2_red        = ui64matrix(0 - r, h-1 + r, 0 - r, w64-1 + r); zero_ui64matrix(T64_pipeline_ilu3_elu2_red       , 0 - r, h-1 + r, 0 - r, w64-1 + r);
    uint64 **T64_pipeline_ilu3_elu2_red_factor = ui64matrix(0 - r, h-1 + r, 0 - r, w64-1 + r); zero_ui64matrix(T64_pipeline_ilu3_elu2_red_factor, 0 - r, h-1 + r, 0 - r, w64-1 + r);
    
    uint8 **T1_pipeline_basic                = ui8matrix(0 - r, h-1 + r, 0 - r, w1-1 + r); zero_ui8matrix(T1_pipeline_basic               , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    uint8 **T1_pipeline_red                  = ui8matrix(0 - r, h-1 + r, 0 - r, w1-1 + r); zero_ui8matrix(T1_pipeline_red                 , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    uint8 **T1_pipeline_ilu3_red             = ui8matrix(0 - r, h-1 + r, 0 - r, w1-1 + r); zero_ui8matrix(T1_pipeline_ilu3_red            , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    uint8 **T1_pipeline_elu2_red             = ui8matrix(0 - r, h-1 + r, 0 - r, w1-1 + r); zero_ui8matrix(T1_pipeline_elu2_red            , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    uint8 **T1_pipeline_elu2_red_factor      = ui8matrix(0 - r, h-1 + r, 0 - r, w1-1 + r); zero_ui8matrix(T1_pipeline_elu2_red_factor     , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    uint8 **T1_pipeline_ilu3_elu2_red        = ui8matrix(0 - r, h-1 + r, 0 - r, w1-1 + r); zero_ui8matrix(T1_pipeline_ilu3_elu2_red       , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    uint8 **T1_pipeline_ilu3_elu2_red_factor = ui8matrix(0 - r, h-1 + r, 0 - r, w1-1 + r); zero_ui8matrix(T1_pipeline_ilu3_elu2_red_factor, 0 - r, h-1 + r, 0 - r, w1-1 + r);
    
    // -------------------
    // -- random number --
    // -------------------
    
    //puts("rand");
    //rand_ui8matrix(X, 0, h-1, 0, w-1, 1, 255);
    //init_ui8matrix_param(X, 0, h-1, 0, w0-1, 1, 1, 2); // genere des oveflows tres rapidement
    //init_ui8matrix_param(X, 0, h-1, 0, w0-1, 1, 1, 0);
    //rand_ui8matrix(X, 0, h-1, 0, w0-1, 1, 255); // niveau de gris [1,255]
    //rand1_ui8matrix(X, 0, h-1, 0, w0-1, 20); // binaire [0,1]
    
    
    //rand1_ui8matrix(X, 0, h-1, 0, w0-1, 80); // binaire [0,1] (afin d'avoir des '1' apres traitement et pas que des '0');
    rand1_ui8matrix(X, 0, h-1, 0, w0-1, 90); // binaire [0,1] (afin d'avoir des '1' apres traitement et pas que des '0');
    
    puts("-----------------");
    puts("-- pack/unpack --");
    puts("-----------------");
        
    pack_ui64matrix  (X  , h, w1 , X64);
    unpack_ui64matrix(X64, h, w64, X1 );
    c = compare_ui8matrix(X, 0, h-1, 0, w0-1, X1, "pack/unpack: X -> X64 -> X1"); putchar('\n');
    
    //display_ui8matrix(X,    0, h-1, 0, w0-1, format1, "X0");putchar('\n');
    //display_ui8matrix(X1,   0, h-1, 0, w0-1, format1, "X1");putchar('\n');
    //return 0;
    
    puts("-----------------");
    puts("-- compute ref --");
    puts("-----------------");
    
    ref_ouverture3_ui8matrix_basic(X, 0, h-1, 0, w0-1, T_ref, Y_ref);
    //display_ui8matrix(Y_ref, 0, h-1, 0, w0-1, format1, "Y_ref");
    
    puts("------------");
    puts("-- fusion --");
    puts("------------");
    
    // --ouverture3_ui8matrix_basic = min puis max
    ouverture3_ui8matrix_basic                      (X, 0, h-1, 0, w0-1, T_basic, Y_basic                      );
    ouverture3_ui8matrix_fusion_basic               (X, 0, h-1, 0, w0-1,          Y_fusion_basic               );
    ouverture3_ui8matrix_fusion_red                 (X, 0, h-1, 0, w0-1,          Y_fusion_red                 );
    ouverture3_ui8matrix_fusion_elu2_red            (X, 0, h-1, 0, w0-1,          Y_fusion_elu2_red            );
    ouverture3_ui8matrix_fusion_ilu5_red            (X, 0, h-1, 0, w0-1,          Y_fusion_ilu5_red            );
    ouverture3_ui8matrix_fusion_ilu5_elu2_red       (X, 0, h-1, 0, w0-1,          Y_fusion_ilu5_elu2_red       );
    ouverture3_ui8matrix_fusion_ilu5_elu2_red_factor(X, 0, h-1, 0, w0-1,          Y_fusion_ilu5_elu2_red_factor);
    ouverture3_ui8matrix_fusion_ilu15_red           (X, 0, h-1, 0, w0-1,          Y_fusion_ilu15_red           );
    
    //display_ui8matrix(Y_basic                      ,  0, h-1, 0, w0-1, format1, "Y_basic                      ");
    //display_ui8matrix(Y_fusion                     ,  0, h-1, 0, w0-1, format1, "Y_fusion                     ");
    //display_ui8matrix(Y_fusion_ilu5_red            ,  0, h-1, 0, w0-1, format1, "Y_fusion_ilu5_red            ");
    //display_ui8matrix(Y_fusion_ilu5_elu2_red       ,  0, h-1, 0, w0-1, format1, "Y_fusion_ilu5_elu2_red       ");
    //display_ui8matrix(Y_fusion_ilu5_elu2_red_factor,  0, h-1, 0, w0-1, format1, "Y_fusion_ilu5_elu2_red_factor");
    //display_ui8matrix(Y_fusion_ilu15_red           ,  0, h-1, 0, w0-1, format1, "Y_fusion_ilu15_red           ");
    
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y_basic                      , "Y_basic                      "); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y_fusion_basic               , "Y_fusion_basic               "); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y_fusion_red                 , "Y_fusion_red                 "); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y_fusion_elu2_red            , "Y_fusion_elu2_red            "); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y_fusion_ilu5_red            , "Y_fusion_ilu5_red            "); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y_fusion_ilu5_elu2_red       , "Y_fusion_ilu5_elu2_red       "); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y_fusion_ilu5_elu2_red_factor, "Y_fusion_ilu5_elu2_red_factor"); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y_fusion_ilu15_red           , "Y_fusion_ilu15_red           "); s += c;
    puts("----------");
    
    //goto pipeline;
    //goto pipeline_mod;
    //goto pipeline_swp8;
    
pipeline:
    puts("--------------");
    puts("-- pipeline --");
    puts("--------------");
    
    // T: full-matrix
    ouverture3_ui8matrix_pipeline_basic               (X, 0, h-1, 0, w0-1, T_pipeline_basic               , Y_pipeline_basic               );
    ouverture3_ui8matrix_pipeline_red                 (X, 0, h-1, 0, w0-1, T_pipeline_red                 , Y_pipeline_red                 );
    ouverture3_ui8matrix_pipeline_ilu3_red            (X, 0, h-1, 0, w0-1, T_pipeline_ilu3_red            , Y_pipeline_ilu3_red            );
    ouverture3_ui8matrix_pipeline_elu2_red            (X, 0, h-1, 0, w0-1, T_pipeline_elu2_red            , Y_pipeline_elu2_red            );
    ouverture3_ui8matrix_pipeline_elu2_red_factor     (X, 0, h-1, 0, w0-1, T_pipeline_elu2_red_factor     , Y_pipeline_elu2_red_factor     );
    ouverture3_ui8matrix_pipeline_ilu3_elu2_red       (X, 0, h-1, 0, w0-1, T_pipeline_ilu3_elu2_red       , Y_pipeline_ilu3_elu2_red       );
    ouverture3_ui8matrix_pipeline_ilu3_elu2_red_factor(X, 0, h-1, 0, w0-1, T_pipeline_ilu3_elu2_red_factor, Y_pipeline_ilu3_elu2_red_factor);
    
    //display_ui8matrix(X,                  0-2*r, h-1+2*r, 0-2*r, w0-1+2*r, format, "X +2r");
    
    //display_ui8matrix(T_basic                        , 0-r, h-1+r, 0-r, w0-1+r, format1, "T_basic                        ");
    //display_ui8matrix(T_pipeline                     , 0-r, h-1+r, 0-r, w0-1+r, format1, "T_pipeline_basic               ");
    //display_ui8matrix(T_pipeline_red                 , 0-r, h-1+r, 0-r, w0-1+r, format1, "T_pipeline_red                 ");
    //display_ui8matrix(T_pipeline_ilu3_red            , 0-r, h-1+r, 0-r, w0-1+r, format1, "T_pipeline_ilu3_red            ");
    //display_ui8matrix(T_pipeline_elu2_red            , 0-r, h-1+r, 0-r, w0-1+r, format1, "T_pipeline_elu2_red            ");
    //display_ui8matrix(T_pipeline_elu2_red_factor     , 0-r, h-1+r, 0-r, w0-1+r, format1, "T_pipeline_elu2_red_factor     ");
    //display_ui8matrix(T_pipeline_ilu3_elu2_red       , 0-r, h-1+r, 0-r, w0-1+r, format1, "T_pipeline_ilu3_elu2_red       ");
    //display_ui8matrix(T_pipeline_ilu3_elu2_red_factor, 0-r, h-1+r, 0-r, w0-1+r, format1, "T_pipeline_ilu3_elu2_red_factor");
        
    //c = compare_ui8matrix(T_basic, 0-r, h-1+r, 0-r, w0-1+r, T_pipeline_basic               , "T_pipeline_basic               ");
    //c = compare_ui8matrix(T_basic, 0-r, h-1+r, 0-r, w0-1+r, T_pipeline_red                 , "T_pipeline_red                 ");
    //c = compare_ui8matrix(T_basic, 0-r, h-1+r, 0-r, w0-1+r, T_pipeline_ilu3_red            , "T_pipeline_ilu3_red            ");
    //c = compare_ui8matrix(T_basic, 0-r, h-1+r, 0-r, w0-1+r, T_pipeline_elu2_red            , "T_pipeline_elu2_red            ");
    //c = compare_ui8matrix(T_basic, 0-r, h-1+r, 0-r, w0-1+r, T_pipeline_elu2_red_factor     , "T_pipeline_elu2_red_factor     ");
    //c = compare_ui8matrix(T_basic, 0-r, h-1+r, 0-r, w0-1+r, T_pipeline_ilu3_elu2_red       , "T_pipeline_ilu3_elu2_red       ");
    //c = compare_ui8matrix(T_basic, 0-r, h-1+r, 0-r, w0-1+r, T_pipeline_ilu3_elu2_red_factor, "T_pipeline_ilu3_elu2_red_factor");
    //putchar('\n');
    
    //display_ui8matrix(Y_basic                        , 0, h-1, 0, w0-1, format1, "Y_basic                        ");
    //display_ui8matrix(Y_pipeline_basic               , 0, h-1, 0, w0-1, format1, "Y_pipeline_basic               ");
    //display_ui8matrix(Y_pipeline_red                 , 0, h-1, 0, w0-1, format1, "Y_pipeline_red                 ");
    //display_ui8matrix(Y_pipeline_ilu3_red            , 0, h-1, 0, w0-1, format1, "Y_pipeline_ilu3_red            ");
    //display_ui8matrix(Y_pipeline_elu2_red            , 0, h-1, 0, w0-1, format1, "Y_pipeline_elu2_red            ");
    //display_ui8matrix(Y_pipeline_elu2_red_factor     , 0, h-1, 0, w0-1, format1, "Y_pipeline_elu2_red_factor     ");
    //display_ui8matrix(Y_pipeline_ilu3_elu2_red       , 0, h-1, 0, w0-1, format1, "Y_pipeline_ilu3_elu2_red       ");
    //display_ui8matrix(Y_pipeline_ilu3_elu2_red_factor, 0, h-1, 0, w0-1, format1, "Y_pipeline_ilu3_elu2_red_factor");
    
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y_pipeline_basic               , "Y_pipeline_basic               "); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y_pipeline_red                 , "Y_pipeline_red                 "); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y_pipeline_ilu3_red            , "Y_pipeline_ilu3_red            "); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y_pipeline_elu2_red            , "Y_pipeline_elu2_red            "); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y_pipeline_elu2_red_factor     , "Y_pipeline_elu2_red_factor     "); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y_pipeline_ilu3_elu2_red       , "Y_pipeline_ilu3_elu2_red       "); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y_pipeline_ilu3_elu2_red_factor, "Y_pipeline_ilu3_elu2_red_factor"); s += c;
    puts("----------");
    
    fusion_swp64:
    
    puts("------------------");
    puts("-- fusion SWP64 --");
    puts("------------------");
    
    ouverture3_ui64matrix_swp64_basic                      (X64, 0, h-1, 0, w64-1, T64_basic, Y64_basic                      );
    ouverture3_ui64matrix_swp64_fusion_basic               (X64, 0, h-1, 0, w64-1,            Y64_fusion_basic               );
    ouverture3_ui64matrix_swp64_fusion_red                 (X64, 0, h-1, 0, w64-1,            Y64_fusion_red                 );
    ouverture3_ui64matrix_swp64_fusion_ilu3_red            (X64, 0, h-1, 0, w64-1,            Y64_fusion_ilu3_red            );
    ouverture3_ui64matrix_swp64_fusion_elu2_red            (X64, 0, h-1, 0, w64-1,            Y64_fusion_elu2_red            );
    ouverture3_ui64matrix_swp64_fusion_ilu3_elu2_red       (X64, 0, h-1, 0, w64-1,            Y64_fusion_ilu3_elu2_red       );
    ouverture3_ui64matrix_swp64_fusion_ilu3_elu2_red_factor(X64, 0, h-1, 0, w64-1,            Y64_fusion_ilu3_elu2_red_factor);
    
    //display_ui64matrix(Y64_basic                      ,  0, h-1, 0, w64-1, format8, "Y64_basic                      ");
    //display_ui64matrix(Y64_fusion_basic               ,  0, h-1, 0, w64-1, format8, "Y64_fusion_basic               ");
    //display_ui64matrix(Y64_fusion_red                 ,  0, h-1, 0, w64-1, format8, "Y64_fusion_red                 ");
    //display_ui64matrix(Y64_fusion_ilu3_red            ,  0, h-1, 0, w64-1, format8, "Y64_fusion_ilu3_red            ");
    //display_ui64matrix(Y64_fusion_ilu3_elu2_red       ,  0, h-1, 0, w64-1, format8, "Y64_fusion_ilu3_elu2_red       ");
    //display_ui64matrix(Y64_fusion_ilu3_elu2_red_factor,  0, h-1, 0, w64-1, format8, "Y64_fusion_ilu3_elu2_red_factor");
    
    //c = compare_ui64matrix(Y64_basic, 0, h-1, 0, w64-1, Y64_fusion_basic               , "Y64_fusion_basic               ");
    //c = compare_ui64matrix(Y64_basic, 0, h-1, 0, w64-1, Y64_fusion_red                 , "Y64_fusion_red                 ");
    //c = compare_ui64matrix(Y64_basic, 0, h-1, 0, w64-1, Y64_fusion_ilu3_red            , "Y64_fusion_ilu3_red            ");
    //c = compare_ui64matrix(Y64_basic, 0, h-1, 0, w64-1, Y64_fusion_elu2_red            , "Y64_fusion_elu2_red            ");
    //c = compare_ui64matrix(Y64_basic, 0, h-1, 0, w64-1, Y64_fusion_ilu3_elu2_red       , "Y64_fusion_ilu3_elu2_red       ");
    //c = compare_ui64matrix(Y64_basic, 0, h-1, 0, w64-1, Y64_fusion_ilu3_elu2_red_factor, "Y64_fusion_ilu3_elu2_red_factor"); putchar('\n');
    
    unpack_ui64matrix(Y64_basic                      , h, w64, Y1_basic                      );
    unpack_ui64matrix(Y64_fusion_basic               , h, w64, Y1_fusion_basic               );
    unpack_ui64matrix(Y64_fusion_red                 , h, w64, Y1_fusion_red                 );
    unpack_ui64matrix(Y64_fusion_ilu3_red            , h, w64, Y1_fusion_ilu3_red            );
    unpack_ui64matrix(Y64_fusion_elu2_red            , h, w64, Y1_fusion_elu2_red            );
    unpack_ui64matrix(Y64_fusion_ilu3_elu2_red       , h, w64, Y1_fusion_ilu3_elu2_red       );
    unpack_ui64matrix(Y64_fusion_ilu3_elu2_red_factor, h, w64, Y1_fusion_ilu3_elu2_red_factor);
    
    //display_ui8matrix(Y1_basic                      ,  0, h-1, 0, w0-1, format1, "Y1_basic                      ");
    //display_ui8matrix(Y1_fusion_basic               ,  0, h-1, 0, w0-1, format1, "Y1_fusion_basic               ");
    //display_ui8matrix(Y1_fusion_red                 ,  0, h-1, 0, w0-1, format1, "Y1_fusion_red                 ");
    //display_ui8matrix(Y1_fusion_ilu3_red            ,  0, h-1, 0, w0-1, format1, "Y1_fusion_ilu3_red            ");
    //display_ui8matrix(Y1_fusion_elu2_red            ,  0, h-1, 0, w0-1, format1, "Y1_fusion_elu2_red            ");
    //display_ui8matrix(Y1_fusion_ilu3_elu2_red       ,  0, h-1, 0, w0-1, format1, "Y1_fusion_ilu3_elu2_red       ");
    //display_ui8matrix(Y1_fusion_ilu3_elu2_red_factor,  0, h-1, 0, w0-1, format1, "Y1_fusion_ilu3_elu2_red_factor");
    
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y1_basic                      , "Y1_basic                      "); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y1_fusion_basic               , "Y1_fusion_basic               "); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y1_fusion_red                 , "Y1_fusion_red                 "); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y1_fusion_ilu3_red            , "Y1_fusion_ilu3_red            "); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y1_fusion_elu2_red            , "Y1_fusion_elu2_red            "); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y1_fusion_ilu3_elu2_red       , "Y1_fusion_ilu3_elu2_red       "); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y1_fusion_ilu3_elu2_red_factor, "Y1_fusion_ilu3_elu2_red_factor"); s += c;
    puts("----------");
    
    //goto free;

    pipeline_swp64:
    puts("--------------------");
    puts("-- pipeline SWP64 --");
    puts("--------------------");
    
    ouverture3_ui64matrix_swp64_pipeline_basic               (X64, 0, h-1, 0, w64-1, T64_pipeline_basic               , Y64_pipeline_basic               );
    ouverture3_ui64matrix_swp64_pipeline_red                 (X64, 0, h-1, 0, w64-1, T64_pipeline_red                 , Y64_pipeline_red                 );
    ouverture3_ui64matrix_swp64_pipeline_ilu3_red            (X64, 0, h-1, 0, w64-1, T64_pipeline_ilu3_red            , Y64_pipeline_ilu3_red            );
    ouverture3_ui64matrix_swp64_pipeline_elu2_red            (X64, 0, h-1, 0, w64-1, T64_pipeline_elu2_red            , Y64_pipeline_elu2_red            );
    ouverture3_ui64matrix_swp64_pipeline_elu2_red_factor     (X64, 0, h-1, 0, w64-1, T64_pipeline_elu2_red_factor     , Y64_pipeline_elu2_red_factor     );
    ouverture3_ui64matrix_swp64_pipeline_ilu3_elu2_red       (X64, 0, h-1, 0, w64-1, T64_pipeline_ilu3_elu2_red       , Y64_pipeline_ilu3_elu2_red       );
    ouverture3_ui64matrix_swp64_pipeline_ilu3_elu2_red_factor(X64, 0, h-1, 0, w64-1, T64_pipeline_ilu3_elu2_red_factor, Y64_pipeline_ilu3_elu2_red_factor);
    
    //unpack_ui64matrix(T64_pipeline_basic               , h, w64, T1_pipeline_basic               );
    //unpack_ui64matrix(T64_pipeline_red                 , h, w64, T1_pipeline_red                 );
    //unpack_ui64matrix(T64_pipeline_ilu3_red            , h, w64, T1_pipeline_ilu3_red            );
    //unpack_ui64matrix(T64_pipeline_elu2_red            , h, w64, T1_pipeline_elu2_red            );
    //unpack_ui64matrix(T64_pipeline_elu2_red_factor     , h, w64, T1_pipeline_elu2_red_factor     );
    //unpack_ui64matrix(T64_pipeline_ilu3_elu2_red       , h, w64, T1_pipeline_ilu3_elu2_red       );
    //unpack_ui64matrix(T64_pipeline_ilu3_elu2_red_factor, h, w64, T1_pipeline_ilu3_elu2_red_factor);
        
    //display_ui64matrix(T1_pipeline_basic               , 0, h-1, 0, w1-1, format1, "T1_pipeline_basic               ");
    //display_ui64matrix(T1_pipeline_red                 , 0, h-1, 0, w1-1, format1, "T1_pipeline_red                 ");
    //display_ui64matrix(T1_pipeline_ilu3_red            , 0, h-1, 0, w1-1, format1, "T1_pipeline_ilu3_red            ");
    //display_ui64matrix(T1_pipeline_elu2_red            , 0, h-1, 0, w1-1, format1, "T1_pipeline_elu2_red            ");
    //display_ui64matrix(T1_pipeline_elu2_red_factor     , 0, h-1, 0, w1-1, format1, "T1_pipeline_elu2_red_factor     ");
    //display_ui64matrix(T1_pipeline_ilu3_elu2_red       , 0, h-1, 0, w1-1, format1, "T1_pipeline_ilu3_elu2_red       ");
    //display_ui64matrix(T1_pipeline_ilu3_elu2_red_factor, 0, h-1, 0, w1-1, format1, "T1_pipeline_ilu3_elu2_red_factor");
    
    unpack_ui64matrix(Y64_pipeline_basic               , h, w64, Y1_pipeline_basic               );
    unpack_ui64matrix(Y64_pipeline_red                 , h, w64, Y1_pipeline_red                 );
    unpack_ui64matrix(Y64_pipeline_ilu3_red            , h, w64, Y1_pipeline_ilu3_red            );
    unpack_ui64matrix(Y64_pipeline_elu2_red            , h, w64, Y1_pipeline_elu2_red            );
    unpack_ui64matrix(Y64_pipeline_elu2_red_factor     , h, w64, Y1_pipeline_elu2_red_factor     );
    unpack_ui64matrix(Y64_pipeline_ilu3_elu2_red       , h, w64, Y1_pipeline_ilu3_elu2_red       );
    unpack_ui64matrix(Y64_pipeline_ilu3_elu2_red_factor, h, w64, Y1_pipeline_ilu3_elu2_red_factor);
    
    //display_ui8matrix(Y_basic                         , 0, h-1, 0, w1-1, format1, "Y_basic                         ");
    //display_ui8matrix(Y1_pipeline_basic               , 0, h-1, 0, w1-1, format1, "Y1_pipeline_basic               ");
    //display_ui8matrix(Y1_pipeline_red                 , 0, h-1, 0, w1-1, format1, "Y1_pipeline_red                 ");
    //display_ui8matrix(Y1_pipeline_ilu3_red            , 0, h-1, 0, w1-1, format1, "Y1_pipeline_ilu3_red            ");
    //display_ui8matrix(Y1_pipeline_elu2_red            , 0, h-1, 0, w1-1, format1, "Y1_pipeline_elu2_red            ");
    //display_ui8matrix(Y1_pipeline_elu2_red_factor     , 0, h-1, 0, w1-1, format1, "Y1_pipeline_elu2_red_factor     ");
    //display_ui8matrix(Y1_pipeline_ilu3_elu2_red       , 0, h-1, 0, w1-1, format1, "Y1_pipeline_ilu3_elu2_red       ");
    //display_ui8matrix(Y1_pipeline_ilu3_elu2_red_factor, 0, h-1, 0, w1-1, format1, "Y1_pipeline_ilu3_elu2_red_factor");
    
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y1_pipeline_basic               , "Y1_pipeline_basic               "); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y1_pipeline_red                 , "Y1_pipeline_red                 "); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y1_pipeline_ilu3_red            , "Y1_pipeline_ilu3_red            "); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y1_pipeline_elu2_red            , "Y1_pipeline_elu2_red            "); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y1_pipeline_elu2_red_factor     , "Y1_pipeline_elu2_red_factor     "); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y1_pipeline_ilu3_elu2_red       , "Y1_pipeline_ilu3_elu2_red       "); s += c;
    c = compare_ui8matrix(Y_ref, 0, h-1, 0, w0-1, Y1_pipeline_ilu3_elu2_red_factor, "Y1_pipeline_ilu3_elu2_red_factor"); s += c;
    puts("----------");

free:
    
    // free volontairement en commentaire ...
    
    
    // ----------------
    // -- free input --
    // ----------------
    
    // X: 2r border
    free_ui8matrix(X , 0 - 2*r, h-1 + 2*r, 0 - 2*r, w1-1 + 2*r);
    free_ui64matrix(X64, 0 - 2*r, h-1 + 2*r, 0 - 1*r, w64-1 + 1*r);
    free_ui8matrix(X1, 0 - 2*r, h-1 + 2*r, 0 - 2*r, w1-1 + 2*r);
    
    // --------------------
    // -- free reference --
    // --------------------
    
    free_ui8matrix(T_ref, 0 - r, h-1 + r, 0 - r, w1-1 + r);
    free_ui8matrix(Y_ref, 0    , h-1    , 0    , w1-1    );
    
    // -------------------
    // -- free fusion --
    // -------------------
    
    free_ui8matrix(Y_basic                      , 0, h-1, 0, w1-1);
    free_ui8matrix(Y_fusion_basic               , 0, h-1, 0, w1-1);
    free_ui8matrix(Y_fusion_red                 , 0, h-1, 0, w1-1);
    free_ui8matrix(Y_fusion_elu2_red            , 0, h-1, 0, w1-1);
    free_ui8matrix(Y_fusion_ilu5_red            , 0, h-1, 0, w1-1);
    free_ui8matrix(Y_fusion_ilu5_elu2_red       , 0, h-1, 0, w1-1);
    free_ui8matrix(Y_fusion_ilu5_elu2_red_factor, 0, h-1, 0, w1-1);
    free_ui8matrix(Y_fusion_ilu15_red           , 0, h-1, 0, w1-1);
    
    // -------------------
    // -- free pipeline --
    // -------------------
    
    // T: 1r border
    free_ui8matrix(T_basic                        , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    free_ui8matrix(T_pipeline_basic               , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    free_ui8matrix(T_pipeline_red                 , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    free_ui8matrix(T_pipeline_ilu3_red            , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    free_ui8matrix(T_pipeline_elu2_red            , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    free_ui8matrix(T_pipeline_elu2_red_factor     , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    free_ui8matrix(T_pipeline_ilu3_elu2_red       , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    free_ui8matrix(T_pipeline_ilu3_elu2_red_factor, 0 - r, h-1 + r, 0 - r, w1-1 + r);
    
    //free_ui8matrix(T_pipeline_mod3, 0 - r, h-1 + r, 0 - r, w1-1 + r);
    //free_ui8matrix(T_pipeline_mod4, 0 - r, h-1 + r, 0 - r, w1-1 + r);
    
    free_ui8matrix(Y_pipeline_basic               , 0, h-1, 0, w1-1);
    free_ui8matrix(Y_pipeline_red                 , 0, h-1, 0, w1-1);
    free_ui8matrix(Y_pipeline_ilu3_red            , 0, h-1, 0, w1-1);
    free_ui8matrix(Y_pipeline_elu2_red            , 0, h-1, 0, w1-1);
    free_ui8matrix(Y_pipeline_elu2_red_factor     , 0, h-1, 0, w1-1);
    free_ui8matrix(Y_pipeline_ilu3_elu2_red       , 0, h-1, 0, w1-1);
    free_ui8matrix(Y_pipeline_ilu3_elu2_red_factor, 0, h-1, 0, w1-1);
    
    // -------------------------------
    // -- free fusion SWP64 + unpack --
    // -------------------------------
    
    free_ui64matrix(Y64_basic                      , 0, h-1, 0, w64-1);
    free_ui64matrix(Y64_fusion_basic               , 0, h-1, 0, w64-1);
    free_ui64matrix(Y64_fusion_red                 , 0, h-1, 0, w64-1);
    free_ui64matrix(Y64_fusion_elu2_red            , 0, h-1, 0, w64-1);
    free_ui64matrix(Y64_fusion_ilu3_red            , 0, h-1, 0, w64-1);
    free_ui64matrix(Y64_fusion_ilu3_elu2_red       , 0, h-1, 0, w64-1);
    free_ui64matrix(Y64_fusion_ilu3_elu2_red_factor, 0, h-1, 0, w64-1);
    
    free_ui8matrix(Y1_basic                      , 0, h-1, 0, w1-1);
    free_ui8matrix(Y1_fusion_basic               , 0, h-1, 0, w1-1);
    free_ui8matrix(Y1_fusion_red                 , 0, h-1, 0, w1-1);
    free_ui8matrix(Y1_fusion_elu2_red            , 0, h-1, 0, w1-1);
    free_ui8matrix(Y1_fusion_ilu3_red            , 0, h-1, 0, w1-1);
    free_ui8matrix(Y1_fusion_ilu3_elu2_red       , 0, h-1, 0, w1-1);
    free_ui8matrix(Y1_fusion_ilu3_elu2_red_factor, 0, h-1, 0, w1-1);
    
    free_ui8matrix(T1_pipeline_basic               , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    free_ui8matrix(T1_pipeline_red                 , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    free_ui8matrix(T1_pipeline_ilu3_red            , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    free_ui8matrix(T1_pipeline_elu2_red            , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    free_ui8matrix(T1_pipeline_elu2_red_factor     , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    free_ui8matrix(T1_pipeline_ilu3_elu2_red       , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    free_ui8matrix(T1_pipeline_ilu3_elu2_red_factor, 0 - r, h-1 + r, 0 - r, w1-1 + r);
    // -----------------------------------
    // -- malloc pipeline SWP8 + unpack --
    // -----------------------------------
    
    free_ui64matrix(Y64_pipeline_basic               , 0, h-1, 0, w64-1);
    free_ui64matrix(Y64_pipeline_red                 , 0, h-1, 0, w64-1);
    free_ui64matrix(Y64_pipeline_ilu3_red            , 0, h-1, 0, w64-1);
    free_ui64matrix(Y64_pipeline_elu2_red            , 0, h-1, 0, w64-1);
    free_ui64matrix(Y64_pipeline_elu2_red_factor     , 0, h-1, 0, w64-1);
    free_ui64matrix(Y64_pipeline_ilu3_elu2_red       , 0, h-1, 0, w64-1);
    free_ui64matrix(Y64_pipeline_ilu3_elu2_red_factor, 0, h-1, 0, w64-1);
    
    free_ui8matrix(Y1_pipeline_basic               , 0, h-1, 0, w1-1);
    free_ui8matrix(Y1_pipeline_red                 , 0, h-1, 0, w1-1);
    free_ui8matrix(Y1_pipeline_ilu3_red            , 0, h-1, 0, w1-1);
    free_ui8matrix(Y1_pipeline_elu2_red            , 0, h-1, 0, w1-1);
    free_ui8matrix(Y1_pipeline_elu2_red_factor     , 0, h-1, 0, w1-1);
    free_ui8matrix(Y1_pipeline_ilu3_elu2_red       , 0, h-1, 0, w1-1);
    free_ui8matrix(Y1_pipeline_ilu3_elu2_red_factor, 0, h-1, 0, w1-1);
    
    free_ui64matrix(T64_basic               , 0 - r, h-1 + r, 0 - r, w64-1 + r);
    free_ui64matrix(T64_pipeline_basic               , 0 - r, h-1 + r, 0 - r, w64-1 + r);
    free_ui64matrix(T64_pipeline_red                 , 0 - r, h-1 + r, 0 - r, w64-1 + r);
    free_ui64matrix(T64_pipeline_ilu3_red            , 0 - r, h-1 + r, 0 - r, w64-1 + r);
    free_ui64matrix(T64_pipeline_elu2_red            , 0 - r, h-1 + r, 0 - r, w64-1 + r);
    free_ui64matrix(T64_pipeline_elu2_red_factor     , 0 - r, h-1 + r, 0 - r, w64-1 + r);
    free_ui64matrix(T64_pipeline_ilu3_elu2_red       , 0 - r, h-1 + r, 0 - r, w64-1 + r);
    free_ui64matrix(T64_pipeline_ilu3_elu2_red_factor, 0 - r, h-1 + r, 0 - r, w64-1 + r);
    
    puts("free done");
    
    return s;
}
// ---------------------------------------
void test_morpho_max_routine(int h, int w0)
// ---------------------------------------
{
    int b = 1; // bord

    int w8 = w0 / 8; if(w0 % 8) w8 = w8+1;
    int w1 = 8 * w8; // w1 >= w

    puts("--------------------------------------------------");
    printf("test_morpho_max_routine h = %d w0 = %d w8 = %d, w1 = %d\n", h, w0, w8, w1);
    if(w1 > w0) puts("w1 > w0");
    uint8 **X;
    uint8 **Y_bas, **Y_reg, **Y_rot, **Y_red, **Y_ilu3, **Y_ilu3r, **Y_elu2r, **Y_elu2rf, **Y_ilu3_elu2rf;
    
    int c; // error

    //puts("malloc");
    X  = ui8matrix(0-b, h-1+b, 0-b, w1-1+b);
    
    Y_bas         = ui8matrix(0, h-1, 0, w1-1);
    Y_reg         = ui8matrix(0, h-1, 0, w1-1);
    Y_rot         = ui8matrix(0, h-1, 0, w1-1);
    Y_red         = ui8matrix(0, h-1, 0, w1-1);
    Y_ilu3        = ui8matrix(0, h-1, 0, w1-1);
    Y_ilu3r       = ui8matrix(0, h-1, 0, w1-1);
    Y_elu2r       = ui8matrix(0, h-1, 0, w1-1);
    Y_elu2rf      = ui8matrix(0, h-1, 0, w1-1);
    Y_ilu3_elu2rf = ui8matrix(0, h-1, 0, w1-1);/**/
        
    //puts("zero");
    zero_ui8matrix(X,  0-b, h-1+b, 0-b, w1-1+b);
    
    zero_ui8matrix(Y_bas,         0, h-1, 0, w1-1);
    zero_ui8matrix(Y_reg,         0, h-1, 0, w1-1);
    zero_ui8matrix(Y_rot,         0, h-1, 0, w1-1);
    zero_ui8matrix(Y_red,         0, h-1, 0, w1-1);
    zero_ui8matrix(Y_ilu3,        0, h-1, 0, w1-1);
    zero_ui8matrix(Y_ilu3r,       0, h-1, 0, w1-1);
    zero_ui8matrix(Y_elu2r,       0, h-1, 0, w1-1);
    zero_ui8matrix(Y_elu2rf,      0, h-1, 0, w1-1);
    zero_ui8matrix(Y_ilu3_elu2rf, 0, h-1, 0, w1-1);
    
    //puts("rand");
    
    // deux generateurs aleatoire au choix
    //rand_ui8matrix(X, 0, h-1, 0, w0-1, 1, 255); // niveau de gris [1,255]
    rand1_ui8matrix(X, 0, h-1, 0, w0-1, 20); // binaire [0,1]: pourcentage de point a 1
        
    //puts("display");
    //display_ui8matrix (X,  0, h-1, 0, w0-1, "%5d", "X0");
   
    //puts("\n-- max3 --");
    max3_ui8matrix_basic               (X, 0, h-1, 0, w0-1, Y_bas);         // puts("bas");
    max3_ui8matrix_reg                 (X, 0, h-1, 0, w0-1, Y_reg);         // puts("reg");
    max3_ui8matrix_rot                 (X, 0, h-1, 0, w0-1, Y_rot);         // puts("rot");
    max3_ui8matrix_red                 (X, 0, h-1, 0, w0-1, Y_red);         // puts("red");
    max3_ui8matrix_ilu3                (X, 0, h-1, 0, w0-1, Y_ilu3);        // puts("ilu3");
    max3_ui8matrix_ilu3_red            (X, 0, h-1, 0, w0-1, Y_ilu3r);       // puts("ilu3_red");
    max3_ui8matrix_elu2_red            (X, 0, h-1, 0, w0-1, Y_elu2r);       // puts("elu2_red");
    max3_ui8matrix_elu2_red_factor     (X, 0, h-1, 0, w0-1, Y_elu2rf);      // puts("elu2_red_factor");
    max3_ui8matrix_ilu3_elu2_red_factor(X, 0, h-1, 0, w0-1, Y_ilu3_elu2rf); // puts("ilu3_elu2_red_factor");
    //puts("done\n");
    
    /*display_ui8matrix(Y_bas,         0, h-1, 0, w0-1, "%5d", "Y bas        ");
    display_ui8matrix(Y_reg,         0, h-1, 0, w0-1, "%5d", "Y reg        ");
    display_ui8matrix(Y_reg,         0, h-1, 0, w0-1, "%5d", "Y reg        ");
    display_ui8matrix(Y_rot,         0, h-1, 0, w0-1, "%5d", "Y rot        ");
    display_ui8matrix(Y_red,         0, h-1, 0, w0-1, "%5d", "Y red        ");
    display_ui8matrix(Y_ilu3,        0, h-1, 0, w0-1, "%5d", "Y ilu3       ");
    display_ui8matrix(Y_ilu3r,       0, h-1, 0, w0-1, "%5d", "Y ilu3r      ");
    display_ui8matrix(Y_elu2r,       0, h-1, 0, w0-1, "%5d", "Y elu2r      ");
    display_ui8matrix(Y_elu2rf,      0, h-1, 0, w0-1, "%5d", "Y elu2rf     ");
    display_ui8matrix(Y_ilu3_elu2rf, 0, h-1, 0, w0-1, "%5d", "Y ilu3 elu2rf");/**/
    
    //puts("-- compare max --");
    c = compare_ui8matrix(Y_bas, 0, h-1, 0, w0-1, Y_reg        , "Y reg                       ");
    c = compare_ui8matrix(Y_bas, 0, h-1, 0, w0-1, Y_rot        , "Y rot                       ");
    c = compare_ui8matrix(Y_bas, 0, h-1, 0, w0-1, Y_red        , "Y red                       ");
    c = compare_ui8matrix(Y_bas, 0, h-1, 0, w0-1, Y_ilu3       , "Y ilu3                      ");
    c = compare_ui8matrix(Y_bas, 0, h-1, 0, w0-1, Y_ilu3r      , "Y ilu3 + red                ");
    c = compare_ui8matrix(Y_bas, 0, h-1, 0, w0-1, Y_elu2r      , "Y elu2 + red                ");
    c = compare_ui8matrix(Y_bas, 0, h-1, 0, w0-1, Y_elu2rf     , "Y ilu3 + red + factor       ");
    c = compare_ui8matrix(Y_bas, 0, h-1, 0, w0-1, Y_ilu3_elu2rf, "Y ilu3 + elu2 + red + factor");
    putchar('\n');/**/
    //puts("done\n");

    // free
    free_ui8matrix(X ,            0-b, h-1+b, 0-b, w1-1+b);
    
    free_ui8matrix(Y_bas        , 0,   h-1,   0,   w1-1);
    free_ui8matrix(Y_reg        , 0,   h-1,   0,   w1-1);
    free_ui8matrix(Y_rot        , 0,   h-1,   0,   w1-1);
    free_ui8matrix(Y_red        , 0,   h-1,   0,   w1-1);
    free_ui8matrix(Y_ilu3       , 0,   h-1,   0,   w1-1);
    free_ui8matrix(Y_ilu3r      , 0,   h-1,   0,   w1-1);
    free_ui8matrix(Y_elu2r      , 0,   h-1,   0,   w1-1);
    free_ui8matrix(Y_elu2rf     , 0,   h-1,   0,   w1-1);
    free_ui8matrix(Y_ilu3_elu2rf, 0,   h-1,   0,   w1-1);
}
// ---------------------------------------
void test_morpho_min_routine(int h, int w0)
// ---------------------------------------
{
    int b = 1; // bord

    int w8 = w0 / 8; if(w0 % 8) w8 = w8+1;
    int w1 = 8 * w8; // w1 >= w

    puts("--------------------------------------------------");
    printf("test_morpho_min_routine h = %d w0 = %d w8 = %d, w1 = %d\n", h, w0, w8, w1);
    if(w1 > w0) puts("w1 > w0");
    uint8 **X;
    uint8 **Y_bas, **Y_reg, **Y_rot, **Y_red, **Y_ilu3, **Y_ilu3r, **Y_elu2r, **Y_elu2rf, **Y_ilu3_elu2rf;
        
    int c; // error

    //puts("malloc");
    X  = ui8matrix(0-b, h-1+b, 0-b, w1-1+b);
    
    Y_bas         = ui8matrix(0, h-1, 0, w1-1);
    Y_reg         = ui8matrix(0, h-1, 0, w1-1);
    Y_rot         = ui8matrix(0, h-1, 0, w1-1);
    Y_red         = ui8matrix(0, h-1, 0, w1-1);
    Y_ilu3        = ui8matrix(0, h-1, 0, w1-1);
    Y_ilu3r       = ui8matrix(0, h-1, 0, w1-1);
    Y_elu2r       = ui8matrix(0, h-1, 0, w1-1);
    Y_elu2rf      = ui8matrix(0, h-1, 0, w1-1);
    Y_ilu3_elu2rf = ui8matrix(0, h-1, 0, w1-1);
    
    
    //puts("zero");
    zero_ui8matrix(X,  0-b, h-1+b, 0-b, w1-1+b);

    zero_ui8matrix(Y_bas        , 0, h-1, 0, w1-1);
    zero_ui8matrix(Y_reg        , 0, h-1, 0, w1-1);
    zero_ui8matrix(Y_rot        , 0, h-1, 0, w1-1);
    zero_ui8matrix(Y_red        , 0, h-1, 0, w1-1);
    zero_ui8matrix(Y_ilu3       , 0, h-1, 0, w1-1);
    zero_ui8matrix(Y_ilu3r      , 0, h-1, 0, w1-1);
    zero_ui8matrix(Y_elu2r      , 0, h-1, 0, w1-1);
    zero_ui8matrix(Y_elu2rf     , 0, h-1, 0, w1-1);
    zero_ui8matrix(Y_ilu3_elu2rf, 0, h-1, 0, w1-1);
        
    //puts("rand");
    //rand_ui8matrix(X, 0, h-1, 0, w0-1, 1, 255); // niveau de gris [1,255]
    rand1_ui8matrix(X, 0, h-1, 0, w0-1, 80); // binaire [0,1]
    
    //puts("\n-- max3 --");
    min3_ui8matrix_basic               (X, 0, h-1, 0, w0-1, Y_bas        ); // puts("bas");
    min3_ui8matrix_reg                 (X, 0, h-1, 0, w0-1, Y_reg        ); // puts("reg");
    min3_ui8matrix_rot                 (X, 0, h-1, 0, w0-1, Y_rot        ); // puts("rot");
    min3_ui8matrix_red                 (X, 0, h-1, 0, w0-1, Y_red        ); // puts("red");
    min3_ui8matrix_ilu3                (X, 0, h-1, 0, w0-1, Y_ilu3       ); // puts("ilu3");
    min3_ui8matrix_ilu3_red            (X, 0, h-1, 0, w0-1, Y_ilu3r      ); // puts("ilu3_red");
    min3_ui8matrix_elu2_red            (X, 0, h-1, 0, w0-1, Y_elu2r      ); // puts("elu2_red");
    min3_ui8matrix_elu2_red_factor     (X, 0, h-1, 0, w0-1, Y_elu2rf     ); // puts("elu2_red_factor");
    min3_ui8matrix_ilu3_elu2_red_factor(X, 0, h-1, 0, w0-1, Y_ilu3_elu2rf); // puts("ilu3_elu2_red_factor");
    //puts("done\n");
    /*
    display_ui8matrix(Y_bas,         0, h-1, 0, w0-1, "%5d", "Y bas        ");
    display_ui8matrix(Y_reg,         0, h-1, 0, w0-1, "%5d", "Y reg        ");
    display_ui8matrix(Y_bas,         0, h-1, 0, w0-1, "%5d", "Y bas        ");
    display_ui8matrix(Y_reg,         0, h-1, 0, w0-1, "%5d", "Y reg        ");
    display_ui8matrix(Y_rot,         0, h-1, 0, w0-1, "%5d", "Y rot        ");
    display_ui8matrix(Y_red,         0, h-1, 0, w0-1, "%5d", "Y red        ");
    display_ui8matrix(Y_ilu3,        0, h-1, 0, w0-1, "%5d", "Y ilu3       ");
    display_ui8matrix(Y_ilu3r,       0, h-1, 0, w0-1, "%5d", "Y ilu3r      ");
    display_ui8matrix(Y_elu2r,       0, h-1, 0, w0-1, "%5d", "Y elu2r      ");
    display_ui8matrix(Y_elu2rf,      0, h-1, 0, w0-1, "%5d", "Y elu2rf     ");
    display_ui8matrix(Y_ilu3_elu2rf, 0, h-1, 0, w0-1, "%5d", "Y ilu3 elu2rf");/**/
    
    //puts("-- compare max --");
    c = compare_ui8matrix(Y_bas, 0, h-1, 0, w0-1, Y_reg        , "Y reg                       ");
    c = compare_ui8matrix(Y_bas, 0, h-1, 0, w0-1, Y_rot        , "Y rot                       ");
    c = compare_ui8matrix(Y_bas, 0, h-1, 0, w0-1, Y_red        , "Y red                       ");
    c = compare_ui8matrix(Y_bas, 0, h-1, 0, w0-1, Y_ilu3       , "Y ilu3                      ");
    c = compare_ui8matrix(Y_bas, 0, h-1, 0, w0-1, Y_ilu3r      , "Y ilu3 + red                ");
    c = compare_ui8matrix(Y_bas, 0, h-1, 0, w0-1, Y_elu2r      , "Y elu2 + red                ");
    c = compare_ui8matrix(Y_bas, 0, h-1, 0, w0-1, Y_elu2rf     , "Y ilu3 + red + factor       ");
    c = compare_ui8matrix(Y_bas, 0, h-1, 0, w0-1, Y_ilu3_elu2rf, "Y ilu3 + elu2 + red + factor");
    putchar('\n');/**/
    //puts("done\n");
    
    
    free_ui8matrix(X,             0-b, h-1+b, 0-b, w1-1+b);
    
    free_ui8matrix(Y_bas,         0,   h-1,   0,   w1-1);
    free_ui8matrix(Y_reg,         0,   h-1,   0,   w1-1);
    free_ui8matrix(Y_rot,         0,   h-1,   0,   w1-1);
    free_ui8matrix(Y_red,         0,   h-1,   0,   w1-1);
    free_ui8matrix(Y_ilu3,        0,   h-1,   0,   w1-1);
    free_ui8matrix(Y_ilu3r,       0,   h-1,   0,   w1-1);
    free_ui8matrix(Y_elu2r,       0,   h-1,   0,   w1-1);
    free_ui8matrix(Y_elu2rf,      0,   h-1,   0,   w1-1);
    free_ui8matrix(Y_ilu3_elu2rf, 0,   h-1,   0,   w1-1);
}
// ------------------------------
void test_morpho_dilatation(void)
// ------------------------------
{
    int h0 = 8;
    int w0 = 16;
    
    int dh = 4;
    int dw = 3;
    
    //printf("test_morpho_max h = %d w = %d\n", h0, w0);
    //test_morpho_max_routine(h0, w0);
    
    for(int h = h0; h <= h0+dh; h++) { // pour tester elu2
        for(int w = w0; w <= w0+dw; w++) { // pour tester ilu3
            test_morpho_dilatation_routine(h, w);
        }
    }
}
// ---------------------------
void test_morpho_erosion(void)
// ---------------------------
{
    int h0 = 8;
    int w0 = 16;
    
    int dh = 4;
    int dw = 3;
    
    //printf("test_morpho_max h = %d w = %d\n", h0, w0);
    //test_morpho_min_routine(h0, w0);
    
    dh = dw = 0;
    
    for(int h = h0; h <= h0+dh; h++) { // pour tester elu2
        for(int w = w0; w <= w0+dw; w++) { // pour tester ilu3
            test_morpho_erosion_routine(h, w);
        }
    }
}
// -----------------------
void test_morpho_max(void)
// -----------------------
{
    int h0 = 8;
    int w0 = 16;
    
    int dh = 4;
    int dw = 3;
    
    for(int h = h0; h <= h0+dh; h++) { // pour tester elu2
        for(int w = w0; w <= w0+dw; w++) { // pour tester ilu3
            test_morpho_max_routine(h, w);
        }
    }
}
// -----------------------
void test_morpho_min(void)
// -----------------------
{
    int h0 = 8;
    int w0 = 16;
    
    int dh = 4;
    int dw = 3;
    
    for(int h = h0; h <= h0+dh; h++) { // pour tester elu2
        for(int w = w0; w <= w0+dw; w++) { // pour tester ilu3
            test_morpho_min_routine(h, w);
        }
    }
}
// -----------------------------
void test_morpho_ouverture(void)
// -----------------------------
{
    puts("---------------------------");
    puts("-- test_morpho_ouverture --");
    puts("---------------------------");
    
    // parametres de base (augmenter w le SWP)
    int h0 = 4;
    int w0 = 64*3;
    
    
    int dh = 4;
    int dw = 3;
    int sw = 8; //step
    
    int s = 0; // nombre de test valides
    
    // version SWP8
    dh = 2;
    dw = 3;

    // version SWP64
    h0 = 8; w0 = 128;
    dh = 2; dw = 128; sw = 64;
    //dh = 0; dw = 3*64; sw = 64;
    //dh = 0; dw = 0; sw = 64;
    
    //dh = dw = 0;
    
    for(int h = h0; h <= h0+dh; h++) { // pour tester elu2
        for(int w = w0; w <= w0+dw; w+= sw) { // pour tester ilu3
            s += test_morpho_ouverture_routine_SWP8(h, w);
            int r = test_morpho_ouverture_routine_SWP64(h, w);
            printf("binome : %s s = %d\n", get_binome_name(), r);
            s += r;
        }
    }
    // BINOME           est defini dans ue_l3_vision.h
    //get_binome_name() est defini dans ue_l3_vision.c
    //printf("binome : %s s = %d\n", BINOME, s);
    printf("BINOME : %s s = %d\n", get_binome_name(), s);
}
// ---------------------------------------------------
void bench_morpho_ouverture(int n0, int n1, int nstep)
// ---------------------------------------------------
{
    int r = 1;
    int h = n1; // max size
    int w = n1; //max size
    int w0 = w;
    
    int w8  = w0 /  8; if(w0 %  8) w8  = w8  + 1;
    int w32 = w0 / 32; if(w0 % 32) w32 = w32 + 1;
    int w64 = w0 / 64; if(w0 % 64) w64 = w64 + 1;
    
    int w1 = 64 * w64;
    
    double cpp_basic;
    double cpp_basic8;
    double cpp_basic32;
    double cpp_basic64;
    
    double cpp_fusion_basic;
    double cpp_fusion_red;
    double cpp_fusion_ilu5_red;
    double cpp_fusion_elu2_red;
    double cpp_fusion_elu2_red_factor;
    double cpp_fusion_ilu5_elu2_red;
    double cpp_fusion_ilu5_elu2_red_factor;
    double cpp_fusion_ilu15_red;
    
    double cpp_pipeline_basic;
    double cpp_pipeline_red;
    double cpp_pipeline_ilu3_red;
    double cpp_pipeline_elu2_red;
    double cpp_pipeline_elu2_red_factor;
    double cpp_pipeline_ilu3_elu2_red;
    double cpp_pipeline_ilu3_elu2_red_factor;
    
    double cpp_fusion8_basic;
    double cpp_fusion8_red;
    double cpp_fusion8_ilu3_red;
    //double cpp_fusion8_elu2_red;
    //double cpp_fusion8_elu2_red_factor;
    double cpp_fusion8_ilu3_elu2_red;
    double cpp_fusion8_ilu3_elu2_red_factor;
    
    double cpp_pipeline8_basic;
    double cpp_pipeline8_red;
    double cpp_pipeline8_ilu3_red;
    double cpp_pipeline8_elu2_red;
    double cpp_pipeline8_elu2_red_factor;
    double cpp_pipeline8_ilu3_elu2_red;
    double cpp_pipeline8_ilu3_elu2_red_factor;

    double cpp_fusion32_basic;
    double cpp_fusion32_red;
    double cpp_fusion32_ilu3_red;
    double cpp_fusion32_elu2_red;
    //double cpp_fusion32_elu2_red_factor;
    double cpp_fusion32_ilu3_elu2_red;
    double cpp_fusion32_ilu3_elu2_red_factor;
    
    double cpp_pipeline32_basic;
    double cpp_pipeline32_red;
    double cpp_pipeline32_ilu3_red;
    double cpp_pipeline32_elu2_red;
    double cpp_pipeline32_elu2_red_factor;
    double cpp_pipeline32_ilu3_elu2_red;
    double cpp_pipeline32_ilu3_elu2_red_factor;
    
    double cpp_fusion64_basic;
    double cpp_fusion64_red;
    double cpp_fusion64_ilu3_red;
    double cpp_fusion64_elu2_red;
    //double cpp_fusion64_elu2_red_factor;
    double cpp_fusion64_ilu3_elu2_red;
    double cpp_fusion64_ilu3_elu2_red_factor;
    
    double cpp_pipeline64_basic;
    double cpp_pipeline64_red;
    double cpp_pipeline64_ilu3_red;
    double cpp_pipeline64_elu2_red;
    double cpp_pipeline64_elu2_red_factor;
    double cpp_pipeline64_ilu3_elu2_red;
    double cpp_pipeline64_ilu3_elu2_red_factor;
    
    char* format = "%8.2f";
    
    format = "%5.0f";
    format = "%6.1f";
    format = "%7.2f";
    
    puts("malloc");
        
    // X 2r-border
    uint8  **X      = ui8matrix (0 - 2*r, h-1 + 2*r, 0 - 2*r, w1 -1 + 2*r); zero_ui8matrix (X  , 0 - 2*r, h-1 + 2*r, 0 - 2*r, w1 -1 + 2*r);
    uint8  **X1     = ui8matrix (0 - 2*r, h-1 + 2*r, 0 - 2*r, w1 -1 + 2*r); zero_ui8matrix (X1 , 0 - 2*r, h-1 + 2*r, 0 - 2*r, w1 -1 + 2*r);
    uint8  **X8     = ui8matrix (0 - 2*r, h-1 + 2*r, 0 - 1*r, w8 -1 + 1*r); zero_ui8matrix (X8 , 0 - 2*r, h-1 + 2*r, 0 - 1*r, w8 -1 + 1*r);
    //uint32 **X32    = ui32matrix(0 - 2*r, h-1 + 2*r, 0 - 1*r, w32-1 + 1*r); zero_ui32matrix(X32, 0 - 2*r, h-1 + 2*r, 0 - 1*r, w32-1 + 1*r);
    uint64 **X64    = ui64matrix(0 - 2*r, h-1 + 2*r, 0 - 1*r, w64-1 + 1*r); zero_ui64matrix(X64, 0 - 2*r, h-1 + 2*r, 0 - 1*r, w64-1 + 1*r);
        
    zero_ui8matrix (X8,  0 - 2*r, h-1 + 2*r, 0 - 1*r, w8 -1 + 1*r);
    zero_ui64matrix (X64,  0 - 2*r, h-1 + 2*r, 0 - 1*r, w64 - 1 + 1*r);
    // T 1r-border
    uint8 **T_basic                         = ui8matrix(0 - r, h-1 + r, 0 - r, w1-1 + r); zero_ui8matrix(T_basic                        , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    uint8 **T_pipeline_basic                = ui8matrix(0 - r, h-1 + r, 0 - r, w1-1 + r); zero_ui8matrix(T_pipeline_basic               , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    uint8 **T_pipeline_red                  = ui8matrix(0 - r, h-1 + r, 0 - r, w1-1 + r); zero_ui8matrix(T_pipeline_red                 , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    uint8 **T_pipeline_ilu3_red             = ui8matrix(0 - r, h-1 + r, 0 - r, w1-1 + r); zero_ui8matrix(T_pipeline_ilu3_red            , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    uint8 **T_pipeline_elu2_red             = ui8matrix(0 - r, h-1 + r, 0 - r, w1-1 + r); zero_ui8matrix(T_pipeline_elu2_red            , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    uint8 **T_pipeline_elu2_red_factor      = ui8matrix(0 - r, h-1 + r, 0 - r, w1-1 + r); zero_ui8matrix(T_pipeline_elu2_red_factor     , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    uint8 **T_pipeline_ilu3_elu2_red        = ui8matrix(0 - r, h-1 + r, 0 - r, w1-1 + r); zero_ui8matrix(T_pipeline_ilu3_elu2_red       , 0 - r, h-1 + r, 0 - r, w1-1 + r);
    uint8 **T_pipeline_ilu3_elu2_red_factor = ui8matrix(0 - r, h-1 + r, 0 - r, w1-1 + r); zero_ui8matrix(T_pipeline_ilu3_elu2_red_factor, 0 - r, h-1 + r, 0 - r, w1-1 + r);
    
    //uint8 **T_pipeline_mod3 = ui8matrix_mod(0 - r, h-1 + r, 0 -r, w1-1 + r, 3); zero_ui8matrix(T_pipeline_mod3 , 0 - r, h-1 + r, 0 -r, w1-1 + r);
    //uint8 **T_pipeline_mod4 = ui8matrix_mod(0 - r, h-1 + r, 0 -r, w1-1 + r, 4); zero_ui8matrix(T_pipeline_mod4 , 0 - r, h-1 + r, 0 -r, w1-1 + r);

    uint8 **Y_basic                       = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_basic                      , 0, h-1, 0, w1-1);
    uint8 **Y_fusion_basic                = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_fusion_basic               , 0, h-1, 0, w1-1);
    uint8 **Y_fusion_red                  = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_fusion_red                 , 0, h-1, 0, w1-1);
    uint8 **Y_fusion_ilu5_red             = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_fusion_ilu5_red            , 0, h-1, 0, w1-1);
    uint8 **Y_fusion_elu2_red             = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_fusion_elu2_red            , 0, h-1, 0, w1-1);
    uint8 **Y_fusion_ilu5_elu2_red        = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_fusion_ilu5_elu2_red       , 0, h-1, 0, w1-1);
    uint8 **Y_fusion_ilu5_elu2_red_factor = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_fusion_ilu5_elu2_red_factor, 0, h-1, 0, w1-1);
    uint8 **Y_fusion_ilu15_red            = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_fusion_ilu15_red           , 0, h-1, 0, w1-1);
    
    uint8 **Y_pipeline_basic                = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_pipeline_basic               , 0, h-1, 0, w1-1);
    uint8 **Y_pipeline_red                  = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_pipeline_red                 , 0, h-1, 0, w1-1);
    uint8 **Y_pipeline_ilu3_red             = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_pipeline_ilu3_red            , 0, h-1, 0, w1-1);
    uint8 **Y_pipeline_elu2_red             = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_pipeline_elu2_red            , 0, h-1, 0, w1-1);
    uint8 **Y_pipeline_elu2_red_factor      = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_pipeline_elu2_red_factor     , 0, h-1, 0, w1-1);
    uint8 **Y_pipeline_ilu3_elu2_red        = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_pipeline_ilu3_elu2_red       , 0, h-1, 0, w1-1);
    uint8 **Y_pipeline_ilu3_elu2_red_factor = ui8matrix(0, h-1, 0, w1-1); zero_ui8matrix(Y_pipeline_ilu3_elu2_red_factor, 0, h-1, 0, w1-1);
    
    uint8 **Y8_basic                       = ui8matrix(0, h-1, 0, w8-1); zero_ui8matrix(Y8_basic                      , 0, h-1, 0, w8-1);
    uint8 **Y8_fusion_basic                = ui8matrix(0, h-1, 0, w8-1); zero_ui8matrix(Y8_fusion_basic               , 0, h-1, 0, w8-1);
    uint8 **Y8_fusion_red                  = ui8matrix(0, h-1, 0, w8-1); zero_ui8matrix(Y8_fusion_red                 , 0, h-1, 0, w8-1);
    uint8 **Y8_fusion_ilu3_red             = ui8matrix(0, h-1, 0, w8-1); zero_ui8matrix(Y8_fusion_ilu3_red            , 0, h-1, 0, w8-1);
    uint8 **Y8_fusion_ilu3_elu2_red        = ui8matrix(0, h-1, 0, w8-1); zero_ui8matrix(Y8_fusion_ilu3_elu2_red       , 0, h-1, 0, w8-1);
    uint8 **Y8_fusion_ilu3_elu2_red_factor = ui8matrix(0, h-1, 0, w8-1); zero_ui8matrix(Y8_fusion_ilu3_elu2_red_factor, 0, h-1, 0, w8-1);
    
    uint8 **T8_basic                         = ui8matrix(0 - r, h-1 + r, 0 - r, w8-1 + r); zero_ui8matrix(T8_basic                        , 0 - r, h-1 + r, 0 - r, w8-1 + r);
    uint8 **T8_pipeline_basic                = ui8matrix(0 - r, h-1 + r, 0 - r, w8-1 + r); zero_ui8matrix(T8_pipeline_basic               , 0 - r, h-1 + r, 0 - r, w8-1 + r);
    uint8 **T8_pipeline_red                  = ui8matrix(0 - r, h-1 + r, 0 - r, w8-1 + r); zero_ui8matrix(T8_pipeline_red                 , 0 - r, h-1 + r, 0 - r, w8-1 + r);
    uint8 **T8_pipeline_ilu3_red             = ui8matrix(0 - r, h-1 + r, 0 - r, w8-1 + r); zero_ui8matrix(T8_pipeline_ilu3_red            , 0 - r, h-1 + r, 0 - r, w8-1 + r);
    uint8 **T8_pipeline_elu2_red             = ui8matrix(0 - r, h-1 + r, 0 - r, w8-1 + r); zero_ui8matrix(T8_pipeline_elu2_red            , 0 - r, h-1 + r, 0 - r, w8-1 + r);
    uint8 **T8_pipeline_elu2_red_factor      = ui8matrix(0 - r, h-1 + r, 0 - r, w8-1 + r); zero_ui8matrix(T8_pipeline_elu2_red_factor     , 0 - r, h-1 + r, 0 - r, w8-1 + r);
    uint8 **T8_pipeline_ilu3_elu2_red        = ui8matrix(0 - r, h-1 + r, 0 - r, w8-1 + r); zero_ui8matrix(T8_pipeline_ilu3_elu2_red       , 0 - r, h-1 + r, 0 - r, w8-1 + r);
    uint8 **T8_pipeline_ilu3_elu2_red_factor = ui8matrix(0 - r, h-1 + r, 0 - r, w8-1 + r); zero_ui8matrix(T8_pipeline_ilu3_elu2_red_factor, 0 - r, h-1 + r, 0 - r, w8-1 + r);
    
    uint8 **Y8_pipeline_basic                = ui8matrix(0, h-1, 0, w8-1); zero_ui8matrix(Y8_pipeline_basic               , 0, h-1, 0, w8-1);
    uint8 **Y8_pipeline_red                  = ui8matrix(0, h-1, 0, w8-1); zero_ui8matrix(Y8_pipeline_red                 , 0, h-1, 0, w8-1);
    uint8 **Y8_pipeline_ilu3_red             = ui8matrix(0, h-1, 0, w8-1); zero_ui8matrix(Y8_pipeline_ilu3_red            , 0, h-1, 0, w8-1);
    uint8 **Y8_pipeline_elu2_red             = ui8matrix(0, h-1, 0, w8-1); zero_ui8matrix(Y8_pipeline_elu2_red            , 0, h-1, 0, w8-1);
    uint8 **Y8_pipeline_elu2_red_factor      = ui8matrix(0, h-1, 0, w8-1); zero_ui8matrix(Y8_pipeline_elu2_red_factor     , 0, h-1, 0, w8-1);
    uint8 **Y8_pipeline_ilu3_elu2_red        = ui8matrix(0, h-1, 0, w8-1); zero_ui8matrix(Y8_pipeline_ilu3_elu2_red       , 0, h-1, 0, w8-1);
    uint8 **Y8_pipeline_ilu3_elu2_red_factor = ui8matrix(0, h-1, 0, w8-1); zero_ui8matrix(Y8_pipeline_ilu3_elu2_red_factor, 0, h-1, 0, w8-1);
    
    //uint32 **Y32_basic                       = ui32matrix(0, h-1, 0, w32-1); zero_ui32matrix(Y32_basic                      , 0, h-1, 0, w32-1);
    //uint32 **Y32_fusion_basic                = ui32matrix(0, h-1, 0, w32-1); zero_ui32matrix(Y32_fusion_basic               , 0, h-1, 0, w32-1);
    //uint32 **Y32_fusion_red                  = ui32matrix(0, h-1, 0, w32-1); zero_ui32matrix(Y32_fusion_red                 , 0, h-1, 0, w32-1);
    //uint32 **Y32_fusion_ilu3_red             = ui32matrix(0, h-1, 0, w32-1); zero_ui32matrix(Y32_fusion_ilu3_red            , 0, h-1, 0, w32-1);
    //uint32 **Y32_fusion_elu2_red             = ui32matrix(0, h-1, 0, w32-1); zero_ui32matrix(Y32_fusion_elu2_red            , 0, h-1, 0, w32-1);
    //uint32 **Y32_fusion_ilu3_elu2_red        = ui32matrix(0, h-1, 0, w32-1); zero_ui32matrix(Y32_fusion_ilu3_elu2_red       , 0, h-1, 0, w32-1);
    //uint32 **Y32_fusion_ilu3_elu2_red_factor = ui32matrix(0, h-1, 0, w32-1); zero_ui32matrix(Y32_fusion_ilu3_elu2_red_factor, 0, h-1, 0, w32-1);
    //
    //uint32 **T32_basic                         = ui32matrix(0 - r, h-1 + r, 0 - r, w32-1 + r); zero_ui32matrix(T32_basic                        , 0 - r, h-1 + r, 0 - r, w32-1 + r);
    //uint32 **T32_pipeline_basic                = ui32matrix(0 - r, h-1 + r, 0 - r, w32-1 + r); zero_ui32matrix(T32_pipeline_basic               , 0 - r, h-1 + r, 0 - r, w32-1 + r);
    //uint32 **T32_pipeline_red                  = ui32matrix(0 - r, h-1 + r, 0 - r, w32-1 + r); zero_ui32matrix(T32_pipeline_red                 , 0 - r, h-1 + r, 0 - r, w32-1 + r);
    //uint32 **T32_pipeline_ilu3_red             = ui32matrix(0 - r, h-1 + r, 0 - r, w32-1 + r); zero_ui32matrix(T32_pipeline_ilu3_red            , 0 - r, h-1 + r, 0 - r, w32-1 + r);
    //uint32 **T32_pipeline_elu2_red             = ui32matrix(0 - r, h-1 + r, 0 - r, w32-1 + r); zero_ui32matrix(T32_pipeline_elu2_red            , 0 - r, h-1 + r, 0 - r, w32-1 + r);
    //uint32 **T32_pipeline_elu2_red_factor      = ui32matrix(0 - r, h-1 + r, 0 - r, w32-1 + r); zero_ui32matrix(T32_pipeline_elu2_red_factor     , 0 - r, h-1 + r, 0 - r, w32-1 + r);
    //uint32 **T32_pipeline_ilu3_elu2_red        = ui32matrix(0 - r, h-1 + r, 0 - r, w32-1 + r); zero_ui32matrix(T32_pipeline_ilu3_elu2_red       , 0 - r, h-1 + r, 0 - r, w32-1 + r);
    //uint32 **T32_pipeline_ilu3_elu2_red_factor = ui32matrix(0 - r, h-1 + r, 0 - r, w32-1 + r); zero_ui32matrix(T32_pipeline_ilu3_elu2_red_factor, 0 - r, h-1 + r, 0 - r, w32-1 + r);
    //
    //uint32 **Y32_pipeline_basic                = ui32matrix(0, h-1, 0, w32-1); zero_ui32matrix(Y32_pipeline_basic               , 0, h-1, 0, w32-1);
    //uint32 **Y32_pipeline_red                  = ui32matrix(0, h-1, 0, w32-1); zero_ui32matrix(Y32_pipeline_red                 , 0, h-1, 0, w32-1);
    //uint32 **Y32_pipeline_ilu3_red             = ui32matrix(0, h-1, 0, w32-1); zero_ui32matrix(Y32_pipeline_ilu3_red            , 0, h-1, 0, w32-1);
    //uint32 **Y32_pipeline_elu2_red             = ui32matrix(0, h-1, 0, w32-1); zero_ui32matrix(Y32_pipeline_elu2_red            , 0, h-1, 0, w32-1);
    //uint32 **Y32_pipeline_elu2_red_factor      = ui32matrix(0, h-1, 0, w32-1); zero_ui32matrix(Y32_pipeline_elu2_red_factor     , 0, h-1, 0, w32-1);
    //uint32 **Y32_pipeline_ilu3_elu2_red        = ui32matrix(0, h-1, 0, w32-1); zero_ui32matrix(Y32_pipeline_ilu3_elu2_red       , 0, h-1, 0, w32-1);
    //uint32 **Y32_pipeline_ilu3_elu2_red_factor = ui32matrix(0, h-1, 0, w32-1); zero_ui32matrix(Y32_pipeline_ilu3_elu2_red_factor, 0, h-1, 0, w32-1);
    
    uint64 **Y64_basic                       = ui64matrix(0, h-1, 0, w64-1); zero_ui64matrix(Y64_basic                      , 0, h-1, 0, w64-1);
    uint64 **Y64_fusion_basic                = ui64matrix(0, h-1, 0, w64-1); zero_ui64matrix(Y64_fusion_basic               , 0, h-1, 0, w64-1);
    uint64 **Y64_fusion_red                  = ui64matrix(0, h-1, 0, w64-1); zero_ui64matrix(Y64_fusion_red                 , 0, h-1, 0, w64-1);
    uint64 **Y64_fusion_ilu3_red             = ui64matrix(0, h-1, 0, w64-1); zero_ui64matrix(Y64_fusion_ilu3_red            , 0, h-1, 0, w64-1);
    uint64 **Y64_fusion_elu2_red             = ui64matrix(0, h-1, 0, w64-1); zero_ui64matrix(Y64_fusion_elu2_red            , 0, h-1, 0, w64-1);
    uint64 **Y64_fusion_ilu3_elu2_red        = ui64matrix(0, h-1, 0, w64-1); zero_ui64matrix(Y64_fusion_ilu3_elu2_red       , 0, h-1, 0, w64-1);
    uint64 **Y64_fusion_ilu3_elu2_red_factor = ui64matrix(0, h-1, 0, w64-1); zero_ui64matrix(Y64_fusion_ilu3_elu2_red_factor, 0, h-1, 0, w64-1);
    
    uint64 **T64_basic                         = ui64matrix(0 - r, h-1 + r, 0 - r, w64-1 + r); zero_ui64matrix(T64_basic                        , 0 - r, h-1 + r, 0 - r, w64-1 + r);
    uint64 **T64_pipeline_basic                = ui64matrix(0 - r, h-1 + r, 0 - r, w64-1 + r); zero_ui64matrix(T64_pipeline_basic               , 0 - r, h-1 + r, 0 - r, w64-1 + r);
    uint64 **T64_pipeline_red                  = ui64matrix(0 - r, h-1 + r, 0 - r, w64-1 + r); zero_ui64matrix(T64_pipeline_red                 , 0 - r, h-1 + r, 0 - r, w64-1 + r);
    uint64 **T64_pipeline_ilu3_red             = ui64matrix(0 - r, h-1 + r, 0 - r, w64-1 + r); zero_ui64matrix(T64_pipeline_ilu3_red            , 0 - r, h-1 + r, 0 - r, w64-1 + r);
    uint64 **T64_pipeline_elu2_red             = ui64matrix(0 - r, h-1 + r, 0 - r, w64-1 + r); zero_ui64matrix(T64_pipeline_elu2_red            , 0 - r, h-1 + r, 0 - r, w64-1 + r);
    uint64 **T64_pipeline_elu2_red_factor      = ui64matrix(0 - r, h-1 + r, 0 - r, w64-1 + r); zero_ui64matrix(T64_pipeline_elu2_red_factor     , 0 - r, h-1 + r, 0 - r, w64-1 + r);
    uint64 **T64_pipeline_ilu3_elu2_red        = ui64matrix(0 - r, h-1 + r, 0 - r, w64-1 + r); zero_ui64matrix(T64_pipeline_ilu3_elu2_red       , 0 - r, h-1 + r, 0 - r, w64-1 + r);
    uint64 **T64_pipeline_ilu3_elu2_red_factor = ui64matrix(0 - r, h-1 + r, 0 - r, w64-1 + r); zero_ui64matrix(T64_pipeline_ilu3_elu2_red_factor, 0 - r, h-1 + r, 0 - r, w64-1 + r);
    
    uint64 **Y64_pipeline_basic                = ui64matrix(0, h-1, 0, w64-1); zero_ui64matrix(Y64_pipeline_basic               , 0, h-1, 0, w64-1);
    uint64 **Y64_pipeline_red                  = ui64matrix(0, h-1, 0, w64-1); zero_ui64matrix(Y64_pipeline_red                 , 0, h-1, 0, w64-1);
    uint64 **Y64_pipeline_ilu3_red             = ui64matrix(0, h-1, 0, w64-1); zero_ui64matrix(Y64_pipeline_ilu3_red            , 0, h-1, 0, w64-1);
    uint64 **Y64_pipeline_elu2_red             = ui64matrix(0, h-1, 0, w64-1); zero_ui64matrix(Y64_pipeline_elu2_red            , 0, h-1, 0, w64-1);
    uint64 **Y64_pipeline_elu2_red_factor      = ui64matrix(0, h-1, 0, w64-1); zero_ui64matrix(Y64_pipeline_elu2_red_factor     , 0, h-1, 0, w64-1);
    uint64 **Y64_pipeline_ilu3_elu2_red        = ui64matrix(0, h-1, 0, w64-1); zero_ui64matrix(Y64_pipeline_ilu3_elu2_red       , 0, h-1, 0, w64-1);
    uint64 **Y64_pipeline_ilu3_elu2_red_factor = ui64matrix(0, h-1, 0, w64-1); zero_ui64matrix(Y64_pipeline_ilu3_elu2_red_factor, 0, h-1, 0, w64-1);
    
    for(int n = n0; n <= n1; n += nstep) {
        
        h = n;
        w1  = n / 1;
        w8  = n / 8;
        w32 = n / 32;
        w64 = n / 64;
        
        //printf("i = %3d\n", n);

        //resize_ui8matrix (X ,  0-2*r, h-1+2*r, 0-2*r, w1 -1+2*r);
        //resize_ui8matrix (X1,  0-2*r, h-1+2*r, 0-2*r, w1 -1+2*r);
        //resize_ui8matrix (X8,  0-2*r, h-1+2*r, 0-1*r, w8 -1+1*r);
        //resize_ui32matrix(X32, 0-2*r, h-1+2*r, 0-1*r, w32-1+1*r);
        
        // T 1r-border
        //resize_ui8matrix(T_basic                        , 0-1*r, h-1+1*r, 0-1*r, w1-1+1*r);
        //resize_ui8matrix(T_pipeline                     , 0-1*r, h-1+1*r, 0-1*r, w1-1+1*r);
        //resize_ui8matrix(T_pipeline_red                 , 0-1*r, h-1+1*r, 0-1*r, w1-1+1*r);
        //resize_ui8matrix(T_pipeline_ilu3_red            , 0-1*r, h-1+1*r, 0-1*r, w1-1+1*r);
        //resize_ui8matrix(T_pipeline_elu2_red            , 0-1*r, h-1+1*r, 0-1*r, w1-1+1*r);
        //resize_ui8matrix(T_pipeline_elu2_red_factor     , 0-1*r, h-1+1*r, 0-1*r, w1-1+1*r);
        //resize_ui8matrix(T_pipeline_ilu3_elu2_red       , 0-1*r, h-1+1*r, 0-1*r, w1-1+1*r);
        //resize_ui8matrix(T_pipeline_ilu3_elu2_red_factor, 0-1*r, h-1+1*r, 0-1*r, w1-1+1*r);
        
        //resize_ui8matrix_mod(T_pipeline_mod3, 0-1*r, h-1+1*r, 0-1*r, w1-1+1*r, 3);
        //resize_ui8matrix_mod(T_pipeline_mod4, 0-1*r, h-1+1*r, 0-1*r, w1-1+1*r, 4);

        // Y 0r-border
        //resize_ui8matrix(Y_basic                      , 0, h-1, 0, w1-1);
        //resize_ui8matrix(Y_fusion                     , 0, h-1, 0, w1-1);
        //resize_ui8matrix(Y_fusion_ilu5_red            , 0, h-1, 0, w1-1);
        //resize_ui8matrix(Y_fusion_ilu5_elu2_red       , 0, h-1, 0, w1-1);
        //resize_ui8matrix(Y_fusion_ilu5_elu2_red_factor, 0, h-1, 0, w1-1);
        //resize_ui8matrix(Y_fusion_ilu15_red           , 0, h-1, 0, w1-1);
        
        //resize_ui8matrix(Y_pipeline_basic               , 0, h-1, 0, w1-1);
        //resize_ui8matrix(Y_pipeline_red                 , 0, h-1, 0, w1-1);
        //resize_ui8matrix(Y_pipeline_ilu3_red            , 0, h-1, 0, w1-1);
        //resize_ui8matrix(Y_pipeline_elu2_red            , 0, h-1, 0, w1-1);
        //resize_ui8matrix(Y_pipeline_elu2_red_factor     , 0, h-1, 0, w1-1);
        //resize_ui8matrix(Y_pipeline_ilu3_elu2_red       , 0, h-1, 0, w1-1);
        //resize_ui8matrix(Y_pipeline_ilu3_elu2_red_factor, 0, h-1, 0, w1-1);
        
        //resize_ui8matrix(Y8_basic                      , 0, h-1, 0, w8-1);
        //resize_ui8matrix(Y8_fusion_basic               , 0, h-1, 0, w8-1);
        //resize_ui8matrix(Y8_fusion_red                 , 0, h-1, 0, w8-1);
        //resize_ui8matrix(Y8_fusion_ilu3_red            , 0, h-1, 0, w8-1);
        //resize_ui8matrix(Y8_fusion_ilu3_elu2_red       , 0, h-1, 0, w8-1);
        //resize_ui8matrix(Y8_fusion_ilu3_elu2_red_factor, 0, h-1, 0, w8-1);
        
        //resize_ui8matrix(T8_pipeline_basic               , 0, h-1, 0, w8-1);
        //resize_ui8matrix(T8_pipeline_red                 , 0, h-1, 0, w8-1);
        //resize_ui8matrix(T8_pipeline_ilu3_red            , 0, h-1, 0, w8-1);
        //resize_ui8matrix(T8_pipeline_elu2_red            , 0, h-1, 0, w8-1);
        //resize_ui8matrix(T8_pipeline_elu2_red_factor     , 0, h-1, 0, w8-1);
        //resize_ui8matrix(T8_pipeline_ilu3_elu2_red       , 0, h-1, 0, w8-1);
        //resize_ui8matrix(T8_pipeline_ilu3_elu2_red_factor, 0, h-1, 0, w8-1);
        
        //resize_ui8matrix(Y8_pipeline                     , 0, h-1, 0, w8-1);
        //resize_ui8matrix(Y8_pipeline_red                 , 0, h-1, 0, w8-1);
        //resize_ui8matrix(Y8_pipeline_ilu3_red            , 0, h-1, 0, w8-1);
        //resize_ui8matrix(Y8_pipeline_elu2_red            , 0, h-1, 0, w8-1);
        //resize_ui8matrix(Y8_pipeline_elu2_red_factor     , 0, h-1, 0, w8-1);
        //resize_ui8matrix(Y8_pipeline_ilu3_elu2_red       , 0, h-1, 0, w8-1);
        //resize_ui8matrix(Y8_pipeline_ilu3_elu2_red_factor, 0, h-1, 0, w8-1);
        
        //resize_ui32matrix(Y32_basic                      , 0, h-1, 0, w32-1);
        //resize_ui32matrix(Y32_fusion_basic               , 0, h-1, 0, w32-1);
        //resize_ui32matrix(Y32_fusion_red                 , 0, h-1, 0, w32-1);
        //resize_ui32matrix(Y32_fusion_ilu3_red            , 0, h-1, 0, w32-1);
        //resize_ui32matrix(Y32_fusion_ilu3_elu2_red       , 0, h-1, 0, w32-1);
        //resize_ui32matrix(Y32_fusion_ilu3_elu2_red_factor, 0, h-1, 0, w32-1);
        
        //resize_ui32matrix(T32_basic                     , 0, h-1, 0, w32-1);
        //resize_ui32matrix(T32_pipeline_basic               , 0, h-1, 0, w32-1);
        //resize_ui32matrix(T32_pipeline_red                 , 0, h-1, 0, w32-1);
        //resize_ui32matrix(T32_pipeline_ilu3_red            , 0, h-1, 0, w32-1);
        //resize_ui32matrix(T32_pipeline_elu2_red            , 0, h-1, 0, w32-1);
        //resize_ui32matrix(T32_pipeline_elu2_red_factor     , 0, h-1, 0, w32-1);
        //resize_ui32matrix(T32_pipeline_ilu3_elu2_red       , 0, h-1, 0, w32-1);
        //resize_ui32matrix(T32_pipeline_ilu3_elu2_red_factor, 0, h-1, 0, w32-1);
        
        //resize_ui32matrix(Y32_pipeline_basic               , 0, h-1, 0, w32-1);
        //resize_ui32matrix(Y32_pipeline_red                 , 0, h-1, 0, w32-1);
        //resize_ui32matrix(Y32_pipeline_ilu3_red            , 0, h-1, 0, w32-1);
        //resize_ui32matrix(Y32_pipeline_elu2_red            , 0, h-1, 0, w32-1);
        //resize_ui32matrix(Y32_pipeline_elu2_red_factor     , 0, h-1, 0, w32-1);
        //resize_ui32matrix(Y32_pipeline_ilu3_elu2_red       , 0, h-1, 0, w32-1);
        //resize_ui32matrix(Y32_pipeline_ilu3_elu2_red_factor, 0, h-1, 0, w32-1);
        
        //resize_ui64matrix(Y64_basic                      , 0, h-1, 0, w64-1);
        //resize_ui64matrix(Y64_fusion_basic               , 0, h-1, 0, w64-1);
        //resize_ui64matrix(Y64_fusion_red                 , 0, h-1, 0, w64-1);
        //resize_ui64matrix(Y64_fusion_ilu3_red            , 0, h-1, 0, w64-1);
        //resize_ui64matrix(Y64_fusion_ilu3_elu2_red       , 0, h-1, 0, w64-1);
        //resize_ui64matrix(Y64_fusion_ilu3_elu2_red_factor, 0, h-1, 0, w64-1);
        
        //resize_ui64matrix(T64_pipeline_basic               , 0, h-1, 0, w64-1);
        //resize_ui64matrix(T64_pipeline_red                 , 0, h-1, 0, w64-1);
        //resize_ui64matrix(T64_pipeline_ilu3_red            , 0, h-1, 0, w64-1);
        //resize_ui64matrix(T64_pipeline_elu2_red            , 0, h-1, 0, w64-1);
        //resize_ui64matrix(T64_pipeline_elu2_red_factor     , 0, h-1, 0, w64-1);
        //resize_ui64matrix(T64_pipeline_ilu3_elu2_red       , 0, h-1, 0, w64-1);
        //resize_ui64matrix(T64_pipeline_ilu3_elu2_red_factor, 0, h-1, 0, w64-1);
                
        //resize_ui64matrix(Y64_pipeline                     , 0, h-1, 0, w64-1);
        //resize_ui64matrix(Y64_pipeline_red                 , 0, h-1, 0, w64-1);
        //resize_ui64matrix(Y64_pipeline_ilu3_red            , 0, h-1, 0, w64-1);
        //resize_ui64matrix(Y64_pipeline_elu2_red            , 0, h-1, 0, w64-1);
        //resize_ui64matrix(Y64_pipeline_elu2_red_factor     , 0, h-1, 0, w64-1);
        //resize_ui64matrix(Y64_pipeline_ilu3_elu2_red       , 0, h-1, 0, w64-1);
        //resize_ui64matrix(Y64_pipeline_ilu3_elu2_red_factor, 0, h-1, 0, w64-1);
        
        // 1 version naive
        BENCH(ouverture3_ui8matrix_basic                      (X, 0, h-1, 0, w1-1, T_basic, Y_basic                      ), n, cpp_basic                      );
        
        // 7 fonction fusion
        BENCH(ouverture3_ui8matrix_fusion_basic               (X, 0, h-1, 0, w1-1,          Y_fusion_basic               ), n, cpp_fusion_basic               );
        BENCH(ouverture3_ui8matrix_fusion_red                 (X, 0, h-1, 0, w1-1,          Y_fusion_red                 ), n, cpp_fusion_red                 );
        BENCH(ouverture3_ui8matrix_fusion_ilu5_red            (X, 0, h-1, 0, w1-1,          Y_fusion_ilu5_red            ), n, cpp_fusion_ilu5_red            );
        BENCH(ouverture3_ui8matrix_fusion_elu2_red            (X, 0, h-1, 0, w1-1,          Y_fusion_elu2_red            ), n, cpp_fusion_elu2_red            );
        BENCH(ouverture3_ui8matrix_fusion_ilu5_elu2_red       (X, 0, h-1, 0, w1-1,          Y_fusion_ilu5_elu2_red       ), n, cpp_fusion_ilu5_elu2_red       );
        BENCH(ouverture3_ui8matrix_fusion_ilu5_elu2_red_factor(X, 0, h-1, 0, w1-1,          Y_fusion_ilu5_elu2_red_factor), n, cpp_fusion_ilu5_elu2_red_factor);
        BENCH(ouverture3_ui8matrix_fusion_ilu15_red           (X, 0, h-1, 0, w1-1,          Y_fusion_ilu15_red           ), n, cpp_fusion_ilu15_red           );
        
        // 7 fonctions pipeline
        BENCH(ouverture3_ui8matrix_pipeline_basic               (X, 0, h-1, 0, w1-1, T_pipeline_basic               , Y_pipeline_basic               ), n, cpp_pipeline_basic               );
        BENCH(ouverture3_ui8matrix_pipeline_red                 (X, 0, h-1, 0, w1-1, T_pipeline_red                 , Y_pipeline_red                 ), n, cpp_pipeline_red                 );
        BENCH(ouverture3_ui8matrix_pipeline_ilu3_red            (X, 0, h-1, 0, w1-1, T_pipeline_ilu3_red            , Y_pipeline_ilu3_red            ), n, cpp_pipeline_ilu3_red            );
        BENCH(ouverture3_ui8matrix_pipeline_elu2_red            (X, 0, h-1, 0, w1-1, T_pipeline_elu2_red            , Y_pipeline_elu2_red            ), n, cpp_pipeline_elu2_red            );
        BENCH(ouverture3_ui8matrix_pipeline_elu2_red_factor     (X, 0, h-1, 0, w1-1, T_pipeline_elu2_red_factor     , Y_pipeline_elu2_red_factor     ), n, cpp_pipeline_elu2_red_factor     );
        BENCH(ouverture3_ui8matrix_pipeline_ilu3_elu2_red       (X, 0, h-1, 0, w1-1, T_pipeline_ilu3_elu2_red       , Y_pipeline_ilu3_elu2_red       ), n, cpp_pipeline_ilu3_elu2_red       );
        BENCH(ouverture3_ui8matrix_pipeline_ilu3_elu2_red_factor(X, 0, h-1, 0, w1-1, T_pipeline_ilu3_elu2_red_factor, Y_pipeline_ilu3_elu2_red_factor), n, cpp_pipeline_ilu3_elu2_red_factor);
        
        BENCH(ouverture3_ui8matrix_swp8_basic                      (X8, 0, h-1, 0, w8-1, T8_basic, Y8_basic                      ), n, cpp_basic8                      );
        BENCH(ouverture3_ui8matrix_swp8_fusion_basic               (X8, 0, h-1, 0, w8-1,           Y8_fusion_basic               ), n, cpp_fusion8_basic                     );
        BENCH(ouverture3_ui8matrix_swp8_fusion_red                 (X8, 0, h-1, 0, w8-1,           Y8_fusion_red                 ), n, cpp_fusion8_red                 );
        BENCH(ouverture3_ui8matrix_swp8_fusion_ilu3_red            (X8, 0, h-1, 0, w8-1,           Y8_fusion_ilu3_red            ), n, cpp_fusion8_ilu3_red            );
        BENCH(ouverture3_ui8matrix_swp8_fusion_ilu3_elu2_red       (X8, 0, h-1, 0, w8-1,           Y8_fusion_ilu3_elu2_red       ), n, cpp_fusion8_ilu3_elu2_red       );
        BENCH(ouverture3_ui8matrix_swp8_fusion_ilu3_elu2_red_factor(X8, 0, h-1, 0, w8-1,           Y8_fusion_ilu3_elu2_red_factor), n, cpp_fusion8_ilu3_elu2_red_factor);
        
        BENCH(ouverture3_ui8matrix_swp8_pipeline_basic               (X8, 0, h-1, 0, w8-1, T8_pipeline_basic               , Y8_pipeline_basic               ), n, cpp_pipeline8_basic               );
        BENCH(ouverture3_ui8matrix_swp8_pipeline_ilu3_red            (X8, 0, h-1, 0, w8-1, T8_pipeline_ilu3_red            , Y8_pipeline_ilu3_red            ), n, cpp_pipeline8_ilu3_red            );
        BENCH(ouverture3_ui8matrix_swp8_pipeline_elu2_red_factor     (X8, 0, h-1, 0, w8-1, T8_pipeline_elu2_red_factor     , Y8_pipeline_elu2_red_factor     ), n, cpp_pipeline8_elu2_red_factor     );
        BENCH(ouverture3_ui8matrix_swp8_pipeline_ilu3_elu2_red_factor(X8, 0, h-1, 0, w8-1, T8_pipeline_ilu3_elu2_red_factor, Y8_pipeline_ilu3_elu2_red_factor), n, cpp_pipeline8_ilu3_elu2_red_factor);
        
        //BENCH(ouverture3_ui32matrix_swp32_basic                      (X32, 0, h-1, 0, w32-1, T32_basic, Y32_basic                      ), n, cpp_basic32                      );
        //BENCH(ouverture3_ui32matrix_swp32_fusion_basic               (X32, 0, h-1, 0, w32-1,            Y32_fusion_basic               ), n, cpp_fusion32                     );
        //BENCH(ouverture3_ui32matrix_swp32_fusion_red                 (X32, 0, h-1, 0, w32-1,            Y32_fusion_red                 ), n, cpp_fusion32_red                 );
        //BENCH(ouverture3_ui32matrix_swp32_fusion_ilu3_red            (X32, 0, h-1, 0, w32-1,            Y32_fusion_ilu3_red            ), n, cpp_fusion32_ilu3_red            );
        //new2:
        //BENCH(ouverture3_ui32matrix_swp32_fusion_elu2_red            (X32, 0, h-1, 0, w32-1,            Y32_fusion_elu2_red            ), n, cpp_fusion32_elu2_red            );
        //BENCH(ouverture3_ui32matrix_swp32_fusion_ilu3_elu2_red       (X32, 0, h-1, 0, w32-1,            Y32_fusion_ilu3_elu2_red       ), n, cpp_fusion32_ilu3_elu2_red       );
        //BENCH(ouverture3_ui32matrix_swp32_fusion_ilu3_elu2_red_factor(X32, 0, h-1, 0, w32-1,            Y32_fusion_ilu3_elu2_red_factor), n, cpp_fusion32_ilu3_elu2_red_factor);
        
        //BENCH(ouverture3_ui32matrix_swp32_pipeline_basic               (X32, 0, h-1, 0, w32-1, T32_pipeline                     , Y32_pipeline                     ), n, cpp_pipeline32_basic               );
        //BENCH(ouverture3_ui32matrix_swp32_pipeline_red                 (X32, 0, h-1, 0, w32-1, T32_pipeline_red                , Y32_pipeline_red                 ), n, cpp_pipeline32_red                  );
        //BENCH(ouverture3_ui32matrix_swp32_pipeline_ilu3_red            (X32, 0, h-1, 0, w32-1, T32_pipeline_ilu3_red            , Y32_pipeline_ilu3_red            ), n, cpp_pipeline32_ilu3_red            );
        //BENCH(ouverture3_ui32matrix_swp32_pipeline_elu2_red_factor     (X32, 0, h-1, 0, w32-1, T32_pipeline_elu2_red_factor     , Y32_pipeline_elu2_red_factor     ), n, cpp_pipeline32_elu2_red_factor     );
        //BENCH(ouverture3_ui32matrix_swp32_pipeline_ilu3_elu2_red_factor(X32, 0, h-1, 0, w32-1, T32_pipeline_ilu3_elu2_red_factor, Y32_pipeline_ilu3_elu2_red_factor), n, cpp_pipeline32_ilu3_elu2_red_factor);
        
        // 1 version naive
        BENCH(ouverture3_ui64matrix_swp64_basic                      (X64, 0, h-1, 0, w64-1, T64_basic, Y64_basic                      ), n, cpp_basic64                      );
        
        // 6 fonctions fusion
        BENCH(ouverture3_ui64matrix_swp64_fusion_basic               (X64, 0, h-1, 0, w64-1,            Y64_fusion_basic               ), n, cpp_fusion64_basic               );
        BENCH(ouverture3_ui64matrix_swp64_fusion_red                 (X64, 0, h-1, 0, w64-1,            Y64_fusion_red                 ), n, cpp_fusion64_red                 );
        BENCH(ouverture3_ui64matrix_swp64_fusion_ilu3_red            (X64, 0, h-1, 0, w64-1,            Y64_fusion_ilu3_red            ), n, cpp_fusion64_ilu3_red            );
        BENCH(ouverture3_ui64matrix_swp64_fusion_elu2_red            (X64, 0, h-1, 0, w64-1,            Y64_fusion_elu2_red            ), n, cpp_fusion64_elu2_red            );
        BENCH(ouverture3_ui64matrix_swp64_fusion_ilu3_elu2_red       (X64, 0, h-1, 0, w64-1,            Y64_fusion_ilu3_elu2_red       ), n, cpp_fusion64_ilu3_elu2_red       );
        BENCH(ouverture3_ui64matrix_swp64_fusion_ilu3_elu2_red_factor(X64, 0, h-1, 0, w64-1,            Y64_fusion_ilu3_elu2_red_factor), n, cpp_fusion64_ilu3_elu2_red_factor);
        
        // 7 fonctions pipeline
        BENCH(ouverture3_ui64matrix_swp64_pipeline_basic               (X64, 0, h-1, 0, w64-1, T64_pipeline_basic               , Y64_pipeline_basic               ), n, cpp_pipeline64_basic               );
        BENCH(ouverture3_ui64matrix_swp64_pipeline_red                 (X64, 0, h-1, 0, w64-1, T64_pipeline_red                 , Y64_pipeline_red                 ), n, cpp_pipeline64_red                 );
        BENCH(ouverture3_ui64matrix_swp64_pipeline_ilu3_red            (X64, 0, h-1, 0, w64-1, T64_pipeline_ilu3_red            , Y64_pipeline_ilu3_red            ), n, cpp_pipeline64_ilu3_red            );
        BENCH(ouverture3_ui64matrix_swp64_pipeline_elu2_red            (X64, 0, h-1, 0, w64-1, T64_pipeline_elu2_red            , Y64_pipeline_elu2_red            ), n, cpp_pipeline64_elu2_red            );
        BENCH(ouverture3_ui64matrix_swp64_pipeline_elu2_red_factor     (X64, 0, h-1, 0, w64-1, T64_pipeline_elu2_red_factor     , Y64_pipeline_elu2_red_factor     ), n, cpp_pipeline64_elu2_red_factor     );
        BENCH(ouverture3_ui64matrix_swp64_pipeline_ilu3_elu2_red       (X64, 0, h-1, 0, w64-1, T64_pipeline_ilu3_elu2_red, Y64_pipeline_ilu3_elu2_red), n, cpp_pipeline64_ilu3_elu2_red       );
        BENCH(ouverture3_ui64matrix_swp64_pipeline_ilu3_elu2_red_factor(X64, 0, h-1, 0, w64-1, T64_pipeline_ilu3_elu2_red_factor, Y64_pipeline_ilu3_elu2_red_factor), n, cpp_pipeline64_ilu3_elu2_red_factor);

        printf("i = %4d", n);
        printf("   ");
        printf(format, cpp_basic                      );
        
        // printf("   ");
        // printf(format, cpp_fusion_basic               );
        // printf(format, cpp_fusion_red                 );
        // printf(format, cpp_fusion_ilu5_red            );
        // printf(format, cpp_fusion_elu2_red            );
        // printf(format, cpp_fusion_ilu5_elu2_red       );
        // printf(format, cpp_fusion_ilu5_elu2_red_factor);
        // printf(format, cpp_fusion_ilu15_red           );
        
        // printf("   ");
        // printf(format, cpp_pipeline_basic               );
        // printf(format, cpp_pipeline_red                 );
        // printf(format, cpp_pipeline_ilu3_red            );
        // printf(format, cpp_pipeline_elu2_red            );
        // printf(format, cpp_pipeline_elu2_red_factor     );
        // printf(format, cpp_pipeline_ilu3_elu2_red       );
        // printf(format, cpp_pipeline_ilu3_elu2_red_factor);
        
        // printf("   ");
        // printf(format, cpp_basic8                      );
        // printf("   ");
        // printf(format, cpp_fusion8_basic);
        // printf(format, cpp_fusion8_red                 );
        // printf(format, cpp_fusion8_ilu3_red            );
        // printf(format, cpp_fusion8_ilu3_elu2_red       );
        // printf(format, cpp_fusion8_ilu3_elu2_red_factor);
        
        // printf("   ");
        // printf(format, cpp_pipeline8_basic               );
        // printf(format, cpp_pipeline8_ilu3_red            );
        // printf(format, cpp_pipeline8_elu2_red_factor     );
        // printf(format, cpp_pipeline8_ilu3_elu2_red_factor);
        
        //printf("   ");
        //printf(format, cpp_basic32                      );
        //printf(format, cpp_fusion32_basic               );
        //printf(format, cpp_fusion32_red                 );
        //printf(format, cpp_fusion32_ilu3_red            );
        //printf(format, cpp_fusion32_elu2_red            );
        //printf(format, cpp_fusion32_ilu3_elu2_red       );
        //printf(format, cpp_fusion32_ilu3_elu2_red_factor);
        
        //printf("   ");
        //printf(format, cpp_pipeline32_basic               );
        //printf(format, cpp_pipeline32_red                 );
        //printf(format, cpp_pipeline32_ilu3_red            );
        //printf(format, cpp_pipeline32_elu2_red_factor     );
        //printf(format, cpp_pipeline32_ilu3_elu2_red_factor);/**/
        
        printf("   ");
        printf(format, cpp_basic64                      );
        // printf("   ");
        // printf(format, cpp_fusion64_basic               );
        // printf(format, cpp_fusion64_red                 );
        // printf(format, cpp_fusion64_ilu3_red            );
        // printf(format, cpp_fusion64_elu2_red            );
        // printf(format, cpp_fusion64_ilu3_elu2_red       );
        // printf(format, cpp_fusion64_ilu3_elu2_red_factor);
        
        printf("   ");
        printf(format, cpp_pipeline64_basic               );
        printf(format, cpp_pipeline64_red                 );
        printf(format, cpp_pipeline64_ilu3_red            );
        printf(format, cpp_pipeline64_elu2_red            );
        printf(format, cpp_pipeline64_elu2_red_factor     );
        printf(format, cpp_pipeline64_ilu3_elu2_red_factor);/**/
        
        putchar('\n');
    }
}
// ====================================
int test_morpho(int argc, char* argv[])
// ====================================
{
    puts("=================");
    puts("== test_morpho ==");
    puts("=================");
    
#ifdef MORPHO_MACRO
    puts("MORPHO_MACRO is ON");
#endif
#ifdef MORPHO_FUNCTION
    puts("MORPHO_FUNCTION is ON");
#endif
#ifdef MORPHO_OPERATOR_IS_ADD
    puts("MORPHO_OPERATOR_IS_ADD is ON");
#endif
#ifdef MORPHO_OPERATOR_IS_AND_OR
    puts("MORPHO_OPERATOR_IS_AND_OR is ON");
#endif


    
    //test_morpho_dilatation();
    //test_morpho_erosion();
    
    //test_morpho_max();
    //test_morpho_min();
    //test_morpho_ouverture();
    
    // exemple de benchmark
    bench_morpho_ouverture(128, 512, 8);
    //bench_morpho_ouverture(128, 1024, 8);
    //bench_morpho_ouverture(512, 1024, 16);
    //bench_morpho_ouverture(1024, 2048,  16); //bug avec pipeline8
    //bench_morpho_ouverture(2048, 4096,  64);
    
  return 0;
}
// int x = a << 8
// int x = shift(a, 8)
// shift(x,s) t = x; for(int i=0; i<s; i++) t = t << 1;