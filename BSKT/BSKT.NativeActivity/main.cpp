#include "android_native_app_glue.h"
#include <android\sensor.h>
#include <EGL\egl.h>
#include <GLES2\gl2.h>
#include "glcontext.h"
#include "assets.h"
#include "state.h"

BSKT::Assets::Pack *assets = NULL;

struct Engine {
	bsktState state;
	BSKT::GLEnv::Env env;
};

static void engine_draw_frame(Engine* engine) {
	if (engine->env.display == NULL) {
		// No display.
		return;
	}

	// Just fill the screen with a color.
	glClearColor(0.0,1.0,1.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	eglSwapBuffers(engine->env.display, engine->env.surface);
}

static int32_t engine_handle_input(struct android_app* app, AInputEvent* event) {
	Engine* engine = (Engine*)app->userData;
	if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
		//TODO
		return 1;
	}
	return 0;
}

static void engine_handle_cmd(struct android_app* app, int32_t cmd) {
	Engine* engine = (Engine*)app->userData;
	switch (cmd) {
		case APP_CMD_WINDOW_RESIZED:
			
			break;
		case APP_CMD_PAUSE:
			break;
		case APP_CMD_SAVE_STATE:
			app->savedState = malloc(sizeof(bsktState));
			*((bsktState*)app->savedState) = engine->state;
			app->savedStateSize = sizeof(bsktState);
			break;
		case APP_CMD_INIT_WINDOW:
			if (app->window != NULL) {
				engine->env = BSKT::GLEnv::initialize(app, assets);
				engine_draw_frame(engine);
			}
			break;
		case APP_CMD_TERM_WINDOW:
			BSKT::GLEnv::terminate(&engine->env);
			break;
		case APP_CMD_GAINED_FOCUS:
			// When our app gains focus, we start monitoring the accelerometer.
			break;
		case APP_CMD_LOST_FOCUS:
			// When our app loses focus, we stop monitoring the accelerometer.
			// This is to avoid consuming battery while not being used.
			engine_draw_frame(engine);
			break;
	}
}

/**
* This is the main entry point of a native application that is using
* android_native_app_glue.  It runs in its own thread, with its own
* event loop for receiving input events and doing other things.
*/
void android_main(struct android_app* app) {

	if (assets == NULL)
		assets = new BSKT::Assets::Pack(BSKT::Assets::createPack(app->activity->assetManager));

	Engine engine;

	memset(&engine, 0, sizeof(Engine));
	app->userData = &engine;
	app->onAppCmd = engine_handle_cmd;
	app->onInputEvent = engine_handle_input;

	if (app->savedState != NULL) 
		engine.state = *(bsktState*) app->savedState;

	while (true) {
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
				BSKT::GLEnv::terminate(&engine.env);
				return;
			}
		}

		if (engine.env.ready) {
			// Done with events; draw next animation frame.

			// Drawing is throttled to the screen update rate, so there
			// is no need to do timing here.
			engine_draw_frame(&engine);
		}
	}
}
