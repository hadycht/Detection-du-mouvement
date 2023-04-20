/* -------------------------- */
/* --- morpho_test_unit.c --- */
/* -------------------------- */

/*
 * Copyright (c) 2004 - 2013, Lionel Lacassagne, All rights reserved
 * University of Paris Sud, Laboratoire de Recherche en Informatique 
 */

/* -------------- */
/* --- main.c --- */
/* -------------- */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <math.h>

#include "nrtype.h"
#include "nrdef.h"
#include "nrutil.h"
//#include "nrutil_ext.h" // set_ui8vector_str
#include "nralloc_mod.h"

#include "macro_bench.h"
#include "x86intrin.h" // _rdtsc()
#include "swp.h"

#include "morpho.h"

#include "morpho_erosion.h"
#include "morpho_erosion_swp8.h"
#include "morpho_erosion_swp32.h"

#include "morpho_dilatation.h"
#include "morpho_dilatation_swp8.h"
#include "morpho_dilatation_swp32.h"

#include "morpho_ouverture.h"
#include "morpho_ouverture_swp8.h"
#include "morpho_ouverture_swp32.h"

#include "morpho_fermeture.h"
#include "morpho_fermeture_swp8.h"
#include "morpho_fermeture_swp32.h"


#include "morpho_test.h"

