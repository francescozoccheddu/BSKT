#ifndef _BSKT_MATRIX_H
#define _BSKT_MATRIX_H

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct {
		float val[4][4];
	} bkMat;

	void bkMat_idt(bkMat *mat);

#ifdef __cplusplus
}
#endif

#endif

