#version 130

attribute vec3 point;

void main(void){
	gl_Position =  vec4(point, 1.0) ;
}
