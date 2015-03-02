#include "framebuffer.h"

Framebuffer :: Framebuffer(GLint _width, GLint _height, unsigned int _numOfColorAttachments, GLenum _format){
	width = _width;
	height = _height;
	numOfColorAttachments = _numOfColorAttachments;
	

	format = _format;
	textures = NULL;
	colorAttachments = NULL;
}



Framebuffer :: ~Framebuffer(){	

	glDeleteTextures(numOfColorAttachments, textures);

	glDeleteFramebuffers(1, &handle);

}

void Framebuffer ::init(){

	glGenFramebuffers(1, &handle);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, handle);

		textures = new GLuint [numOfColorAttachments];
		glGenTextures(numOfColorAttachments, textures);

		if(numOfColorAttachments)
			colorAttachments = new GLenum [numOfColorAttachments];

		int i = 0;
		while(i < numOfColorAttachments){

			colorAttachments[i] = GL_COLOR_ATTACHMENT0 + i;
			glBindTexture(GL_TEXTURE_2D, textures[i]);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0,  GL_RGB, GL_FLOAT, NULL);
			glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textures[i], 0);
			++i;
		}

		

	GLenum Status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);

     if (Status != GL_FRAMEBUFFER_COMPLETE) {
         //printf("FB error, status: 0x%x\n", Status);
		 MessageBox(NULL, "shit fuck", "Buffer crapped", MB_OK);
     }

	 glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

}

void Framebuffer::bind(){
	glViewport(0,0, width, height);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, handle);

}

void Framebuffer ::unbind(){

	glViewport(0,0, 640, 480);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

}

void Framebuffer :: bindAttachments(GLint size, GLenum* attachments){
	glViewport(0,0, width, height);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, handle);
	glDrawBuffers(size, attachments);

}

void Framebuffer :: bindAttachment(GLenum attachment){
	glViewport(0,0, width, height);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, handle);
	glDrawBuffer(attachment);

}


void Framebuffer::clear(GLbitfield clearMask){
	glClear(clearMask);
}
