#ifndef _BSKT_ASSETS_H
#define _BSKT_ASSETS_H

#include <GLES2\gl2.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BKPROG_ATTR_POS 0
#define BKPROG_ATTR_NORM 1
#define BKPROG_ATTR_INDX 2

	typedef struct {
		char *vertexShader;
		char *fragmentShader;
	} bkProgSrc;

	extern const float bkMeshVerts[];
	extern const int bkMeshVertsLen;
	extern const short bkMeshTrInds[];
	extern const int bkMeshTrIndsLen;
	extern const short bkMeshLnInds[];
	extern const int bkMeshLnIndsLen;

	extern const bkProgSrc bkProgSrcCel;
	extern const bkProgSrc bkProgSrcDepth;
	extern const bkProgSrc bkProgSrcDepthExt;
	extern const bkProgSrc bkProgSrcDiffuse;
	extern const bkProgSrc bkProgSrcDiffuseExt;

#ifdef __cplusplus
}
#endif

#endif