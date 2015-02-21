#include "quad.h"

Quad::Quad() {

	indices[0] = 0;	indices[1] = 1;	indices[2] = 2;	
	indices[3] = 1;	indices[4] = 3;	indices[5] = 2;	

	vertices[0].x = -1; vertices[0].y = -1; vertices[0].z =  0;
	vertices[1].x =  1; vertices[1].y = -1; vertices[1].z =  0;
	vertices[2].x = -1; vertices[2].y =  1; vertices[2].z =  0;
	vertices[3].x =  1; vertices[3].y =  1; vertices[3].z =  0;


	//glGenBuffers(1, &vaoID);
	//glBindVertexArray(vaoID);

    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*4, vertices, GL_STATIC_DRAW);

	
	//glEnableVertexAttribArray(0);    
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//glDisableVertexAttribArray(0); 

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	//glBindVertexArray(0);

	glGenBuffers(1, &indexID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*6, indices, GL_STATIC_DRAW);  
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);		

}

void Quad::Render() {

	//glBindVertexArray(vaoID);
	glEnableVertexAttribArray(0);	
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexID);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);   		
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	glDisableVertexAttribArray(0);
	//glBindVertexArray(0);

}

Quad::~Quad() {

	glDeleteBuffers(1, &vboID);
	glDeleteBuffers(1, &indexID);	

}

