/* -------------------- */
/* --- morpho_max.c --- */
/* -------------------- */

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
#include "morpho.h"
#include "morpho_max.h"

// ------------------------------------------------------------------------
void line_max3_ui8matrix_basic(uint8 **X, int i, int j0, int j1, uint8 **Y)
// -------------------------------------------------------------------------------
{
    for(int j=j0; j<=j1; j++) {
        store2(Y, i, j, ui8max9(load2(X,i-1,j-1), load2(X,i-1,j), load2(X,i-1,j+1),
                                load2(X,i  ,j-1), load2(X,i  ,j), load2(X,i  ,j+1),
                                load2(X,i+1,j-1), load2(X,i+1,j), load2(X,i+1,j+1)));
    }
}
// -----------------------------------------------------------------------------
void line_max3_ui8matrix_reg(uint8 **X, int i, int j0, int j1, uint8 **Y)
// -----------------------------------------------------------------------------
{
    for (int j=j0; j<=j1; j++) {

        uint8 x0 = ui8max3(load2(X,i-1,j-1), load2(X,i-1,j), load2(X,i-1,j+1));
        uint8 x1 = ui8max3(load2(X,i,j-1), load2(X,i,j), load2(X,i,j+1));
        uint8 x2 = ui8max3(load2(X,i+1,j-1), load2(X,i+1,j), load2(X,i+1,j+1));

        store2(Y, i, j, ui8max3(x0, x1, x2));

    }
}
// -----------------------------------------------------------------------------
void line_max3_ui8matrix_rot(uint8 **X, int i, int j0, int j1, uint8 **Y)
// -----------------------------------------------------------------------------
{
    // on lit les valeurs par colonne

    uint8 x0 = load2(X, i-1, j0-1);
    uint8 x1 = load2(X, i, j0-1);
    uint8 x2 = load2(X, i+1, j0-1);
    
    uint8 x3 = load2(X, i-1, j0);
    uint8 x4 = load2(X, i, j0);
    uint8 x5 = load2(X, i+1,j0);


    for (int j=j0+1; j<=j1+1; j++) {

        uint8 x6 = load2(X, i-1, j);
        uint8 x7 = load2(X, i, j);
        uint8 x8 = load2(X, i+1, j);
        
        store2(Y, i, j-1, ui8max9(x0,x1,x2,x3,x4,x5,x6,x7,x8));

        // la on fait la rotation des registres

        x0 = x3;
        x1 = x4;
        x2 = x5;

        x3 = x6;
        x4 = x7;
        x5 = x8;

    }
}
// -----------------------------------------------------------------------------
void line_max3_ui8matrix_red(uint8 **X, int i, int j0, int j1, uint8 **Y)
// -----------------------------------------------------------------------------
{
    uint8 x0, x1, x2, ra, rb, rc, y;

    x0 = load2(X,i-1,j0-1);
    x1 = load2(X, i, j0-1);
    x2 = load2(X, i+1,j0-1);

    ra = ui8max3(x0,x1,x2); //reduction de la premiere colonne

    x0 = load2(X,i-1,j0);
    x1 = load2(X, i, j0);
    x2 = load2(X, i+1,j0);

    rb = ui8max3(x0,x1,x2); //reduction de la deuxieme colonne

    for (int j = j0; j<= j1; j++) {
        x0 = load2(X,i-1,j+1);
        x1 = load2(X, i, j+1);
        x2 = load2(X, i+1,j+1); 

        rc = ui8max3(x0,x1,x2); //reduction de la troisième colonne

        y = ui8max3(ra, rb, rc);
        store2(Y,i,j, y);

        // rotation des colonnes
        ra = rb;
        rb = rc;

    }
}
// ------------------------------------------------------------------------------
void line_max3_ui8matrix_ilu3(uint8 **X, int i, int j0, int j1, uint8 **Y)
// ------------------------------------------------------------------------------
{
    uint8 x1, x2, x3, y;
    int r = (j1 - j0 + 1)%3;
    int j;
    for (j = j0; j< j1-r-1; j+=3) {

        store2(Y, i, j, ui8max9(load2(X, i-1, j-1), load2(X,i-1, j), load2(X, i-1, j+1),
                                load2(X, i  , j-1), load2(X, i , j), load2(X, i  , j+1),
                                load2(X, i+1, j-1), load2(X, i+1,j), load2(X, i+1,j+1)));

        store2(Y, i, j+1, ui8max9(load2(X, i-1, j), load2(X,i-1, j+1), load2(X, i-1, j+2),
                                  load2(X, i  , j), load2(X, i , j+1), load2(X, i  , j+2),
                                  load2(X, i+1, j), load2(X, i+1,j+1), load2(X, i+1,j+2)));

        store2(Y, i, j+2, ui8max9(load2(X, i-1, j+1), load2(X,i-1, j+2), load2(X, i-1, j+3),
                                  load2(X, i  , j+1), load2(X, i , j+2), load2(X, i  , j+3),
                                  load2(X, i+1, j+1), load2(X, i+1,j+2), load2(X, i+1,j+3)));
    
    }
    if (r) {
        for (int j=j1-r-1; j <= j1; j++) {
            store2(Y, i, j, ui8max9(load2(X, i-1, j-1), load2(X,i-1, j), load2(X, i-1, j+1),
                                    load2(X, i  , j-1), load2(X, i , j), load2(X, i  , j+1),
                                    load2(X, i+1, j-1), load2(X, i+1,j), load2(X, i+1,j+1)));
        
        }
    }
}
// ----------------------------------------------------------------------------------
void line_max3_ui8matrix_ilu3_red(uint8 **X, int i, int j0, int j1, uint8 **Y)
// ----------------------------------------------------------------------------------
{
    int r = (j1 - j0 + 1)%3;

    uint8 x0, x1, x2, ra, rb, rc, y0, y1, y2;

    x0 = load2(X, i-1, j0-1);
    x1 = load2(X, i, j0-1);
    x2 = load2(X, i+1, j0-1);

    ra = ui8max3(x0,x1,x2); //reduction de la premiere colonne

    x0 = load2(X, i-1, j0);
    x1 = load2(X, i, j0);
    x2 = load2(X, i+1, j0);

    rb = ui8max3(x0,x1,x2); //reduction de la deuxieme colonne
    int j;
    for (j = j0; j< j1-r-1; j+=3) {

        x0 = load2(X, i-1, j+1);
        x1 = load2(X, i, j+1);
        x2 = load2(X, i+1, j+1); 

        rc = ui8max3(x0,x1,x2); //reduction de la troisième colonne

        y0 = ui8max3(ra, rb, rc);
        store2(Y,i,j, y0);

        // rotation des colonnes
        ra = rb;
        rb = rc; 

        x0 = load2(X, i-1, j+2);
        x1 = load2(X, i, j+2);
        x2 = load2(X, i+1, j+2);

        rc = ui8max3(x0, x1, x2);

        y1 = ui8max3(ra, rb, rc);
        store2(Y, i, j+1, y1);

        ra = rb;
        rb = rc; 

        x0 = load2(X, i-1, j+3);
        x1 = load2(X, i, j+3);
        x2 = load2(X, i+1, j+3);

        rc = ui8max3(x0, x1, x2);

        y2 = ui8max3(ra, rb, rc);
        store2(Y, i, j+2, y2);

        ra = rb;
        rb = rc;

    }
    if (r) {
        for (int k = 0; k < r; k++) {
            store2(Y, i, j+k, ui8max9(load2(X, i-1, j+k-1), load2(X,i-1, j+k), load2(X, i-1, j+k+1),
                        load2(X, i  , j+k-1), load2(X, i , j+k), load2(X, i  , j+k+1),
                        load2(X, i+1, j+k-1), load2(X, i+1,j+k), load2(X, i+1,j+k+1)));
        
        } 
    } 
}
// ----------------------------------------------------------------------------------
void line_max3_ui8matrix_elu2_red(uint8 **X, int i, int j0, int j1, uint8 **Y)
// ----------------------------------------------------------------------------------
{
    uint8 x0, x1, x2, x3, ra, ra1, rb, rb1, rc, rc1, y;
        
    for (int j = j0; j<= j1; j++) {
        x0 = load2(X, i-1, j-1);
        x1 = load2(X, i, j-1);
        x2 = load2(X, i+1, j-1);

        x3 = load2(X, i+2, j-1);

        ra = ui8max3(x0,x1,x2); //reduction de la premiere colonne
        ra1 = ui8max3(x1, x2, x3); //reduction de la premiere colonne bis
        

        x0 = load2(X, i-1, j);
        x1 = load2(X, i, j);
        x2 = load2(X, i+1, j);

        x3 = load2(X, i+2, j);

        rb = ui8max3(x0,x1,x2); //reduction de la deuxieme colonne
        rb1 = ui8max3(x1, x2, x3); //reduction de la deuxieme colonne bis

        x0 = load2(X, i-1, j+1);
        x1 = load2(X, i, j+1);
        x2 = load2(X, i+1, j+1);

        x3 = load2(X, i+2, j+1);

        rc = ui8max3(x0,x1,x2); //reduction de la troisième colonne
        rc1 = ui8max3(x1, x2, x3); //reduction de la troisieme colonne bis

        y = ui8max3(ra, rb, rc); 
        store2(Y,i,j, y);

        y = ui8max3(ra1, rb1, rc1);
        store2(Y,i+1,j, y);

        //rotation des colonnes
        ra = rb;
        rb = rc; 
        //rotation des colonnes bis
        ra1 = rb1;
        rb1 = rc1;
    }
}
// -----------------------------------------------------------------------------------------
void line_max3_ui8matrix_elu2_red_factor(uint8 **X, int i, int j0, int j1, uint8 **Y)
// -----------------------------------------------------------------------------------------
{
    uint8 x0, x1, x2, x3, ra, ra1, rb, rb1, rc, rc1, y, tmp;
    
    x0 = load2(X, i-1, j0-1);
    x1 = load2(X, i, j0-1);
    x2 = load2(X, i+1, j0-1);

    x3 = load2(X, i+2, j0-1);

    tmp = ui8max2(x1, x2); //on factorise le calcul de x1 et x2
    ra = ui8max2(x0,tmp); //reduction de la premiere colonne
    ra1 = ui8max2(tmp, x3); //reduction de la premiere colonne bis
    

    x0 = load2(X, i-1, j0);
    x1 = load2(X, i, j0);
    x2 = load2(X, i+1, j0);

    x3 = load2(X, i+2, j0);

    tmp = ui8max2(x1, x2); //on factorise le calcul de x1 et x2
    rb = ui8max2(x0,tmp); //reduction de la deuxieme colonne
    rb1 = ui8max2(tmp, x3); //reduction de la deuxieme colonne bis
    
    for (int j = j0; j<= j1; j++) {
        x0 = load2(X, i-1, j+1);
        x1 = load2(X, i, j+1);
        x2 = load2(X, i+1, j+1);

        x3 = load2(X, i+2, j+1);

        tmp = ui8max2(x1, x2); //on factorise le calcul de x1 et x2
        rc = ui8max2(x0,tmp); //reduction de la troisième colonne
        rc1 = ui8max2(tmp, x3); //reduction de la troisième colonne bis

        y = ui8max3(ra, rb, rc); 
        store2(Y,i,j, y);

        y = ui8max3(ra1, rb1, rc1);
        store2(Y,i+1,j, y);

        //rotation des colonnes
        ra = rb;
        rb = rc; 
        //rotation des colonnes bis
        ra1 = rb1;
        rb1 = rc1;
    }
}
// ---------------------------------------------------------------------------------------
void line_max3_ui8matrix_ilu3_elu2_red(uint8 **X, int i, int j0, int j1, uint8 **Y)
// ---------------------------------------------------------------------------------------
{
    int r = (j1 - j0 + 1) % 3;

    uint8 x0, x1, x2, x3, ra, ra1, rb, rb1, rc, rc1, y0, y1, y2;

    int j;
    for (j = j0; j< j1-r-1; j+=3) {

        x0 = load2(X, i-1, j-1);
        x1 = load2(X, i, j-1);
        x2 = load2(X, i+1, j-1);

        x3 = load2(X, i+2, j-1);

        ra = ui8max3(x0,x1,x2); //reduction de la premiere colonne
        ra1 = ui8max3(x1, x2, x3); //reduction de la premiere colonne bis
        

        x0 = load2(X, i-1, j);
        x1 = load2(X, i, j);
        x2 = load2(X, i+1, j);

        x3 = load2(X, i+2, j);

        rb = ui8max3(x0,x1,x2); //reduction de la deuxieme colonne
        rb1 = ui8max3(x1, x2, x3); //reduction de la deuxieme colonne bis

        x0 = load2(X, i-1, j+1);
        x1 = load2(X, i, j+1);
        x2 = load2(X, i+1, j+1);

        x3 = load2(X, i+2, j+1);

        rc = ui8max3(x0,x1,x2); //reduction de la troisième colonne
        rc1 = ui8max3(x1, x2, x3); //reduction de la troisième colonne bis

        y0 = ui8max3(ra, rb, rc);
        store2(Y, i, j, y0);

        y0 = ui8max3(ra1, rb1, rc1);
        store2(Y, i+1, j, y0);

        //rotation des colonnes 
        ra = rb;
        rb = rc; 
        //rotation des colonnes bis
        ra1 = rb1;
        rb1 = rc1;

        x0 = load2(X, i-1, j+2);
        x1 = load2(X, i, j+2);
        x2 = load2(X, i+1, j+2);

        x3 = load2(X, i+2, j+2);


        rc = ui8max3(x0, x1, x2); 
        rc1 = ui8max3(x1, x2, x3);

        y1 = ui8max3(ra, rb, rc);
        store2(Y, i, j+1, y1);

        y1 = ui8max3(ra1, rb1, rc1);
        store2(Y, i+1, j+1, y1);

        //rotation des colonnes 
        ra = rb;
        rb = rc; 
        //rotation des colonnes bis
        ra1 = rb1;
        rb1 = rc1;

        x0 = load2(X, i-1, j+3);
        x1 = load2(X, i, j+3);
        x2 = load2(X, i+1, j+3);

        x3 = load2(X, i+2, j+3);


        rc = ui8max3(x0, x1, x2);
        rc1 = ui8max3(x1, x2, x3);

        y2 = ui8max3(ra, rb, rc);
        store2(Y, i, j+2, y2);

        y2 = ui8max3(ra1, rb1, rc1);
        store2(Y, i+1, j+2, y2);

        //rotation des colonnes 
        ra = rb;
        rb = rc; 
        //rotation des colonnes bis
        ra1 = rb1;
        rb1 = rc1;

    }
    if (r) {
        for (int k = 0; k < r; k++) {
            store2(Y, i, j+k, ui8max9(load2(X, i-1, j+k-1), load2(X, i-1,j+k), load2(X, i-1, j+k+1),
                                      load2(X, i  , j+k-1), load2(X, i , j+k), load2(X, i  , j+k+1),
                                      load2(X, i+1, j+k-1), load2(X, i+1,j+k), load2(X, i+1, j+k+1)));

            store2(Y, i+1, j+k, ui8max9(load2(X, i, j+k-1), load2(X, i,j+k), load2(X, i, j+k+1),
                                        load2(X, i+1  , j+k-1), load2(X, i+1 , j+k), load2(X, i+1  , j+k+1),
                                        load2(X, i+2, j+k-1), load2(X, i+2,j+k), load2(X, i+2, j+k+1)));
        
        }
    }
}
// ----------------------------------------------------------------------------------------------
void line_max3_ui8matrix_ilu3_elu2_red_factor(uint8 **X, int i, int j0, int j1, uint8 **Y)
// ----------------------------------------------------------------------------------------------
{
    int r = (j1 - j0 + 1) % 3;

    uint8 x0, x1, x2, x3, ra, ra1, rb, rb1, rc, rc1, y0, y1, y2, tmp;

    x0 = load2(X, i-1, j0-1);
    x1 = load2(X, i, j0-1);
    x2 = load2(X, i+1, j0-1);

    x3 = load2(X, i+2, j0-1);

    tmp = ui8max2(x1, x2); //on factorise le calcul x1 et x2
    ra = ui8max2(x0,tmp); //reduction de la premiere colonne
    ra1 = ui8max2(tmp, x3); //reduction de la premiere colonne bis
    

    x0 = load2(X, i-1, j0);
    x1 = load2(X, i, j0);
    x2 = load2(X, i+1, j0);

    x3 = load2(X, i+2, j0);

    tmp = ui8max2(x1, x2); //on factorise le calcul x1 et x2
    rb = ui8max2(x0,tmp); //reduction de la deuxieme colonne
    rb1 = ui8max2(tmp, x3); //reduction de la deuxieme colonne bis

    int j;
    for (j = j0; j< j1-r-1; j+=3) {
        x0 = load2(X, i-1, j+1);
        x1 = load2(X, i, j+1);
        x2 = load2(X, i+1, j+1);

        x3 = load2(X, i+2, j+1);

        tmp = ui8max2(x1, x2); //on factorise le calcul x1 et x2
        rc = ui8max2(x0,tmp); //reduction de la troisième colonne
        rc1 = ui8max2(tmp, x3); //reduction de la troisieme colonne bis

        y0 = ui8max3(ra, rb, rc);
        store2(Y, i, j, y0);

        y0 = ui8max3(ra1, rb1, rc1);
        store2(Y, i+1, j, y0);

        //rotation des colonnes
        ra = rb;
        rb = rc; 
        //rotation des colonnes bis
        ra1 = rb1;
        rb1 = rc1;

        x0 = load2(X, i-1, j+2);
        x1 = load2(X, i, j+2);
        x2 = load2(X, i+1, j+2);

        x3 = load2(X, i+2, j+2);

        tmp = ui8max2(x1, x2);
        rc = ui8max2(x0, tmp);
        rc1 = ui8max2(tmp, x3);

        y1 = ui8max3(ra, rb, rc);
        store2(Y, i, j+1, y1);

        y1 = ui8max3(ra1, rb1, rc1);
        store2(Y, i+1, j+1, y1);
        //rotation des colonnes
        ra = rb;
        rb = rc; 
        //rotation des colonnes bis
        ra1 = rb1;
        rb1 = rc1;

        x0 = load2(X, i-1, j+3);
        x1 = load2(X, i, j+3);
        x2 = load2(X, i+1, j+3);

        x3 = load2(X, i+2, j+3);

        tmp = ui8max2(x1, x2);
        rc = ui8max2(x0, tmp);
        rc1 = ui8max2(tmp, x3);

        y2 = ui8max3(ra, rb, rc);
        store2(Y, i, j+2, y2);

        y2 = ui8max3(ra1, rb1, rc1);
        store2(Y, i+1, j+2, y2);
        //rotation des colonnes
        ra = rb;
        rb = rc; 
        //rotation des colonnes bis
        ra1 = rb1;
        rb1 = rc1;

    }
    if (r) {
        for (int k = 0; k < r; k++) {
            store2(Y, i, j+k, ui8max9(load2(X, i-1, j+k-1), load2(X, i-1,j+k), load2(X, i-1, j+k+1),
                                    load2(X, i  , j+k-1), load2(X, i , j+k), load2(X, i  , j+k+1),
                                    load2(X, i+1, j+k-1), load2(X, i+1,j+k), load2(X, i+1, j+k+1)));

            store2(Y, i+1, j+k, ui8max9(load2(X, i, j+k-1), load2(X, i,j+k), load2(X, i, j+k+1),
                                    load2(X, i+1  , j+k-1), load2(X, i+1 , j+k), load2(X, i+1  , j+k+1),
                                    load2(X, i+2, j+k-1), load2(X, i+2,j+k), load2(X, i+2, j+k+1)));
        
        }
    }
}

