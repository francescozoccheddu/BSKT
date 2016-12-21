#ifndef _BSKT_STATE_H
#define _BSKT_STATE_H

#include "bkSpace.h"
#define MODELS_COUNT 2

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct {
		bkRay cam;
		bkRay light;
		bkMat modelMats[MODELS_COUNT];
		GLfloat colors[MODELS_COUNT][4];
		float lightDisp;
		int temp;
	} bkSceneState;

	typedef struct {
		int level;
		int record;
		int tBallThrown;
		int rBallThrown;
	} bkGameState;

	typedef struct {
		bkGameState game;
		bkSceneState scene;
	} bkState;

#ifdef __cplusplus
}
#endif

#endif