#pragma once

#include "HashSet.h"
#include "List.h"
#include "../Object.h"


namespace Sapphire
{

	class AsyncExecRequest;

	/// 返回文件
	static const unsigned SCAN_FILES = 0x1;
	/// 返回目录
	static const unsigned SCAN_DIRS = 0x2;
	/// 返回隐藏文件
	static const unsigned SCAN_HIDDEN = 0x4;

	// 文件和目录操作和访问控制的子系统
	class SAPPHIRE_API FileSystem : public Object
	{
		SAPPHIRE_OBJECT(FileSystem, Object);

	public:
		 
		FileSystem(Context* context);

		~FileSystem();

		/// 设置当前工作目录
		bool SetCurrentDir(const String& pathName);
		/// 创建一个目录
		bool CreateDir(const String& pathName);
		// 设置是否执行作为特定操作系统命令的引擎控制台命令
		void SetExecuteConsoleCommands(bool enable);
		// 用命令解释器运行一个程序，阻塞并直到返回退出码。如果任何允许的路径被定义将失败。
		int SystemCommand(const String& commandLine, bool redirectStdOutToLog = false);
		// 用运行一个程序，阻塞并直到返回退出码。如果任何允许的路径被定义将失败。
		int SystemRun(const String& fileName, const Vector<String>& arguments);
		//用命令解释器异步运行一个程序。 返回一个request ID 或 如果失败返回M_MAX_UNSIGNED。 这个退出码将连同在一个AsyncExecFinished事件的request ID发出。如果任何允许的路径被定义将失败。
		unsigned SystemCommandAsync(const String& commandLine);
		//异步运行一个程序。 返回一个request ID 或 如果失败返回M_MAX_UNSIGNED。 这个退出码将连同在一个AsyncExecFinished事件的request ID发出。如果任何允许的路径被定义将失败。
		unsigned SystemRunAsync(const String& fileName, const Vector<String>& arguments);
		/// 用指定的模式“edit”在一个外部程序打开一个文件，如果任何允许的路径被定义将失败。
		bool SystemOpen(const String& fileName, const String& mode = String::EMPTY);
		/// 复制一个文件。如果成功返回true
		bool Copy(const String& srcFileName, const String& destFileName);
		/// 重命名文件
		bool Rename(const String& srcFileName, const String& destFileName);
		/// 删除文件
		bool Delete(const String& fileName);
		/// 注册一个允许访问的路径。如果没有路径被注册，则所有都可被访问
		void RegisterPath(const String& pathName);
		/// 设置文件上一次修改时间 
		bool SetLastModifiedTime(const String& fileName, unsigned newTime);

		///返回当前绝对路径
		String GetCurrentDir() const;

		/// 返回引擎是否执行控制台命令
		bool GetExecuteConsoleCommands() const { return executeConsoleCommands_; }

		/// 路径是否已经被注册了
		bool HasRegisteredPaths() const { return allowedPaths_.Size() > 0; }

		/// 检查路径访问许可
		bool CheckAccess(const String& pathName) const;
		/// 返回文件自1.1.1970起的上一次修改的时间， 如果为0则无法访问
		unsigned GetLastModifiedTime(const String& fileName) const;
		/// 文件是否存在
		bool FileExists(const String& fileName) const;
		/// 目录是否存在
		bool DirExists(const String& pathName) const;
		/// 扫描目录
		void ScanDir(Vector<String>& result, const String& pathName, const String& filter, unsigned flags, bool recursive) const;
		/// 返回程序的目录。
		String GetProgramDir() const;
		/// Return the user documents directory.
		String GetUserDocumentsDir() const;
		/// Return the application preferences directory.
		String GetAppPreferencesDir(const String& org, const String& app) const;

	private:
		/// 扫描目录，内部调用
		void ScanDirInternal
			(Vector<String>& result, String path, const String& startPath, const String& filter, unsigned flags, bool recursive) const;
		/// 处理BeginFrame事件，检查已完成的异步执行
		void HandleBeginFrame(StringHash eventType, VariantMap& eventData);
		/// 处理一个控制台命令的事件
		void HandleConsoleCommand(StringHash eventType, VariantMap& eventData);

		/// 允许访问的目录
		HashSet<String> allowedPaths_;
		/// 程序目录的缓存
		mutable String programDir_;
		/// 异步执行序列
		List<AsyncExecRequest*> asyncExecQueue_;
		/// 下一次异步的执行ID
		unsigned nextAsyncExecID_;
		/// 游戏控制台执行操作系统命令的标志
		bool executeConsoleCommands_;
	};

	/// Split a full path to path, filename and extension. The extension will be converted to lowercase by default.
	SAPPHIRE_API void
		SplitPath(const String& fullPath, String& pathName, String& fileName, String& extension, bool lowercaseExtension = true);
	/// Return the path from a full path.
	SAPPHIRE_API String GetPath(const String& fullPath);
	/// Return the filename from a full path.
	SAPPHIRE_API String GetFileName(const String& fullPath);
	/// 返回一个完整路径的扩展名，默认转为小写
	SAPPHIRE_API String GetExtension(const String& fullPath, bool lowercaseExtension = true);
	/// Return the filename and extension from a full path. The case of the extension is preserved by default, so that the file can be opened in case-sensitive operating systems.
	SAPPHIRE_API String GetFileNameAndExtension(const String& fullPath, bool lowercaseExtension = false);
	/// Replace the extension of a file name with another.
	SAPPHIRE_API String ReplaceExtension(const String& fullPath, const String& newExtension);
	/// Add a slash at the end of the path if missing and convert to internal format (use slashes.)
	SAPPHIRE_API String AddTrailingSlash(const String& pathName);
	/// Remove the slash from the end of a path if exists and convert to internal format (use slashes.)
	SAPPHIRE_API String RemoveTrailingSlash(const String& pathName);
	/// Return the parent path, or the path itself if not available.
	SAPPHIRE_API String GetParentPath(const String& pathName);
	/// Convert a path to internal format (use slashes.)
	SAPPHIRE_API String GetInternalPath(const String& pathName);
	/// Convert a path to the format required by the operating system.
	SAPPHIRE_API String GetNativePath(const String& pathName);
	/// Convert a path to the format required by the operating system in wide characters.
	SAPPHIRE_API WString GetWideNativePath(const String& pathName);
	/// Return whether a path is absolute.
	SAPPHIRE_API bool IsAbsolutePath(const String& pathName);
}
