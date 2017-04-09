#include "Texture2D.h"
#include "Resource\Image.h"

Sapphire::Texture2D::Texture2D()
{
}

Sapphire::Texture2D::~Texture2D()
{
	if (mImage)
	{
		delete mImage;
	}
}

void Sapphire::Texture2D::SetSize(int weight, int height, int depth)
{
	mWidth = weight;
	mHeight = height;
	mDepth = depth;
}

int Sapphire::Texture2D::GetDepth()
{
	return mDepth;
}

int Sapphire::Texture2D::GetWidth()
{
	return mWidth;
}

int Sapphire::Texture2D::GetHeight()
{
	return mHeight;
}

void Sapphire::Texture2D::SetData(Image * image)
{
	mImage = image;
}

void Sapphire::Texture2D::SetData(void * data, int width, int height, int depth)
{
	if (mImage)
	{
		delete mImage;
	}
	mImage->SetData((unsigned char*)data);
	mImage->Resize(width, height);
	mDepth = depth;
	mWidth = width;
	mHeight = height;
}

bool Sapphire::Texture2D::GetData(void * dest)
{
	if (mImage)
	{
		return mImage->GetData();
	}
	return false;
}
