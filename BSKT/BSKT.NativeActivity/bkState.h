#ifndef _BSKT_STATE_H
#define _BSKT_STATE_H

#include "bkMatrix.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct {
		int bog;
	} bkSceneState;

	typedef struct {
		int level;
		bkSceneState sceneState;
	} bkGameState;

#ifdef __cplusplus
}
#endif

#endif