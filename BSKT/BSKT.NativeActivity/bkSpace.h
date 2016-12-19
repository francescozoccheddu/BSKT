#ifndef _BSKT_MATRIX_H
#define _BSKT_MATRIX_H

#include <GLES2\gl2.h>
#include "math3D.h"

#define PI 3.14159265359
#define TO_DEG 57.2957795131
#define TO_RAD 0.01745329251
#define BKCAM_FAR 100.0
#define BKCAM_NEAR 0.1
#define BKCAM_ANGLE 90.0

#ifdef __cplusplus
extern "C" {
#endif

	typedef vec3_t bkVec;
	typedef mat4_t bkMat;

	typedef struct {
		bkVec position;
		bkVec target;
		GLfloat zoom;
	} bkCam;

	bkMat bkMat_idt (void);
	bkMat bkMat_mul (const bkMat *a, const bkMat *b);
	bkMat bkMat_proj (int w, int h);
	bkMat bkMat_view (const bkCam *cam);

#ifdef __cplusplus
}
#endif

#endif

