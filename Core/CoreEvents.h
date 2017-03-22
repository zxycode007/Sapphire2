#pragma once

#include "Object.h"


namespace Sapphire
{
	/// 帧开始事件
	SAPPHIRE_EVENT(E_BEGINFRAME, BeginFrame)
	{
		SAPPHIRE_PARAM(P_FRAMENUMBER, FrameNumber);      // unsigned
		SAPPHIRE_PARAM(P_TIMESTEP, TimeStep);            // float
	}

	/// 用于程序逻辑更新事件
	SAPPHIRE_EVENT(E_UPDATE, Update)
	{
		SAPPHIRE_PARAM(P_TIMESTEP, TimeStep);            // float
	}

	/// 应用程序逻辑后更新事件
	SAPPHIRE_EVENT(E_POSTUPDATE, PostUpdate)
	{
		SAPPHIRE_PARAM(P_TIMESTEP, TimeStep);            // float
	}

	/// 渲染更新事件
	SAPPHIRE_EVENT(E_RENDERUPDATE, RenderUpdate)
	{
		SAPPHIRE_PARAM(P_TIMESTEP, TimeStep);            // float
	}

	/// 后期渲染更新事件
	SAPPHIRE_EVENT(E_POSTRENDERUPDATE, PostRenderUpdate)
	{
		SAPPHIRE_PARAM(P_TIMESTEP, TimeStep);            // float
	}

	/// 帧结束事件
	SAPPHIRE_EVENT(E_ENDFRAME, EndFrame)
	{
	}
}