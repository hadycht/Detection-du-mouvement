/* ---------------------- */
/* --- sigmadelta16.h --- */
/* ---------------------- */

/*
* Copyright (c) 2020 - 2021, Lionel Lacassagne, All rights reserved
* Sorbonne University, LIP6, CNRS
*/

#ifndef __SIGMADELTA16_H__
#define __SIGMADELTA16_H__

#ifdef __cplusplus
#ifdef PRAGMA_VERBOSE
#pragma message ("C++")
#endif
extern "C" {
#endif

void SigmaDelta16_FirstStep(uint16 **I, int i0, int i1, int j0, int j1, uint16 **M, uint16 **D, uint16 **V, byte **E);
void SigmaDelta16_1Step    (uint16 **I, int i0, int i1, int j0, int j1, uint16 **M, uint16 **D, uint16 **V, byte **E, int k);

#ifdef __cplusplus
}
#endif

#endif // __SIGMADELTA16_H__
