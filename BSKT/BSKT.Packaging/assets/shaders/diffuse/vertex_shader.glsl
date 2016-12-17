attribute lowp vec3 a_position;
attribute lowp float a_index;

uniform lowp mat4 u_transform[1];
uniform lowp vec4 u_color[1];
uniform lowp mat4 u_projection;
uniform lowp vec3 u_lightpos;

varying lowp vec4 v_color;

void main(void) {
	lowp int index = int(a_index);
	lowp vec4 spacepos = u_transform[index] * vec4 ( a_position, 1.0 );
	v_color = vec4 ( u_color[index].rgb * ( 1.0 - distance ( u_lightpos , spacepos.xyz ) / 10.0 ), u_color[index].a );
	gl_Position = u_projection * spacepos;
}
