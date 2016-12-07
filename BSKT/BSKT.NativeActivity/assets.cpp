#include "assets.h"

namespace BSKT {

	const GLProgramSource parseProgramSource(const AAssetManager *assets, const char *name) {
		GLProgramSource ps;
		ps.vertexShader = parseFile((const char *[]) { SHADERS_DIR, name, VERTEX_SHADER_FILE }, 3, assets);
		ps.fragmentShader = parseFile((const char *[]) { SHADERS_DIR, name, FRAGMENT_SHADER_FILE }, 3, assets);
		return ps;
	}

	const Mesh parseMesh(const AAssetManager *assets, const char *name) {
		const char *path[] = { MESHES_DIR, name };
		const char *str = parseFile(path, 2, assets);
		int len = strlen(str);
		int count = 1;
		for (int i = 0; i < len; i++)
			if (str[i] == MESH_VERT_SEP)
				count++;
		float* verts = new float[count];
		for (int i = 0; i < len; i++) {
			char *end;
			verts[i] = strtof(str, &end);
			str = end + 1;
		}
		Mesh m = { verts, count };
		return m;
	}

	const char* parseFile(const char *path, const AAssetManager *assets) {
		AAsset* asset = AAssetManager_open((AAssetManager*) assets, path, AASSET_MODE_BUFFER);
		off_t len = AAsset_getLength(asset);
		char *dst = new char[len + 1];
		AAsset_read(asset, dst, len);
		AAsset_close(asset);
		dst[len] = '\0';
		return dst;
	}

	const char* parseFile(const char ** path, int count, const AAssetManager *assets) {
		int len = count;
		for (int i = 0; i < count; i++)
			len += strlen(path[i]);
		char *str = new char[len];
		strcpy(str, *path);
		for (int i = 1; i < count; i++) {
			strcat(str, FILE_SEP);
			strcat(str, path[i]);
		}
		return parseFile(str, assets);
	}

	const char** splitString(const char *src, int &num, char regex) {
		char *dst = strdup(src);
		int count = 1;
		int len = strlen(dst);
		for (int i = 0; i < len; i++) {
			if (dst[i] == regex) {
				count++;
				dst[i] = '\0';
			}
		}
		num = count;
		const char **tok = new const char*[count];
		for (int i = 0; i < count; i++) {
			tok[i] = dst;
			dst += strlen(dst);
		}
		return tok;
	}

}