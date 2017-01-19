#include "Predefined.h"

#include "Mutex.h"

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

#include "DebugNew.h"

namespace Sapphire
{

#ifdef WIN32

	Mutex::Mutex() :
		handle_(new CRITICAL_SECTION)
	{
		InitializeCriticalSection((CRITICAL_SECTION*)handle_);
	}

	Mutex::~Mutex()
	{
		CRITICAL_SECTION* cs = (CRITICAL_SECTION*)handle_;
		DeleteCriticalSection(cs);
		delete cs;
		handle_ = 0;
	}

	void Mutex::Acquire()
	{
		EnterCriticalSection((CRITICAL_SECTION*)handle_);
	}

	void Mutex::Release()
	{
		LeaveCriticalSection((CRITICAL_SECTION*)handle_);
	}

#else

	Mutex::Mutex() :
		handle_(new pthread_mutex_t)
	{
		pthread_mutex_t* mutex = (pthread_mutex_t*)handle_;
		pthread_mutexattr_t attr;
		pthread_mutexattr_init(&attr);
		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
		pthread_mutex_init(mutex, &attr);
	}

	Mutex::~Mutex()
	{
		pthread_mutex_t* mutex = (pthread_mutex_t*)handle_;
		pthread_mutex_destroy(mutex);
		delete mutex;
		handle_ = 0;
	}

	void Mutex::Acquire()
	{
		pthread_mutex_lock((pthread_mutex_t*)handle_);
	}

	void Mutex::Release()
	{
		pthread_mutex_unlock((pthread_mutex_t*)handle_);
	}

#endif

	MutexLock::MutexLock(Mutex& mutex) :
		mutex_(mutex)
	{
		mutex_.Acquire();
	}

	MutexLock::~MutexLock()
	{
		mutex_.Release();
	}

}