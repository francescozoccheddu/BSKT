#include "bkSpace.h"

bkMat bkMat_idt (void) {
	return m4_identity ();
}

bkMat bkMat_mul (const bkMat *a, const bkMat *b) {
	return m4_mul (*a, *b);
}

bkMat bkMat_proj (const bkCam * cam, int w, int h) {
	return m4_perspective (BKCAM_ANGLE, h / (float) w, BKCAM_NEAR, BKCAM_FAR);
}

bkMat bkMat_view (const bkCam * cam) {
	return m4_look_at (cam->position, cam->target, (vec3_t) { 0.0, 1.0, 0.0 });
}
