
#ifndef _BSKT_RESOURCES_H
#define _BSKT_RESOURCES_H

#include <GLES2\gl2.h>
#include "log.h"

#define NULL_RES 0

namespace BSKT {

	struct Resources {
		const GLuint program;
		Resources(GLuint);

		void destroy();
	};

	Resources createResources();

}
#endif