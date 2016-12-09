#include "bkEnv.h"

static int bk_initDisp(const struct android_app *app, bkEnv *env);
static GLuint bk_createShader(GLenum, const char*);
static GLuint bk_createProgram(const char*, const char*);

static GLuint bk_createShader(GLenum type, const char *source) {
	GLuint shader;
	if ((shader = glCreateShader(type)) == 0) {
		LOGE("No shader created");
		return 0;
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
				GLchar* log = malloc(sizeof(GLchar) * logLen);
				glGetShaderInfoLog(shader, logLen, NULL, log);
				LOGE("%s", log);
				free(log);
			}
			glDeleteShader(shader);
			return 0;
		}
	}
	return shader;
}

static GLuint bk_createProgram(const char *vertexSource, const char *fragmentSource) {
	GLuint program;
	if ((program = glCreateProgram()) == 0) {
		LOGE("No program created");
		return 0;
	}
	GLuint vertexShader, fragmentShader;
	if ((vertexShader = bk_createShader(GL_VERTEX_SHADER, vertexSource)) == 0)
		return 0;
	if ((fragmentShader = bk_createShader(GL_FRAGMENT_SHADER, fragmentSource)) == 0)
		return 0;
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
				GLchar* log = malloc(sizeof(GLchar) * logLen);
				glGetProgramInfoLog(program, logLen, NULL, log);
				LOGE("%s", log);
				free(log);
			}
			glDeleteProgram(program);
			return 0;
		}
	}
	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return program;
}

static int bk_initDisp(const struct android_app *app, bkEnv *env) {

	EGLint w, h;
	EGLSurface surface;
	EGLContext context;
	EGLDisplay display;

	if ((display = eglGetDisplay(EGL_DEFAULT_DISPLAY)) == EGL_NO_DISPLAY) {
		LOGE("No display");
		return 0;
	}

	if (!eglInitialize(display, 0, 0)) {
		if (eglGetError() == EGL_BAD_DISPLAY)
			LOGE("EGL bad display");
		LOGE("EGL not initialized");
		return 0;
	}

	EGLConfig config;
	{
		EGLint numConfigs;
		const EGLint attribs[] = {
			EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			EGL_BLUE_SIZE, 8,
			EGL_GREEN_SIZE, 8,
			EGL_RED_SIZE, 8,
			EGL_NONE
		};
		eglChooseConfig(display, attribs, &config, 1, &numConfigs);
		EGLint format;
		eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
		ANativeWindow_setBuffersGeometry(app->window, 0, 0, format);
	}

	{
		const EGLint attribList[] = {
			EGL_RENDER_BUFFER, EGL_BACK_BUFFER,
			EGL_NONE
		};
		if ((surface = eglCreateWindowSurface(display, config, app->window, attribList)) == EGL_NO_SURFACE) {
			switch (eglGetError())
			{
			case EGL_BAD_MATCH:
				LOGE("EGL bad match");
				break;
			case EGL_BAD_CONFIG:
				LOGE("EGL bad config");
				break;
			case EGL_BAD_NATIVE_WINDOW:
				LOGE("EGL bad native window");
				break;
			case EGL_BAD_ALLOC:
				LOGE("EGL bad alloc");
				break;
			}
			LOGE("No surface created");
			return 0;
		}
	}

	{
		const EGLint attribList[] = {
			EGL_CONTEXT_CLIENT_VERSION, 2,
			EGL_NONE
		};
		if ((context = eglCreateContext(display, config, EGL_NO_CONTEXT, attribList)) == EGL_NO_CONTEXT) {
			if (eglGetError() == EGL_BAD_CONFIG)
				LOGE("EGL bad config");
			LOGE("No context created");
			return 0;
		}
	}

	if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
		LOGE("Unable to eglMakeCurrent");
		return 0;
	}

	eglQuerySurface(display, surface, EGL_WIDTH, &w);
	eglQuerySurface(display, surface, EGL_HEIGHT, &h);

	env->display = display;
	env->context = context;
	env->surface = surface;
	env->width = w;
	env->height = h;

	glEnable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
		
	return 1;
}

const bkEnv bkEnv_init(const struct android_app *app, const bkAssetPack *pack)
{
	bkEnv env;
	env.valid = bk_initDisp(app, &env);
	return env;
}

void bkGLEnv_term(bkEnv *env)
{
	if (env->display != EGL_NO_DISPLAY) {
		eglMakeCurrent(env->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		if (env->context != EGL_NO_CONTEXT) {
			eglDestroyContext(env->display, env->context);
		}
		if (env->surface != EGL_NO_SURFACE) {
			eglDestroySurface(env->display, env->surface);
		}
		eglTerminate(env->display);
	}
	env->ready = 0;
	env->display = EGL_NO_DISPLAY;
	env->context = EGL_NO_CONTEXT;
	env->surface = EGL_NO_SURFACE;
}
