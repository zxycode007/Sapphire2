#pragma once
#include "Object.h"

namespace Sapphire
{
	class SAPPHIRE_API Engine :public Object
	{
		SAPPHIRE_OBJECT(Engine, Object);
	public:
		Engine(Context* ctx);
		virtual ~Engine();

		void initialize();

	private:


	};
}