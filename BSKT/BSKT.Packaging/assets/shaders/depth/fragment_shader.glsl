//Depth Fragment Shader

varying lowp vec4 v_position;

void main (void) {
	float depth = v_position.z / v_position.w;
	gl_FragColor  = vec4( depth, depth, depth, 1.0 );
}
