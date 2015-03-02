
uniform vec2 resolution;
uniform sampler2D finalTex;

void main(void) {
	vec2 uv = gl_FragCoord.xy / resolution;
	//uv *= 2.0;
	//uv -= 1.0;
	gl_FragColor = vec4(texture(finalTex, uv).xyz, 1.0);
}