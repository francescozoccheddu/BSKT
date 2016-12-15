#include "bkAssets.h"

const bkProgSrc bkProgSrc_load(AAssetManager *assets, const char *name) {
	bkProgSrc ps;
	const char *path[] = { SHADERS_DIR, name, VERTEX_SHADER_FILE };
	ps.vertexShader = bkAssets_loadp(assets, path, 3);
	path[2] = FRAGMENT_SHADER_FILE;
	ps.fragmentShader = bkAssets_loadp(assets, path, 3);
	return ps;
}

const bkMesh bkMesh_load(AAssetManager *assets, const char *name) {
	bkMesh m;
	const char *path[] = { MESHES_DIR, name, MESH_VERTICES_FILE };
	{
		char *vertsStr = bkAssets_loadp(assets, path, 3);
		char **vertsStrs = bkAssets_splitStr(vertsStr, &m.vertsCount, MESH_VERT_SEP);
		GLfloat *verts = malloc(sizeof(GLfloat) * m.vertsCount);
		int i;
		for (i = 0; i < m.vertsCount; i++)
			verts[i] = (GLfloat) atof(vertsStrs[i]);
		m.vertices = verts;
		free(vertsStr);
		bkAssets_freeSplitStr(vertsStrs);
	}
	path[2] = MESH_INDICES_FILE;
	{
		char *indsStr = bkAssets_loadp(assets, path, 3);
		char **indsStrs = bkAssets_splitStr(indsStr, &m.indsCount, MESH_VERT_SEP);
		GLushort *inds = malloc(sizeof(GLushort) * m.indsCount);
		int i;
		for (i = 0; i < m.indsCount; i++)
			inds[i] = (GLushort) atoi(indsStrs[i]);
		m.indices = inds;
		free(indsStr);
		bkAssets_freeSplitStr(indsStrs);
	}
	return m;
}

const bkMesh bkMesh_batch(const bkMesh *parts, int count, int stride) {
	bkMesh m;
	{
		int len = 0, p;
		GLfloat *verts, *v;
		for (p = 0; p < count; p++)
			len += parts[p].vertsCount;
		len = len / stride * (stride + 1);
		verts = malloc(sizeof(GLfloat) * len);
		v = verts;
		for (p = 0; p < count; p++) {
			const bkMesh part = parts[p];
			int mv = 0, i;
			while (mv < part.vertsCount) {
				for (i = 0; i < stride; i++)
					*v++ = part.vertices[mv++];
				*v++ = p;
			}
		}
		m.vertices = verts;
		m.vertsCount = len;
	}
	{
		int len = 0, p, offset = 0;
		GLushort *inds, *i;
		for (p = 0; p < count; p++)
			len += parts[p].indsCount;
		inds = malloc(sizeof(GLushort) * len);
		i = inds;
		for (p = 0; p < count; p++) {
			const bkMesh part = parts[p];
			int ic = part.indsCount, mi;
			for (mi = 0; mi < ic; mi++)
				*i++ = part.indices[mi] + offset;
			offset += part.vertsCount / stride;
		}
		m.indices = inds;
		m.indsCount = len;
	}
	return m;
}

const bkAssetPack bkAssetPack_load(AAssetManager *assets)
{
	bkAssetPack p;
	p.programSource = bkProgSrc_load(assets, "solid2d");
	bkMesh triangle = bkMesh_load(assets, "suzanne");
	p.meshBatch = bkMesh_batch((const bkMesh[]) { triangle }, 1, 3);
	return p;
}

char* bkAssets_load(AAssetManager *assets, const char *path) {
	AAsset* asset = AAssetManager_open(assets, path, AASSET_MODE_BUFFER);
	off_t len = AAsset_getLength(asset);
	char *dst = malloc(sizeof(char) * (len + 1));
	AAsset_read(asset, dst, len);
	AAsset_close(asset);
	dst[len] = '\0';
	return dst;
}

char* bkAssets_loadp(AAssetManager *assets, const char **path, int count) {
	int len = count, i;
	char *str;
	char *ret;
	for (i = 0; i < count; i++)
		len += strlen(path[i]);
	str = malloc(sizeof(char) * len);
	strcpy(str, *path);
	for (i = 1; i < count; i++) {
		strcat(str, FILE_SEP);
		strcat(str, path[i]);
	}
	ret = bkAssets_load(assets, str);
	free(str);
	return ret;
}

char** bkAssets_splitStr(const char *src, int *num, char regex) {
	char *dst = strdup(src);
	int count = 1, len = strlen(dst), i;
	char **tok;
	for (i = 0; i < len; i++) {
		if (dst[i] == regex) {
			count++;
			dst[i] = '\0';
		}
	}
	*num = count;
	tok = malloc(sizeof(const char*) * count);
	for (i = 0; i < count; i++) {
		tok[i] = dst;
		dst += strlen(dst) + 1;
	}
	return tok;
}

void bkAssets_freeSplitStr(char** str) {
	free((void*)*str);
	free(str);
}
