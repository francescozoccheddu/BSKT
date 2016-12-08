#ifndef _BSKT_RESOURCES_H
#define _BSKT_RESOURCES_H

#include "android_native_app_glue.h"
#include <EGL\egl.h>
#include <GLES2\gl2.h>
#include "log.h"
#include "assets.h"
#include <stdlib.h>

#define NULL_RES 0

namespace BSKT {

	namespace GLEnv {

		struct Env {
			bool valid;
			GLuint program;
			bool ready;
			EGLDisplay display;
			EGLSurface surface;
			EGLContext context;
			int32_t width;
			int32_t height;
		};

		const Env initialize(const android_app *app, const Assets::Pack *pack);
		void terminate(Env *env);

	}

}
#endif