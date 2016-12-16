#include "bkAssets.h"

const bkProgSrc bkProgSrc_load(AAssetManager *assets, const char *name) {
	bkProgSrc ps;
	char *path = malloc (sizeof (char) * (SHADERS_PATH_STRLEN + strlen (name) + 1));
	strcpy (path, SHADERS_DIR);
	strcat (path, name);
	strcat (path, VERTEX_SHADER_FILE);
	ps.vertexShader = bkAssets_loadStr (assets, path);
	strcpy (path, SHADERS_DIR);
	strcat (path, name);
	strcat (path, FRAGMENT_SHADER_FILE);
	ps.fragmentShader = bkAssets_loadStr(assets, path);
	free (path);
	return ps;
}

const bkMesh bkMesh_load(AAssetManager *assets, const char *name) {
	bkMesh m;
	int i;
	char *path = malloc (sizeof (char) * (MESHES_PATH_STRLEN + strlen(name) + 1));
	strcpy (path, MESHES_DIR);
	strcat (path, name);
	char *fileStr = bkAssets_loadStr(assets, path);
	char **fileStrs = bkAssets_splitStr(fileStr, NULL, MESH_FIELD_SEP);
	char **vertsStrs = bkAssets_splitStr (fileStrs[0], &m.vertsCount, MESH_VALUE_SEP);
	GLfloat *verts = malloc(sizeof(GLfloat) * m.vertsCount);
	for (i = 0; i < m.vertsCount; i++)
		verts[i] = (GLfloat) atof(vertsStrs[i]);
	m.vertices = verts;
	free(vertsStrs);
	char **indsStrs = bkAssets_splitStr(fileStrs[1], &m.indsCount, MESH_VALUE_SEP);
	GLushort *inds = malloc(sizeof(GLushort) * m.indsCount);
	for (i = 0; i < m.indsCount; i++)
		inds[i] = (GLushort) atoi(indsStrs[i]);
	m.indices = inds;
	free(indsStrs);
	free (fileStrs);
	free (fileStr);
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
	p.programSource = bkProgSrc_load(assets, "diffuse");
	bkMesh triangle = bkMesh_load(assets, "manichino");
	p.meshBatch = bkMesh_batch((const bkMesh[]) { triangle }, 1, 3);
	return p;
}

char* bkAssets_loadStr(AAssetManager *assets, const char *path) {
	AAsset* asset = AAssetManager_open(assets, path, AASSET_MODE_BUFFER);
	off_t len = AAsset_getLength(asset);
	char *dst = malloc(sizeof(char) * (len + 1));
	AAsset_read(asset, dst, len);
	AAsset_close(asset);
	dst[len] = '\0';
	return dst;
}

char** bkAssets_splitStr(char *str, int *num, char regex) {
	int count = 1, len = strlen(str), i;
	for (i = 0; i < len; i++)
		if (str[i] == regex) {
			count++;
			str[i] = '\0';
		}
	if (num != NULL)
		*num = count;
	char **tok = malloc(sizeof(char*) * count);
	for (i = 0; i < count; i++) {
		tok[i] = str;
		str += strlen(str) + 1;
	}
	return tok;
}
