attribute lowp vec3 a_position;
attribute lowp float a_index;

uniform lowp mat4 u_transform[1];
uniform lowp mat4 u_projection;

void main(void) {
	gl_Position = u_projection * u_transform[int(a_index)] * vec4 ( a_position, 1.0 );
}
