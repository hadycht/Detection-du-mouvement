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

//#include "sigmadelta.h"
//#include "morpho.h"

#include "nralloc_mod.h"
#include "nralloc_mod_test.h"

//#include "nrutil_ext_test.h"
#include "swp_test.h"

#include "morpho_test.h"
#include "motion_test.h"

#define PREFIX(X) ETU_##X

void PREFIX(line_morpho_max_ui8matrix)(uint8 *X, int j0, int j1, uint8 *Y)
{
    for(int j = j0; j <= j1; j++) {
        Y[j] = X[j];
    }
}

void PREFIX(morpho_max_ui8matrix)(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
{
    for(int i = i0; i <= i1; i++) {
        PREFIX(line_morpho_max_ui8matrix)(X[i], j0, j1, Y[i]);
    }
}
// =============================
int main(int argc, char *argv[])
// =============================
{
    
    //test_nralloc_mod(argc, argv);
    //test_nrutil_ext(argc, argv); //manipulation et affichage en bit
    //test_swp(argc, argv);
    
    test_morpho(argc, argv);
    //test_motion(argc, argv);
    return 0;
}
