#pragma once

#include "Sapphire.h"


namespace Sapphire
{
	/// 互斥体
	class SAPPHIRE_API Mutex
	{
	public:
		Mutex();
		~Mutex();

		//  得到互斥体，如果已经被得到，则阻塞
		void Acquire();
		/// 释放互斥体
		void Release();

	private:
		/// 互斥体句柄
		void* handle_;
	};

	// 自动获得和释放互斥体的锁
	class SAPPHIRE_API MutexLock
	{
	public:
		/// 构造并获得互斥体
		MutexLock(Mutex& mutex);
		/// 析构并释放互斥体
		~MutexLock();

	private:
		/// 阻止拷贝构造函数
		MutexLock(const MutexLock& rhs);
		/// 阻止赋值
		MutexLock& operator =(const MutexLock& rhs);

		/// 互斥体引用
		Mutex& mutex_;
	};
}