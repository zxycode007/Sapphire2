
#include "Predefined.h"

#include "Thread.h"

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

#include "DebugNew.h"

namespace Sapphire
{

#ifdef SAPPHIRE_THREADING
#ifdef WIN32

	DWORD WINAPI ThreadFunctionStatic(void* data)
	{
		Thread* thread = static_cast<Thread*>(data);
		thread->ThreadFunction();
		return 0;
	}

#else

	void* ThreadFunctionStatic(void* data)
	{
		Thread* thread = static_cast<Thread*>(data);
		thread->ThreadFunction();
		pthread_exit((void*)0);
		return 0;
	}

#endif
#endif

	ThreadID Thread::mainThreadID;

	Thread::Thread() :
		handle_(0),
		shouldRun_(false)
	{
	}

	Thread::~Thread()
	{
		Stop();
	}

	bool Thread::Run()
	{
#ifdef SAPPHIRE_THREADING
		// Check if already running
		if (handle_)
			return false;

		shouldRun_ = true;
#ifdef WIN32
		handle_ = CreateThread(0, 0, ThreadFunctionStatic, this, 0, 0);
#else
		handle_ = new pthread_t;
		pthread_attr_t type;
		pthread_attr_init(&type);
		pthread_attr_setdetachstate(&type, PTHREAD_CREATE_JOINABLE);
		pthread_create((pthread_t*)handle_, &type, ThreadFunctionStatic, this);
#endif
		return handle_ != 0;
#else
		return false;
#endif
	}

	void Thread::Stop()
	{
#ifdef SAPPHIRE_THREADING
		// Check if already stopped
		if (!handle_)
			return;

		shouldRun_ = false;
#ifdef WIN32
		WaitForSingleObject((HANDLE)handle_, INFINITE);
		CloseHandle((HANDLE)handle_);
#else
		pthread_t* thread = (pthread_t*)handle_;
		if (thread)
			pthread_join(*thread, 0);
		delete thread;
#endif
		handle_ = 0;
#endif
	}

	void Thread::SetPriority(int priority)
	{
#ifdef SAPPHIRE_THREADING
#ifdef WIN32
		if (handle_)
			SetThreadPriority((HANDLE)handle_, priority);
#endif
#if defined(__linux__) && !defined(ANDROID) && !defined(__EMSCRIPTEN__)
		pthread_t* thread = (pthread_t*)handle_;
		if (thread)
			pthread_setschedprio(*thread, priority);
#endif
#endif
	}

	void Thread::SetMainThread()
	{
		mainThreadID = GetCurrentThreadID();
	}

	ThreadID Thread::GetCurrentThreadID()
	{
#ifdef WIN32
		return GetCurrentThreadId();
#else
		return pthread_self();
#endif
	}

	bool Thread::IsMainThread()
	{
#ifdef SAPPHIRE_THREADING
		return GetCurrentThreadID() == mainThreadID;
#else
		return true;
#endif
	}

}