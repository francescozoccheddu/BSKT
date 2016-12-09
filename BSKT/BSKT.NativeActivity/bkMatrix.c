#include "bkMatrix.h"

void bkMat_idt(bkMat *mat)
{
	float *v = &mat->val;
	v[0] = 1;
	v[1] = 0;
	v[2] = 0;
	v[3] = 0;
	v[4] = 0;
	v[5] = 1;
	v[6] = 0;
	v[7] = 0;
	v[8] = 0;
	v[9] = 0;
	v[10] = 1;
	v[11] = 0;
	v[12] = 0;
	v[13] = 0;
	v[14] = 0;
	v[15] = 1;
}
