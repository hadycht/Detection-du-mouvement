/* -------------------- */
/* --- sigmadelta.c --- */
/* -------------------- */

/*
 * Copyright (c) 2004 - 2013, Lionel Lacassagne, All rights reserved
 * University of Paris Sud, Laboratoire de Recherche en Informatique 
 * Creation: 2004-05-18 :
 * Creation: 2021-01-06 : version line pour pipeline
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <math.h>

#include "nrtype.h"
#include "nrdef.h"
#include "nrutil.h"
#include "morpho.h"

#include "sigmadelta.h"

// -----------------------------------------------------------------------------------------
void SigmaDelta_Step0_line(uint8 *I, uint8 *M, uint8 *O, uint8 *V, uint8 *E, int j0, int j1)
// -----------------------------------------------------------------------------------------
{
    for (int j = j0; j <= j1; j++) { 
        store1(M, j, (load1(I, j)));
        store1(V, j, SD_VMIN);

        store1(O, j, load1(M, j) - load1(I, j));

        if (load1(O, j) < load1(V ,j)) {
            store1(E, j, 0);
        }
        else {
            store1(E, j, 1);
        }
    }
}
// ------------------------------------------------------------------------------------------------
void SigmaDelta_1Step_line(uint8 *I, uint8 *M, uint8 *O, uint8 *V, uint8 *E, int k, int j0, int j1)
// ------------------------------------------------------------------------------------------------
{
    for (int j=j0; j <= j1; j++) {

        // Mise à jour de la matrice M (Moyenne)

        // Scalaraisation
        int M_t_1 = load1(M, j);
        int I_t = load1(I, j);

        if (M_t_1 < I_t) {
            store1(M, j, M_t_1 + 1);
        }
        else if (M_t_1 > I_t) {
            store1(M, j, M_t_1 - 1);
        }
        else {
            store1(M, j, M_t_1);
        }

        // Mise à jour de la Matrice O (Observation)
        store1(O, j, abs(load1(M, j), load1(I, j)));

        // Mise à jour de la Matrice V (VAriante)

        // Scalaraisation
        int V_t_1 = load1(V, j);
        int O_t = load1(O, j);
        int update = k * O_t;

        if (V_t_1 < update) {
            store1(V, j, V_t_1 + 1);
        }
        else if (V_t_1 > update) {
            store1(V, j, V_t_1 - 1);
        }
        else {
            store1(V, j, V_t_1);
        } 
        store1(V, j, max(min(load1(V, j), SD_VMAX), SD_VMIN));


        // Mise à jour de la Matrice E ()

        if (load1(O, j) < load1(V, j)) {
            store1(E, j, 0);
        }
        else {
            store1(E, j, 255);
        }
    }
}
// ---------------------------------------------------------------------------------------------------------
void SigmaDelta_Step0(uint8 **I, uint8 **M, uint8 **O, uint8 **V, uint8 **E, int i0, int i1, int j0, int j1)
// ---------------------------------------------------------------------------------------------------------
{
    for(int i=i0; i<=i1; i++) {
        SigmaDelta_Step0_line(I[i], M[i], O[i], V[i], E[i], j0, j1);
    }
}
// ----------------------------------------------------------------------------------------------------------------
void SigmaDelta_1Step(uint8 **I, uint8 **M, uint8 **O, uint8 **V, uint8 **E, int k, int i0, int i1, int j0, int j1)
// ----------------------------------------------------------------------------------------------------------------
{
    for(int i=i0; i<=i1; i++) {
        SigmaDelta_1Step_line(I[i], M[i], O[i], V[i], E[i], k, j0, j1);
    }
}
