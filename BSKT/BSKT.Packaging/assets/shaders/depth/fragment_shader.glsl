//Depth Fragment Shader

varying lowp vec4 v_position;

vec4 EncodeFloatRGBA( float v ) {
  vec4 enc = vec4 (1.0, 255.0, 65025.0, 160581375.0) * v;
  enc = fract(enc);
  enc -= enc.yzww * vec4(1.0/255.0,1.0/255.0,1.0/255.0,0.0);
  return enc;
}

void main (void) {
	float depth = v_position.z / v_position.w;
	gl_FragColor  = EncodeFloatRGBA( depth );
}