#pragma once
#include "../Object.h"


namespace Sapphire
{
	/// 资源重加载开始
	SAPPHIRE_EVENT(E_RELOADSTARTED, ReloadStarted)
	{
	}

	/// 资源重加载完成
	SAPPHIRE_EVENT(E_RELOADFINISHED, ReloadFinished)
	{
	}

	/// 资源重加载失败
	SAPPHIRE_EVENT(E_RELOADFAILED, ReloadFailed)
	{
	}

	/// 在资源目录中跟踪到文件发生改变
	SAPPHIRE_EVENT(E_FILECHANGED, FileChanged)
	{
		SAPPHIRE_PARAM(P_FILENAME, FileName);                    // String
		SAPPHIRE_PARAM(P_RESOURCENAME, ResourceName);            // String
	}

	/// 资源加载失败
	SAPPHIRE_EVENT(E_LOADFAILED, LoadFailed)
	{
		SAPPHIRE_PARAM(P_RESOURCENAME, ResourceName);            // String
	}

	/// 资源没有找到
	SAPPHIRE_EVENT(E_RESOURCENOTFOUND, ResourceNotFound)
	{
		SAPPHIRE_PARAM(P_RESOURCENAME, ResourceName);            // String
	}

	/// 未知的资源类型
	SAPPHIRE_EVENT(E_UNKNOWNRESOURCETYPE, UnknownResourceType)
	{
		SAPPHIRE_PARAM(P_RESOURCETYPE, ResourceType);            // StringHash
	}

	/// 资源后台加载完成
	SAPPHIRE_EVENT(E_RESOURCEBACKGROUNDLOADED, ResourceBackgroundLoaded)
	{
		SAPPHIRE_PARAM(P_RESOURCENAME, ResourceName);            // String
		SAPPHIRE_PARAM(P_SUCCESS, Success);                      // bool
		SAPPHIRE_PARAM(P_RESOURCE, Resource);                    // Resource pointer
	}

	/// 语言改变
	SAPPHIRE_EVENT(E_CHANGELANGUAGE, ChangeLanguage)
	{
	}
}