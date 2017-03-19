#pragma once


#include "ProcessUtil.h"

#if defined(WIN32) && !defined(SAPPHIRE_WIN32_CONSOLE)
#include "../Core/MiniDump.h"
#include <windows.h>
#ifdef _MSC_VER
#include <crtdbg.h>
#endif
#endif

//定义一个平台特定的主函数

// MSVC debug模式：使用内存泄露报告
#if defined(_MSC_VER) && defined(_DEBUG) && !defined(SAPPHIRE_WIN32_CONSOLE)
//WINMAIN函数定义
#define SAPPHIRE_DEFINE_MAIN(function) \
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) \
{ \
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); \
    Sapphire::ParseArguments(GetCommandLineW()); \
    return function; \
}

// MSVC release 模式：在crash时写入minidump
#elif defined(_MSC_VER) && defined(SAPPHIRE_MINIDUMPS) && !defined(SAPPHIRE_WIN32_CONSOLE)
#define SAPPHIRE_DEFINE_MAIN(function) \
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) \
{ \
    Sapphire::ParseArguments(GetCommandLineW()); \
    int exitCode; \
    __try \
	    { \
        exitCode = function; \
	    } \
    __except(Sapphire::WriteMiniDump("SAPPHIRE", GetExceptionInformation())) \
	    { \
	    } \
    return exitCode; \
}

//另外的WIN32或者关闭minidump
#elif defined(WIN32) && !defined(SAPPHIRE_WIN32_CONSOLE)
#define SAPPHIRE_DEFINE_MAIN(function) \
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) \
{ \
    Sapphire::ParseArguments(GetCommandLineW()); \
    return function; \
}
// Android 或 iOS: 使用SDL
#elif defined(ANDROID) || defined(IOS)
#define SAPPHIRE_DEFINE_MAIN(function) \
extern "C" int SDL_main(int argc, char** argv); \
int SDL_main(int argc, char** argv) \
{ \
    SAPPHIRE::ParseArguments(argc, argv); \
    return function; \
}
// 其它
#else
#define SAPPHIRE_DEFINE_MAIN(function) \
int main(int argc, char** argv) \
{ \
    SAPPHIRE::ParseArguments(argc, argv); \
    return function; \
}
#endif