#include "renderer.h"

Renderer::Renderer() {
	time = 0.0;
	mainCamera = new Camera();
	buffers[0] = new Framebuffer(g_rRect.right, g_rRect.bottom, 2, GL_RGB8);
	buffers[1] = new Framebuffer(g_rRect.right, g_rRect.bottom, 2, GL_RGB8);
	buffers[0]->init();
	buffers[1]->init();




	rt = new Shader("shaders/rt.vert", "shaders/rt.frag");
		rt->setFragData(0, "color");
		rt->setFragData(1, "depth");
		rt->linkProgram();
	rt->bind();		
		rt->passUniform(1.0, "firstPass");
		rt->passTexture(0, "colorTex");
		rt->passTexture(1, "depthTex");
		rt->passTexture(2, "reprojectionCoords");

		rt->passUniform(mainCamera->position, "position");
		rt->passUniform(mainCamera->direction, "direction");
		rt->passUniform(mainCamera->upVector, "upVector");
		rt->passUniform(vec2(g_rRect.right, g_rRect.bottom), "resolution");		
	rt->unbind();


	/*

	reprojection = new Shader("shaders/reprojection.vert", "shaders/reprojection.frag");
		reprojection->setFragData(2, "reprojectionCoords");
		reprojection->linkProgram();
	reprojection->bind();
		reprojection->passUniform(vec2(g_rRect.right, g_rRect.bottom), "resolution");		
	reprojection->unbind();

	*/



 
	base = new Shader("shaders/default.vert", "shaders/default.frag");	
		 base->setAttribute(0, "point");
		 base->linkProgram();
	base->bind();
		base->passTexture(0, "finalTex");
		base->passUniform(vec2(g_rRect.right, g_rRect.bottom), "resolution");		
	base->unbind(); 
	quad = new Quad();	




	GLenum bufs []= {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	buffers[0]->bind();
	buffers[0]->bindAttachments(2, bufs);

	rt->bind();
		quad->Render();
		rt->passUniform(0.0, "firstPass");
	rt->unbind();


	last = 0;
	current = 1;

}





void Renderer::RenderScene() {

	

		mainCamera->Update();
		buffers[current]->bind();
		GLenum bufs []= {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		buffers[current]->bindAttachments(2, bufs);
				
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, buffers[last]->textures[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, buffers[last]->textures[1]);

		rt->bind();
			rt->passUniform(mainCamera->position, "position");
			rt->passUniform(mainCamera->direction, "direction");
			rt->passUniform(mainCamera->upVector, "upVector");
			rt->passUniform(mainCamera->viewMatrix, "viewMatrix");
			rt->passUniform(mainCamera->projectionMatrix, "projectionMatrix");
			rt->passUniform(mainCamera->inverseMatrix, "inverseMatrix");
			rt->passUniform(mainCamera->previousViewMatrix, "previousViewMatrix");



			quad->Render();
		rt->unbind();


		
		

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);	
	glViewport(0,0,g_rRect.right, g_rRect.bottom);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, buffers[last]->textures[0]);

	base->bind();		
		quad->Render(); //flat texture screen shader 
	base->unbind();

	std::swap(last, current);

}



