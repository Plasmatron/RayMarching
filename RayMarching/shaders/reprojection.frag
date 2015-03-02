#version 150

uniform vec2 resolution;
uniform vec3 deltaTranslation;
uniform vec3 inverseMatrix;
uniform vec3 viewMatrix;
uniform vec3 projectionMatrix;


uniform sampler2D depthTex;

out vec3 reprojectionCoords;




vec3 unproject() {
	vec2 pixelSize = 1.0 / resolution;//(1.0/resolution.x, 1.0/resolution.y);	
	vec3 pos = vec3 (( gl_FragCoord.x * pixelSize.x ) , ( gl_FragCoord.y * pixelSize.y ) , 0.0);
	pos.z = texture( depthTex, pos.xy).r ;		
	vec4 clip = inverseMatrix * vec4 ( pos * 2.0 - 1.0 , 1.0);
	pos = clip . xyz / clip . w;
	return pos;
}

vec2 project(vec3 pos) {
	pos = vec3(vec4(pos, 1.0)*viewMatrix*projectionMatrix);
	return pos.xy;
}


vec3 coordsToRGB(vec2 screenPosition) {

	
	int number = int(floor(screenPosition.x * screenPosition.y)) ;
	vec3 color; 
		color.x = number / 65536;
		color.y = number / 256;
		color.z = number % 256;	

	return color;

	
}



void main( void ) {

	vec3 previousPosition = unproject(); //previous position of object
	vec3 newPosition = deltaTranslation + previousPosition; 
	vec2 screenPosition = project(newPosition);



	vec3 col = vec3(0.0);
	
	
	

}