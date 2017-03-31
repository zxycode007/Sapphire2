#pragma once
#include "../Object.h"


namespace Sapphire
{
	/// Resource reloading started.
	SAPPHIRE_EVENT(E_RELOADSTARTED, ReloadStarted)
	{
	}

	/// Resource reloading finished successfully.
	SAPPHIRE_EVENT(E_RELOADFINISHED, ReloadFinished)
	{
	}

	/// Resource reloading failed.
	SAPPHIRE_EVENT(E_RELOADFAILED, ReloadFailed)
	{
	}

	/// Tracked file changed in the resource directories.
	SAPPHIRE_EVENT(E_FILECHANGED, FileChanged)
	{
		SAPPHIRE_PARAM(P_FILENAME, FileName);                    // String
		SAPPHIRE_PARAM(P_RESOURCENAME, ResourceName);            // String
	}

	/// Resource loading failed.
	SAPPHIRE_EVENT(E_LOADFAILED, LoadFailed)
	{
		SAPPHIRE_PARAM(P_RESOURCENAME, ResourceName);            // String
	}

	/// Resource not found.
	SAPPHIRE_EVENT(E_RESOURCENOTFOUND, ResourceNotFound)
	{
		SAPPHIRE_PARAM(P_RESOURCENAME, ResourceName);            // String
	}

	/// Unknown resource type.
	SAPPHIRE_EVENT(E_UNKNOWNRESOURCETYPE, UnknownResourceType)
	{
		SAPPHIRE_PARAM(P_RESOURCETYPE, ResourceType);            // StringHash
	}

	/// Resource background loading finished.
	SAPPHIRE_EVENT(E_RESOURCEBACKGROUNDLOADED, ResourceBackgroundLoaded)
	{
		SAPPHIRE_PARAM(P_RESOURCENAME, ResourceName);            // String
		SAPPHIRE_PARAM(P_SUCCESS, Success);                      // bool
		SAPPHIRE_PARAM(P_RESOURCE, Resource);                    // Resource pointer
	}

	/// Language changed.
	SAPPHIRE_EVENT(E_CHANGELANGUAGE, ChangeLanguage)
	{
	}
}