#pragma once


#include "Sapphire.h"


namespace Sapphire
{

	// 线程可否等待的条件
	class SAPPHIRE_API Condition
	{
	public:
		
		Condition();

		~Condition();

		/// 设置条件
		void Set();

		/// 等待这个条件
		void Wait();

	private:
#ifndef WIN32
		/// 事件互斥量， ptheads实现必须
		void* mutex_;
#endif
		/// 操作系统的特定事件
		void* event_;
	};
}