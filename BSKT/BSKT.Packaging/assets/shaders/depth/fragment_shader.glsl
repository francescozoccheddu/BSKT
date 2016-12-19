//Depth Fragment Shader

#extension OES_depth_texture : enable 

#ifdef OES_depth_texture

void main (void) {
	
}

#else

varying lowp vec4 v_position;

vec4 pack (lowp float depth)
{
	const lowp vec4 bitSh = vec4 ( 256.0 * 256.0 * 256.0, 256.0 * 256.0, 256.0, 1.0 );
	const lowp vec4 bitMsk = vec4 ( 0, 1.0 / 256.0, 1.0 / 256.0, 1.0 / 256.0 );
	lowp vec4 comp = fract ( depth * bitSh );
	comp -= comp.xxyz * bitMsk;
	return comp;
}

void main (void) {
	/*lowp float normalizedDistance  = v_position.z / v_position.w;
	normalizedDistance = ( normalizedDistance + 1.0 ) / 2.0;
	gl_FragColor = pack ( normalizedDistance );*/
	lowp float l = v_position.z / v_position.w;
	gl_FragColor = vec4( l, l, l, 1.0 );
}

#endif
