//Diffuse Ext Fragment Shader

#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif 

uniform highp sampler2D u_depthmap;

varying vec4 v_color;
varying vec4 v_position;

const float bias = 0.0005;

void main(void) {
	float hshad = 1.0 -float ( texture2DProj ( u_depthmap, v_position.xyw ).z < ( v_position.z - bias ) / v_position.w );
	vec2 rad = v_position.xy - 0.5;
	float fallof = 1.0 - dot ( rad, rad ) * 2.0;
	gl_FragColor = vec4( vec3(fallof * hshad * v_color.r), 1.0);
}

