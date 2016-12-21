//Diffuse Ext Fragment Shader

#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif 

uniform highp sampler2D u_depthmap;

varying vec4 v_color;
varying vec4 v_position;

void main(void) {
	float smpd = texture2DProj( u_depthmap, v_position.xyw ).z;
	//float fallof = 1.0 - length(v_position.xy - 0.5);
	const float bias = 0.0005;
	float ddif = smpd < (v_position.z-bias) / v_position.w ? 0.0 : 1.0;
	gl_FragColor = vec4( vec3(ddif * ), 1.0);
}

