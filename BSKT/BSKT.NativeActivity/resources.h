#ifndef _BSKT_RESOURCES_H
#define _BSKT_RESOURCES_H

#include <GLES2\gl2.h>
#include "log.h"
#include "assets.h"
#include <stdlib.h>

#define NULL_RES 0

namespace BSKT {

	struct Context {
		GLuint program;
		Context(Assets);

		void destroy();
	};

}
#endif