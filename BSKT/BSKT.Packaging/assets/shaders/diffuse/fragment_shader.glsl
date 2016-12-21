//Diffuse Fragment Shader

uniform sampler2D u_depthmap;

varying lowp vec4 v_color;
varying lowp vec4 v_position;

float DecodeFloatRGBA( vec4 rgba ) {
  return dot( rgba, vec4 ( 1.0, 1.0 / 255.0, 1.0 / 65025.0, 1.0 / 160581375.0 ) );
}

void main(void) {
	vec3 proj = v_position.xyz / v_position.w;
	float sampledepth = DecodeFloatRGBA ( texture2D( u_depthmap, proj.xy * 0.5 + 0.5 ) );
	float fallof = 1.0 - length(proj.xy);
	float shadow = sampledepth < proj.z ? 0.0 : 1.0;
	gl_FragColor = vec4( vec3(sampledepth), 1.0);
}
