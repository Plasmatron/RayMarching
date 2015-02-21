#include "camera.h"
#include "main.h"

Camera::Camera() {
	position = vec3(  1,0,0);
	direction = vec3( -1,0,0);
	upVec = vec3(0,1,0);
	zNear = 0.1;
	zFar = 100;
	viewMat = lookAt( direction-position, position, upVec);
	projMat = perspective<float>(55,(GLfloat)SCREEN_WIDTH/(GLfloat)SCREEN_HEIGHT, zNear, zFar);

}
