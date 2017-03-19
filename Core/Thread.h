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
	/// 操作系统线程封装
	class SAPPHIRE_API Thread
	{
	public:
		/// 构造函数不会自动开始线程
		Thread();
		/// 析构，如果运行状态，停止并等待线程完成
		virtual ~Thread();

		/// 这个线程要运行的函数
		virtual void ThreadFunction() = 0;

		/// 开始这个线程。如果成功返回true，如果失败可能已经运行或者没能创建这个线程
		bool Run();
		/// 设置标志位false，并等待线程完成
		void Stop();
		/// 设置线程的优先级
		void SetPriority(int priority);

		/// 返回这个线程释放存在
		bool IsStarted() const { return handle_ != 0; }

		/// 设置当前线程作为主线程
		static void SetMainThread();
		/// 返回当前线程ID
		static ThreadID GetCurrentThreadID();
		/// 返回释放在主线程中执行
		static bool IsMainThread();

	protected:
		/// 线程句柄
		void* handle_;
		/// 运行标志
		volatile bool shouldRun_;

		/// 主线程的线程ID
		static ThreadID mainThreadID;
	};
}

