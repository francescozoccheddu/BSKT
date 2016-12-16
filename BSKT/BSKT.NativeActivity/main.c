#include "android_native_app_glue.h"
#include <EGL\egl.h>
#include "bkEnv.h"
#include "bkAssets.h"
#include "bkState.h"

bkAssetPack *assetPack = NULL;

typedef struct {
	bkState state;
	bkEnv env;
} bkEngine;

static void bk_drawFrame(bkEngine* engine) {
	if (engine->env.display == NULL) 
		return;
	glClearColor(0.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	engine->state.scene.cam.position = (bkVec) {
		1.0, 2.0, 4.0
	};
	engine->state.scene.cam.target = (bkVec) {
		0.0, 0.0, 0.0
	};
	bkEnv_draw(&engine->env, &(engine->state.scene));
	eglSwapBuffers(engine->env.display, engine->env.surface);
}

static int32_t bk_handleInput(struct android_app* app, AInputEvent* event) {
	bkEngine* engine = (bkEngine*)app->userData;
	if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
		//TODO
		return 1;
	}
	return 0;
}

static void bk_handleCmd(struct android_app* app, int32_t cmd) {
	bkEngine* engine = (bkEngine*)app->userData;
	switch (cmd) {
		case APP_CMD_WINDOW_RESIZED:
			
			break;
		case APP_CMD_PAUSE:
			break;
		case APP_CMD_SAVE_STATE:
			app->savedState = malloc(sizeof(bkState));
			*((bkState*) app->savedState) = engine->state;
			app->savedStateSize = sizeof(bkState);
			break;
		case APP_CMD_INIT_WINDOW:
			if (app->window != NULL) {
				engine->env = bkEnv_init(app, assetPack);
				bk_drawFrame(engine);
			}
			break;
		case APP_CMD_TERM_WINDOW:
			bkEnv_term(&engine->env);
			break;
		case APP_CMD_GAINED_FOCUS:
			// When our app gains focus, we start monitoring the accelerometer.
			break;
		case APP_CMD_LOST_FOCUS:
			// When our app loses focus, we stop monitoring the accelerometer.
			// This is to avoid consuming battery while not being used.
			bk_drawFrame(engine);
			break;
	}
}

void android_main(struct android_app* app) {

	if (assetPack == NULL) {
		assetPack = malloc(sizeof(bkAssetPack));
		*assetPack = bkAssetPack_load(app->activity->assetManager);
	}

	bkEngine engine;

	memset(&engine, 0, sizeof(bkEngine));
	app->userData = &engine;
	app->onAppCmd = bk_handleCmd;
	app->onInputEvent = bk_handleInput;

	if (app->savedState != NULL) 
		engine.state = *(bkState*) app->savedState;

	while (1) {
		// Read all pending events.
		int ident;
		int events;
		struct android_poll_source* source;

		// If not animating, we will block forever waiting for events.
		// If animating, we loop until all events are read, then continue
		// to draw the next frame of animation.
		while ((ident = ALooper_pollAll(engine.env.ready ? 0 : -1, NULL, &events,
			(void**)&source)) >= 0) {

			// Process this event.
			if (source != NULL) {
				source->process(app, source);
			}

			// Check if we are exiting.
			if (app->destroyRequested != 0) {
				bkEnv_term(&engine.env);
				return;
			}
		}

		if (engine.env.ready) {
			// Done with events; draw next animation frame.

			// Drawing is throttled to the screen update rate, so there
			// is no need to do timing here.
			bk_drawFrame(&engine);
		}
	}
}