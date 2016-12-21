//Diffuse Ext Vertex Shader

attribute lowp vec4 a_position;
attribute lowp vec4 a_normal;
attribute lowp float a_index;

uniform lowp mat4 u_model[2];
uniform lowp vec4 u_color[2];
uniform lowp mat4 u_projview;
uniform lowp vec3 u_lightpos;
uniform lowp float u_dispersion;
uniform lowp mat4 u_lightprojview;

varying lowp vec4 v_color;
varying lowp vec4 v_position;

void main(void) {
	lowp int index = int( a_index );
	lowp vec4 modelpos = u_model[index] * a_position;
	lowp vec3 lightvec = u_lightpos - modelpos.xyz;
	lowp float square = dot( lightvec, lightvec );
	lowp float diffuse = max( dot( ( u_model[index] * a_normal ).xyz, lightvec / sqrt( square ) ), 0.1 ) / ( 1.0 + ( u_dispersion * square ) ); 
	v_color = vec4 ( u_color[index].rgb * diffuse, u_color[index].a );
	v_position = u_lightprojview * modelpos;
	gl_Position = u_projview * modelpos;
}
