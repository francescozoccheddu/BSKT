attribute lowp vec3 a_position;
attribute lowp float a_index;

uniform lowp mat4 u_transform[1];
uniform lowp vec4 u_color[1];
uniform lowp mat4 u_projection;

varying lowp vec4 v_color;

void main(void) {
	lowp int index = int(a_index);
	v_color = u_color[index];
	gl_Position = u_projection * u_transform[int(a_index)] * vec4 ( a_position, 1.0 );
}
