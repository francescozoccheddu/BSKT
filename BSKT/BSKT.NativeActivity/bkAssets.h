#ifndef _BSKT_ASSETS_H
#define _BSKT_ASSETS_H

#include "log.h"
#include <android\asset_manager.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <GLES2\gl2.h>

#define SHADERS_DIR "shaders/"
#define SHADERS_PATH_STRLEN 29
#define VERTEX_SHADER_FILE "/vertex_shader.glsl"
#define FRAGMENT_SHADER_FILE "/fragment_shader.glsl"

#define MESHES_DIR "meshes/"
#define MESHES_PATH_STRLEN 7
#define MESH_VALUE_SEP ','
#define MESH_FIELD_SEP ';'

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct {
		char *vertexShader;
		char *fragmentShader;
	} bkProgSrc;

	const bkProgSrc bkProgSrc_load(AAssetManager *assets, const char *name);

	typedef struct {
		GLfloat *vertices;
		int vertsCount;
		GLushort *indices;
		int indsCount;
	} bkMesh;

	const bkMesh bkMesh_load(AAssetManager *assets, const char *name);
	const bkMesh bkMesh_batch(const bkMesh *meshes, int count, int stride);

	typedef struct {
		bkProgSrc programSource;
		bkMesh meshBatch;
	} bkAssetPack;

	const bkAssetPack bkAssetPack_load(AAssetManager *assets);

	char *bkAssets_loadStr(AAssetManager *assets, const char *filename);
	char **bkAssets_splitStr(char *string, int *tokensCount, char delimiter);

#ifdef __cplusplus
}
#endif

#endif