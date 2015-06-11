#ifndef SHADER_H
#define SHADER_H

#include "utility.h"
#include "math.h"

#include <gl/glew.h>

#include "textfile.h"

using namespace glm;

class Shader{
	public:

	Shader(char* vertLocation, char* fragLocation){

		char* strError = vertLocation;

		char *vs = NULL,*fs = NULL;

		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		vs = textFileRead(vertLocation);
		fs = textFileRead(fragLocation);

	
		const char * vv = vs;
		const char * ff = fs;


		glShaderSource(vertexShader,   1, &vv,NULL);
		glShaderSource(fragmentShader, 1, &ff,NULL);


		free(vs);free(fs);


		glCompileShader(vertexShader);
		glCompileShader(fragmentShader);
		GLint infoLogLength;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &infoLogLength);
		
				
		glGetShaderInfoLog(vertexShader, infoLogLength, NULL, buffer);
		glGetShaderInfoLog(fragmentShader, infoLogLength, NULL, buffer);

		programHandle = glCreateProgram();
		
		
		glAttachShader(programHandle, vertexShader);
		glAttachShader(programHandle, fragmentShader);


	}


	GLuint programHandle;
	GLuint vertexShader;
	GLuint fragmentShader;

	std :: map<char*, GLint> variables; 


	char* vertCompilationLog;
	char* fragCompilationLog;
	char* linkingLog;
	char buffer[3600];


	void passUniform(vec4 value, char* variable){

		if( !variables.count(variable) )//не нашли переменную с таким именем
			//variables[variable] = glGetUniformLocation(programHandle, variable);
			variables.insert(std::pair <char*, GLint> (variable, glGetUniformLocation(programHandle, variable) ) );  //кладем переменную с сопоставленным ей ключом
		//else
			glUniform4f(variables[variable], value.x, value.y, value.z, value.w);
	}
	void passUniform(int value, char* variable){

		if( !variables.count(variable) )//не нашли переменную с таким именем
			//variables[variable] = glGetUniformLocation(programHandle, variable);
			variables.insert(std::pair <char*, GLint> (variable, glGetUniformLocation(programHandle, variable) ) );  //кладем переменную с сопоставленным ей ключом
		//else
			glUniform1i(variables[variable], value);
	}


	void passUniform(vec3 value, char* variable){
		if(!variables.count(variable) )
			//variables[variable] = glGetUniformLocation(programHandle, variable);
			variables.insert(std::pair <char*, GLint> (variable, glGetUniformLocation(programHandle, variable) ) ); 
		//else
		glUniform3f(variables[variable], value.x, value.y, value.z);
	}

	void passUniform(vec2 value, char* variable){
		if(variables.find(variable) == variables.end() )
			//variables[variable] = glGetUniformLocation(programHandle, variable);
			variables.insert(std::pair <char*, GLint> (variable, glGetUniformLocation(programHandle, variable) ) ); 
		//else
		glUniform2f(variables[variable], value.x, value.y);
	}

	void passUniform(float value, char* variable){
		if(variables.find(variable) == variables.end() )
			//variables[variable] = glGetUniformLocation(programHandle, variable);
			variables.insert(std::pair <char*, GLint> (variable, glGetUniformLocation(programHandle, variable) ) ); 
		//else
		glUniform1f(variables[variable], value);
	}

	void passUniform(mat4 matrix, char* variable){
		if(variables.find(variable) == variables.end() )
			//variables[variable] = glGetUniformLocation(programHandle, variable);
			variables.insert(std::pair <char*, GLint> (variable, glGetUniformLocation(programHandle, variable) ) ); 
		//else
		glUniformMatrix4fv(variables[variable], 1, false, &matrix[0][0]);
	}

	void passUniform(mat3 matrix, char* variable){

		if(variables.find(variable) == variables.end() )
			//variables[variable] = glGetUniformLocation(programHandle, variable);
			variables.insert(std::pair <char*, GLint> (variable, glGetUniformLocation(programHandle, variable) ) );
		//else
		 glUniformMatrix3fv(variables[variable], 1, false, &matrix[0][0]);

	}

	void setAttribute(unsigned int location, char* variable){
		glBindAttribLocation(programHandle, location, variable);
	}

	void setFragData(unsigned int location, char* variable){
		glBindFragDataLocation(programHandle, location, variable);
	}

	void passTexture(unsigned int location, char* texture){
		glUniform1i(glGetUniformLocation(programHandle, texture), location);
	}



	void linkProgram(){

		glLinkProgram(programHandle);
		GLint infoLogLength;
		GLint param;
		glGetProgramiv(programHandle, GL_LINK_STATUS, &param);
		glGetShaderiv(programHandle, GL_INFO_LOG_LENGTH, &infoLogLength);
		param = param;

		char errorLog[256];

		if(param == 0)
		{
			glGetProgramInfoLog(programHandle, infoLogLength, NULL, errorLog);
			MessageBox(NULL, buffer, "Missing texture", MB_OK);
		}
		param=param;

	}

	void bind(){
		glUseProgram(programHandle);
	}

	void unbind(){
		glUseProgram(0);
	}




};

#endif