// --------------------
void test_set_str(void)
// --------------------
{
    int n = 8;
    uint8 *v;
    
    v = ui8vector(0, n-1);
    //zero_ui8vector(v, 0, n-1);
    display_ui8vector(v, 0, n-1, "%4d", "v0");
    //set_ui8vector_str(v, 0, n-1, ".1.1.");
    display_ui8vector(v, 0, n-1, "%4d", "v1");
    set_ui8vector_str(v, 0, n-1, "12345678");
    display_ui8vector(v, 0, n-1, "%4d", "v2");
}
// ----------------------
void test_wikipedia(void)
// ----------------------
{
    //
    int b = 1; // bord rayon de l'element structurant
    int h = 17;
    int w = 20+4;
    
    // images binaires
    uint8 **X;
    uint8 **E; // erosion
    uint8 **D; // dilatation
    uint8 **O; // ouverture
    uint8 **F; // fermeture
    
    // images packees
    uint8 **X8;
    uint8 **E8; // erosion
    uint8 **D8; // dilatation
    uint8 **O8; // ouverture
    uint8 **F8; // fermeture
    
    // images unpackees
    uint8 **E1; // erosion
    uint8 **D1; // dilatation
    uint8 **O1; // ouverture
    uint8 **F1; // fermeture
    
    X = ui8matrix(0-2*b, h-1+2*b, 0-2*b, w-1+2*b);
    E = ui8matrix(0-1*b, h-1+1*b, 0-1*b, w-1+1*b);
    D = ui8matrix(0-1*b, h-1+1*b, 0-1*b, w-1+1*b);
    O = ui8matrix(0-0*b, h-1+0*b, 0-0*b, w-1+0*b);
    F = ui8matrix(0-0*b, h-1+0*b, 0-0*b, w-1+0*b);
    
    zero_ui8matrix(X, 0-2*b, h-1+2*b, 0-2*b, w-1+2*b);
    zero_ui8matrix(E, 0-1*b, h-1+1*b, 0-1*b, w-1+1*b);
    zero_ui8matrix(D, 0-1*b, h-1+1*b, 0-1*b, w-1+1*b);
    zero_ui8matrix(O, 0-0*b, h-1+0*b, 0-0*b, w-1+0*b);
    zero_ui8matrix(F, 0-0*b, h-1+0*b, 0-0*b, w-1+0*b);
    
    X8 = ui8matrix(0-1*b, h-1+1*b, 0-1*b, w/8-1+1*b);
    E8 = ui8matrix(0-1*b, h-1+1*b, 0-1*b, w/8-1+1*b);
    D8 = ui8matrix(0-1*b, h-1+1*b, 0-1*b, w/8-1+1*b);
    O8 = ui8matrix(0-0*b, h-1+0*b, 0-0*b, w/8-1+0*b);
    F8 = ui8matrix(0-0*b, h-1+0*b, 0-0*b, w/8-1+0*b);
    
    zero_ui8matrix(X8, 0-1*b, h-1+1*b, 0-1*b, w/8-1+1*b);
    zero_ui8matrix(E8, 0-1*b, h-1+1*b, 0-1*b, w/8-1+1*b);
    zero_ui8matrix(D8, 0-1*b, h-1+1*b, 0-1*b, w/8-1+1*b);
    zero_ui8matrix(O8, 0-0*b, h-1+0*b, 0-0*b, w/8-1+0*b);
    zero_ui8matrix(F8, 0-0*b, h-1+0*b, 0-0*b, w/8-1+0*b);
    
    E1 = ui8matrix(0-1*b, h-1+1*b, 0-1*b, w-1+1*b);
    D1 = ui8matrix(0-1*b, h-1+1*b, 0-1*b, w-1+1*b);
    O1 = ui8matrix(0-0*b, h-1+0*b, 0-0*b, w-1+0*b);
    F1 = ui8matrix(0-0*b, h-1+0*b, 0-0*b, w-1+0*b);
    
    zero_ui8matrix(E1, 0-1*b, h-1+1*b, 0-1*b, w-1+1*b);
    zero_ui8matrix(D1, 0-1*b, h-1+1*b, 0-1*b, w-1+1*b);
    zero_ui8matrix(O1, 0-0*b, h-1+0*b, 0-0*b, w-1+0*b);
    zero_ui8matrix(F1, 0-0*b, h-1+0*b, 0-0*b, w-1+0*b);
    
    int i = 0;
    
    // il faut un bord de 1 dans l'image pour que la modification (dilatation) soit visible *dans* l'image
    
    set_ui8vector_str(X[i++], 0, w-1, ".....................");
    set_ui8vector_str(X[i++], 0, w-1, "....11.........11....");
    set_ui8vector_str(X[i++], 0, w-1, ".111..1..111..1..111.");
    set_ui8vector_str(X[i++], 0, w-1, ".111...1111111...111.");
    set_ui8vector_str(X[i++], 0, w-1, ".111..111111111..111.");
    set_ui8vector_str(X[i++], 0, w-1, ".....11111111111.....");
    set_ui8vector_str(X[i++], 0, w-1, ".....11111111111.....");
    set_ui8vector_str(X[i++], 0, w-1, "....1111.111.1111....");
    set_ui8vector_str(X[i++], 0, w-1, "....1111111111111....");
    set_ui8vector_str(X[i++], 0, w-1, "....111111.111111....");
    set_ui8vector_str(X[i++], 0, w-1, "....1111111111111....");
    set_ui8vector_str(X[i++], 0, w-1, ".....1111...1111.....");
    set_ui8vector_str(X[i++], 0, w-1, ".....1111...1111.....");
    set_ui8vector_str(X[i++], 0, w-1, "......111...111......");
    set_ui8vector_str(X[i++], 0, w-1, ".......1111111.......");
    set_ui8vector_str(X[i++], 0, w-1, ".........111.........");
    set_ui8vector_str(X[i++], 0, w-1, ".....................");
    
    display_ui8matrix_positive(X, 0, h-1, 0, w-1, 3, "X");
    
    // ------------------------
    // operateurs 1 bit / pixel
    // ------------------------
    erosion3_ui8matrix_basic(X, 0, h-1, 0, w-1, E);
    dilatation3_ui8matrix_basic(X, 0, h-1, 0, w-1, D);
    
    //dilatation3_ui8matrix_basic(E, 0, h-1, 0, w-1, O);
    //erosion3_ui8matrix_basic(D, 0, h-1, 0, w-1, F);
    
    display_ui8matrix_positive(E, 0, h-1, 0, w-1, 3, "Erosion");
    //display_ui8matrix_positive(D, 0, h-1, 0, w-1, 3, "Dilatation");
    
    //display_ui8matrix_positive(O, 0, h-1, 0, w-1, 3, "Ouverture");
    //display_ui8matrix_positive(F, 0, h-1, 0, w-1, 3, "Fermeture");
    
    // OK
    
    // -------------------------
    // operateurs 8 bits / pixel
    // -------------------------
    pack_ui8matrix(X, h, w, X8);
    //display_ui8matrix_positive(X8, 0, h-1, 0, w/8-1, 5, "X8");
    displayM_ui8matrix(X8, 0, h-1, 0, w/8-1, "X8");
    
    erosion3_ui8matrix_swp8_basic(X8, 0, h-1, 0, w-1, E8);
    displayM_ui8matrix(E8, 0, h-1, 0, w/8-1, "E8");
    
    unpack_ui8matrix(E8, h, w, E1);
    display_ui8matrix_positive(E1, 0, h-1, 0, w-1, 3, "E1");
    
    
    free_ui8matrix(X, 0-2*b, h-1+2*b, 0-2*b, w-1+2*b);
    free_ui8matrix(E, 0-1*b, h-1+1*b, 0-1*b, w-1+1*b);
    free_ui8matrix(D, 0-1*b, h-1+1*b, 0-1*b, w-1+1*b);
    free_ui8matrix(O, 0-0*b, h-1+0*b, 0-0*b, w-1+0*b);
    free_ui8matrix(F, 0-0*b, h-1+0*b, 0-0*b, w-1+0*b);
}
// ==========================================
int test_morpho_extra(int argc, char* argv[])
// ==========================================
{
    puts("=======================");
    puts("== test_morpho_extra ==");
    puts("=======================");
    
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

    test_set_str();
    test_wikipedia();
    
  return 0;
}
