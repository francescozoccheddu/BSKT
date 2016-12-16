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

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct {
		GLuint program;
		GLuint attrPosition;
		GLuint attrIndex;
		GLuint unifTransform[1];
		GLuint unifColor[1];
		GLuint unifProjection;
		GLuint vbo;
		GLuint ibo;
		GLuint indsCount;
	} bkProgDiffuse;

	typedef struct {
		int valid;
		bkProgDiffuse programDiffuse;
		int ready;
		EGLDisplay display;
		EGLSurface surface;
		EGLContext context;
		int32_t width;
		int32_t height;
	} bkEnv;

	const bkEnv bkEnv_init(const struct android_app *app, const bkAssetPack *pack);
	void bkEnv_term(bkEnv *env);
	void bkEnv_viewport(bkEnv *env);
	void bkEnv_draw(const bkEnv *env, const bkSceneState *state);

#ifdef __cplusplus
}
#endif

#endif