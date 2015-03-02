#ifndef COMPUTE_SHADER_H
#define COMPUTE_SHADER_H

#include "utility.h"
#include "math.h"

#include <gl/glew.h>

#include "textfile.h"

#define LOG_MAX 2000

class ComputeShader {
public:
	char* computeShaderSource;

	GLuint programHandle;
	GLuint computeShader;
	char buffer[3600];

	ComputeShader(char* location) {


		char *s = NULL;
		s = textFileRead(location);
		const char * ss = s;		

		computeShader = glCreateShader(GL_COMPUTE_SHADER);
		glShaderSource(computeShader,1, &ss, NULL);		
		programHandle = glCreateProgram();		
		
		GLint infoLogLength;
		glGetShaderiv(computeShader, GL_INFO_LOG_LENGTH, &infoLogLength);
		glGetShaderInfoLog(computeShader, infoLogLength, NULL, buffer);
		glAttachShader(programHandle, computeShader);		
		free(s);
	}

	void linkProgram(){
		glLinkProgram(programHandle);
		GLint infoLogLength;
		GLint param;
		glGetProgramiv(programHandle, GL_LINK_STATUS, &param);						

		if(!param)
		{
			char* log;
			glGetProgramInfoLog(programHandle, LOG_MAX, NULL, log);
			MessageBox(NULL, log, "Missing texture", MB_OK);
		}		
	}

	void bind(){
		glUseProgram(programHandle);
	}

	void unbind(){
		glUseProgram(0);
	}



};
#endif