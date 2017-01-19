#pragma once

#include "Object.h"


namespace Sapphire
{
	/// Frame begin event.
	SAPPHIRE_EVENT(E_BEGINFRAME, BeginFrame)
	{
		SAPPHIRE_PARAM(P_FRAMENUMBER, FrameNumber);      // unsigned
		SAPPHIRE_PARAM(P_TIMESTEP, TimeStep);            // float
	}

	/// Application-wide logic update event.
	SAPPHIRE_EVENT(E_UPDATE, Update)
	{
		SAPPHIRE_PARAM(P_TIMESTEP, TimeStep);            // float
	}

	/// Application-wide logic post-update event.
	SAPPHIRE_EVENT(E_POSTUPDATE, PostUpdate)
	{
		SAPPHIRE_PARAM(P_TIMESTEP, TimeStep);            // float
	}

	/// Render update event.
	SAPPHIRE_EVENT(E_RENDERUPDATE, RenderUpdate)
	{
		SAPPHIRE_PARAM(P_TIMESTEP, TimeStep);            // float
	}

	/// Post-render update event.
	SAPPHIRE_EVENT(E_POSTRENDERUPDATE, PostRenderUpdate)
	{
		SAPPHIRE_PARAM(P_TIMESTEP, TimeStep);            // float
	}

	/// Frame end event.
	SAPPHIRE_EVENT(E_ENDFRAME, EndFrame)
	{
	}
}