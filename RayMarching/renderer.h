#include "utility.h"
#include "camera.h"
#include "quad.h"
#include "shader.h"


struct Renderer {
	float time;
	Camera *mainCamera;
	Quad *quad;
	Shader *base;
	Renderer();
	//~Renderer();
	void RenderScene();

};