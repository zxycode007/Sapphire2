#include "Context.h"
#include "Textrue2D.h"
#include "Resource\Image.h"


Sapphire::Texture2D::Texture2D(Context * ctx):Texture(ctx)
{
	GPUObject(ctx->GetSubsystem<VideoDriver>());
}

Sapphire::Texture2D::~Texture2D()
{
	if (mImage)
	{
		delete mImage;
	}
}

void Sapphire::Texture2D::RegisterObject(Context * context)
{
	context->RegisterFactory<Texture2D>();
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

bool Sapphire::Texture2D::GetData(void*& dest)
{
	if (mImage)
	{
		dest = mImage->GetData();
		return true;
	}
	return false;
}

bool Sapphire::Texture2D::BeginLoad(Deserializer & source)
{
	mImage = new Image(context_);
	if (!mImage->Load(source))
	{
		//¼ÓÔØÊ§°Ü
		mImage.Reset();
		return false;
	}
	if (GetAsyncLoadState() == ASYNC_LOADING)
		mImage->PrecalculateLevels();

	return true;
}

bool Sapphire::Texture2D::EndLoad()
{
	if (mImage.Null())
	{
		return false;
	}
	SetData(mImage);
	mImage.Reset();
	return true;
}
