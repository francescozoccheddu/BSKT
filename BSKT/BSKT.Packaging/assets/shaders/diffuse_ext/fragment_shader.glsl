//Diffuse Ext Fragment Shader

uniform sampler2D u_depthmap;

varying lowp vec4 v_color;
varying lowp vec4 v_position;

void main(void) {
	vec3 proj = v_position.xyz / v_position.w;
	float sampledepth = texture2D( u_depthmap, proj.xy * 0.5 + 0.5 ).z;
	float fallof = 1.0 - length(proj.xy);
	float shadow = sampledepth < proj.z ? 0.0 : 1.0;
	gl_FragColor = vec4( vec3(sampledepth), 1.0);
}

