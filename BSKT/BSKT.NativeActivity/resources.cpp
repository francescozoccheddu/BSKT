
#include "resources.h"

namespace BSKT {

	static GLuint createProgram(const char*, const char*);
	static GLuint createProgram(const GLProgramSource);
	static GLuint createShader(GLenum, const char*);

	static GLuint createShader(GLenum type, const char *source) {
		GLuint shader;
		if ((shader = glCreateShader(type)) == 0) {
			LOGE("No shader created");
			return NULL_RES;
		}
		glShaderSource(shader, 1, &source, NULL);
		glCompileShader(shader);
		{
			GLint compileStatus;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
			if (compileStatus == GL_FALSE) {
				LOGE("Shader not compiled");
				GLint logLen;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
				if (logLen > 1) {
					GLchar* log = new GLchar[logLen];
					glGetShaderInfoLog(shader, logLen, NULL, log);
					LOGE("%s", log);
					delete[] log;
				}
				glDeleteShader(shader);
				return NULL_RES;
			}
		}
		return shader;
	}

	static GLuint createProgram(const char *vertexSource, const char *fragmentSource) {
		GLuint program;
		if ((program = glCreateProgram()) == 0) {
			LOGE("No program created");
			return NULL_RES;
		}
		GLuint vertexShader, fragmentShader;
		if ((vertexShader = createShader(GL_VERTEX_SHADER, vertexSource)) == NULL_RES)
			return NULL_RES;
		if ((fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentSource)) == NULL_RES)
			return NULL_RES;
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);
		{
			int linkStatus;
			glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
			if (linkStatus == GL_FALSE) {
				GLint logLen;
				glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
				if (logLen > 1) {
					GLchar* log = new GLchar[logLen];
					glGetProgramInfoLog(program, logLen, NULL, log);
					LOGE("%s", log);
				}
				glDeleteProgram(program);
				return NULL_RES;
			}
		}
		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		return program;
	}

	static GLuint createProgram(const GLProgramSource source) {
		return createProgram(source.vertexShader, source.vertexShader);
	}

	GLProgramSource::GLProgramSource() { }

	GLProgramSource::GLProgramSource(AAssetManager *assets, const char *name) {
		const char *path[] = { SHADERS_DIR, name, VERTEX_SHADER_FILE };
		vertexShader = parseFile(path, 3, assets);
		path[2] = FRAGMENT_SHADER_FILE;
		fragmentShader = parseFile(path, 3, assets);
	}

	Mesh::Mesh() { }

	Mesh::Mesh(AAssetManager *assets, const char *name) {
		const char *path[] = { MESHES_DIR, name };
		const char *str = parseFile(path, 2, assets);
		int len = strlen(str);
		count = 1;
		for (int i = 0; i < len; i++)
			if (str[i] == MESH_VERT_SEP)
				count++;
		vertices = new float[count];
		for (int i = 0; i < len; i++) {
			char *end;
			vertices[i] = strtof(str, &end);
			str = end + 1;
		}
	}

	Assets::Assets(GLProgramSource program) { 
		programSource = program;
	}

	GLResources::GLResources(Assets assets) {
		program = createProgram(assets.programSource);
	}

	void GLResources::destroy() {
		glDeleteProgram(program);
	}

	char* parseFile(const char *path, AAssetManager *assets) {
		AAsset* asset = AAssetManager_open(assets, path, AASSET_MODE_BUFFER);
		off_t len = AAsset_getLength(asset);
		char *dst = new char[len + 1];
		AAsset_read(asset, dst, len);
		AAsset_close(asset);
		dst[len] = '\0';
		return dst;
	}

	char* parseFile(const char **path, int count, AAssetManager *assets) {
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

}