#ifndef _DUNE_BAKE_BUILDFILEHEADER_
#define _DUNE_BAKE_BUILDFILEHEADER_

#include <Sable\Setup.h>

#include <vadefs.h>

namespace Sb = Sable;

#if defined( SETUP_IMPLTYPE_MSW )

#include <windows.h>
#include <Dbghelp.h>
#include <stdarg.h>
#include <stdio.h>

#endif

#if defined( SETUP_IMPLTYPE_DIRECTX )

#include <d3d10.h>
#include <d3dx10.h>

#endif

#if defined( SETUP_IMPLTYPE_GLES )

#include <EGL\egl.h>
#include <GLES2\gl2.h>

#endif


#endif