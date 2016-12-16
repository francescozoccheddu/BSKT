#ifndef _BSKT_STATE_H
#define _BSKT_STATE_H

#include "bkSpace.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct {
		bkCam cam;
		bkVec lightPos;
		bkVec lightDir;
		bkMat basketMat;
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