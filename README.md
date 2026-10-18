# UTGLR OpenGLDrv

UTGLR is Chris Dohnal's enhanced OpenGL render device for Unreal Tournament v436 (with build
configurations for Deus Ex, Rune and Unreal 227), distributed as source from
https://www.cwdohnal.com/utglr/. It is built on Epic Games' and Loki Software's OpenGLDrv by Daniel
Vogel, with the contributors `OpenGL.cpp` names: John Fulmer, Jeroen Janssen, Fredrik Gustafsson,
Steve Sinclair, Jason Dick, NitroGL (TruForm), Leonhard Gruenschloss (TruForm, Deus Ex) and
Smirftsch / OldUnreal.

This repository, [10-p/utglr-copy](https://github.com/10-p/utglr-copy), holds the author's source
drop unmodified as its first commit and our changes on top. The drop carries no licence file: its
sources carry only Epic Games copyright headers ("Copyright 1997-1999 Epic Games, Inc. All Rights
Reserved." and "Portions copyright 1999 Epic Games, Inc."), and the bundled Khronos `glext.h` and
`wglext.h` carry the SGI Free Software License B 1.1.

## Branches

| Branch | Engine | Built by |
|---|---|---|
| `main` | Unreal Tournament v400 | engine-ut99, engine-ut99-436-compat |
| `hp2` | Harry Potter and the Chamber of Secrets (UE1 build 433) | engine-hp2, engine-hp2-retail |

The engines fetch a branch by name. `UTGLR_ENGINE_UT99DC` selects the SDL2 platform layer,
`UTGLR_USE_GL4ES` the gl4es back end and `UTGLR_ENGINE_HP2` the HP2 interface; a build that defines
none of them compiles the author's code paths.

Both branches add:

* **UE1 v400 and SDL2 platform**: NSDLDrv owns the window, the GL context and the buffer swap; the
  driver uses SDL2 with no-op shims for the SDL 1.2 calls, v400's 3-argument `DECLARE_CLASS` and
  `FLOAT` frame timestamps, and reads `UseS3TC` from the ini with a default of on.
* **gl4es back end**: with `UTGLR_USE_GL4ES` every GL entry point resolves through gl4es (desktop GL
  on GLES 2 / WebGL), which `Init` configures and initialises; on a threaded web build gl4es uses
  Emscripten's entry-point resolver.
* **Web rendering fixes**: 2D overlay tiles draw without the depth test, zone fog maps are enabled,
  and without the combine extensions the fog pass falls back to `GL_MODULATE`.
* **MinGW Windows target**: `UTGLR_USE_WGL` / `UTGLR_USE_SDL` choose between the driver's own WGL
  window and SDL, and the inline assembly and SEH blocks compile only with MSVC.
* **Multisampled back buffer on the web**: `UseAA` asks the engine for a back buffer with `NumAASamples`
  samples (`ufront_backbuffer_samples`, WebGL 2 only).
* **LP64 pointer arithmetic**: pointer alignment and the bind-map node offset go through `UPTRINT`.

`hp2` also adds:

* **Engine-type macros**: `UTGLR_ENGINE_V400` (UT99 v400 only) is split from the platform switch, so
  HP2 433 uses v436's `DECLARE_CLASS` and `FTime`; `UTGLR_HAS_ALPHABLEND` enables the Rune / 227
  straight-alpha path, and `PF_AlphaBlend` is a driver-internal poly flag on HP2.
* **HP2 433 render-device interface**: the 5-argument `DrawComplexSurface` (Lumos surfaces at their
  opacity), `MaxVertices()`, `DrawTriangles` for indexed lists, fans and plain lists through the
  buffered Gouraud path, HP2's alpha-palette tiles and `LINE_PreTransformed` lines.
* **Straight alpha and vertex alpha**: fading actors (`PF_Highlighted|PF_Translucent`) blend with the
  vertex alpha from `Light.W`, without `PF_Occlude` or an additive fog pass.
* **Lumos opacity in the fixed-function path**: the base pass carries the surface opacity, later
  texture units pass it through, the fog-map pass fades with it, and the ARB fragment programs are
  off.
