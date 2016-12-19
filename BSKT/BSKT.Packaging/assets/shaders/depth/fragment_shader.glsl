//Depth Fragment Shader

void main(void) {
	lowp float l = gl_FragCoord.z; 
	gl_FragColor = vec4( l, l, l, 1.0 );
}

