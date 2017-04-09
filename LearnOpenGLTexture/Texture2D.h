#pragma once

#include "ITexture.h"


namespace Sapphire
{
	class Texture2D : public ITexture2D
	{
	public:
		Texture2D();
		~Texture2D();

		virtual void SetSize(int weight, int height, int depth=32);
		virtual int GetDepth();
		virtual int GetWidth();
		virtual int GetHeight();
		virtual void SetData(Image* image);
		virtual void SetData(void* data, int width, int height, int depth=32);
		virtual bool GetData(void* dest);

	private:
		int     mWidth;
		int     mHeight;
		int     mDepth;
		Image*  mImage;

	};
}