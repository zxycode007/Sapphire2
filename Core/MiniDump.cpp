#include "Predefined.h"

#if defined(_MSC_VER) && defined(SAPPHIRE_MINIDUMPS)
#include "ProcessUtil.h"

#include <cstdio>
#include <io.h>
#include <fcntl.h>
#include <time.h>
#include <windows.h>
#include <dbghelp.h>
#include <SDL/include/SDL.h>

namespace Sapphire
{

	static bool miniDumpWritten = false;

	SAPPHIRE_API int WriteMiniDump(const char* applicationName, void* exceptionPointers)
	{
		// In case of recursive or repeating exceptions, only write the dump once
		/// \todo This function should not allocate any dynamic memory
		if (miniDumpWritten)
			return EXCEPTION_EXECUTE_HANDLER;

		miniDumpWritten = true;

		MINIDUMP_EXCEPTION_INFORMATION info;
		info.ThreadId = GetCurrentThreadId();
		info.ExceptionPointers = (EXCEPTION_POINTERS*)exceptionPointers;
		info.ClientPointers = TRUE;

		static time_t sysTime;
		time(&sysTime);
		const char* dateTime = ctime(&sysTime);
		String dateTimeStr = String(dateTime);
		dateTimeStr.Replace("\n", "");
		dateTimeStr.Replace(":", "");
		dateTimeStr.Replace("/", "");
		dateTimeStr.Replace(' ', '_');

		char* pathName = SDL_GetPrefPath("Sapphire", "crashdumps");
		String miniDumpDir(pathName);
		String miniDumpName = miniDumpDir + String(applicationName) + "_" + dateTimeStr + ".dmp";
		if (pathName)
			SDL_free(pathName);

		CreateDirectoryW(WString(miniDumpDir).CString(), 0);
		HANDLE file = CreateFileW(WString(miniDumpName).CString(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ,
			0, CREATE_ALWAYS, 0, 0);

		BOOL success = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), file, MiniDumpWithDataSegs, &info, 0, 0);
		CloseHandle(file);

		if (success)
			ErrorDialog(applicationName, "An unexpected error occurred. A minidump was generated to " + miniDumpName);
		else
			ErrorDialog(applicationName, "An unexpected error occurred. Could not write minidump.");

		return EXCEPTION_EXECUTE_HANDLER;
	}

}

#endif