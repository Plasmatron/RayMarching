#include "oculus.h"

Oculus :: Oculus() {

	ovr_Initialize();

	hmd = ovrHmd_Create(0);
	
	if (!hmd)
	{
		printf("No Oculus Rift device attached, using virtual version...\n");
		hmd = ovrHmd_CreateDebug(ovrHmd_DK1);
	}

	clientSize.w = hmd->Resolution.w/2;
	clientSize.h = hmd->Resolution.h/2;


	eyeTextureSizes[ovrEye_Left] = ovrHmd_GetFovTextureSize(hmd, ovrEye_Left, hmd->MaxEyeFov[ovrEye_Left], 1.0f);
	eyeTextureSizes[ovrEye_Right] = ovrHmd_GetFovTextureSize(hmd, ovrEye_Right, hmd->MaxEyeFov[ovrEye_Right], 1.0f);

	// Combine for one texture for both eyes...
	renderTargetSize.w = eyeTextureSizes[ovrEye_Left].w + eyeTextureSizes[ovrEye_Right].w;
	renderTargetSize.h = (eyeTextureSizes[ovrEye_Left].h>eyeTextureSizes[ovrEye_Right].h ? eyeTextureSizes[ovrEye_Left].h : eyeTextureSizes[ovrEye_Right].h);

}

void Oculus::Configure(GLuint textureID, GLFWwindow *window){

	// Left eye...
	eyeTextures[ovrEye_Left].Header.API = ovrRenderAPI_OpenGL;
	eyeTextures[ovrEye_Left].Header.TextureSize = renderTargetSize;
	eyeTextures[ovrEye_Left].Header.RenderViewport.Pos.x = 0;
	eyeTextures[ovrEye_Left].Header.RenderViewport.Pos.y = 0;
	eyeTextures[ovrEye_Left].Header.RenderViewport.Size = eyeTextureSizes[ovrEye_Left];
	((ovrGLTexture&)(eyeTextures[ovrEye_Left])).OGL.TexId = textureID;

	// Right eye (mostly the same as left but with the viewport on the right side of the texture)... 
	eyeTextures[ovrEye_Right] = eyeTextures[ovrEye_Left];
	eyeTextures[ovrEye_Right].Header.RenderViewport.Pos.x = (renderTargetSize.w+1)/2;
	eyeTextures[ovrEye_Right].Header.RenderViewport.Pos.y = 0;
   
	// Oculus Rift eye configurations... 
	cfg.OGL.Header.API = ovrRenderAPI_OpenGL;
	cfg.OGL.Header.BackBufferSize.w = clientSize.w;
	cfg.OGL.Header.BackBufferSize.h = clientSize.h;
	cfg.OGL.Window = glfwGetWin32Window(window);
	cfg.OGL.DC = GetDC(cfg.OGL.Window);

	g_DistortionCaps = 0 ;//| ovrDistortionCap_Vignette | ovrDistortionCap_Chromatic | ovrDistortionCap_Overdrive;

	// Enable capabilities... 
	ovrHmd_SetEnabledCaps(hmd, ovrHmdCap_LowPersistence | ovrHmdCap_DynamicPrediction);

	ovrBool l_ConfigureResult = ovrHmd_ConfigureRendering(hmd, &cfg.Config, g_DistortionCaps, hmd->MaxEyeFov, eyeRenderDesc);
	if (!l_ConfigureResult)
	{
		printf("Configure failed.\n");
		exit(EXIT_FAILURE);
	}

	// Start the sensor which provides the Rift’s pose and motion...
	uint32_t l_SupportedSensorCaps = ovrTrackingCap_Orientation | ovrTrackingCap_MagYawCorrection | ovrTrackingCap_Position;
	uint32_t l_RequiredTrackingCaps = 0;
	ovrBool l_TrackingResult = ovrHmd_ConfigureTracking(hmd, l_SupportedSensorCaps, l_RequiredTrackingCaps);
	if (!l_TrackingResult)
	{
		printf("Could not start tracking...");
		exit(EXIT_FAILURE);
	}
	
	

	// Projection matrici for each eye will not change at runtime, we can set them here...
	projection[ovrEye_Left] = ovrMatrix4f_Projection(eyeRenderDesc[ovrEye_Left].Fov, 0.3f, 100.0f, true);
	projection[ovrEye_Right] = ovrMatrix4f_Projection(eyeRenderDesc[ovrEye_Right].Fov, 0.3f, 100.0f, true);

	// IPD offset values will not change at runtime, we can set them here... 
	eyeOffsets[ovrEye_Left] = eyeRenderDesc[ovrEye_Left].HmdToEyeViewOffset;
	eyeOffsets[ovrEye_Right] = eyeRenderDesc[ovrEye_Right].HmdToEyeViewOffset;

	ovrHmd_AttachToWindow(hmd, glfwGetWin32Window(window), NULL, NULL);

	// Do a single recenter to calibrate orientation to current state of the Rift...
	ovrHmd_RecenterPose(hmd);


}