// -----------------------------------------------------------------------------------
void max3_ui8matrix_basic(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// -----------------------------------------------------------------------------------
{
    for(int i=i0; i<=i1; i++) {
        //printf("i = %d\n", i);
        line_max3_ui8matrix_basic(X, i, j0, j1, Y);
    }
}
// ---------------------------------------------------------------------------------
void max3_ui8matrix_reg(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// ---------------------------------------------------------------------------------
{
    for (int i = i0; i <= i1; i++) {
        line_max3_ui8matrix_reg(X, i, j0, j1, Y);
    }
}
// ---------------------------------------------------------------------------------
void max3_ui8matrix_rot(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// ---------------------------------------------------------------------------------
{
    for (int i = i0; i <= i1; i++) {
        line_max3_ui8matrix_rot(X, i, j0, j1, Y);
    }
}
// ---------------------------------------------------------------------------------
void max3_ui8matrix_red(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// ---------------------------------------------------------------------------------
{
    for (int i = i0; i <= i1; i++) {
        line_max3_ui8matrix_reg(X, i, j0, j1, Y);
    }
}
// ----------------------------------------------------------------------------------
void max3_ui8matrix_ilu3(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// ----------------------------------------------------------------------------------
{
    for (int i = i0; i <= i1; i++) {
        line_max3_ui8matrix_ilu3(X, i, j0, j1, Y);
    }
}
// --------------------------------------------------------------------------------------
void max3_ui8matrix_ilu3_red(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// --------------------------------------------------------------------------------------
{
    for (int i = i0; i <= i1; i++) {
        line_max3_ui8matrix_ilu3_red(X, i, j0, j1, Y);
    }
}
// --------------------------------------------------------------------------------------
void max3_ui8matrix_elu2_red(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// --------------------------------------------------------------------------------------
{
    int r = (i1 - i0 + 1) % 2;
    int i;
    for(i = i0; i<= i1-r; i+=2) {
        line_max3_ui8matrix_elu2_red(X, i, j0, j1, Y);
    } 
    if (r) {
        line_max3_ui8matrix_elu2_red(X, i-1, j0, j1, Y);
    }
}
// ---------------------------------------------------------------------------------------------
void max3_ui8matrix_elu2_red_factor(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// ---------------------------------------------------------------------------------------------
{
    int r = (i1 - i0 + 1) % 2;
    int i;
    for(i = i0; i<= i1-r; i+=2) {
        line_max3_ui8matrix_elu2_red_factor(X, i, j0, j1, Y);
    } 
    if (r) {
        line_max3_ui8matrix_elu2_red_factor(X, i-1, j0, j1, Y);
    }
}
// -------------------------------------------------------------------------------------------
void max3_ui8matrix_ilu3_elu2_red(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// -------------------------------------------------------------------------------------------
{
    int r = (i1 - i0 + 1) % 2;
    int i;
    for(i = i0; i<= i1-r; i+=2) {
        line_max3_ui8matrix_ilu3_elu2_red(X, i, j0, j1, Y);
    } 
    if (r) {
        line_max3_ui8matrix_ilu3_elu2_red(X, i-1, j0, j1, Y);
    }
}
// --------------------------------------------------------------------------------------------------
void max3_ui8matrix_ilu3_elu2_red_factor(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// --------------------------------------------------------------------------------------------------
{
    int r = (i1 - i0 + 1) % 2;
    int i;
    for(i = i0; i<= i1-r; i+=2) {
        line_max3_ui8matrix_ilu3_elu2_red_factor(X, i, j0, j1, Y);
    } 
    if (r) {
        line_max3_ui8matrix_ilu3_elu2_red_factor(X, i-1, j0, j1, Y);
    }
}

