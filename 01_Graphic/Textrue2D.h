#pragma once

#include "ITexture.h"


namespace Sapphire
{
	class XMLFile;
	class XMLElement;

	class SAPPHIRE_API Texture2D : public Texture
	{
		SAPPHIRE_OBJECT(Texture2D, Texture);

	public:

		Texture2D(Context* ctx);
		virtual ~Texture2D();

		static void RegisterObject(Context* context);

		void SetSize(int weight, int height, int depth = 32);
		int GetDepth();
		int GetWidth();
		int GetHeight();
		void SetData(Image* image);
		void SetData(void* data, int width, int height, int depth = 32);
		bool GetData(void*& dest);

		

		virtual bool BeginLoad(Deserializer& source);
		virtual bool EndLoad();

	private:
		int     mWidth;
		int     mHeight;
		int     mDepth;

		SharedPtr<Image>  mImage;
		//XML≈‰÷√≤Œ ˝
		SharedPtr<XMLFile>  mloadParameters;

	};
}