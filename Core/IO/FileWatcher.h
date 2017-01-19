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

		/// Return the path being watched, or empty if not watching.
		const String& GetPath() const { return path_; }

		/// Return the delay in seconds for notifying file changes.
		float GetDelay() const { return delay_; }

	private:
		/// Filesystem.
		SharedPtr<FileSystem> fileSystem_;
		/// The path being watched.
		String path_;
		/// Pending changes. These will be returned and removed from the list when their timer has exceeded the delay.
		HashMap<String, Timer> changes_;
		/// Mutex for the change buffer.
		Mutex changesMutex_;
		/// Delay in seconds for notifying changes.
		float delay_;
		/// Watch subdirectories flag.
		bool watchSubDirs_;

#ifdef WIN32

		/// Directory handle for the path being watched.
		void* dirHandle_;

#elif __linux__

		/// HashMap for the directory and sub-directories (needed for inotify's int handles).
		HashMap<int, String> dirHandle_;
		/// Linux inotify needs a handle.
		int watchHandle_;

#elif defined(__APPLE__) && !defined(IOS)

		/// Flag indicating whether the running OS supports individual file watching.
		bool supported_;
		/// Pointer to internal MacFileWatcher delegate.
		void* watcher_;

#endif
	};
}