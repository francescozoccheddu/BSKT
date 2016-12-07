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

namespace BSKT {

	struct GLProgramSource {
		const char *vertexShader;
		const char *fragmentShader;
	};

	const GLProgramSource parseProgramSource(const AAssetManager*, const char*);

	struct Mesh {
		const float *vertices;
	    int count;
	};

	const Mesh parseMesh(const AAssetManager*, const char*);

	struct MeshBatch {
		const float *vertices;
		int count;
	};

	struct Assets {
		GLProgramSource programSource;
	};

	const char* parseFile(const char*, const AAssetManager*);
	const char* parseFile(const char**, int, const AAssetManager*);
	const char** splitString(const char*, int&, char);

}
#endif