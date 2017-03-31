#pragma once

#include "List.h"
#include "../Mutex.h"
#include "../Object.h"
#include "../Thread.h"
#include "../Timer.h"


namespace Sapphire
{

	class FileSystem;

	// 观察一个目录和他的子目录的文件是否被修改
	class SAPPHIRE_API FileWatcher : public Object, public Thread
	{
		SAPPHIRE_OBJECT(FileWatcher, Object);

	public:
		 
		FileWatcher(Context* context);
		 
		virtual ~FileWatcher();

		//  目录观察循环
		virtual void ThreadFunction();

		// 开始观察一个目录。 如果成功返回true
		bool StartWatching(const String& pathName, bool watchSubDirs);
		/// 结束目录观察
		void StopWatching();
		//设置在文件改变通知前的延迟时间。这可以避免当一个文件仍然处于保存进度中，默认1秒
		void SetDelay(float interval);
		//  添加一个文件到改变序列
		void AddChange(const String& fileName);
		// 返回一个文件的改变（如果未找到，返回false)
		bool GetNextChange(String& dest);

		/// 返回被观察的目录
		const String& GetPath() const { return path_; }

		/// 返回文件改变后延迟通知的毫秒数
		float GetDelay() const { return delay_; }

	private:
		/// 文件系统
		SharedPtr<FileSystem> fileSystem_;
		/// 观察路径
		String path_;
		/// 等待改变，当他们的timer超时，会从列表中返回和移除
		HashMap<String, Timer> changes_;
		/// 缓冲区改变的互斥量
		Mutex changesMutex_;
		/// 改变后通知的延迟秒数
		float delay_;
		/// 是否观察子目录
		bool watchSubDirs_;

#ifdef WIN32

		/// 观察的目录句柄
		void* dirHandle_;

#elif __linux__

		/// 目录和子目录的句柄 
		HashMap<int, String> dirHandle_;
		/// linux需要的句柄
		int watchHandle_;

#elif defined(__APPLE__) && !defined(IOS)

		/// 是否允许允许OS单独文件观察标志
		bool supported_;
		///指向MacFileWatcher内部代理的指针
		void* watcher_;

#endif
	};
}