/* ---------------------- */
/* --- ue_l3_vision.c --- */
/* ---------------------- */

/*
 * Copyright (c) 2021 - 2021, Lionel Lacassagne, All rights reserved
 * LIP6 - Sorbonne University
 */

#include "ue_l3_vision.h"

// ------------------------
char* get_binome_name(void)
// ------------------------
{
#ifdef BINOME
    return BINOME;
#else
    return "personne";
#endif
}
