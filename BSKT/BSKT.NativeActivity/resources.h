
#ifndef _BSKT_RESOURCES_H
#define _BSKT_RESOURCES_H

#include <GLES2\gl2.h>
#include "log.h"
#include <android\asset_manager.h>
#include <android\asset_manager_jni.h>
#include <cstring>
#include <math.h>
#include <stdlib.h>

#define NULL_RES 0

#define FILE_SEP "/"

#define SHADERS_DIR "shaders"
#define VERTEX_SHADER_FILE "vertex_shader.glsl"
#define FRAGMENT_SHADER_FILE "fragment_shader.glsl"

#define MESHES_DIR "meshes"
#define MESH_VERT_SEP ','

namespace BSKT {

	struct GLProgramSource {
		char *vertexShader;
		char *fragmentShader;

		GLProgramSource();

		GLProgramSource(AAssetManager*, const char*);
	};

	struct Mesh {
		GLfloat *vertices;
		int count;

		Mesh();

		Mesh(AAssetManager*, const char*);
	};

	struct Assets {
		GLProgramSource programSource;

		Assets(GLProgramSource);
	};

	struct GLResources {
		GLuint program;
		GLResources(Assets);

		void destroy();
	};

	char* parseFile(const char*, AAssetManager*);
	char* parseFile(const char**, int, AAssetManager*);

}
#endif