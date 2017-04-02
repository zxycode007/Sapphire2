#pragma once 

#include "../Object.h"

namespace Sapphire
{

	/// Log message事件
	SAPPHIRE_EVENT(E_LOGMESSAGE, LogMessage)
	{
		SAPPHIRE_PARAM(P_MESSAGE, Message);              // String
		SAPPHIRE_PARAM(P_LEVEL, Level);                  // int
	}

	/// 异步系统命令执行完成
	SAPPHIRE_EVENT(E_ASYNCEXECFINISHED, AsyncExecFinished)
	{
		SAPPHIRE_PARAM(P_REQUESTID, RequestID);          // unsigned
		SAPPHIRE_PARAM(P_EXITCODE, ExitCode);            // int
	}

}