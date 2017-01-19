
#include "Predefined.h"

#include "Condition.h"

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

namespace Sapphire
{

#ifdef WIN32

	Condition::Condition() :
		event_(0)
	{
		event_ = CreateEvent(0, FALSE, FALSE, 0);
	}

	Condition::~Condition()
	{
		CloseHandle((HANDLE)event_);
		event_ = 0;
	}

	void Condition::Set()
	{
		SetEvent((HANDLE)event_);
	}

	void Condition::Wait()
	{
		WaitForSingleObject((HANDLE)event_, INFINITE);
	}

#else

	Condition::Condition() :
		mutex_(new pthread_mutex_t),
		event_(new pthread_cond_t)
	{
		pthread_mutex_init((pthread_mutex_t*)mutex_, 0);
		pthread_cond_init((pthread_cond_t*)event_, 0);
	}

	Condition::~Condition()
	{
		pthread_cond_t* cond = (pthread_cond_t*)event_;
		pthread_mutex_t* mutex = (pthread_mutex_t*)mutex_;

		pthread_cond_destroy(cond);
		pthread_mutex_destroy(mutex);
		delete cond;
		delete mutex;
		event_ = 0;
		mutex_ = 0;
	}

	void Condition::Set()
	{
		pthread_cond_signal((pthread_cond_t*)event_);
	}

	void Condition::Wait()
	{
		pthread_cond_t* cond = (pthread_cond_t*)event_;
		pthread_mutex_t* mutex = (pthread_mutex_t*)mutex_;

		pthread_mutex_lock(mutex);
		pthread_cond_wait(cond, mutex);
		pthread_mutex_unlock(mutex);
	}

#endif

}