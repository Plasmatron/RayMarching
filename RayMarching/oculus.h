#ifndef OCULUS_H
#define OCULUS_H

#include "main.h"

class Oculus{
public:
	ovrHmd hmd;
	ovrGLConfig cfg;
	ovrSizei eyeTextureSizes[2];
	OVR::Sizei renderTargetSize;
	ovrMatrix4f projection[2]; 
	ovrEyeRenderDesc eyeRenderDesc[2];
	ovrVector3f eyeOffsets[2];
	ovrPosef eyePoses[2];
	ovrTexture eyeTextures[2];
	ovrSizei clientSize;
	int g_DistortionCaps;
	Oculus();
	void Configure(GLuint textureID, GLFWwindow *window);
};


#endif