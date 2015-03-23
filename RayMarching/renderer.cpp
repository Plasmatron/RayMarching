#include "renderer.h"

Renderer::Renderer() {
	time = 0.0;
	mainCamera = new Camera();
	buffers[0] = new Framebuffer(g_rRect.right, g_rRect.bottom, 1, GL_RGB8, 1);
	buffers[1] = new Framebuffer(g_rRect.right, g_rRect.bottom, 1, GL_RGB8, 1);
	buffers[0]->init();
	buffers[1]->init();




	rt = new Shader("shaders/rt.vert", "shaders/rt.frag");
		rt->setFragData(0, "color");
		rt->setFragData(1, "depthRGB");
		rt->linkProgram();
	rt->bind();		
		rt->passUniform(1.0, "firstPass");
		rt->passTexture(0, "colorTex");
		rt->passTexture(1, "depthTex");		
		rt->passUniform(time, "time");

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

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_ALWAYS);
 
	base = new Shader("shaders/default.vert", "shaders/default.frag");	
		 base->setAttribute(0, "point");
		 base->linkProgram();
	base->bind();
		base->passTexture(0, "finalTex");
		base->passUniform(vec2(g_rRect.right, g_rRect.bottom), "resolution");		
	base->unbind(); 
	quad = new Quad();	




	//GLenum bufs []= {GL_COLOR_ATTACHMENT0 };
	buffers[0]->bind();
	buffers[0]->bindAttachment(GL_COLOR_ATTACHMENT0);

	rt->bind();
		quad->Render();
		rt->passUniform(0.0, "firstPass");
	rt->unbind();


	last = 0;
	current = 1;

}


/*
float sdBox( vec3 p, vec3 b )
{
  vec3 d = abs(p) - b;
  float t1 = max(d.y,d.z);
  float t2 = max(d.x, t1);
  return min(t2, 0.0f) + length(max(d, vec3(0.0f) ));
}



float menger( vec3 p ) {

	float d = sdBox(p,vec3(1.0));
	float s = 1.0;
	for( int m=0; m<4; m++ )
	{
		vec3 a = mod( p*s, 2.0 )-vec3(1.0);
		s *= 3.0;
		vec3 r = abs(vec3(1.0f, 1.0f, 1.0f) - abs(a)*3.0f);

		float da = max(r.x,r.y);
		float db = max(r.y,r.z);
		float dc = max(r.z,r.x);
		float c = (min(da,min(db,dc))-1.0)/s;

		d = max(d,c);
	}
	return d;

}


float distanceEstimation(vec3 p) {
	vec3 c = vec3(2.7, 2.7, 2.7);// 2);
	vec3 q = mod(p,c)-0.5f*c;
	return menger(q);

}


*/


void Renderer::RenderScene() {
	time += 0.1;
	
	
		mainCamera->Update();		
		buffers[current]->bind();
		//GLenum bufs []= {GL_COLOR_ATTACHMENT0};
		buffers[current]->bindAttachment(GL_COLOR_ATTACHMENT0);
				
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
			rt->passUniform(mainCamera->previousDirection, "previousDirection");
			rt->passUniform(last, "face");
			rt->passUniform(time, "time");
			quad->Render();

		rt->unbind();
		


	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);	
	glViewport(0,0,g_rRect.right, g_rRect.bottom);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, buffers[current]->textures[0]);

	base->bind();		
		quad->Render(); //flat texture screen shader 
	base->unbind();

	std::swap(last, current);

}



