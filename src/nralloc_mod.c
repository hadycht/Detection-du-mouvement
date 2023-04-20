/* ------------------- */
/* -- nralloc_mod.c -- */
/* ------------------- */


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

// --------------------------------------------------------------
uint8** ui8matrix_mod(int i0, int i1, int j0, int j1, int modulo)
// --------------------------------------------------------------
{
    int nrow = i1-i0+1;
    int ncol = j1-j0+1;
    
    uint8 **m;
    
    // allocate pointers to rows
    m=(uint8 **) malloc((size_t)((nrow)*sizeof(float32*)));
    if (!m) nrerror("allocation failure 1 in ui8matrix_mod()");
    m -= i0;
    
    // allocate rows and set pointers to them
    m[i0]=(uint8 *) malloc((size_t)(modulo*ncol)*sizeof(uint8));
    if (!m[i0]) nrerror("allocation failure 2 in ui8matrix_mod()");
    m[i0] -= j0;
    
    // set line-pointer to real modular memory
    for(int i=i0+1; i<i0+modulo; i++) m[i] = m[i-1] + ncol;
    
    // duplicate pointers
    for(int i=i0+modulo; i<=i1; i++) m[i]=m[i-modulo];
    
    return m;
}
// -----------------------------------------------------------------------------
void resize_ui8matrix_mod(uint8 **m, int i0, int i1, int j0, int j1, int modulo)
// -----------------------------------------------------------------------------
{
    // from ui8matrix_mod
    int nrow = i1-i0+1;
    int ncol = j1-j0+1;
    
    // set line-pointer to real modular memory
    for(int i=i0+1; i<i0+modulo; i++) m[i] = m[i-1]+ncol;
    
    // duplicate pointers
    for(int i=i0+modulo; i<=i1; i++) m[i] = m[i-modulo];
}
// ---------------------------------------------------------------------------
void free_ui8matrix_mod(uint8** m, int i0, int i1, int j0, int j1, int modulo)
// ---------------------------------------------------------------------------
{
    // same than free_ui8matrix
    free(m[i0]+j0);
    free(m+i0);
}
