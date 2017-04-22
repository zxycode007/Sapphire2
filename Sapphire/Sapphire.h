#ifndef _SAPPHIRE_3D_API_
#define _SAPPHIRE_3D_API_

#pragma warning(disable: 4251)
#pragma warning(disable: 4275)


#define SAPPHIRE_STATIC_DEFINE

#ifdef SAPPHIRE_STATIC_DEFINE
#  define SAPPHIRE_API
#  define SAPPHIRE_NO_EXPORT
#else
#  ifndef SAPPHIRE_API
#    ifdef SAPPHIRE_EXPORTS
/* We are building this library */
#      define SAPPHIRE_API __declspec(dllexport)
#    else
/* We are using this library */
#      define SAPPHIRE_API __declspec(dllimport)
#    endif
#  endif

#  ifndef SAPPHIRE_NO_EXPORT
#    define SAPPHIRE_NO_EXPORT 
#  endif
#endif

#ifndef SAPPHIRE_DEPRECATED
#  define SAPPHIRE_DEPRECATED __declspec(deprecated)
#endif

#ifndef SAPPHIRE_DEPRECATED_EXPORT
#  define SAPPHIRE_DEPRECATED_EXPORT SAPPHIRE_API SAPPHIRE_DEPRECATED
#endif

#ifndef SAPPHIRE_DEPRECATED_NO_EXPORT
#  define SAPPHIRE_DEPRECATED_NO_EXPORT SAPPHIRE_NO_EXPORT SAPPHIRE_DEPRECATED
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define SAPPHIRE_NO_DEPRECATED
#endif

#define NONSCRIPTABLE
#define SAPPHIRE_OPENGL_DRV
#define SAPPHIRE_OPENGL_VERSION_MAJOR 4
#define SAPPHIRE_OPENGL_VERSION_MIRROR 5


#define SAPPHIRE_MAX_TEXTURE_UNIT    8

#include <Windows.h>

#endif