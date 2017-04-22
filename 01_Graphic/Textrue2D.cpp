#include "Context.h"
#include "Textrue2D.h"
#include "Resource\Image.h"
#include <GL\glew.h>
#include "FileSystem.h"
#include "Resource\XMLFile.h"
#include "Resource\ResourceCache.h"



Sapphire::Texture2D::Texture2D(Context * ctx):Texture(ctx)
{
	m_target = GL_TEXTURE_2D;
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
		//加载失败
		mImage.Reset();
		return false;
	}
	//如果异步读取状态
	if (GetAsyncLoadState() == ASYNC_LOADING)
		mImage->PrecalculateLevels();   //预计数MIP

	 

	// 读取XML配置
	ResourceCache* cache = GetSubsystem<ResourceCache>();
	String xmlName = ReplaceExtension(GetName(), ".xml");
	mloadParameters = cache->GetTempResource<XMLFile>(xmlName, false);


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
