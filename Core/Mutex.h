#pragma once

#include "Sapphire.h"


namespace Sapphire
{
	/// Operating system mutual exclusion primitive.
	class SAPPHIRE_API Mutex
	{
	public:
		/// Construct.
		Mutex();
		/// Destruct.
		~Mutex();

		/// Acquire the mutex. Block if already acquired.
		void Acquire();
		/// Release the mutex.
		void Release();

	private:
		/// Mutex handle.
		void* handle_;
	};

	/// Lock that automatically acquires and releases a mutex.
	class SAPPHIRE_API MutexLock
	{
	public:
		/// Construct and acquire the mutex.
		MutexLock(Mutex& mutex);
		/// Destruct. Release the mutex.
		~MutexLock();

	private:
		/// Prevent copy construction.
		MutexLock(const MutexLock& rhs);
		/// Prevent assignment.
		MutexLock& operator =(const MutexLock& rhs);

		/// Mutex reference.
		Mutex& mutex_;
	};
}