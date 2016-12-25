#ifndef _BSKT_ASSETS_H
#define _BSKT_ASSETS_H

#include <GLES2\gl2.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct {
		char *vertexShader;
		char *fragmentShader;
	} bkProgSrc;

	extern const float bkMeshVerts[];
	extern const int bkMeshVertsLen;
	extern const short bkMeshTrInds[];
	extern const int bkMeshTrIndsLen;

	extern const bkProgSrc bkProgSrcDepth;
	extern const bkProgSrc bkProgSrcDepthExt;
	extern const bkProgSrc bkProgSrcDiffuse;
	extern const bkProgSrc bkProgSrcDiffuseExt;

#ifdef __cplusplus
}
#endif

#endif