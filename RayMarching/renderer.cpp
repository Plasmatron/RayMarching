	#include "renderer.h"



Renderer::Renderer(int width, int height) {	
	
	time = 0.0;
	mainCamera = new Camera();
	//oculus = new Oculus();

	//leftMesh = new ovrDistortionMesh();

	//ovrHmd_CreateDistortionMesh(oculus->hmd, ovrEye_Left, oculus->hmd->MaxEyeFov[ovrEye_Left], oculus->g_DistortionCaps, leftMesh);
	
	buffers[0] = new Framebuffer(width, height, 1, GL_RGB8, 0);
	buffers[0]->init();	
	buffers[1] = new Framebuffer(width, height, 1, GL_RGB8, 0);
	buffers[1]->init();		

	
	last = new Shader("shaders/last.vert", "shaders/last.frag");
		last->setFragData(0, "color");
		//last->setFragData(1, "depth");
		last->linkProgram();
	last->bind();		
		//last->passUniform(float(1.0), "firstPass");
		last->passTexture(0, "colorTex");		
		//last->passTexture(1, "depthTex");	

		last->passUniform(time, "time");

		last->passUniform(mainCamera->position, "position");
		last->passUniform(mainCamera->direction, "direction");
		last->passUniform(mainCamera->upVector, "upVector");
		last->passUniform(vec2(width, height), "resolution");		


	last->unbind();	

	

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_ALWAYS);
 
	base = new Shader("shaders/default.vert", "shaders/default.frag");
		 base->setAttribute(0, "point");
		 base->linkProgram();
	base->bind();
		base->passTexture(0, "finalTex");
		base->passUniform(vec2(width, height), "resolution");		
	base->unbind(); 

	quad = new Quad();	
	

	
	buffers[0]->bind();
	buffers[0]->bindAttachment(GL_COLOR_ATTACHMENT0);

	
	last->bind();
		last->passUniform(0, "frameindex");
		quad->Render();
		//last->passUniform(float(0.0), "firstPass");
	last->unbind();
	
	previous = 0;
	current = 1;

	//frameindex = 1;	
	

}

void Renderer::RenderScene() {


	frameindex++;


	time += 0.1;
	
	//ovrHmd_BeginFrame(oculus->hmd, frameindex);	
	//ovrHmd_GetEyePoses(oculus->hmd, frameindex, oculus->eyeOffsets, oculus->eyePoses, NULL);
	/*
	mat4 rot = glm::toMat4(glm::quat(oculus->eyePoses->Orientation.w, oculus->eyePoses->Orientation.x, oculus->eyePoses->Orientation.y, oculus->eyePoses->Orientation.z));
	vec3 position = vec3(oculus->eyePoses->Position.x, oculus->eyePoses->Position.y, oculus->eyePoses->Position.z) + mainCamera->position;
	mat4 pos;
	glm::translate(pos, position);
	*/
	
		mainCamera->Update();		
		buffers[current]->bind();		
		buffers[current]->bindAttachment(GL_COLOR_ATTACHMENT0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, buffers[previous]->textures[0]);
 	
		
		last->bind();

			last->passUniform(mainCamera->position, "position");
			last->passUniform(frameindex, "frameindex");

			last->passUniform(mainCamera->direction, "direction");
			last->passUniform(mainCamera->upVector, "upVector");		
			last->passUniform(mainCamera->viewMatrix, "viewMatrix");
			last->passUniform(mainCamera->projectionMatrix, "projectionMatrix");
			last->passUniform(mainCamera->inverseMatrix, "inverseMatrix");
			last->passUniform(mainCamera->previousViewMatrix, "previousViewMatrix");
			last->passUniform(mainCamera->previousPosition, "previousPosition");
			last->passUniform(mainCamera->previousDirection, "previousDirection");			

			last->passUniform(time, "time");
			
			quad->Render();

		last->unbind();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, buffers[current]->textures[0]);


	
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);	
	glViewport(0,0,width, height);
	
	base->bind();		
		quad->Render(); //flat texture screen shader 
	base->unbind();

	//if(frameindex%5 != 1)
	//	Sleep(0.1);
	
	//ovrHmd_EndFrame(oculus->hmd, oculus->eyePoses, oculus->eyeTextures);
	std::swap(previous, current);
}



