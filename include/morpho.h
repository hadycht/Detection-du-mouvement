/* ---------------- */
/* --- morpho.h --- */
/* ---------------- */

/*
* Copyright (c) 2020 - 2021, Lionel Lacassagne, All rights reserved
* Sorbonne University, LIP6, CNRS
*/

#ifndef __MORPHO_H__
#define __MORPHO_H__
#define load1(X, i) X[i]
#define store1(Y, i, y) Y[i]=y

#define load2(X, i, j) X[i][j]
#define store2(Y, i, j, y) Y[i][j]=y
#ifdef __cplusplus
#ifdef PRAGMA_VERBOSE
#pragma message ("C++")
#endif
extern "C" {
#endif

//#define MORPHO_VERBOSE_DEFINE
// activation des pragma message

#define MORPHO_MACRO
//#define MORPHO_FUNCTION

// MORPHO_MACRO    : les operateurs sont des macros
// MORPHO_FUNCTION: les operateurs sont des fonctions

//#define MORPHO_OPERATOR_IS_ADD
#define MORPHO_OPERATOR_IS_AND_OR
// - remplacement de *tous* les operateurs (min,max,or,and) par des add
// - remplacement de *tous* les operateurs (min,max,or,and) par des and/or (pour morpho 1 bit)

// tout changement necessite de tout recompiler: make clean; make

// -- ne pas modifier les #define en dessous de cette ligne -- //

// ================
#ifdef MORPHO_MACRO
// ================

#ifdef MORPHO_VERBOSE_DEFINE
#pragma message("MORPHO_MACRO")
#endif

// --------------------------
#ifdef MORPHO_OPERATOR_IS_ADD
// --------------------------
// toutes les macros sont remplacees par des add

#ifdef MORPHO_VERBOSE_DEFINE
#pragma message("MORPHO_OPERATOR_IS_ADD")
#endif

#define add2(x0, x1) (x0) + (x1)
#define add3(x0, x1, x2) (x0) + (x1) + (x2)
#define add9(x0, x1, x2, x3, x4, x5, x6, x7, x8) (x0) + (x1) + (x2) + (x3) + (x4) + (x5) + (x6) + (x7) + (x8)

// max
#define ui8max2(x0, x1) add2(x0, x1)
#define ui8max3(x0, x1, x2) add3(x0, x1, x2)
#define ui8max9(x0, x1, x2, x3, x4, x5, x6, x7, x8) add9(x0, x1, x2, x3, x4, x5, x6, x7, x8)
#define ui32max2(x0, x1) add2(x0, x1)
#define ui32max3(x0, x1, x2) add3(x0, x1, x2)
#define ui32max9(x0, x1, x2, x3, x4, x5, x6, x7, x8) add9(x0, x1, x2, x3, x4, x5, x6, x7, x8)
#define ui64max2(x0, x1) add2(x0, x1)
#define ui64max3(x0, x1, x2) add3(x0, x1, x2)
#define ui64max9(x0, x1, x2, x3, x4, x5, x6, x7, x8) add9(x0, x1, x2, x3, x4, x5, x6, x7, x8)

// min
#define ui8min2(x0, x1) add2(x0, x1)
#define ui8min3(x0, x1, x2) add3(x0, x1, x2)
#define ui8min9(x0, x1, x2, x3, x4, x5, x6, x7, x8) add9(x0, x1, x2, x3, x4, x5, x6, x7, x8)
#define ui32min2(x0, x1) add2(x0, x1)
#define ui32min3(x0, x1, x2) add3(x0, x1, x2)
#define ui32min9(x0, x1, x2, x3, x4, x5, x6, x7, x8) add9(x0, x1, x2, x3, x4, x5, x6, x7, x8)
#define ui64min2(x0, x1) add2(x0, x1)
#define ui64min3(x0, x1, x2) add3(x0, x1, x2)
#define ui64min9(x0, x1, x2, x3, x4, x5, x6, x7, x8) add9(x0, x1, x2, x3, x4, x5, x6, x7, x8)

// and
#define ui8and2(x0, x1) add2(x0, x1)
#define ui8and3(x0, x1, x2) add3(x0, x1, x2)
#define ui8and9(x0, x1, x2, x3, x4, x5, x6, x7, x8) add9(x0, x1, x2, x3, x4, x5, x6, x7, x8)
#define ui32and2(x0, x1) add2(x0, x1)
#define ui32and3(x0, x1, x2) add3(x0, x1, x2)
#define ui32and9(x0, x1, x2, x3, x4, x5, x6, x7, x8) add9(x0, x1, x2, x3, x4, x5, x6, x7, x8)
#define ui64and2(x0, x1) add2(x0, x1)
#define ui64and3(x0, x1, x2) add3(x0, x1, x2)
#define ui64and9(x0, x1, x2, x3, x4, x5, x6, x7, x8) add9(x0, x1, x2, x3, x4, x5, x6, x7, x8)

// or
#define ui8or2(x0, x1) add2(x0, x1)
#define ui8or3(x0, x1, x2) add3(x0, x1, x2)
#define ui8or9(x0, x1, x2, x3, x4, x5, x6, x7, x8) add9(x0, x1, x2, x3, x4, x5, x6, x7, x8)
#define ui32or2(x0, x1) add2(x0, x1)
#define ui32or3(x0, x1, x2) add3(x0, x1, x2)
#define ui32or9(x0, x1, x2, x3, x4, x5, x6, x7, x8) add9(x0, x1, x2, x3, x4, x5, x6, x7, x8)
#define ui64or2(x0, x1) add2(x0, x1)
#define ui64or3(x0, x1, x2) add3(x0, x1, x2)
#define ui64or9(x0, x1, x2, x3, x4, x5, x6, x7, x8) add9(x0, x1, x2, x3, x4, x5, x6, x7, x8)

#endif // MORPHO_OPERATOR_IS_ADD

// -----------------------------
#ifdef MORPHO_OPERATOR_IS_AND_OR
// -----------------------------

#ifdef MORPHO_VERBOSE_DEFINE
#pragma message("MORPHO_OPERATOR_IS_AND_OR")
#endif

// pour la morpho binaire: min/max sont remplaces par and/or
// and
#define and2(x0, x1) (x0) & (x1)
#define and3(x0, x1, x2) (x0) & (x1) & (x2)
#define and9(x0, x1, x2, x3, x4, x5, x6, x7, x8) (x0) & (x1) & (x2) & (x3) & (x4) & (x5) & (x6) & (x7) & (x8)
// and
#define ui8and2(x0, x1) and2(x0, x1)
#define ui8and3(x0, x1, x2) and3(x0, x1, x2)
#define ui8and9(x0, x1, x2, x3, x4, x5, x6, x7, x8) and9(x0, x1, x2, x3, x4, x5, x6, x7, x8)
#define ui32and2(x0, x1) and2(x0, x1)
#define ui32and3(x0, x1, x2) and3(x0, x1, x2)
#define ui32and9(x0, x1, x2, x3, x4, x5, x6, x7, x8) and9(x0, x1, x2, x3, x4, x5, x6, x7, x8)
#define ui64and2(x0, x1) and2(x0, x1)
#define ui64and3(x0, x1, x2) and3(x0, x1, x2)
#define ui64and9(x0, x1, x2, x3, x4, x5, x6, x7, x8) and9(x0, x1, x2, x3, x4, x5, x6, x7, x8)

// or
#define or2(x0, x1) (x0) | (x1)
#define or3(x0, x1, x2) (x0) | (x1) | (x2)
#define or9(x0, x1, x2, x3, x4, x5, x6, x7, x8) (x0) | (x1) | (x2) | (x3) | (x4) | (x5) | (x6) | (x7) | (x8)
// or
#define ui8or2(x0, x1) or2(x0, x1)
#define ui8or3(x0, x1, x2) or3(x0, x1, x2)
#define ui8or9(x0, x1, x2, x3, x4, x5, x6, x7, x8) or9(x0, x1, x2, x3, x4, x5, x6, x7, x8)
#define ui32or2(x0, x1) or2(x0, x1)
#define ui32or3(x0, x1, x2) or3(x0, x1, x2)
#define ui32or9(x0, x1, x2, x3, x4, x5, x6, x7, x8) or9(x0, x1, x2, x3, x4, x5, x6, x7, x8)
#define ui64or2(x0, x1) or2(x0, x1)
#define ui64or3(x0, x1, x2) or3(x0, x1, x2)
#define ui64or9(x0, x1, x2, x3, x4, x5, x6, x7, x8) or9(x0, x1, x2, x3, x4, x5, x6, x7, x8)
// max
#define ui8max2(x0, x1) or2(x0, x1)
#define ui8max3(x0, x1, x2) or3(x0, x1, x2)
#define ui8max9(x0, x1, x2, x3, x4, x5, x6, x7, x8) or9(x0, x1, x2, x3, x4, x5, x6, x7, x8)
#define ui32max2(x0, x1) or2(x0, x1)
#define ui32max3(x0, x1, x2) or3(x0, x1, x2)
#define ui32max9(x0, x1, x2, x3, x4, x5, x6, x7, x8) or9(x0, x1, x2, x3, x4, x5, x6, x7, x8)
#define ui64max2(x0, x1) or2(x0, x1)
#define ui64max3(x0, x1, x2) or3(x0, x1, x2)
#define ui64max9(x0, x1, x2, x3, x4, x5, x6, x7, x8) or9(x0, x1, x2, x3, x4, x5, x6, x7, x8)
// min
#define ui8min2(x0, x1) and2(x0, x1)
#define ui8min3(x0, x1, x2) and3(x0, x1, x2)
#define ui8min9(x0, x1, x2, x3, x4, x5, x6, x7, x8) and9(x0, x1, x2, x3, x4, x5, x6, x7, x8)
#define ui32min2(x0, x1) and2(x0, x1)
#define ui32min3(x0, x1, x2) and3(x0, x1, x2)
#define ui32min9(x0, x1, x2, x3, x4, x5, x6, x7, x8) and9(x0, x1, x2, x3, x4, x5, x6, x7, x8)
#define ui64min2(x0, x1) and2(x0, x1)
#define ui64min3(x0, x1, x2) and3(x0, x1, x2)
#define ui64min9(x0, x1, x2, x3, x4, x5, x6, x7, x8) and9(x0, x1, x2, x3, x4, x5, x6, x7, x8)

#endif // MORPHO_OPERATOR_IS_AND_OR

// ------------------------------------------------------------------------
#if !defined(MORPHO_OPERATOR_IS_ADD) && !defined(MORPHO_OPERATOR_IS_AND_OR)
// ------------------------------------------------------------------------
// fonctionnement nominal: du coup pas de min/max en macro
#define MIN_MAX_MACRO_ERROR puts("Min Max are not defined in macro mode"); exit(-1)

// max
#define ui8max2(x0, x1) x0; MIN_MAX_MACRO_ERROR
#define ui8max3(x0, x1, x2) x0; MIN_MAX_MACRO_ERROR
#define ui8max9(x0, x1, x2, x3, x4, x5, x6, x7, x8) x0; MIN_MAX_MACRO_ERROR
#define ui32max2(x0, x1) x0; MIN_MAX_MACRO_ERROR
#define ui32max3(x0, x1, x2) x0; MIN_MAX_MACRO_ERROR
#define ui32max9(x0, x1, x2, x3, x4, x5, x6, x7, x8) x0; MIN_MAX_MACRO_ERROR
#define ui64max2(x0, x1) x0; MIN_MAX_MACRO_ERROR
#define ui64max3(x0, x1, x2) x0; MIN_MAX_MACRO_ERROR
#define ui64max9(x0, x1, x2, x3, x4, x5, x6, x7, x8) x0; MIN_MAX_MACRO_ERROR

// min
#define ui8min2(x0, x1) x0; MIN_MAX_MACRO_ERROR
#define ui8min3(x0, x1, x2) x0; MIN_MAX_MACRO_ERROR
#define ui8min9(x0, x1, x2, x3, x4, x5, x6, x7, x8) x0; MIN_MAX_MACRO_ERROR
#define ui32min2(x0, x1)  x0; MIN_MAX_MACRO_ERROR
#define ui32min3(x0, x1, x2)  x0; MIN_MAX_MACRO_ERROR
#define ui32min9(x0, x1, x2, x3, x4, x5, x6, x7, x8)  x0; MIN_MAX_MACRO_ERROR
#define ui64min2(x0, x1)  x0; MIN_MAX_MACRO_ERROR
#define ui64min3(x0, x1, x2)  x0; MIN_MAX_MACRO_ERROR
#define ui64min9(x0, x1, x2, x3, x4, x5, x6, x7, x8)  x0; MIN_MAX_MACRO_ERROR

// and
#define and2(x0, x1) (x0) & (x1)
#define and3(x0, x1, x2) (x0) & (x1) & (x2)
#define and9(x0, x1, x2, x3, x4, x5, x6, x7, x8) (x0) & (x1) & (x2) & (x3) & (x4) & (x5) & (x6) & (x7) & (x8)
#define ui8and2(x0, x1) and2(x0, x1)
#define ui8and3(x0, x1, x2) and3(x0, x1, x2)
#define ui8and9(x0, x1, x2, x3, x4, x5, x6, x7, x8) and9(x0, x1, x2, x3, x4, x5, x6, x7, x8)
#define ui32and2(x0, x1) and2(x0, x1)
#define ui32and3(x0, x1, x2) and3(x0, x1, x2)
#define ui32and9(x0, x1, x2, x3, x4, x5, x6, x7, x8) and9(x0, x1, x2, x3, x4, x5, x6, x7, x8)
#define ui64and2(x0, x1) and2(x0, x1)
#define ui64and3(x0, x1, x2) and3(x0, x1, x2)
#define ui64and9(x0, x1, x2, x3, x4, x5, x6, x7, x8) and9(x0, x1, x2, x3, x4, x5, x6, x7, x8)

// or
#define or2(x0, x1) (x0) | (x1)
#define or3(x0, x1, x2) (x0) | (x1) | (x2)
#define or9(x0, x1, x2, x3, x4, x5, x6, x7, x8) (x0) | (x1) | (x2) | (x3) | (x4) | (x5) | (x6) | (x7) | (x8)
#define ui8or2(x0, x1) or2(x0, x1)
#define ui8or3(x0, x1, x2) or3(x0, x1, x2)
#define ui8or9(x0, x1, x2, x3, x4, x5, x6, x7, x8) or9(x0, x1, x2, x3, x4, x5, x6, x7, x8)
#define ui32or2(x0, x1) or2(x0, x1)
#define ui32or3(x0, x1, x2) or3(x0, x1, x2)
#define ui32or9(x0, x1, x2, x3, x4, x5, x6, x7, x8) or9(x0, x1, x2, x3, x4, x5, x6, x7, x8)
#define ui64or2(x0, x1) or2(x0, x1)
#define ui64or3(x0, x1, x2) or3(x0, x1, x2)
#define ui64or9(x0, x1, x2, x3, x4, x5, x6, x7, x8) or9(x0, x1, x2, x3, x4, x5, x6, x7, x8)

#endif // !defined(MORPHO_OPERATOR_IS_ADD) && !defined(MORPHO_OPERATOR_IS_AND_OR)

#endif // MORPHO_MACRO


#ifdef MORPHO_FUNCTION

#ifdef MORPHO_VERBOSE_DEFINE
#pragma message("MORPHO_FUNCTION")
#endif

uint8 ui8max2(uint8 x0, uint8 x1);
uint8 ui8max3(uint8 x0, uint8 x1, uint8 x2);
uint8 ui8max9(uint8 x0, uint8 x1, uint8 x2, uint8 x3, uint8 x4, uint8 x5, uint8 x6, uint8 x7, uint8 x8);

uint32 ui32max2(uint32 x0, uint32 x1);
uint32 ui32max3(uint32 x0, uint32 x1, uint32 x2);
uint32 ui32max9(uint32 x0, uint32 x1, uint32 x2, uint32 x3, uint32 x4, uint32 x5, uint32 x6, uint32 x7, uint32 x8);

uint64 ui64max2(uint64 x0, uint64 x1);
uint64 ui64max3(uint64 x0, uint64 x1, uint64 x2);
uint64 ui64max9(uint64 x0, uint64 x1, uint64 x2, uint64 x3, uint64 x4, uint64 x5, uint64 x6, uint64 x7, uint64 x8);

uint8 ui8min2(uint8 x0, uint8 x1);
uint8 ui8min3(uint8 x0, uint8 x1, uint8 x2);
uint8 ui8min9(uint8 x0, uint8 x1, uint8 x2, uint8 x3, uint8 x4, uint8 x5, uint8 x6, uint8 x7, uint8 x8);

uint32 ui32min2(uint32 x0, uint32 x1);
uint32 ui32min3(uint32 x0, uint32 x1, uint32 x2);
uint32 ui32min9(uint32 x0, uint32 x1, uint32 x2, uint32 x3, uint32 x4, uint32 x5, uint32 x6, uint32 x7, uint32 x8);

uint64 ui64min2(uint64 x0, uint64 x1);
uint64 ui64min3(uint64 x0, uint64 x1, uint64 x2);
uint64 ui64min9(uint64 x0, uint64 x1, uint64 x2, uint64 x3, uint64 x4, uint64 x5, uint64 x6, uint64 x7, uint64 x8);

uint8 ui8and2(uint8 x0, uint8 x1);
uint8 ui8and3(uint8 x0, uint8 x1, uint8 x2);
uint8 ui8and9(uint8 x0, uint8 x1, uint8 x2, uint8 x3, uint8 x4, uint8 x5, uint8 x6, uint8 x7, uint8 x8);

uint32 ui32and2(uint32 x0, uint32 x1);
uint32 ui32and3(uint32 x0, uint32 x1, uint32 x2);
uint32 ui32and9(uint32 x0, uint32 x1, uint32 x2, uint32 x3, uint32 x4, uint32 x5, uint32 x6, uint32 x7, uint32 x8);

uint64 ui64and2(uint64 x0, uint64 x1);
uint64 ui64and3(uint64 x0, uint64 x1, uint64 x2);
uint64 ui64and9(uint64 x0, uint64 x1, uint64 x2, uint64 x3, uint64 x4, uint64 x5, uint64 x6, uint64 x7, uint64 x8);

uint8 ui8or2(uint8 x0, uint8 x1);
uint8 ui8or3(uint8 x0, uint8 x1, uint8 x2);
uint8 ui8or9(uint8 x0, uint8 x1, uint8 x2, uint8 x3, uint8 x4, uint8 x5, uint8 x6, uint8 x7, uint8 x8);

uint32 ui32or2(uint32 x0, uint32 x1);
uint32 ui32or3(uint32 x0, uint32 x1, uint32 x2);
uint32 ui32or9(uint32 x0, uint32 x1, uint32 x2, uint32 x3, uint32 x4, uint32 x5, uint32 x6, uint32 x7, uint32 x8);

uint64 ui64or2(uint64 x0, uint64 x1);
uint64 ui64or3(uint64 x0, uint64 x1, uint64 x2);
uint64 ui64or9(uint64 x0, uint64 x1, uint64 x2, uint64 x3, uint64 x4, uint64 x5, uint64 x6, uint64 x7, uint64 x8);

#endif // MORPHO_FUNCTION

#ifdef __cplusplus
}
#endif

#endif // __MORPHO_H__
