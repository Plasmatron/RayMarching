#include "utility.h"
#include "camera.h"
#include "quad.h"
#include "shader.h"
#include "framebuffer.h"
#include "oculus.h"


struct Renderer {
	float time;
	Framebuffer * buffers[2];
	Framebuffer * output;
	Camera *mainCamera;
	Quad *quad;
	
	Shader *last;
	Shader *base;
	Oculus * oculus;

	ovrDistortionMesh * leftMesh;
	ovrDistortionMesh * rightMesh;

	int previous, current;
	int width, height;

	int frameindex;
	
	Renderer(int width, int height);	
	void RenderScene();	
	
};