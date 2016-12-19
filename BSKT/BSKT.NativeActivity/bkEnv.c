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
		GLuint buf[2];
		glGenBuffers (2, buf);
		glBindBuffer (GL_ARRAY_BUFFER, buf[0]);
		glBufferData (GL_ARRAY_BUFFER, sizeof (GLfloat) * pack->meshBatch.vertsCount, pack->meshBatch.vertices, GL_STATIC_DRAW);
		env.vbo = buf[0];
		glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, buf[1]);
		glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof (GLushort) * pack->meshBatch.indsCount, pack->meshBatch.indices, GL_STATIC_DRAW);
		env.ibo = buf[1];
		env.indsCount = pack->meshBatch.indsCount;
		{
			bkProgDiffuse p;
			p.program = bk_createProgram (pack->diffuseProgSrc.vertexShader, pack->diffuseProgSrc.fragmentShader);
			if (p.program == 0)
				return env;

			GLuint attrPosition = glGetAttribLocation (p.program, "a_position");
			GLuint attrNormal = glGetAttribLocation (p.program, "a_normal");
			GLuint attrIndex = glGetAttribLocation (p.program, "a_index");
			glEnableVertexAttribArray (attrPosition);
			glEnableVertexAttribArray (attrNormal);
			glEnableVertexAttribArray (attrIndex);
			glVertexAttribPointer (attrPosition, 3, GL_FLOAT, GL_FALSE, 7 * sizeof (GLfloat), (const GLvoid*) 0);
			glVertexAttribPointer (attrNormal, 3, GL_FLOAT, GL_FALSE, 7 * sizeof (GLfloat), (const GLvoid*) (3 * sizeof (GLfloat)));
			glVertexAttribPointer (attrIndex, 1, GL_FLOAT, GL_FALSE, 7 * sizeof (GLfloat), (const GLvoid*) (6 * sizeof (GLfloat)));

			p.unifProjView = glGetUniformLocation (p.program, "u_projview");
			p.unifLightPos = glGetUniformLocation (p.program, "u_lightpos");
			p.unifDispersion = glGetUniformLocation (p.program, "u_dispersion");
			p.unifModel = glGetUniformLocation (p.program, "u_model");
			p.unifColor = glGetUniformLocation (p.program, "u_color");

			env.programDiffuse = p;
		}
		{
			bkProgDepth p;
			p.program = bk_createProgram (pack->depthProgSrc.vertexShader, pack->depthProgSrc.fragmentShader);
			if (p.program == 0)
				return env;

			GLuint attrPosition = glGetAttribLocation (p.program, "a_position");
			GLuint attrIndex = glGetAttribLocation (p.program, "a_index");
			glEnableVertexAttribArray (attrPosition);
			glEnableVertexAttribArray (attrIndex);
			glVertexAttribPointer (attrPosition, 3, GL_FLOAT, GL_FALSE, 7 * sizeof (GLfloat), (const GLvoid*) 0);
			glVertexAttribPointer (attrIndex, 1, GL_FLOAT, GL_FALSE, 7 * sizeof (GLfloat), (const GLvoid*) (6 * sizeof (GLfloat)));

			p.unifProjView = glGetUniformLocation (p.program, "u_projview");
			p.unifModel = glGetUniformLocation (p.program, "u_model");

			glGenFramebuffers (1, &p.fbo);
			p.hasAttachments = 0;
			p.supportsDepthTex = 1; //TODO Check
			env.programDepth = p;
		}
		env.valid = 1;
		env.ready = 1;
		bkEnv_resize (&env);
	}
	return env;
}

void bkEnv_term(bkEnv *env)
{
	if (env->valid) {
		glDeleteProgram (env->programDiffuse.program);
		glDeleteProgram (env->programDepth.program);
		glDeleteFramebuffers (1, &env->programDepth.fbo);
		if (env->programDepth.hasAttachments) {
			glDeleteTextures (1, &env->programDepth.fboTexture);
			if (!env->programDepth.supportsDepthTex)
				glDeleteRenderbuffers (1, &env->programDepth.fboRenderBuf);
		}
		glDeleteBuffers (2, (const GLuint[]) {env->ibo, env->vbo});
	}
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
	env->valid = 0;
	env->display = EGL_NO_DISPLAY;
	env->context = EGL_NO_CONTEXT;
	env->surface = EGL_NO_SURFACE;
}

