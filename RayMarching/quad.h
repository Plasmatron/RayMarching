#include "main.h"
#include "utility.h"

class Quad {
public:

    Vertex vertices[4];
    unsigned int indices[6];
	GLuint vaoID;
	GLuint vboID;		
    GLuint indexID;
    Quad();    
	~Quad();
	void Render();
	
};