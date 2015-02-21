#include "renderer.h"

Renderer::Renderer() {
	time = 0.0;
	mainCamera = new Camera();
	base = new Shader("shaders/default.vert", "shaders/cube.frag");	
		 base->setAttribute(0, "point");
		 base->linkProgram();
	base->bind();
		base->passUniform(mainCamera->viewMat, "viewMatrix");
		base->passUniform(mainCamera->projMat, "projectionMatrix");	
		base->passUniform(vec2(1600,900), "resolution");
	base->unbind(); 
	quad = new Quad();	
}
void Renderer::RenderScene() {
	time += 0.1;
	base->bind();
		base->passUniform(time, "time");

		quad->Render();
	base->unbind();


}