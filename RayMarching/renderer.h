#include "utility.h"
#include "camera.h"
#include "quad.h"
#include "shader.h"
#include "framebuffer.h"


struct Renderer {
	float time;
	Framebuffer * buffers[2];


	Camera *mainCamera;
	Quad *quad;
	Shader *rt;
	Shader *reprojection;
	Shader *base;
	Renderer();
	//~Renderer();
	void RenderScene();

	int last;
	int current;




};