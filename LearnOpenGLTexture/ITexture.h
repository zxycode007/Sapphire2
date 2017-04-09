#pragma once




namespace Sapphire
{
	class Image;

	class ITexture2D
	{
	public:

		virtual void SetSize(int weight, int height, int depth=32) = 0;
		virtual int GetDepth() = 0;
		virtual int GetWidth() = 0;
		virtual int GetHeight() = 0;
		virtual void SetData(Image* image) = 0;
		virtual void SetData(void* data, int width, int height, int depth=32) = 0;
		virtual bool GetData(void* dest) = 0;

	};

}
