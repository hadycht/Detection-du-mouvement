/* -------------------- */
/* --- sigmadelta.h --- */
/* -------------------- */

/*
* Copyright (c) 2020 - 2021, Lionel Lacassagne, All rights reserved
* Sorbonne University, LIP6, CNRS
*/

#ifndef __SIGMADELTA_H__
#define __SIGMADELTA_H__

#ifdef VERBOSE_PRAGMA
#pragma message ("- *** include sigmadelta.h ***")
#endif

#ifdef __cplusplus
#ifdef PRAGMA_VERBOSE
#pragma message ("C++")
#endif
extern "C" {
#endif

#define SD_VMAX 253
#define SD_VMIN 2

#define min(x, y) ((x) < (y) ? (x) : (y))  

#define max(x, y) ((x) > (y) ? (x) : (y))  

#define abs(a, b) ((a) > (b) ? ((a) - (b)) : ((b) - (a)))


//#define SD_CHRONO(X,t) X
#define SD_CHRONO(X,t) do { double t0, t1, dt; t0 = (double)_rdtsc(); X; t1 = (double)_rdtsc(); dt = t1-t0; t = dt; } while (0)
//#define SD_CHRONO(X,t) do { double t0, t1, dt; t0 = (double) dcycle(); X; t1 = (double) dcycle(); dt = t1-t0; t = dt; } while (0)

void SigmaDelta_Step0_line(uint8 *I, uint8 *M, uint8 *O, uint8 *V, uint8 *E,        int j0, int j1);
void SigmaDelta_1Step_line(uint8 *I, uint8 *M, uint8 *O, uint8 *V, uint8 *E, int k, int j0, int j1);

void SigmaDelta_Step0(uint8 **I, uint8 **M, uint8 **O, uint8 **V, uint8 **E,        int i0, int i1, int j0, int j1);
void SigmaDelta_1Step(uint8 **I, uint8 **M, uint8 **O, uint8 **V, uint8 **E, int k, int i0, int i1, int j0, int j1);

#ifdef __cplusplus
}
#endif

#endif // __SIGMADELTA_H__
