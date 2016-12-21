//Diffuse Ext Vertex Shader

#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif 

attribute vec4 a_position;
attribute vec4 a_normal;
attribute float a_index;

uniform mat4 u_model[2];
uniform vec4 u_color[2];
uniform mat4 u_projview;
uniform vec3 u_lightpos;
uniform float u_dispersion;
uniform mat4 u_lightbiasprojview;

varying vec4 v_color;
varying vec4 v_position;

void main(void) {
	int index = int( a_index );
	vec4 modelpos = u_model[index] * a_position;
	vec3 lightvec = u_lightpos - modelpos.xyz;
	float square = dot( lightvec, lightvec );
	float diffuse = max( dot( ( u_model[index] * a_normal ).xyz, lightvec / sqrt( square ) ), 0.1 ) / ( 1.0 + ( u_dispersion * square ) ); 
	v_color = vec4 ( u_color[index].rgb * diffuse, u_color[index].a );
	v_position = u_lightbiasprojview * modelpos;
	gl_Position = u_projview * modelpos;
}
