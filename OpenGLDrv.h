/*=============================================================================
	OpenGLDrv.h: Unreal OpenGL support header.
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.

	Revision history:
	* Created by Tim Sweeney
	* Multitexture and context support - Andy Hanson (hanson@3dfx.com) and
	  Jack Mathews (jack@3dfx.com)
	* Unified by Daniel Vogel

=============================================================================*/

/*-----------------------------------------------------------------------------
	Includes.
-----------------------------------------------------------------------------*/

#ifdef WIN32
#include <windows.h>
#endif
// SDL2 for every build except upstream's WGL configuration: with UTGLR_ENGINE_UT99DC, NSDLDrv owns the
// window, the GL context and the buffer swap on MinGW Windows as on Linux and the web.
#if !defined(WIN32) || defined(UTGLR_ENGINE_UT99DC)
#include "SDL2/SDL.h"
#endif
#include <GL/gl.h>
#ifdef WIN32
#include "glext.h"
#include "wglext.h"
#else
#include <GL/glext.h>
#endif

// UTGLR_USE_WGL: the driver owns a WGL window, pixel format, context and gamma ramp (upstream Windows
// builds). With UTGLR_ENGINE_UT99DC every target, MinGW Windows included, uses SDL instead.
#if defined(WIN32) && !defined(UTGLR_ENGINE_UT99DC)
#define UTGLR_USE_WGL 1
#else
// SDL owns the window, the GL context, the swap and the gamma ramp.
#define UTGLR_USE_SDL 1
#endif

#if defined(UTGLR_ENGINE_UT99DC)
/*-----------------------------------------------------------------------------
	SDL 1.2 -> SDL2 compatibility shims.

	UTGLR's SDL path is written for SDL 1.2; here SDL2 and NSDLDrv own the window, the GL
	context and the buffer swap. The SDL 1.2 entry points SDL2 removed become no-ops:
	  - SDL_GL_SwapBuffers : NSDLDrv presents (SDL_GL_SwapWindow).
	  - SDL_SetGammaRamp   : no hardware gamma (SDL2 needs the window, which the device
	                         does not own).
	  - SDL_ListModes / SDL_FULLSCREEN : "DGL GetRes" reports any mode (the host owns the
	                         resolution).
-----------------------------------------------------------------------------*/
static inline void SDL_GL_SwapBuffers() {}
static inline int  SDL_SetGammaRamp( const Uint16*, const Uint16*, const Uint16* ) { return 0; }
#define SDL_FULLSCREEN 0
static inline SDL_Rect** SDL_ListModes( void*, Uint32 ) { return (SDL_Rect**)-1; }
#endif

#define UTGLR_NO_APP_MALLOC
#include <stdlib.h>

#include "Engine.h"
#include "UnRender.h"

#ifdef UTGLR_UNREAL_227_BUILD
// Hack so I don't have to move UOpenGLRenderDevice's interface into a header.  --ryan.
//#define AUTO_INITIALIZE_REGISTRANTS_OPENGLDRV UOpenGLRenderDevice::StaticClass();
extern "C" { void autoInitializeRegistrantsOpenGLDrv(void); }
// #define AUTO_INITIALIZE_REGISTRANTS_OPENGLDRV autoInitializeRegistrantsOpenGLDrv();
#endif

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/
