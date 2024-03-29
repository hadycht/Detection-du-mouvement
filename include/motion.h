/* ---------------- */
/* --- motion.h --- */
/* ---------------- */

/*
* Copyright (c) 2020 - 2021, Lionel Lacassagne, All rights reserved
* Sorbonne University, LIP6, CNRS
*/

#ifndef __MOTION_H__
#define __MOTION_H__

#ifdef __cplusplus
#ifdef PRAGMA_VERBOSE
#pragma message ("C++")
#endif
extern "C" {
#endif

// sequence car3 (ne pas oublier le '/' a la fin)

#define SEQUENCE_SRC_PATH "car3/"
#define SEQUENCE_FILENAME "car_3"
#define SEQUENCE_NDIGIT 3
#define SEQUENCE_HEIGHT 240
#define SEQUENCE_WIDTH 320
#define SEQUENCE_TSTART 0
#define SEQUENCE_TSTOP 199
#define SEQUENCE_TSTEP 1

#define SEQUENCE_DST_PATH "results/"

void motion_detection_morpho(void);

#ifdef __cplusplus
}
#endif

#endif // __MOTION_H__

