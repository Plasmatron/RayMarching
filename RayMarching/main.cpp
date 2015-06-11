#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"glfw/glfw3dll.lib")
#pragma comment(lib,"libovr.lib")



#include "main.h"

#include "renderer.h"

#define OVR_OS_WIN32




static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

int main(void)
{
    GLFWwindow* window;

	int width, height;


    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    window = glfwCreateWindow(1600, 900, "Simple example", glfwGetPrimaryMonitor(), NULL);

    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
	glewInit();
    glfwSetKeyCallback(window, key_callback);
	
	 
	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0,0, width, height);

	Renderer * renderer = new Renderer(width, height);
		 	


    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		renderer->RenderScene();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}


