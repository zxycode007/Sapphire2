#pragma once

#include "HashSet.h"
#include "List.h"
#include "../Object.h"


namespace Sapphire
{

	class AsyncExecRequest;

	/// Return files.
	static const unsigned SCAN_FILES = 0x1;
	/// Return directories.
	static const unsigned SCAN_DIRS = 0x2;
	/// Return also hidden files.
	static const unsigned SCAN_HIDDEN = 0x4;

	// 文件和目录操作和访问控制的子系统
	class SAPPHIRE_API FileSystem : public Object
	{
		SAPPHIRE_OBJECT(FileSystem, Object);

	public:
		/// Construct.
		FileSystem(Context* context);
		/// Destruct.
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
		/// Rename a file. Return true if successful.
		bool Rename(const String& srcFileName, const String& destFileName);
		/// Delete a file. Return true if successful.
		bool Delete(const String& fileName);
		/// 注册一个允许访问的路径。
		//Register a path as allowed to access. If no paths are registered, all are allowed. Registering allowed paths is considered securing the Urho3D execution environment: running programs and opening files externally through the system will fail afterward.
		void RegisterPath(const String& pathName);
		/// Set a file's last modified time as seconds since 1.1.1970. Return true on success.
		bool SetLastModifiedTime(const String& fileName, unsigned newTime);

		/// Return the absolute current working directory.
		String GetCurrentDir() const;

		/// Return whether is executing engine console commands as OS-specific system command.
		bool GetExecuteConsoleCommands() const { return executeConsoleCommands_; }

		/// Return whether paths have been registered.
		bool HasRegisteredPaths() const { return allowedPaths_.Size() > 0; }

		/// Check if a path is allowed to be accessed. If no paths are registered, all are allowed.
		bool CheckAccess(const String& pathName) const;
		/// Returns the file's last modified time as seconds since 1.1.1970, or 0 if can not be accessed.
		unsigned GetLastModifiedTime(const String& fileName) const;
		/// Check if a file exists.
		bool FileExists(const String& fileName) const;
		/// Check if a directory exists.
		bool DirExists(const String& pathName) const;
		/// Scan a directory for specified files.
		void ScanDir(Vector<String>& result, const String& pathName, const String& filter, unsigned flags, bool recursive) const;
		/// Return the program's directory. If it does not contain the Urho3D default CoreData and Data directories, and the current working directory does, return the working directory instead.
		String GetProgramDir() const;
		/// Return the user documents directory.
		String GetUserDocumentsDir() const;
		/// Return the application preferences directory.
		String GetAppPreferencesDir(const String& org, const String& app) const;

	private:
		/// 扫描目录，内部调用
		void ScanDirInternal
			(Vector<String>& result, String path, const String& startPath, const String& filter, unsigned flags, bool recursive) const;
		/// Handle begin frame event to check for completed async executions.
		void HandleBeginFrame(StringHash eventType, VariantMap& eventData);
		/// Handle a console command event.
		void HandleConsoleCommand(StringHash eventType, VariantMap& eventData);

		/// 允许访问的目录
		HashSet<String> allowedPaths_;
		/// Cached program directory.
		mutable String programDir_;
		/// 异步执行序列
		List<AsyncExecRequest*> asyncExecQueue_;
		/// 下一次异步的执行ID
		unsigned nextAsyncExecID_;
		/// Flag for executing engine console commands as OS-specific system command. Default to true.
		bool executeConsoleCommands_;
	};

	/// Split a full path to path, filename and extension. The extension will be converted to lowercase by default.
	SAPPHIRE_API void
		SplitPath(const String& fullPath, String& pathName, String& fileName, String& extension, bool lowercaseExtension = true);
	/// Return the path from a full path.
	SAPPHIRE_API String GetPath(const String& fullPath);
	/// Return the filename from a full path.
	SAPPHIRE_API String GetFileName(const String& fullPath);
	/// Return the extension from a full path, converted to lowercase by default.
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
