/* ------------------- */
/* -- nralloc_mod.h -- */
/* ------------------- */

/*
* Copyright (c) 2020 - 2021, Lionel Lacassagne, All rights reserved
* Sorbonne University, LIP6, CNRS
*/

#ifndef __NR_ALLOC_MOD_H__
#define __NR_ALLOC_MOD_H__

#ifdef __cplusplus
extern "C" {
#endif

uint8**        ui8matrix_mod(           int i0, int i1, int j0, int j1, int modulo);
void    resize_ui8matrix_mod(uint8 **m, int i0, int i1, int j0, int j1, int modulo);
void      free_ui8matrix_mod(uint8** m, int i0, int i1, int j0, int j1, int modulo);

#ifdef __cplusplus
}
#endif 

#endif // __NR_ALLOC_MOD_H__
