#version 130

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;


attribute vec3 point;

void main(void){
	gl_Position =  vec4(point, 1.0) ;
}
