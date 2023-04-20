/* ------------------ */
/* --- swp_test.c --- */
/* ------------------ */

/*
 * Copyright (c) 2021 - 2021, Lionel Lacassagne, All rights reserved
 * LIP6 - Sorbonne University
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <math.h>

#include "nrtype.h"
#include "nrdef.h"
#include "nrutil.h"
#include "nralloc_mod.h"

#include "swp.h"
#include "swp_test.h"

// --------------------
void test_printf8(void)
// --------------------
{
    puts("------------------");
    puts("-- test_printf8 --");
    puts("------------------");
    
    bit_t b0,b1,b2,b3,b4,b5,b6,b7;
    
    puts("construction 'Reg'");
    //              7 6 5 4 3 2 1 0
    uint8 x1 = B8R(1,1,0,0,1,0,1,0);
    uint8 x2 = B8R(0,1,0,1,0,0,1,1);
    
    R8B(x1, b7,b6,b5,b4,b3,b2,b1,b0); printf("x1 = (%x)h = (%d%d%d%d%d%d%d%d)b\n", x1, b7,b6,b5,b4,b3,b2,b1,b0);
    R8B(x2, b7,b6,b5,b4,b3,b2,b1,b0); printf("x2 = (%x)h = (%d%d%d%d%d%d%d%d)b\n", x2, b7,b6,b5,b4,b3,b2,b1,b0);
    printfR8(x1, "x1");
    printfR8(x2, "x2");
    printf("msb(x1) = %d lsb(x1) = %d\n", (x1>>7) & 1, x1 & 1);
    printf("msb(x2) = %d lsb(x2) = %d\n", (x2>>7) & 1, x2 & 1);
    putchar('\n');
    
    puts("construction 'Mem'");
    //        0 1 2 3 4 5 6 7
    x1 = B8M(1,1,0,0,1,0,1,0);
    x2 = B8M(0,1,0,1,0,0,1,1);
    
    M8B(x1, b0,b1,b2,b3,b4,b5,b6,b7); printf("(%x)h = (%d%d%d%d%d%d%d%d)b\n", x1, b0,b1,b2,b3,b4,b5,b6,b7);
    M8B(x2, b0,b1,b2,b3,b4,b5,b6,b7); printf("(%x)h = (%d%d%d%d%d%d%d%d)b\n", x2, b0,b1,b2,b3,b4,b5,b6,b7);
    printfM8(x1, "x1");
    printfM8(x2, "x2");
    printf("msb(x1) = %d lsb(x1) = %d\n", (x1>>7) & 1, x1 & 1);
    printf("msb(x2) = %d lsb(x2) = %d\n", (x2>>7) & 1, x2 & 1);
}
// ---------------------
void test_printf16(void)
// ---------------------
{
    puts("-------------------");
    puts("-- test_printf16 --");
    puts("-------------------");
    
    bit_t b0,b1,b2,b3,b4,b5,b6,b7;
    bit_t b8,b9,ba,bb,bc,bd,be,bf;
    
    puts("construction 'Reg'");
    //              f e d c b a ç 8  7 6 5 4 3 2 1 0
    uint16_t x1 = B16R(1,1,0,0,1,0,1,0, 1,1,1,0,1,0,0,0);
    uint16_t x2 = B16R(0,1,0,1,0,0,1,1, 0,1,0,0,0,1,1,1);
    
    R16B(x1, bf,be,bd,bc,bb,ba,b9,b8, b7,b6,b5,b4,b3,b2,b1,b0); printf("x1 = (%x)h = (%d%d%d%d%d%d%d%d %d%d%d%d%d%d%d%d)b\n", x1, bf,be,bd,bc,bb,ba,b9,b8, b7,b6,b5,b4,b3,b2,b1,b0);
    R16B(x2, bf,be,bd,bc,bb,ba,b9,b8, b7,b6,b5,b4,b3,b2,b1,b0); printf("x2 = (%x)h = (%d%d%d%d%d%d%d%d %d%d%d%d%d%d%d%d)b\n", x2, bf,be,bd,bc,bb,ba,b9,b8, b7,b6,b5,b4,b3,b2,b1,b0); putchar('\n');
    
    printfR16(x1, "x1");
    printfR16(x2, "x2"); putchar('\n');
    
    printfR(x1, 16, "X1");
    printfR(x2, 16, "X2"); putchar('\n');
    
    printf("msb(x1) = %d lsb(x1) = %d\n", (x1>>15) & 1, x1 & 1);
    printf("msb(x2) = %d lsb(x2) = %d\n", (x2>>15) & 1, x2 & 1); putchar('\n');
    
    puts("construction 'Mem'");
    //        0 1 2 3 4 5 6 7  8 9 a b c d e g
    x1 = B16M(1,1,0,0,1,0,1,0, 1,1,1,0,1,0,0,0);
    x2 = B16M(0,1,0,1,0,0,1,1, 0,1,0,0,0,1,1,1);
    
    M16B(x1, b0,b1,b2,b3,b4,b5,b6,b7, b8,b9,ba,bb,bc,bd,be,bf); printf("(%x)h = (%d%d%d%d%d%d%d%d %d%d%d%d%d%d%d%d)b\n", x1, b0,b1,b2,b3,b4,b5,b6,b7, b8,b9,ba,bb,bc,bd,be,bf);
    M16B(x2, b0,b1,b2,b3,b4,b5,b6,b7, b8,b9,ba,bb,bc,bd,be,bf); printf("(%x)h = (%d%d%d%d%d%d%d%d %d%d%d%d%d%d%d%d)b\n", x2, b0,b1,b2,b3,b4,b5,b6,b7, b8,b9,ba,bb,bc,bd,be,bf); putchar('\n');
    
    printfM16(x1, "x1");
    printfM16(x2, "x2"); putchar('\n');
    
    printfM(x1, 16, "X1");
    printfM(x2, 16, "X2"); putchar('\n');
    
    printf("msb(x1) = %d lsb(x1) = %d\n", (x1>>15) & 1, x1 & 1);
    printf("msb(x2) = %d lsb(x2) = %d\n", (x2>>15) & 1, x2 & 1);
}
// -------------------
void test_printf(void)
// -------------------
{
    test_printf8();
    test_printf16();
}
// ----------------
void test_str(void)
// ----------------
{
    puts("--------------");
    puts("-- test_str --");
    puts("--------------");
    
    puts("construction 'Reg'");
    uint8_t  x8r  = (uint8_t) str2Reg("11001010"); printfR(x8r, 8, "x8r ");
    uint16_t x16r = (uint16_t) str2Reg("1100101011101000"); printfR(x16r, 16, "x16r");
    putchar('\n');
    
    puts("construction 'Mem'");
    uint8_t  x8m  = (uint8_t) str2Mem("11001010"); printfM(x8m, 8, "x8m ");
    uint16_t x16m = (uint16_t) str2Mem("1100101011101000"); printfM(x16m, 16, "x16m");
}
// ---------------------
void test_set_str1(void)
// ---------------------
{
    uint8_t **X;
    int i;

    int h =  4;
    int w =  9;
    
    X = ui8matrix(    0, h-1, 0, w-1);
    zero_ui8matrix(X, 0, h-1, 0, w-1);
    
    i =  0;
    //                                 0000000000
    //                                 0123456789
    set_ui8vector_str(X[i++], 0, w-1, "1.......1"); // 0
    set_ui8vector_str(X[i++], 0, w-1, "1...1.111"); // 1
    set_ui8vector_str(X[i++], 0, w-1, "1.11111.."); // 2
    set_ui8vector_str(X[i++], 0, w-1, "111......"); // 3
    
    display_ui8matrix_positive(X, 0, h-1, 0, w-1, 3, "forme1"); puts("");
    //write_ui8matrix_positive  (X, 0, h-1, 0, w-1, 4, "forme1.txt");
}

// -----------------------------
void test_ishexa_atox_atob(void)
// -----------------------------
{
    for(int i = 32; i <127; i++) {
        printf("%3d %3c %2d %x %d\n", i, (char)i, ishexa(i), atox((char)i), atob((char)i));
    }
}
// -----------------------
void test_str1_str16(void)
// -----------------------
{
    uint8 T[18];
    char* str = "0123456789abcdefghij";
    
    set_ui8vector_str1(T, 0, 18-1, str);
    display_ui8vector(T, 0, 18-1, "%1x", "T01: ");
    
    set_ui8vector_str16(T, 0, 18-1, str);
    display_ui8vector(T, 0, 18-1, "%1x", "T16: ");
}
// --------------------------
void test_pack4_unpack4(void)
// --------------------------
{
    puts("------------------------");
    puts("-- test_pack4_unpack4 --");
    puts("------------------------");
    
    int width = 16;
    
    uint8 X4[16+1];
    uint8 Y1[16*4+1];
    uint8 Z4[16+1];
    
    char* str = "0123456789abcdefghij";
    
    set_ui8vector_str16(X4, 0, 16-1, str);
    display_ui8vector(X4, 0, 16-1, "%1x", "X4: ");
    displayM_ui4vector(X4,0, width-1, "X4: ");
    displayR_ui4vector(X4,0, width-1, "X4: ");
    return;
    
    unpack_ui4vector(X4, width, Y1);
    
    display_ui8vector(Y1, 0, 4*width-1, "%1x ", "Y1: ");
    
    pack_ui4vector(Y1, 4*width, Z4);
    
    display_ui8vector(Z4, 0, width-1, "%1x ", "Z4: ");
}
// --------------------------
void test_pack8_unpack8(void)
// --------------------------
{
    puts("------------------------");
    puts("-- test_pack8_unpack8 --");
    puts("------------------------");
    
    int width = 256;
    
    uint8 X8[256];
    uint8 Y1[256*8];
    uint8 Z8[256];
    
    init_ui8vector_param(X8, 0, width-1, 0, 1);
    display_ui8vector(X8, 0, width-1, "%2x ", "X8: ");
    displayM_ui8vector(X8,0, width-1, "X8: ");
    displayR_ui8vector(X8,0, width-1, "X8: ");
    
    return;
    unpack_ui8vector(X8, width, Y1);
    
    display_ui8vector(Y1, 0, 8*width-1, "%1x ", "Y1: ");
    
    pack_ui8vector(Y1, 8*width, Z8);
    
    display_ui8vector(Z8, 0, width-1, "%1x ", "Z8: ");
}

// ------------------------
void test_bin_indices(void)
// ------------------------
{
    int i0, i1; // byte indices
    int b0, b1; // bin indices
    int m0, m1; // memory byte indices
    int c;      // cardinal
    
    c = 8;
    i0 = -1; i1 = (2*c - 1);
    B2b(i0, i1, c, &b0, &b1);
    b2B(b0, b1, c, &m0, &m1);
    printf("%3d %3d -> %3d %3d -> %3d %3d\n", i0, i1, b0, b1, m0, m1);
    
    i0 = -c; i1 = (2*c - 1) - 1;
    B2b(i0, i1, c, &b0, &b1);
    b2B(b0, b1, c, &m0, &m1);
    printf("%3d %3d -> %3d %3d -> %3d %3d\n", i0, i1, b0, b1, m0, m1);
    
    i0 = -c-1; i1 = (2*c - 1) + 1;
    B2b(i0, i1, c, &b0, &b1);
    b2B(b0, b1, c, &m0, &m1);
    printf("%3d %3d -> %3d %3d -> %3d %3d\n", i0, i1, b0, b1, m0, m1);
    putchar('\n');
    
    c = 16;
    i0 = -1; i1 = (1*c - 1);
    B2b(i0, i1, c, &b0, &b1);
    b2B(b0, b1, c, &m0, &m1);
    printf("%3d %3d -> %3d %3d -> %3d %3d\n", i0, i1, b0, b1, m0, m1);
    
    i0 = -c; i1 = (1*c - 1) - 1;
    B2b(i0, i1, c, &b0, &b1);
    b2B(b0, b1, c, &m0, &m1);
    printf("%3d %3d -> %3d %3d -> %3d %3d\n", i0, i1, b0, b1, m0, m1);
    
    i0 = -c-1; i1 = (1*c - 1) + 1;
    B2b(i0, i1, c, &b0, &b1);
    b2B(b0, b1, c, &m0, &m1);
    printf("%3d %3d -> %3d %3d -> %3d %3d\n", i0, i1, b0, b1, m0, m1);
    
    //  -1  15 ->  -1   1 ->  -8  15
    //  -8  14 ->  -1   1 ->  -8  15
    //  -9  16 ->  -2   2 -> -16  23
    
    //  -1  15 ->  -1   0 -> -16  15
    // -16  14 ->  -1   0 -> -16  15
    // -17  16 ->  -2   1 -> -32  31
}
// -----------------------------
void test_bin_indices_loop(void)
// -----------------------------
{
    int i, i0, i1;
    int a = 0;
    int c = 0;
    
    i0 = -2; i1 = 30;
    for(i=i0; i<=i1; i++){
        if(i%8 == 0) {
            printf("i = %2d a = %3d c = %d -> [%d]\n", i, a, c, i/8);
            a = 0;
            c = 0;
        } else {
            a += i;
            c += 1;
        }
    }
    printf("i = %2d a = %3d c = %d -> [%d]\n", i, a, c, i/8);
}
// -------------------------
void test_bin_vector32(void)
// -------------------------
{
    // extraction et endianness
    puts("-----------------------");
    puts("-- test_bin_vector32 --");
    puts("-----------------------");
    
    uint32 T32[2];
    uint16 *p16 = (uint16*) &T32[0];
    uint8  *p8  = (uint8*)  &T32[0];
    
    T32[0] = 0x12345678;
    T32[1] = 0x9abcdef0;
    
    printf("T32: %8x  %8x\n", T32[0], T32[1]);
    printf("p16: %4x  %4x  %4x  %4x\n", p16[0], p16[1], p16[2], p16[3]);
    printf("p8 : %2x  %2x  %2x  %2x  %2x  %2x  %2x  %2x\n", p8[0], p8[1], p8[2], p8[3], p8[4], p8[5], p8[6], p8[7]);
}
// -------------------------
void test_bin_vector16(void)
// -------------------------
{
    // extraction et endianness
    puts("-----------------------");
    puts("-- test_bin_vector16 --");
    puts("-----------------------");
    
    uint16 T16[4];
    uint32 *p32 = (uint32*) &T16[0];
    uint8  *p8  = (uint8*)  &T16[0];
    
    T16[0] = 0x1234;
    T16[1] = 0x5678;
    T16[2] = 0x9abc;
    T16[3] = 0xdef0;
    
    printf("p32: %8x  %8x\n", p32[0], p32[1]);
    printf("T16: %4x  %4x  %4x  %4x\n", T16[0], T16[1], T16[2], T16[3]);
    printf("p8 : %2x  %2x  %2x  %2x  %2x  %2x  %2x  %2x\n", p8[0], p8[1], p8[2], p8[3], p8[4], p8[5], p8[6], p8[7]);
}
// ------------------------
void test_bin_vector8(void)
// ------------------------
{
    // extraction et endianness
    puts("----------------------");
    puts("-- test_bin_vector8 --");
    puts("----------------------");
    
    uint8 T8[8];
    uint32 *p32 = (uint32*) &T8[0];
    uint16 *p16 = (uint16*) &T8[0];
    
    T8[0] = 0x12;
    T8[1] = 0x34;
    T8[2] = 0x56;
    T8[3] = 0x78;
    
    T8[4] = 0x9a;
    T8[5] = 0xbc;
    T8[6] = 0xde;
    T8[7] = 0xf0;
    
    printf("T8 : %2x  %2x  %2x  %2x  %2x  %2x  %2x  %2x\n", T8[0], T8[1], T8[2], T8[3], T8[4], T8[5], T8[6], T8[7]);
    printf("p16: %4x  %4x  %4x  %4x\n", p16[0], p16[1], p16[2], p16[3]);
    printf("p32: %8x  %8x\n", p32[0], p32[1]);
}
// ------------------------
void test_bin_quartet(void)
// ------------------------
{
    // extraction et endianness
    puts("----------------------");
    puts("-- test_bin_quartet --");
    puts("----------------------");
    
    uint8 q0, q1, q2, q3, q4, q5, q6, q7, q8, q9, q10, q11, q12, q13, q14, q15;
    uint8 o0, o1, o2, o3, o4, o5, o6, o7;
    uint16 s0, s1, s2, s3;
    uint32 i0, i1;
    
    uint8 T8[8];
    uint16 T16[4];
    uint32 T32[2];
    
    q0 = 1;
    q1 = 2;
    q2 = 3;
    q3 = 4;
    q4 = 5;
    q5 = 6;
    q6 = 7;
    q7 = 8;
    q8 = 9;
    q9 = 10;
    q10 = 11;
    q11 = 12;
    q12 = 13;
    q13 = 14;
    q14 = 15;
    q15 = 15;
    
    o0 = (q1  << 4) | q0;
    o1 = (q3  << 4) | q2;
    o2 = (q5  << 4) | q4;
    o3 = (q7  << 4) | q6;
    o4 = (q9  << 4) | q8;
    o5 = (q11 << 4) | q10;
    o6 = (q13 << 4) | q12;
    o7 = (q15 << 4) | q14;
    
    s0 = (o1  << 8) | o0;
    s1 = (o3  << 8) | o2;
    s2 = (o5  << 8) | o4;
    s3 = (o7  << 8) | o6;
    
    i0 = (s1 << 16) | s0;
    i1 = (s3 << 16) | s2;
    
    puts("-- variables -- ");
    printf("%1x %1x %1x %1x %1x %1x %1x %1x %1x %1x %1x %1x %1x %1x %1x %1x\n", q0, q1, q2, q3, q4, q5, q6, q7, q8, q9, q10, q11, q12, q13, q14, q15);
    printf("%2x  %2x  %2x  %2x  %2x  %2x  %2x  %2x\n", o0, o1, o2, o3, o4, o5, o6, o7);
    printf("%4x    %4x    %4x    %4x\n", s0, s1, s2, s3);
    printf("%8x        %8x\n", i0, i1);
    
    T8[0] = o0, T8[1] = o1; T8[2] = o2; T8[3] = o3; T8[4] = o4; T8[5] = o5; T8[6] = o6; T8[7] = o7;
    T16[0] = s0; T16[1] = s1; T16[2] = s2; T16[3] = s3;
    T32[0] = i0; T32[1] = i1;
    
    puts("-- array -- ");
    for(int i=0; i<8; i++) printf("%2x  ", T8[i]); putchar('\n');
    for(int i=0; i<4; i++) printf("%4x    ", T16[i]); putchar('\n');
    for(int i=0; i<2; i++) printf("%8x        ", T32[i]); putchar('\n');
}
// -------------------------------
void test_left_right_quartet(void)
// -------------------------------
{
    puts("-----------------------------");
    puts("-- test_left_right_quartet --");
    puts("-----------------------------");
    
    uint8 q0, q1, q2, q3, q4, q5, q6, q7, q8, q9, q10, q11, q12, q13, q14, q15;
    uint8 o0, o1, o2, o3, o4, o5, o6, o7;
    uint16 s0, s1, s2, s3;
    uint32 i0, i1;
    
    uint8 T8[8];
    uint16 T16[4];
    uint32 T32[2];
    
    q0 = 1;
    q1 = 2;
    q2 = 3;
    q3 = 4;
    q4 = 5;
    q5 = 6;
    q6 = 7;
    q7 = 8;
    q8 = 9;
    q9 = 10;
    q10 = 11;
    q11 = 12;
    q12 = 13;
    q13 = 14;
    q14 = 15;
    q15 = 0;
    
    o0 = (q1  << 4) | q0;
    o1 = (q3  << 4) | q2;
    o2 = (q5  << 4) | q4;
    o3 = (q7  << 4) | q6;
    o4 = (q9  << 4) | q8;
    o5 = (q11 << 4) | q10;
    o6 = (q13 << 4) | q12;
    o7 = (q15 << 4) | q14;
    
    s0 = (o1  << 8) | o0;
    s1 = (o3  << 8) | o2;
    s2 = (o5  << 8) | o4;
    s3 = (o7  << 8) | o6;
    
    i0 = (s1 << 16) | s0;
    i1 = (s3 << 16) | s2;
    
    puts("-- variables -- ");
    printf(" 4 bits: %1x %1x %1x %1x %1x %1x %1x %1x %1x %1x %1x %1x %1x %1x %1x %1x\n", q0, q1, q2, q3, q4, q5, q6, q7, q8, q9, q10, q11, q12, q13, q14, q15);
    printf(" 8 bits: %2x  %2x  %2x  %2x  %2x  %02x  %2x  %2x\n", o0, o1, o2, o3, o4, o5, o6, o7);
    printf("16 bits: %4x    %4x    %4x    %4x\n", s0, s1, s2, s3);
    printf("32 bits: %8x        %8x\n", i0, i1);
    putchar('\n');
    
    T8[0] = o0, T8[1] = o1; T8[2] = o2; T8[3] = o3; T8[4] = o4; T8[5] = o5; T8[6] = o6; T8[7] = o7;
    T16[0] = s0; T16[1] = s1; T16[2] = s2; T16[3] = s3;
    T32[0] = i0; T32[1] = i1;
    
    
    puts("----------------");
    puts("-- tableau 16 --");
    puts("----------------");
    
    uint16 a16, b16, c16;
    a16 = T16[0];
    b16 = T16[1];
    c16 = T16[2];
    
    printf("s16: c = %4x b = %4x a = %4x\n", c16, b16, a16);
    
    uint16 a_3   = a16 >> (3*4); // 16 - 1*4
    uint16 b_012 = b16 << (1*4);
    uint16 left16 = a_3 | b_012;
    
    puts("affichage en reverse lire de droite -> gauche");
    printf("left  s16: %04x + %04x = %04x\n", b_012, a_3, left16);
    
    uint16 c_0   = c16 << (3*4); // 16 - 1*4
    uint16 b_123 = b16 >> (1*4);
    uint16 right16 = b_123 | c_0;
    
    printf("right s16: %04x + %04x = %04x\n", c_0, b_123, right16);
    putchar('\n');
    
    printf("left16 (a,b) = %4x\n", ui16left (a16, b16, 4));
    printf("right16(a,b) = %4x\n", ui16right(b16, c16, 4));
    
    puts("---------------");
    puts("-- tableau 8 --");
    puts("---------------");
    
    uint16 a8, b8, c8;
    a8 = T8[0];
    b8 = T8[1];
    c8 = T8[2];
    
    printf("s8: c = %2x b = %2x a = %2x\n", c8, b8, a8);
    
    printf("left8 (a,b) = %4x\n", ui8left (a8, b8, 4));
    printf("right8(a,b) = %4x\n", ui8right(b8, c8, 4));
    
    puts("----------------");
    puts("-- tableau 32 --");
    puts("----------------");
    
    uint32 a32, b32, c32;
    a32 = T32[0];
    b32 = T32[1];
    //c32 = T32[2];
   
    printf("s32: c = %8x b = %8x a = %8x\n", c32, b32, a32);
   
    printf("left32 (a,b) = %4x\n", ui32left (a32, b32, 4));
    //printf("right32(a,b) = %4x\n", ui32right(b32, c32, 4));
    
    // -> les fonctions left et right sont OK
}
// ----------------
void test_add(void)
// ----------------
{
    uint32 a, b, c;
    uint32 m = 0x7f7f7f7f;
    uint32 am, bm, cm;
    
    // sans retenu
    a = 0x10203040;
    b = 0x01010101;
    c = a + b;
    printf("%08x + %08x = %08x\n", a, b, c);
    
    // avec retenu et clamp
    m = 0x7fff7fff;
    printf("m = %08x\n", m);
    
    a  = 0x50607080;
    b  = 0x20202020;
    
    m = 0x7fff7fff;
    a = 0x12345678;
    b = 0x456789a0;
    am = a & m;
    bm = b & m;
    c = am + bm;
    cm = c & m;
    //printf("%08x + %08x\n", a, b);
    //printf("%08x   %08x\n", m, m);
    //printf("%08x + %08x\n", am, bm);
    //printf("%08x + %08x -> %08x + %08x = %08x -> %08x\n", a, b, am, bm, c, cm);
    printf("a = %08x -> %08x\n", a, am);
    printf("b = %08x -> %08x\n", b, bm);
    printf("c = %08x -> %08x\n", c, cm);
}
// ----------------
void test_mul(void)
// ----------------
{
    uint32 a, b, c;

    a = 0x00120034;
    b = 0x00560078;
    c = a * b;
    printf("%08x + %08x = %08x\n", a, b, c);
}
// -----------------------
void test_left_right(void)
// -----------------------
{
    uint8 a, b, c;
    uint8 ml1, ml2, mr1, mr2;
    uint8 fl1, fl2, fr1, fr2;
    a = 0x40; b = 0xF0; c = 0x2;
    
    ml1 = ui8left1(a, b); mr1 = ui8right1(b, c);
    ml2 = ui8left2(a, b); mr2 = ui8right2(b, c);
    
    fl1 = ui8left(a, b, 1); fr1 = ui8right(b, c, 1);
    fl2 = ui8left(a, b, 2); fr2 = ui8right(b, c, 2);
    
    //printf("%2x  %2x  %2x\n", a, b, c);
    
    printfM8(a, "a  ");
    printfM8(b, "b  ");
    printfM8(c, "c  ");putchar('\n');
    
    printfM8(ml1, "ml1"); printfM8(fl1, "fl1");
    printfM8(ml2, "ml2"); printfM8(fl2, "fl2");putchar('\n');
    
    printfM8(mr1, "mr1"); printfM8(fr1, "fr1");
    printfM8(mr2, "mr2"); printfM8(fr2, "fr2");
             
    //printf("left  1 = %2x = %2x\n", ml1, fl1);
    //printf("left  2 = %2x = %2x\n", ml2, fl2);
    //printf("right 1 = %2x = %2x\n", mr1, fr1);
    //printf("right 2 = %2x = %2x\n", mr2, fr2);
}
// =================================
int test_swp(int argc, char* argv[])
// =================================
{
    puts("==============");
    puts("== test_SWP ==");
    puts("==============");
    
    //test_printf();
    //test_str();
    //test_set_str1(); // (existe aussi dans morpho_test)
    //test_ishexa_atox_atob(); // OK

    // test_str1_str16(); // OK
    // test_pack4_unpack4();
    // test_pack8_unpack8();
    
    // test_bin_indices();
    // test_bin_indices_loop();
    // test_bin_vector32();
    // test_bin_vector8();
    
    // test_bin_quartet();
    // test_left_right_quartet(); // OK left right
    // test_left_right();
    
    test_add();
    test_mul();
    
  return 0;
}
