#ifndef _BSKT_MATRIX_H
#define _BSKT_MATRIX_H

#include <GLES2\gl2.h>
#include "math3D.h"

#define PI 3.14159265359
#define TO_DEG 57.2957795131
#define TO_RAD 0.01745329251
#define BK_CAM_FAR 10.0
#define BK_CAM_NEAR 0.1
#define BK_CAM_ANGLE 80.0
#define BK_LIGHT_FAR 5.0
#define BK_LIGHT_NEAR 1.0
#define BK_LIGHT_ANGLE 45.0

#ifdef __cplusplus
extern "C" {
#endif

	typedef vec3_t bkVec;
	typedef mat4_t bkMat;

	typedef struct {
		bkVec position;
		bkVec target;
	} bkRay;

	bkMat bkMat_idt (void);
	bkMat bkMat_mul (const bkMat *a, const bkMat *b);
	bkMat bkMat_proj (int w, int h, float angle, float near, float far);
	bkMat bkMat_view (const bkRay *ray);

	extern const bkMat bkMat_bias;

#ifdef __cplusplus
}
#endif

#endif

