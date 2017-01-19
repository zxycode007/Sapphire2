#include "../Core/Object.h"

namespace Sapphire
{
	SAPPHIRE_EVENT(E_CONSOLECOMMAND, ConsoleCommand)
	{
		SAPPHIRE_PARAM(P_COMMAND, Command);
		SAPPHIRE_PARAM(P_ID, Id);
	}
}