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
			EGL_DEPTH_SIZE, 4,
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


	env->display = display;
	env->context = context;
	env->surface = surface;

	bkEnv_viewport(env);

	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable (GL_CULL_FACE);
	glCullFace (GL_BACK);
	glFrontFace (GL_CCW);
	glDepthMask (GL_TRUE);
	glDepthFunc (GL_LEQUAL);
	return 1;
}

const bkEnv bkEnv_init(const struct android_app *app, const bkAssetPack *pack)
{
	bkEnv env;
	if (bk_initDisp(app, &env)) {
		bkProgDiffuse p;
		p.program = bk_createProgram(pack->programSource.vertexShader, pack->programSource.fragmentShader);
		if (p.program == 0)
			return env;
		glUseProgram(p.program);
		p.attrPosition = glGetAttribLocation (p.program, "a_position");
		p.attrNormal = glGetAttribLocation(p.program, "a_normal");
		p.attrIndex = glGetAttribLocation(p.program, "a_index");
		p.unifProjView = glGetUniformLocation (p.program, "u_projview");
		p.unifLightPos = glGetUniformLocation (p.program, "u_lightpos");
		p.unifDispersion = glGetUniformLocation (p.program, "u_dispersion");
		p.unifModel[0] = glGetUniformLocation(p.program, "u_model[0]");
		p.unifColor[0] = glGetUniformLocation (p.program, "u_color[0]");
		p.unifModel[1] = glGetUniformLocation (p.program, "u_model[1]");
		p.unifColor[1] = glGetUniformLocation (p.program, "u_color[1]");
		glUseProgram(0);
		GLuint *buf = malloc(sizeof(GLuint) * 2);
		glGenBuffers(2, buf);
		glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * pack->meshBatch.vertsCount, pack->meshBatch.vertices, GL_STATIC_DRAW);
		p.vbo = buf[0];
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * pack->meshBatch.indsCount, pack->meshBatch.indices, GL_STATIC_DRAW);
		p.ibo = buf[1];
		free(buf);
		p.indsCount = pack->meshBatch.indsCount;
		env.programDiffuse = p;
		env.valid = 1;
		env.ready = 1;
	}
	return env;
}

void bkEnv_term(bkEnv *env)
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

void bkEnv_viewport(bkEnv * env)
{
	eglQuerySurface(env->display, env->surface, EGL_WIDTH, &env->width);
	eglQuerySurface(env->display, env->surface, EGL_HEIGHT, &env->height);
	glViewport(0, 0, env->width, env->height);
}


void bkEnv_draw(const bkEnv * env, const bkSceneState * state)
{
	glClearColor (0.0, 1.0, 1.0, 1.0);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(env->programDiffuse.program);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, env->programDiffuse.ibo);
	glBindBuffer(GL_ARRAY_BUFFER, env->programDiffuse.vbo);
	glEnableVertexAttribArray (env->programDiffuse.attrPosition);
	glEnableVertexAttribArray(env->programDiffuse.attrNormal);
	glEnableVertexAttribArray(env->programDiffuse.attrIndex);
	glVertexAttribPointer (env->programDiffuse.attrPosition, 3, GL_FLOAT, GL_FALSE, 7 * sizeof (GLfloat), (const GLvoid*) 0);
	glVertexAttribPointer(env->programDiffuse.attrNormal, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (const GLvoid*) (3 * sizeof (GLfloat)));
	glVertexAttribPointer(env->programDiffuse.attrIndex, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (const GLvoid*) (6 * sizeof(GLfloat)));
	bkMat proj = bkMat_proj (&state->cam, env->width, env->height);
	bkMat view = bkMat_view (&state->cam);
	bkMat comb = bkMat_mul (&proj, &view);
	glUniform3f (env->programDiffuse.unifLightPos, 0.5, 0.7, 0);
	glUniform1f (env->programDiffuse.unifDispersion, state->lightDisp);
	glUniformMatrix4fv (env->programDiffuse.unifProjView, 1, GL_FALSE, (GLfloat *) &comb);
	int m;
	for (m = 0; m < MODELS_COUNT; m++) {
		glUniformMatrix4fv(env->programDiffuse.unifModel[m], 1, GL_FALSE, (GLfloat *) &state->modelMats[m]);
		glUniform4f (env->programDiffuse.unifColor[m], 1, 1, 1, 1);
	}
	glDrawElements(GL_TRIANGLES, env->programDiffuse.indsCount, GL_UNSIGNED_SHORT, (const void*) 0);
	glUseProgram(0);
}

