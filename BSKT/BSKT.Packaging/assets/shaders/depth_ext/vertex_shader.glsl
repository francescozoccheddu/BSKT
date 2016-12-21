//Depth Ext Fragment Shader

#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif 

attribute vec4 a_position;
attribute float a_index;

uniform mat4 u_model[2];
uniform mat4 u_projview;

void main(void) {
	gl_Position = u_projview * u_model[int(a_index)] * a_position;
}
