//Depth Ext Fragment Shader

attribute lowp vec4 a_position;
attribute lowp float a_index;

uniform lowp mat4 u_model[2];
uniform lowp mat4 u_projview;

void main(void) {
	gl_Position = u_projview * u_model[int(a_index)] * a_position;
}
