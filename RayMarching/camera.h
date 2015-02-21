#include "utility.h"

using namespace glm;

struct Camera{

	vec3 position;
	vec3 direction;
	vec3 upVec;
	mat4 viewMat;
	mat4 projMat;
	float zNear;
	float zFar;
	Camera();
    //~Camera();




};