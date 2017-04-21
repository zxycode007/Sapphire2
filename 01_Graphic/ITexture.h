#pragma once
#include "Resource\Resource.h"
#include "IGPUObject.h"

namespace Sapphire
{
	class Image;

	class SAPPHIRE_API Texture : public Resource, public GPUObject
	{
		SAPPHIRE_OBJECT(Texture, Resource);

	public:

		Texture(Context* ctx);
		virtual ~Texture();

		void SetSize(int width, int height, int depth = 32);
		void SetWidth(int width);
		void SetHeight(int height);
		void SetDepth(int depth);
		int GetDepth();
	    int GetWidth();
		int GetHeight();


	private:

		int m_width;
		int m_height;
		int m_depth;


	};

}

