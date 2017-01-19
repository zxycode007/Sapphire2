
#pragma once

#include "ArrayPtr.h"
#include "../Mutex.h"
#include "../Object.h"
#include "../StringUtil.h"
#include "List.h"

namespace Sapphire
{

	/// Fictional message level to indicate a stored raw message.
	static const int LOG_RAW = -1;
	/// Debug message level. By default only shown in debug mode.
	static const int LOG_DEBUG = 0;
	/// Informative message level.
	static const int LOG_INFO = 1;
	/// Warning message level.
	static const int LOG_WARNING = 2;
	/// Error message level.
	static const int LOG_ERROR = 3;
	/// Disable all log messages.
	static const int LOG_NONE = 4;

	class File;

	/// Stored log message from another thread.
	struct StoredLogMessage
	{
		/// Construct undefined.
		StoredLogMessage()
		{
		}

		/// Construct with parameters.
		StoredLogMessage(const String& message, int level, bool error) :
			message_(message),
			level_(level),
			error_(error)
		{
		}

		/// Message text.
		String message_;
		/// Message level. -1 for raw messages.
		int level_;
		/// Error flag for raw messages.
		bool error_;
	};

	/// Logging subsystem.
	class SAPPHIRE_API Log : public Object
	{
		SAPPHIRE_OBJECT(Log, Object);

	public:
		/// Construct.
		Log(Context* context);
		/// Destruct. Close the log file if open.
		virtual ~Log();

		/// Open the log file.
		void Open(const String& fileName);
		/// Close the log file.
		void Close();
		/// Set logging level.
		void SetLevel(int level);
		/// Set whether to timestamp log messages.
		void SetTimeStamp(bool enable);
		/// Set quiet mode ie. only print error entries to standard error stream (which is normally redirected to console also). Output to log file is not affected by this mode.
		void SetQuiet(bool quiet);

		/// Return logging level.
		int GetLevel() const { return level_; }

		/// Return whether log messages are timestamped.
		bool GetTimeStamp() const { return timeStamp_; }

		/// Return last log message.
		String GetLastMessage() const { return lastMessage_; }

		/// Return whether log is in quiet mode (only errors printed to standard error stream).
		bool IsQuiet() const { return quiet_; }

		/// Write to the log. If logging level is higher than the level of the message, the message is ignored.
		static void Write(int level, const String& message);
		/// Write raw output to the log.
		static void WriteRaw(const String& message, bool error = false);

	private:
		/// Handle end of frame. Process the threaded log messages.
		void HandleEndFrame(StringHash eventType, VariantMap& eventData);

		/// Mutex for threaded operation.
		Mutex logMutex_;
		/// Log messages from other threads.
		List<StoredLogMessage> threadMessages_;
		/// Log file.
		SharedPtr<File> logFile_;
		/// Last log message.
		String lastMessage_;
		/// Logging level.
		int level_;
		/// Timestamp log messages flag.
		bool timeStamp_;
		/// In write flag to prevent recursion.
		bool inWrite_;
		/// Quiet mode flag.
		bool quiet_;
	};

#ifdef SAPPHIRE_LOGGING
#define SAPPHIRE_LOGDEBUG(message) Sapphire::Log::Write(Sapphire::LOG_DEBUG, message)
#define SAPPHIRE_LOGINFO(message) Sapphire::Log::Write(Sapphire::LOG_INFO, message)
#define SAPPHIRE_LOGWARNING(message) Sapphire::Log::Write(Sapphire::LOG_WARNING, message)
#define SAPPHIRE_LOGERROR(message) Sapphire::Log::Write(Sapphire::LOG_ERROR, message)
#define SAPPHIRE_LOGRAW(message) Sapphire::Log::WriteRaw(message)
#define SAPPHIRE_LOGDEBUGF(format, ...) Sapphire::Log::Write(Sapphire::LOG_DEBUG, Sapphire::ToString(format, ##__VA_ARGS__))
#define SAPPHIRE_LOGINFOF(format, ...) Sapphire::Log::Write(Sapphire::LOG_INFO, Sapphire::ToString(format, ##__VA_ARGS__))
#define SAPPHIRE_LOGWARNINGF(format, ...) Sapphire::Log::Write(Sapphire::LOG_WARNING, Sapphire::ToString(format, ##__VA_ARGS__))
#define SAPPHIRE_LOGERRORF(format, ...) Sapphire::Log::Write(Sapphire::LOG_ERROR, Sapphire::ToString(format, ##__VA_ARGS__))
#define SAPPHIRE_LOGRAWF(format, ...) Sapphire::Log::WriteRaw(Sapphire::ToString(format, ##__VA_ARGS__))
#else
#define SAPPHIRE_LOGDEBUG(message) ((void)0)
#define SAPPHIRE_LOGINFO(message) ((void)0)
#define SAPPHIRE_LOGWARNING(message) ((void)0)
#define SAPPHIRE_LOGERROR(message) ((void)0)
#define SAPPHIRE_LOGRAW(message) ((void)0)
#define SAPPHIRE_LOGDEBUGF(...) ((void)0)
#define SAPPHIRE_LOGINFOF(...) ((void)0)
#define SAPPHIRE_LOGWARNINGF(...) ((void)0)
#define SAPPHIRE_LOGERRORF(...) ((void)0)
#define SAPPHIRE_LOGRAWF(...) ((void)0)
#endif

}