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
// engine-ut99 (ufront 2.34): SDL2 is needed on EVERY platform this fork builds for, Windows included —
// NSDLDrv owns the window, the GL context and the buffer swap there exactly as it does on Linux and the
// web, so the SDL2 shims below compile in on MinGW too. Upstream UTGLR (MSVC, its own WGL window) keeps
// the original "Windows means no SDL" arrangement.
#if !defined(WIN32) || defined(UTGLR_ENGINE_UT99DC)
#include "SDL2/SDL.h"	// engine-ut99: SDL2 (was SDL 1.2 <SDL/SDL.h>); NSDLDrv owns the window/context
#endif
#include <GL/gl.h>
#ifdef WIN32
#include "glext.h"
#include "wglext.h"
#else
#include <GL/glext.h>
#endif

// ufront 2.34 — "this build owns a WGL context" is NOT the same thing as "this build targets Windows".
// Every engine-ut99 target drives UTGLR through NSDLDrv/SDL2, so the pixel-format / HGLRC / gamma-ramp
// paths stay out of the MinGW build as well; SDL creates the one context and makes it current. (Same
// fix and same reasoning as engine-ut2004's UT_OPENGLDRV_WGL in ufront 2.33.)
#if defined(WIN32) && !defined(UTGLR_ENGINE_UT99DC)
#define UTGLR_USE_WGL 1
#else
// SDL owns the window, the GL context, the swap and the gamma ramp. This is what these sources used
// to spell `__UNIX__` — a name that stopped being true when this fork gained a Windows SDL target.
#define UTGLR_USE_SDL 1
#endif

#if defined(UTGLR_ENGINE_UT99DC)
/*-----------------------------------------------------------------------------
	SDL 1.2 -> SDL2 compatibility shims (engine-ut99, gated).

	UTGLR's __UNIX__ path was written for SDL 1.2; engine-ut99 uses SDL2 and NSDLDrv owns
	the window, GL context, and buffer swap. These map the SDL 1.2 entry points removed in
	SDL2 to no-ops / neutral results:
	  - SDL_GL_SwapBuffers : presentation is done by NSDLDrv (SDL_GL_SwapWindow), not here.
	  - SDL_SetGammaRamp   : hardware gamma not applied (SDL2 needs the window handle, which
	                         the device doesn't own); harmless for rendering. Future work.
	  - SDL_ListModes / SDL_FULLSCREEN : the "DGL GetRes" enumeration returns "any" (NSDLDrv
	                         / the host owns resolution).
	Gated by UTGLR_ENGINE_UT99DC so upstream UTGLR / the future v200 port stay clean.
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
