// GLFWOculusRiftTest
// (c) cThrough 2014 (Daniel Dekkers)
// Version 2014120800 Based on DK2, OculusSDK 4.0.4

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"glfw/glfw3dll.lib")
#pragma comment(lib,"libovr.lib")



#include "main.h"

#include "renderer.h"



const bool l_MultiSampling = false;
const bool l_Spin = false;

int g_DistortionCaps = 0
   | ovrDistortionCap_Vignette
   | ovrDistortionCap_Chromatic
   | ovrDistortionCap_Overdrive
   // | ovrDistortionCap_TimeWarp // Turning this on gives ghosting???
   ;

ovrHmd g_Hmd;
ovrGLConfig g_Cfg;
ovrEyeRenderDesc g_EyeRenderDesc[2];
ovrVector3f g_EyeOffsets[2];
ovrPosef g_EyePoses[2];
ovrTexture g_EyeTextures[2];
OVR::Matrix4f g_ProjectionMatrici[2];
OVR::Sizei g_RenderTargetSize;


// ============================================================================

static void ErrorCallback(int p_Error, const char* p_Description)
{
   fputs(p_Description, stderr);
}

// ============================================================================



// ============================================================================

static void WindowSizeCallback(GLFWwindow* p_Window, int p_Width, int p_Height)
{
   if (p_Width>0 && p_Height>0)
   {
      g_Cfg.OGL.Header.BackBufferSize.w = p_Width;
      g_Cfg.OGL.Header.BackBufferSize.h = p_Height;

      ovrBool l_ConfigureResult = ovrHmd_ConfigureRendering(g_Hmd, &g_Cfg.Config, g_DistortionCaps, g_Hmd->MaxEyeFov, g_EyeRenderDesc);
      if (!l_ConfigureResult)
      {
         printf("Configure failed.\n");
         exit(EXIT_FAILURE);
      }
   }
}

// ============================================================================



int main(void)
{
   printf("[R] to recenter, [Esc] to quit, dismiss the HSW with any key (after the hidden timer runs out)...\n");

   

   // Initialize LibOVR...
   ovr_Initialize();

   // Check for attached head mounted display...
   //ovrHmd g_Hmd = ovrHmd_Create(0);

   /*
   if (!g_Hmd)
   {
      printf("No Oculus Rift device attached, using virtual version...\n");
      g_Hmd = ovrHmd_CreateDebug(ovrHmd_DK1);
   }
   */
   // Create a window...
   GLFWwindow* l_Window;

   glfwSetErrorCallback(ErrorCallback);

   if (!glfwInit()) exit(EXIT_FAILURE);   

   // Check to see if we are running in "Direct" or "Extended Desktop" mode...
   bool l_DirectMode = true;//((g_Hmd->HmdCaps & ovrHmdCap_ExtendDesktop)==0);

   GLFWmonitor* l_Monitor;
   ovrSizei l_ClientSize;

   l_Monitor = NULL;

   l_ClientSize.w = 1280/2;
   l_ClientSize.h = 800/2;

  
    // Create the window based on the parameters set above...
   l_Window = glfwCreateWindow(l_ClientSize.w, l_ClientSize.h, "GLFW Oculus Rift Test", l_Monitor, NULL);

   // Check if window creation was succesfull...
   if (!l_Window)
   {
      glfwTerminate();
      exit(EXIT_FAILURE);
   }

   // Make the context current for this window...
   glfwMakeContextCurrent(l_Window);

   // Don't forget to initialize Glew, turn glewExperimental on to avoid problem fetching function pointers...
   glewExperimental = GL_TRUE;
   GLenum l_GlewResult = glewInit();
   if (l_GlewResult!=GLEW_OK)
   {
      printf("glewInit() error.\n");
      exit(EXIT_FAILURE);
   }
   
   Renderer * renderer = new Renderer(l_Window);
   

   while (!glfwWindowShouldClose(l_Window))
   {      
	  renderer->RenderScene();
      glfwPollEvents();
   }



   // Clean up Oculus...
   ovrHmd_Destroy(renderer->oculus->hmd);
   ovr_Shutdown();

   // Clean up window...
   glfwDestroyWindow(l_Window);
   glfwTerminate();

   exit(EXIT_SUCCESS);
}