void bkEnv_resize(bkEnv * env)
{
	int w, h;
	eglQuerySurface(env->display, env->surface, EGL_WIDTH, &w);
	eglQuerySurface(env->display, env->surface, EGL_HEIGHT, &h);
	env->width = w;
	env->height = h;
	env->programDiffuse.projection = bkMat_proj (w, h, BK_CAM_ANGLE, BK_CAM_NEAR, BK_CAM_FAR);

	bkProgDepth *prog = &env->programDepth;
	int attSize = pow( 2, (int) ( log2( w * h ) / 2.0 + 0.5 ) );
	prog->projection = bkMat_proj (attSize, attSize, BK_LIGHT_ANGLE, BK_LIGHT_NEAR, BK_LIGHT_FAR);

	if (prog->hasAttachments) {
		if (prog->attSize == attSize)
			return;
		glDeleteTextures (1, &prog->fboTexture);
		if (!prog->supportsDepthTex) 
			glDeleteRenderbuffers (1, &prog->fboRenderBuf);
	}

	prog->attSize = attSize;
	prog->hasAttachments = 1;

	GLuint texture;
	glGenTextures (1, &texture);
	glBindTexture (GL_TEXTURE_2D, texture);
	GLint format = prog->supportsDepthTex ? GL_DEPTH_COMPONENT : GL_RGBA;
	glTexImage2D (GL_TEXTURE_2D, 0, format, attSize, attSize, 0, format, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	prog->fboTexture = texture;

	if (prog->supportsDepthTex) {
		glBindFramebuffer (GL_FRAMEBUFFER, prog->fbo);
		glFramebufferTexture2D (GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
		glBindFramebuffer (GL_FRAMEBUFFER, 0);
	}
	else {
		GLuint renderBuffer;
		glGenRenderbuffers (1, &renderBuffer);
		glBindRenderbuffer (GL_RENDERBUFFER, renderBuffer);
		glRenderbufferStorage (GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, attSize, attSize);
		prog->fboRenderBuf = renderBuffer;

		glBindFramebuffer (GL_FRAMEBUFFER, prog->fbo);
		glFramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
		glFramebufferRenderbuffer (GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);
		glBindFramebuffer (GL_FRAMEBUFFER, 0);
	}

}


void bkEnv_draw(const bkEnv * env, const bkSceneState * state)
{
	{
		const bkProgDepth *prog = &env->programDepth;
		glViewport (0, 0, prog->attSize, prog->attSize);
		glBindFramebuffer (GL_FRAMEBUFFER, prog->fbo);
		glClearColor (0.0, 0.0, 0.0, 0.0);
		if (prog->supportsDepthTex) {
			glClear (GL_DEPTH_BUFFER_BIT);
			glColorMask (GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		}
		else {
			glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		glUseProgram (prog->program);
		bkMat view = bkMat_view (&state->light);
		bkMat comb = bkMat_mul (&prog->projection, &view);
		glUniformMatrix4fv (prog->unifProjView, 1, GL_FALSE, (GLfloat *) &comb);
		glUniformMatrix4fv (prog->unifModel, MODELS_COUNT, GL_FALSE, (GLfloat *) &state->modelMats);
		glDrawElements (GL_TRIANGLES, env->indsCount, GL_UNSIGNED_SHORT, (const GLvoid*) 0);
		glBindFramebuffer (GL_FRAMEBUFFER, 0);
		if (prog->supportsDepthTex)
			glColorMask (GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	}
	{
		const bkProgDiffuse *prog = &env->programDiffuse;
		glViewport (0, 0, env->width, env->height);
		glClearColor (0.0, 0.0, 0.0, 1.0);
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram (prog->program);
		bkMat view = bkMat_view (&state->cam);
		bkMat comb = bkMat_mul (&prog->projection, &view);
		bkVec lightpos = state->light.position;
		glUniform3f (prog->unifLightPos, lightpos.x, lightpos.y, lightpos.z );
		glUniform1f (prog->unifDispersion, state->lightDisp);
		glUniformMatrix4fv (prog->unifProjView, 1, GL_FALSE, (GLfloat *) &comb);
		glUniformMatrix4fv (prog->unifModel, MODELS_COUNT, GL_FALSE, (GLfloat *) &state->modelMats);
		glUniform4fv (prog->unifColor, MODELS_COUNT, (GLfloat *) state->colors);
		glDrawElements (GL_TRIANGLES, env->indsCount, GL_UNSIGNED_SHORT, (const GLvoid*) 0);
		glUseProgram (0);
	}
}

