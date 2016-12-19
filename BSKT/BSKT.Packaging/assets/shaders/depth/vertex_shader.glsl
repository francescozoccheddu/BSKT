//Depth Fragment Shader

#extension OES_depth_texture : enable 

attribute lowp vec4 a_position;
attribute lowp float a_index;

uniform lowp mat4 u_model[2];
uniform lowp mat4 u_projview;

#ifdef OES_depth_texture

void main(void) {
	gl_Position = u_projview * u_model[int(a_index)] * a_position;
}

#else

varying lowp vec4 v_position;

void main(void) {
	gl_Position = v_position = u_projview * u_model[int(a_index)] * a_position;
}

#endif