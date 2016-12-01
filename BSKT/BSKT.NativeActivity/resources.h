
#ifndef RESOURCES_H
#define RESOURCES_H

#define NULL_RES 0

struct Resources{
	const GLuint program;
	Resources(GLuint);
	
	void destroy();
};

Resources createResources();

#endif