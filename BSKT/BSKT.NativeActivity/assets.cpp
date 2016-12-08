#include "assets.h"

namespace BSKT {

	namespace Assets {

		const GLProgramSource parseProgramSource(const AAssetManager *assets, const char *name) {
			GLProgramSource ps;
			const char *path[] = { SHADERS_DIR, name, VERTEX_SHADER_FILE };
			ps.vertexShader = parseFile(assets, path, 3);
			path[2] = FRAGMENT_SHADER_FILE;
			ps.fragmentShader = parseFile(assets, path, 3);
			return ps;
		}

		const Mesh parseMesh(const AAssetManager *assets, const char *name) {
			Mesh m;
			const char *path[] = { MESHES_DIR, name, MESH_VERTICES_FILE };
			{
				const char *vertsStr = parseFile(assets, path, 3);
				const char **vertsStrs = splitString(vertsStr, m.vertsCount, MESH_VERT_SEP);
				delete[] vertsStr;
				float *verts = new float[m.vertsCount];
				for (int i = 0; i < m.vertsCount; i++)
					verts[i] = atof(vertsStrs[i]);
				m.vertices = verts;
				deleteSplitString(vertsStrs);
			}
			path[2] = MESH_INDICES_FILE;
			{
				const char *indsStr = parseFile(assets, path, 3);
				const char **indsStrs = splitString(indsStr, m.indsCount, MESH_VERT_SEP);
				delete[] indsStr;
				int *inds = new int[m.indsCount];
				for (int i = 0; i < m.indsCount; i++)
					inds[i] = atoi(indsStrs[i]);
				m.indices = inds;
				deleteSplitString(indsStrs);
			}
			return m;
		}

		const Mesh createMeshBatch(const Mesh *parts, int count, int stride) {
			Mesh m;
			{
				int len = 0;
				for (int p = 0; p < count; p++)
					len += parts[p].vertsCount;
				len = len / stride * (stride + 1);
				float *verts = new float[len];
				float *v = verts;
				for (int p = 0; p < count; p++) {
					const Mesh part = parts[p];
					int mv = 0;
					while (mv < part.vertsCount) {
						for (int i = 0; i < stride; i++)
							*v++ = part.vertices[mv++];
						*v++ = p;
					}
				}
				m.vertices = verts;
				m.vertsCount = len;
			}
			{
				int len = 0;
				for (int i = 0; i < count; i++)
					len += parts[i].indsCount;
				int offset = 0;
				int *inds = new int[len];
				int *i = inds;
				for (int p = 0; p < count; p++) {
					const Mesh part = parts[p];
					int ic = part.indsCount;
					for (int mi = 0; mi < ic; mi++)
						*i++ = part.indices[mi] + offset;
					offset += part.vertsCount / stride;
				}
				m.indices = inds;
				m.indsCount = len;
			}
			return m;
		}

		const Pack createPack(const AAssetManager * assets)
		{
			Pack p;
			p.programSource = parseProgramSource(assets, "solid2d");
			Mesh triangle = parseMesh(assets, "triangle");
			p.meshBatch = createMeshBatch((const Mesh[]) { triangle }, 1, 2);
			return p;
		}

		const char* parseFile(const AAssetManager *assets, const char *path) {
			AAsset* asset = AAssetManager_open((AAssetManager*)assets, path, AASSET_MODE_BUFFER);
			off_t len = AAsset_getLength(asset);
			char *dst = new char[len + 1];
			AAsset_read(asset, dst, len);
			AAsset_close(asset);
			dst[len] = '\0';
			return dst;
		}

		const char* parseFile(const AAssetManager *assets, const char ** path, int count) {
			int len = count;
			for (int i = 0; i < count; i++)
				len += strlen(path[i]);
			char *str = new char[len];
			strcpy(str, *path);
			for (int i = 1; i < count; i++) {
				strcat(str, FILE_SEP);
				strcat(str, path[i]);
			}
			const char *ret = parseFile(assets, str);
			delete[] str;
			return ret;
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
				dst += strlen(dst) + 1;
			}
			return tok;
		}

		void deleteSplitString(const char** str) {
			free((void*)*str);
			delete[] str;
		}

	}

}