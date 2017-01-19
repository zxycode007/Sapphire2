#pragma once


#include "Sapphire.h"


#ifndef WIN32
#include <pthread.h>
typedef pthread_t ThreadID;
#else
typedef unsigned ThreadID;
#endif

namespace Sapphire
{
	/// Operating system thread.
	class SAPPHIRE_API Thread
	{
	public:
		/// Construct. Does not start the thread yet.
		Thread();
		/// Destruct. If running, stop and wait for thread to finish.
		virtual ~Thread();

		/// The function to run in the thread.
		virtual void ThreadFunction() = 0;

		/// Start running the thread. Return true if successful, or false if already running or if can not create the thread.
		bool Run();
		/// Set the running flag to false and wait for the thread to finish.
		void Stop();
		/// Set thread priority. The thread must have been started first.
		void SetPriority(int priority);

		/// Return whether thread exists.
		bool IsStarted() const { return handle_ != 0; }

		/// Set the current thread as the main thread.
		static void SetMainThread();
		/// Return the current thread's ID.
		static ThreadID GetCurrentThreadID();
		/// Return whether is executing in the main thread.
		static bool IsMainThread();

	protected:
		/// Thread handle.
		void* handle_;
		/// Running flag.
		volatile bool shouldRun_;

		/// Main thread's thread ID.
		static ThreadID mainThreadID;
	};
}

