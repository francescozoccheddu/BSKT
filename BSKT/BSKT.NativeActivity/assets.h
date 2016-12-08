#ifndef _BSKT_ASSETS_H
#define _BSKT_ASSETS_H

#include "log.h"
#include <android\asset_manager.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define FILE_SEP "/"

#define SHADERS_DIR "shaders"
#define VERTEX_SHADER_FILE "vertex_shader.glsl"
#define FRAGMENT_SHADER_FILE "fragment_shader.glsl"

#define MESHES_DIR "meshes"
#define MESH_VERT_SEP ','
#define MESH_VERTICES_FILE "vertices.bsktm"
#define MESH_INDICES_FILE "indices.bsktm"

namespace BSKT {

	namespace Assets {

		struct GLProgramSource {
			const char *vertexShader;
			const char *fragmentShader;
		};

		const GLProgramSource parseProgramSource(const AAssetManager *assets, const char *name);

		struct Mesh {
			const float *vertices;
			int vertsCount;
			const int *indices;
			int indsCount;
		};

		const Mesh parseMesh(const AAssetManager *assets, const char *name);
		const Mesh createMeshBatch(const Mesh *meshes, int count, int stride);

		struct Pack {
			GLProgramSource programSource;
			Mesh meshBatch;
		};

		const Pack createPack(const AAssetManager *assets);

		const char *parseFile(const AAssetManager *assets, const char *filename);
		const char *parseFile(const AAssetManager *assets, const char **filepath, int pathSize);
		const char **splitString(const char *string, int &tokensCount, char delimiter);
		void deleteSplitString(const char **string);

	}

}
#endif