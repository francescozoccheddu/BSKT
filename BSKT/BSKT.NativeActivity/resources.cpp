
#include "resources.h"

static GLuint createProgram(const char*, const char*);
static GLuint createShader(GLenum, const char*);

Resources::Resources(GLuint program) : program(program) { };

void Resources::destroy() {
	glDeleteProgram(program);
}

Resources createResources() {
	return Resources(createProgram("ca", "zzo"));
}

static GLuint createShader(GLenum type, const char* source) {
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

static GLuint createProgram(const char* vertexSource, const char* fragmentSource) {
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
	return program;
}