#ifndef UTILITY_H
#define UTILITY_H

#include <windows.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <queue>
#include <stack>
#include <deque>
#include <string>
#include <map>
#include <algorithm>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtx/quaternion.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtx/transform2.hpp>


struct Vertex {
	float x;
	float y;
	float z;

};



#define coin if((float)rand()/RAND_MAX < 0.5) 				

#endif