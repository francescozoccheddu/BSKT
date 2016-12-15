#ifndef _BSKT_ASSETS_H
#define _BSKT_ASSETS_H

#include "log.h"
#include <android\asset_manager.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <GLES2\gl2.h>

#define FILE_SEP "/"

#define SHADERS_DIR "shaders"
#define VERTEX_SHADER_FILE "vertex_shader.glsl"
#define FRAGMENT_SHADER_FILE "fragment_shader.glsl"

#define MESHES_DIR "meshes"
#define MESH_VERT_SEP ','
#define MESH_VERTICES_FILE "vertices.bsktm"
#define MESH_INDICES_FILE "indices.bsktm"

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

	char *bkAssets_load(AAssetManager *assets, const char *filename);
	char *bkAssets_loadp(AAssetManager *assets, const char **filepath, int pathSize);
	char **bkAssets_splitStr(const char *string, int *tokensCount, char delimiter);
	void bkAssets_freeSplitStr(char **string);

#ifdef __cplusplus
}
#endif

#endif