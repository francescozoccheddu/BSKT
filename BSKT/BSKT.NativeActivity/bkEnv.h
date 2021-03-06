#ifndef _BSKT_RESOURCES_H
#define _BSKT_RESOURCES_H

#include "android_native_app_glue.h"
#include <EGL\egl.h>
#include <GLES2\gl2.h>
#include "log.h"
#include "bkAssets.h"
#include "bkSpace.h"
#include "bkState.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define BK_DEPTHMAP_TEXT_UNIT 0

#ifdef __cplusplus
extern "C" {
#endif


	typedef struct {
		GLuint program;
		GLuint unifModel;
		GLuint unifColor;
		GLuint unifProjView;
		GLuint unifLightPos;
		GLuint unifDispersion;
		GLuint unifLightBiasProjView;
	} bkProgDiffuse;

	typedef struct {
		GLuint program;
		GLuint fbo;
		GLuint fboTexture;
		GLuint fboRenderBuf;
		GLuint unifModel;
		GLuint unifProjView;
		int hasAttachments;
		int attSize;
		bkMat projection;
		int supportsDepthTex;
	} bkProgDepth;

	typedef struct {
		GLuint program;
		GLuint ibo;
		GLuint unifModel;
		GLuint unifProjView;
	} bkProgCel;


	typedef struct {
		GLuint vbo;
		GLuint ibo;
		int valid;
		bkProgDiffuse programDiffuse;
		bkProgDepth programDepth;
		bkProgCel programCel;
		int ready;
		EGLDisplay display;
		EGLSurface surface;
		EGLContext context;
		int32_t width;
		int32_t height;
		bkMat projection;
	} bkEnv;

	const bkEnv bkEnv_init(const struct android_app *app);
	void bkEnv_term(bkEnv *env);
	void bkEnv_resize(bkEnv *env);
	void bkEnv_draw(const bkEnv *env, const bkSceneState *state);

#ifdef __cplusplus
}
#endif

#endif