#include "bkSpace.h"

bkMat bkMat_idt (void) {
	return m4_identity ();
}

bkMat bkMat_mul (const bkMat *a, const bkMat *b) {
	return m4_mul (*a, *b);
}

bkMat bkMat_proj (int w, int h, float angle, float near, float far) {
	return m4_perspective (angle, w / (float) h, near, far);
}

bkMat bkMat_view (const bkRay * ray) {
	return m4_look_at (ray->position, ray->target, (vec3_t) { 0.0, 1.0, 0.0 });
}
