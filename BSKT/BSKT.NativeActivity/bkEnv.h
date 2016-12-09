#ifndef _BSKT_RESOURCES_H
#define _BSKT_RESOURCES_H

#include "android_native_app_glue.h"
#include <EGL\egl.h>
#include <GLES2\gl2.h>
#include "log.h"
#include "bkAssets.h"
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct {
		int valid;
		GLuint program;
		GLuint vbo;
		GLuint ibo;
		int ready;
		EGLDisplay display;
		EGLSurface surface;
		EGLContext context;
		int32_t width;
		int32_t height;
	} bkEnv;

	const bkEnv bkEnv_init(const struct android_app *app, const bkAssetPack *pack);
	void bkGLEnv_term(bkEnv *env);

#ifdef __cplusplus
}
#endif

#endif