/* ------------- */
/* --- swp.c --- */
/* ------------- */

/*
 * Copyright (c) 2021 - 2021, Lionel Lacassagne, All rights reserved
 * LIP6 - Sorbonne University
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include <ctype.h> // isdigit
#include <inttypes.h> // printf 64 bits

#include "nrtype.h"
#include "nrdef.h"
#include "nrutil.h"

#include "swp.h"
#include "swp_test.h"

// =================================================
void B2b(int i0, int i1, int card, int* b0, int* b1)
// =================================================
{
    if(i0 % card) {
        *b0 = (i0 - card) / card;
    } else {
        *b0 = i0 / card;
    }
    *b1 = i1 / card;
}
// -----------------------------------------
void Btob8(int i0, int i1, int* b0, int* b1)
// -----------------------------------------
{
    B2b(i0, i1, 8, b0, b1);
}
// ------------------------------------------
void Btob16(int i0, int i1, int* b0, int* b1)
// ------------------------------------------
{
    B2b(i0, i1, 16, b0, b1);
}
// ------------------------------------------
void Btob32(int i0, int i1, int* b0, int* b1)
// ------------------------------------------
{
    B2b(i0, i1, 32, b0, b1);
}
// ------------------------------------------
void Btob64(int i0, int i1, int* b0, int* b1)
// ------------------------------------------
{
    B2b(i0, i1, 64, b0, b1);
}
// =================================================
void b2B(int b0, int b1, int card, int* m0, int* m1)
// =================================================
{
    *m0 = card * b0;
    *m1 = card * (b1+1) - 1;
}
// -----------------------------------------
void b8toB(int b0, int b1, int* m0, int* m1)
// -----------------------------------------
{
    b2B(b0, b1, 8, m0, m1);
}
// ------------------------------------------
void b16toB(int b0, int b1, int* m0, int* m1)
// ------------------------------------------
{
    b2B(b0, b1, 16, m0, m1);
}
// ------------------------------------------
void b32toB(int b0, int b1, int* m0, int* m1)
// ------------------------------------------
{
    b2B(b0, b1, 32, m0, m1);
}
// ------------------------------------------
void b64toB(int b0, int b1, int* m0, int* m1)
// ------------------------------------------
{
    b2B(b0, b1, 64, m0, m1);
}
// -----------------------------------
uint8 ui8left(uint8 a, uint8 b, int k)
// -----------------------------------
{
    return ( (a << (8-k)) | (b >> k) );
}
// ---------------------------------------
uint16 ui16left(uint16 a, uint16 b, int k)
// ---------------------------------------
{
    return ( (a << (16-k)) | (b >> k) );
}
// ---------------------------------------
uint32 ui32left(uint32 a, uint32 b, int k)
// ---------------------------------------
{
   return ( (a << (32-k)) | (b >> k) );
}
// ---------------------------------------
uint64 ui64left(uint64 a, uint64 b, int k)
// ---------------------------------------
{
    return ( (a << (64-k)) | (b >> k) );
}
// ------------------------------------
uint8 ui8right(uint8 b, uint8 c, int k)
// ------------------------------------
{
    return ( (b >> (8-k)) | ( c << k) );
}
// ----------------------------------------
uint16 ui16right(uint16 b, uint16 c, int k)
// ----------------------------------------
{
    return ( (b >> (16-k)) | ( c << k) );
}
// ----------------------------------------
uint32 ui32right(uint32 b, uint32 c, int k)
// ----------------------------------------
{
    return ( (b >> (32-k)) | ( c << k) );
}
// ----------------------------------------
uint64 ui64right(uint64 b, uint64 c, int k)
// ----------------------------------------
{
    return ( (b >> (64-k)) | ( c << k) );
}

// -----------------------------------
void printfR8(uint8_t x8r, char* name)
// -----------------------------------
{
    // affichage en binaire "Register-like"
    bit_t b0,b1,b2,b3,b4,b5,b6,b7;
    
    R8B(x8r, b7,b6,b5,b4,b3,b2,b1,b0);
    
    if(name) {
        printf("%s = (%02x)h = (%d%d%d%d%d%d%d%d)b\n", name, x8r, b7,b6,b5,b4,b3,b2,b1,b0);
    } else {
        printf("%d%d%d%d%d%d%d%d ", b7,b6,b5,b4,b3,b2,b1,b0);
    }
}
// ---------------------------------
void printfM8(uint8_t x8m, char* name)
// ---------------------------------
{
    // affichage en binaire "Memory-like"
    bit_t b0,b1,b2,b3,b4,b5,b6,b7;
    
    M8B(x8m, b0,b1,b2,b3,b4,b5,b6,b7);
    
    if(name) {
        printf("%s = (%02x)h = (%d%d%d%d%d%d%d%d)b\n", name, x8m, b0,b1,b2,b3,b4,b5,b6,b7);
    } else {
        printf("%d%d%d%d%d%d%d%d ", b0,b1,b2,b3,b4,b5,b6,b7);
    }
}
// --------------------------------------
void printfR16(uint16_t x16r, char* name)
// --------------------------------------
{
    // affichage en binaire "Register-like"
    bit_t b0,b1,b2,b3,b4,b5,b6,b7;
    bit_t b8,b9,ba,bb,bc,bd,be,bf;
    
    R8B(((x16r >> 8) & 0xff), bf,be,bd,bc,bb,ba,b9,b8);
    R8B((x16r        & 0xff), b7,b6,b5,b4,b3,b2,b1,b0);
    
    if(name) {
        printf("%s = (%04x)h = (%d%d%d%d%d%d%d%d,%d%d%d%d%d%d%d%d)b\n", name, x16r, bf,be,bd,bc,bb,ba,b9,b8, b7,b6,b5,b4,b3,b2,b1,b0);
    } else {
        printf("%d%d%d%d%d%d%d%d %d%d%d%d%d%d%d%d", bf,be,bd,bc,bb,ba,b9,b8, b7,b6,b5,b4,b3,b2,b1,b0);
    }
}
// --------------------------------------
void printfM16(uint16_t x16m, char* name)
// --------------------------------------
{
    // affichage en binaire "Memory-like"
    bit_t b0,b1,b2,b3,b4,b5,b6,b7;
    bit_t b8,b9,ba,bb,bc,bd,be,bf;
    
    M8B(((x16m >> 16)& 0xff), b8,b9,ba,bb,bc,bd,be,bf);
    M8B((x16m        & 0xff), b0,b1,b2,b3,b4,b5,b6,b7);
    
    if(name) {
        printf("%s = (%04x)h = (%d%d%d%d%d%d%d%d,%d%d%d%d%d%d%d%d)b\n", name, x16m, b0,b1,b2,b3,b4,b5,b6,b7, b8,b9,ba,bb,bc,bd,be,bf);
    } else {
        printf("%d%d%d%d%d%d%d%d %d%d%d%d%d%d%d%d", b0,b1,b2,b3,b4,b5,b6,b7, b8,b9,ba,bb,bc,bd,be,bf);
    }
}

// -----------------------------------
void printfR32(uint32_t x, char* name)
// -----------------------------------
{
    // affichage en binaire "Register-like"
    int n = 32;
    if(name) printf("%s = (%x)h = (", name, x);
    
    for(int i=n; i>0; i--) {
        if( !(i&7) && (i != n) ) putchar(' ');
        bit_t b = (x >> (i-1)) & 1;
        printf("%d", b);
        
    }
    if(name) puts(")b");
}
// -----------------------------------
void printfM32(uint32_t x, char* name)
// -----------------------------------
{
    // affichage en binaire "Memory-like"
    int n = 32;
    if(name) printf("%s = (%x)h = (", name, x);
    
    for(int i=0; i<n; i++) {
        if( !(i&7) && i ) putchar(' ');
        bit_t b = (x >> i) & 1;
        printf("%d", b);
        
    }
    if(name) puts(")b");
}
// ----------------------------------------
void printfR(uint64_t x, int n, char* name)
// ----------------------------------------
{
    // affichage en binaire "Register-like"
    if(name) printf("%s = (%" PRIx64 ")h = (", name, x);
    
    for(int i=n; i>0; i--) {
        if( !(i&7) && (i != n) ) putchar(' ');
        bit_t b = (x >> (i-1)) & 1;
        printf("%d", b);
        
    }
    if(name) puts(")b");
}
// ----------------------------------------
void printfM(uint64_t x, int n, char* name)
// ----------------------------------------
{
    // affichage en binaire "Memory-like"
    if(name) printf("%s = (%" PRIx64 ")h = (", name, x);
    
    for(int i=0; i<n; i++) {
        if( !(i&7) && i ) putchar(' ');
        bit_t b = (x >> i) & 1;
        printf("%d", b);
        
    }
    if(name) puts(")b");
}
// -------------------
bit_t char2bit(char c)
// -------------------
{
    bit_t b;
    
    if(isdigit(c)) {
        uint8_t d = atoi((const char*) &c);
        if(d>1) d = 1; // saturation
        b = (bit_t) d;
    } else {
        b = 0;
    }
    return b;
}
// ------------------------
uint32_t str2Reg(char* str)
// ------------------------
{
    int len = strlen(str);
    int n = (len < 32) ? len : 32; // clamping au min
    
    uint32_t x = 0;
    
    for(int i=0; i<n; i++) {
    
        char c = str[i];
        
        bit_t b = char2bit(c);
        
        x = (x << 1) | b;
    }
    return x;
}
// ------------------------
uint32_t str2Mem(char* str)
// ------------------------
{
    int len = strlen(str);
    int n = (len < 32) ? len : 32; // clamping au min
    
    uint32_t x = 0;
    
    for(int i=0; i<n; i++) {
        
        char c = str[i];
        
        bit_t b = char2bit(c);
        
        x = x | (b << i);
    }
    return x;
}
// -----------------------------------------------------------
void set_ui8vector_str_v0(uint8 *v, int i0, int i1, char *str)
// -----------------------------------------------------------
{
    // unsafe
    char c[2];
    
    for(int i=i0; i<=i1; i++) {
        *c = *str++;
        if(isdigit(*c))
            v[i] = (uint8) atoi(c);
        else
            v[i] = (uint8) 0;
    } // i
}
// -----------------------------------------------------------
void set_ui8vector_str_v2(uint8 *v, int i0, int i1, char *str)
// -----------------------------------------------------------
{
    int len = strlen(str);
    int ncol = i1-i0+1;
    int m;
    char c[2];
    
    m = (len < ncol) ? len : ncol;
    
    for(int i=i0; i<=i0+m-1; i++) {
        *c = *str++;
        if(isdigit(*c))
            v[i] = (uint8) atoi(c);
        else
            v[i] = (uint8) 0;
    }
    
    for(int i=i0+m; i<=i1; i++) {
        v[i] = (uint8) 0;
    }
}
// --------------------------------------------------------
void set_ui8vector_str(uint8 *v, int i0, int i1, char *str)
// --------------------------------------------------------
{
    //set_ui8vector_str_v0(v, i0, i1, str);
    //set_ui8vector_str_v1(v, i0, i1, str);
    set_ui8vector_str_v2(v, i0, i1, str);
}
// ---------------
int ishexa(char x)
// ---------------
{
    if( ((x >= '0') && (x <= '9')) ||
       ((x >= 'a') && (x <= 'f')) ||
       ((x >= 'A') && (x <= 'F')) ) {
        return 1;
    } else {
        return 0;
    }
}
// -------------
int atob(char x)
// -------------
{
    // restriction of atoi for bit
    if( ((x >= '0') && (x <= '1')) ) return (x - '0');
    return 0; // default value
}
// -------------
int atox(char x)
// -------------
{
    // extension of atoi for hexa number
    if( ((x >= '0') && (x <= '9')) ) return (x - '0');
    if( ((x >= 'a') && (x <= 'f')) ) return (x - 'a' + 10);
    if( ((x >= 'A') && (x <= 'F')) ) return (x - 'A' + 10);
    return 0; // default value
}
// ---------------------------------------------------------
void set_ui8vector_str1(uint8 *v, int i0, int i1, char *str)
// ---------------------------------------------------------
{
    int len = strlen(str);
    int ncol = i1-i0+1;
    int m;
    char c;
    
    m = (len < ncol) ? len : ncol;
    
    for(int i=i0; i<=i0+m-1; i++) {
        c = *str++;
        v[i] = (uint8) atob(c);
    }
    
    for(int i=i0+m; i<=i1; i++) {
        v[i] = (uint8) 0;
    }
}
// ---------------------------------------------------------
void set_ui8vector_str16(uint8 *v, int i0, int i1, char *str)
// ---------------------------------------------------------
{
    int len = strlen(str);
    int ncol = i1-i0+1;
    int m;
    char c;
    
    m = (len < ncol) ? len : ncol;
    
    for(int i=i0; i<=i0+m-1; i++) {
        c = *str++;
        v[i] = (uint8) atox(c);
    }
    
    for(int i=i0+m; i<=i1; i++) {
        v[i] = (uint8) 0;
    }
}
// -------------------------------------------------------
void unpack_ui4vector(uint8* X4, int src_width, uint8* Y1)
// -------------------------------------------------------
{
    int card = 4;
    for(int i = 0; i < src_width; i++) {
    
        uint8 x = X4[i];
    
        for(int k = 0; k < card; k++) {
            Y1[card * i + k] = x & 1;
            x = x >> 1;
        }
    }
}
// -------------------------------------------------------
void unpack_ui8vector(uint8* X8, int src_width, uint8* Y1)
// -------------------------------------------------------
{
    int card = 8;
    for(int j = 0; j < src_width; j++) {
        
        uint8 x = X8[j];
        
        for(int k = 0; k < card; k++) {
            Y1[card * j + k] = x & 1;
            x = x >> 1;
        }
    }
}
// ----------------------------------------------------------
void unpack_ui16vector(uint16* X16, int src_width, uint8* Y1)
// ----------------------------------------------------------
{
    int card = 16;
    for(int j = 0; j < src_width; j++) {
        
        uint16 x = X16[j];
        
        for(int k = 0; k < card; k++) {
            Y1[card * j + k] = x & 1;
            x = x >> 1;
        }
    }
}
// ----------------------------------------------------------
void unpack_ui32vector(uint32* X32, int src_width, uint8* Y1)
// ----------------------------------------------------------
{
    int card = 32;
    for(int j = 0; j < src_width; j++) {
        
        uint32 x = X32[j];
        
        for(int k=0; k < card; k++) {
            Y1[card * j + k] = x & 1;
            x = x >> 1;
        }
    }
}
// ----------------------------------------------------------
void unpack_ui64vector(uint64* X64, int src_width, uint8* Y1)
// ----------------------------------------------------------
{
    const int card = 64;
    for(int j = 0; j < src_width; j++) {
        
        uint64 x = X64[j];
        
        for(int k = 0; k < card; k++) {
            Y1[card * j + k] = x & 1;
            x = x >> 1;
        }
    }
}
// -----------------------------------------------------
void pack_ui4vector(uint8* X1, int src_width, uint8* Y4)
// -----------------------------------------------------
{
    const int card = 4;
    int dst_width = src_width / card;
    
    for(int j = 0; j < src_width; j++) {
    
        uint8 x = 0;
        
        for(int k = 0; k < card; k++) {
            
            uint8 b = X1[card * j + k];
            x = x | (b << k);
        }
        Y4[j] = x;
    }
}
// -----------------------------------------------------
void pack_ui8vector(uint8* X1, int src_width, uint8* Y8)
// -----------------------------------------------------
{
    const int card = 8;
    int dst_width = src_width / card;
    
    for(int j = 0; j < dst_width; j++) {
        
        uint8 x = 0;
        
        for(int k = 0; k < card; k++) {
            
            uint8 b = X1[card * j + k];
            x = x | (b << k);
            //printf("b = %d x = %x\n", b, x);
        }
        Y8[j] = x;
        //printf("j = %d ", j); printfM8(x, "x");
        //putchar('\n');
    }
}
// --------------------------------------------------------
void pack_ui16vector(uint8* X1, int src_width, uint16* Y16)
// --------------------------------------------------------
{
    const int card = 16;
    int dst_width = src_width / card;
    
    for(int j = 0; j < dst_width; j++) {
        
        uint16 x = 0;
        
        for(int k = 0; k < card; k++) {
            
            uint16 b = X1[card * j + k];
            x = x | (b << k);
        }
        Y16[j] = x;
    }
}
// --------------------------------------------------------
void pack_ui32vector(uint8* X1, int src_width, uint32* Y32)
// --------------------------------------------------------
{
    const int card = 32;
    int dst_width = src_width / card;
    
    for(int j = 0; j < dst_width; j++) {
        
        uint32 x = 0;
        
        for(int k = 0; k < card; k++) {
            
            uint32 b = X1[card * j + k];
            x = x | (b << k);
        }
        Y32[j] = x;
    }
}
// --------------------------------------------------------
void pack_ui64vector(uint8* X1, int src_width, uint64* Y64)
// --------------------------------------------------------
{
    const int card = 64;
    int dst_width = src_width / card;
    
    for(int j = 0; j < dst_width; j++) {
        
        uint64 x = 0;
        
        for(int k = 0; k < card; k++) {
            
            uint64 b = X1[card * j + k];
            x = x | (b << k);
        }
        Y64[j] = x;
    }
}
// -----------------------------------------------------------------------
void pack_ui4matrix(uint8** X1, int src_height, int src_width, uint8** Y8)
// -----------------------------------------------------------------------
{
    for(int i = 0; i < src_height; i++) {
        pack_ui4vector(X1[i], src_width, Y8[i]);
    }
}
// -----------------------------------------------------------------------
void pack_ui8matrix(uint8** X1, int src_height, int src_width, uint8** Y8)
// -----------------------------------------------------------------------
{
    for(int i = 0; i < src_height; i++) {
        pack_ui8vector(X1[i], src_width, Y8[i]);
    }
}
// --------------------------------------------------------------------------
void pack_ui16matrix(uint8** X1, int src_height, int src_width, uint16** Y16)
// --------------------------------------------------------------------------
{
    for(int i = 0; i < src_height; i++) {
        pack_ui16vector(X1[i], src_width, Y16[i]);
    }
}
// --------------------------------------------------------------------------
void pack_ui32matrix(uint8** X1, int src_height, int src_width, uint32** Y32)
// --------------------------------------------------------------------------
{
    for(int i = 0; i < src_height; i++) {
        pack_ui32vector(X1[i], src_width, Y32[i]);
    }
}
// --------------------------------------------------------------------------
void pack_ui64matrix(uint8** X1, int src_height, int src_width, uint64** Y64)
// --------------------------------------------------------------------------
{
    for(int i = 0; i < src_height; i++) {
        pack_ui64vector(X1[i], src_width, Y64[i]);
        //return;
    }
}
// -------------------------------------------------------------------------
void unpack_ui4matrix(uint8** X8, int src_height, int src_width, uint8** Y1)
// -------------------------------------------------------------------------
{
    for(int i = 0; i < src_height; i++) {
        unpack_ui4vector(X8[i], src_width, Y1[i]);
    }
}
// -------------------------------------------------------------------------
void unpack_ui8matrix(uint8** X8, int src_height, int src_width, uint8** Y1)
// -------------------------------------------------------------------------
{
    for(int i = 0; i < src_height; i++) {
        unpack_ui8vector(X8[i], src_width, Y1[i]);
    }
}
// ----------------------------------------------------------------------------
void unpack_ui16matrix(uint16** X16, int src_height, int src_width, uint8** Y1)
// ----------------------------------------------------------------------------
{
    for(int i = 0; i < src_height; i++) {
        unpack_ui16vector(X16[i], src_width, Y1[i]);
    }
}
// ----------------------------------------------------------------------------
void unpack_ui32matrix(uint32** X32, int src_height, int src_width, uint8** Y1)
// ----------------------------------------------------------------------------
{
    for(int i = 0; i < src_height; i++) {
        unpack_ui32vector(X32[i], src_width, Y1[i]);
    }
}
// ----------------------------------------------------------------------------
void unpack_ui64matrix(uint64** X64, int src_height, int src_width, uint8** Y1)
// ----------------------------------------------------------------------------
{
    for(int i = 0; i < src_height; i++) {
        unpack_ui64vector(X64[i], src_width, Y1[i]);
        //return;
    }
}
// ----------------------------------------------------------
void displayR_ui4vector(uint8 *v, int i0, int i1, char *name)
// ----------------------------------------------------------
{
    if(name != NULL) printf("%s", name);
    
    for(int i=i0; i<=i1; i++) {
        printfR(v[i], 4, NULL); // 8-bit display
        putchar(' ');
    }
    if(name) putchar('\n');
}
// ----------------------------------------------------------
void displayR_ui8vector(uint8 *v, int i0, int i1, char *name)
// ----------------------------------------------------------
{
    if(name != NULL) printf("%s", name);
    
    for(int i=i0; i<=i1; i++) {
        printfR(v[i], 8, NULL); // 8-bit display
        putchar(' ');
    }
    if(name) putchar('\n');
}
// ----------------------------------------------------------
void displayM_ui4vector(uint8 *v, int i0, int i1, char *name)
// ----------------------------------------------------------
{
    if(name != NULL) printf("%s", name);
    
    for(int i=i0; i<=i1; i++) {
        printfM(v[i], 4, NULL); // 4-bit display
        putchar(' ');
    }
    if(name) putchar('\n');
}
// ----------------------------------------------------------
void displayM_ui8vector(uint8 *v, int i0, int i1, char *name)
// ----------------------------------------------------------
{
    if(name != NULL) printf("%s", name);
    
    for(int i=i0; i<=i1; i++) {
        printfM(v[i], 8, NULL); // 8-bit display
        putchar(' ');
    }
    if(name) putchar('\n');
}
// ------------------------------------------------------------
void displayM_ui16vector(uint16 *v, int i0, int i1, char *name)
// ------------------------------------------------------------
{
    if(name != NULL) printf("%s", name);
    
    for(int i=i0; i<=i1; i++) {
        printfM(v[i], 16, NULL); // 16-bit display
        putchar(' ');
    }
    if(name) putchar('\n');
}
// ------------------------------------------------------------
void displayM_ui32vector(uint32 *v, int i0, int i1, char *name)
// ------------------------------------------------------------
{
    if(name != NULL) printf("%s", name);
    
    for(int i=i0; i<=i1; i++) {
        printfM(v[i], 32, NULL); // 32-bit display
        putchar(' ');
    }
    if(name) putchar('\n');
}
// ------------------------------------------------------------
void displayM_ui64vector(uint64 *v, int i0, int i1, char *name)
// ------------------------------------------------------------
{
    if(name != NULL) printf("%s", name);
    
    for(int i=i0; i<=i1; i++) {
        printfM(v[i], 64, NULL); // 32-bit display
        putchar(' ');
    }
    if(name) putchar('\n');
}
// ---------------------------------------------------------------------------
void displayR_ui4matrix(uint8 **m, int i0, int i1, int j0, int j1, char *name)
// ---------------------------------------------------------------------------
{
    if(name != NULL) printf("%s\n", name);
    
    for(int i=i0; i<=i1; i++) {
        displayR_ui4vector(m[i], j0, j1, NULL);
        putchar('\n');
    }
    if(name) putchar('\n');
}
// ---------------------------------------------------------------------------
void displayR_ui8matrix(uint8 **m, int i0, int i1, int j0, int j1, char *name)
// ---------------------------------------------------------------------------
{
    if(name != NULL) printf("%s\n", name);
    
    for(int i=i0; i<=i1; i++) {
        displayR_ui8vector(m[i], j0, j1, NULL);
        putchar('\n');
    }
    if(name) putchar('\n');
}
// ---------------------------------------------------------------------------
void displayM_ui4matrix(uint8 **m, int i0, int i1, int j0, int j1, char *name)
// ---------------------------------------------------------------------------
{
    if(name != NULL) printf("%s\n", name);
    
    for(int i=i0; i<=i1; i++) {
        displayM_ui4vector(m[i], j0, j1, NULL);
        putchar('\n');
    }
    if(name) putchar('\n');
}
// ---------------------------------------------------------------------------
void displayM_ui8matrix(uint8 **m, int i0, int i1, int j0, int j1, char *name)
// ---------------------------------------------------------------------------
{
    if(name != NULL) printf("%s\n", name);
    
    for(int i=i0; i<=i1; i++) {
        displayM_ui8vector(m[i], j0, j1, NULL);
        putchar('\n');
    }
    if(name) putchar('\n');
}
// -----------------------------------------------------------------------------
void displayM_ui16matrix(uint16 **m, int i0, int i1, int j0, int j1, char *name)
// -----------------------------------------------------------------------------
{
    if(name != NULL) printf("%s\n", name);
    
    for(int i=i0; i<=i1; i++) {
        displayM_ui16vector(m[i], j0, j1, NULL);
        putchar('\n');
    }
    if(name) putchar('\n');
}
// -----------------------------------------------------------------------------
void displayM_ui32matrix(uint32 **m, int i0, int i1, int j0, int j1, char *name)
// -----------------------------------------------------------------------------
{
    if(name != NULL) printf("%s\n", name);
    
    for(int i=i0; i<=i1; i++) {
        displayM_ui32vector(m[i], j0, j1, NULL);
        putchar('\n');
    }
    if(name) putchar('\n');
}
// -----------------------------------------------------------------------------
void displayM_ui64matrix(uint64 **m, int i0, int i1, int j0, int j1, char *name)
// -----------------------------------------------------------------------------
{
    if(name != NULL) printf("%s\n", name);
    
    for(int i=i0; i<=i1; i++) {
        displayM_ui64vector(m[i], j0, j1, NULL);
        putchar('\n');
    }
    if(name) putchar('\n');
}

// --------------------------------------------------------
void compact_ui8matrix2(uint8 **X, int h, int w, uint8 **Y)
// --------------------------------------------------------
{
    for(int i=0; i<=h-1; i++) {
        for(int j=0; j<=w-1; j++) {
        
            uint8 x = X[i][j];
            uint8 y = y << 1 | x;
            
            if((j+1)%8 == 0) {
                Y[i][(j+1)/8] = y;
                y = 0;
            }
        }
    }
}
// -------------------------------------------------------------------------
void expand_ui8matrix2(uint8 **X, int i0, int i1, int j0, int j1, uint8 **Y)
// -------------------------------------------------------------------------
{
    puts("A FAIRE");
}
