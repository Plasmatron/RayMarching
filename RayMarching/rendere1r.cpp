#include "renderer.h"


static void KeyCallback(GLFWwindow* p_Window, int p_Key, int p_Scancode, int p_Action, int p_Mods)
{
   if (p_Action==GLFW_PRESS)
   {
      switch (p_Key)
      {
         case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(p_Window, GL_TRUE);
         break;  
		 case GLFW_KEY_W:
			 printf("w");
		 break;


      }


      // Remove HSW on every key...

   }
}


Renderer::Renderer(GLFWwindow* window) {

	oculus = new Oculus();

	glfwSetKeyCallback(window, KeyCallback);
	
	height = oculus->renderTargetSize.h;
	width = oculus->renderTargetSize.w;
	
    
	time = 0.0;
	mainCamera = new Camera();
	
	buffers[0] = new Framebuffer(width, height, 1, GL_RGB8, 1);
	buffers[0]->init();	
	buffers[1] = new Framebuffer(width, height, 1, GL_RGB8, 1);
	buffers[1]->init();	
	

	output = new Framebuffer(width, height, 1, GL_RGB8, 1);
	output->init();

	
	rt = new Shader("shaders/rt.vert", "shaders/rt.frag");
		rt->setFragData(0, "color");
		rt->setFragData(1, "coordRGB");
		rt->linkProgram();
	rt->bind();		
		rt->passUniform(1.0, "firstPass");
		rt->passTexture(0, "colorTex");		
		rt->passTexture(1, "depthTex");	

		rt->passUniform(time, "time");

		rt->passUniform(mainCamera->position, "position");
		rt->passUniform(mainCamera->direction, "direction");
		rt->passUniform(mainCamera->upVector, "upVector");
		rt->passUniform(vec2(width, height), "resolution");		
		rt->passUniform(mainCamera->zNear, "zNear");
		rt->passUniform(mainCamera->zFar, "zFar");

	rt->unbind();	

	

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

	
	rt->bind();
		quad->Render();
		rt->passUniform(0.0, "firstPass");
	rt->unbind();
	

	last = 0;
	current = 1;


	frameindex = 0;
	oculus->Configure(output->textures[0], window);

	ovrHSWDisplayState l_HasWarningState;
    ovrHmd_GetHSWDisplayState(oculus->hmd, &l_HasWarningState);
    if (l_HasWarningState.Displayed) 
		ovrHmd_DismissHSWDisplay(oculus->hmd);	

	
	glViewport(
            oculus->eyeTextures[ovrEye_Left].Header.RenderViewport.Pos.x,
            oculus->eyeTextures[ovrEye_Left].Header.RenderViewport.Pos.y,
            oculus->eyeTextures[ovrEye_Left].Header.RenderViewport.Size.w,
            oculus->eyeTextures[ovrEye_Left].Header.RenderViewport.Size.h
         );
	

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
			rt->passUniform(mainCamera->previousPosition, "previousPosition");
			rt->passUniform(mainCamera->previousDirection, "previousDirection");			

			rt->passUniform(time, "time");
			
			quad->Render();

		rt->unbind();	

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, buffers[current]->textures[0]);
	
	/*
	output->bind();
	output->bindAttachment(GL_COLOR_ATTACHMENT0);
	*/

	base->bind();		
		quad->Render(); //flat texture screen shader 
	base->unbind();
	
	//ovrHmd_EndFrame(oculus->hmd, oculus->eyePoses, oculus->eyeTextures);
	


	std::swap(last, current);
}



