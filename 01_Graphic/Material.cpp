#include "Material.h"
#include "ShaderManager.h"
#include "ITexture.h"

Sapphire::Material::Material(Context* context) :Resource(context)
{
	mFillMode = FILL_SOLID;
}

Sapphire::Material::~Material()
{
}

void Sapphire::Material::SetShaderParameter(String name, Variant parameter)
{
	HashMap<String, MaterialShaderParameter>::Iterator it = mShaderParameters.Find(name);
	if (it != mShaderParameters.End())
	{
		it->second_.value_ = parameter;
	}
	else
	{
		MaterialShaderParameter msp;
		msp.name_ = name;
		msp.value_ = parameter;
		mShaderParameters[name] = msp;
	}
}

void Sapphire::Material::SetTexture(String name, Texture * texture)
{
	HashMap<String, SharedPtr<Texture>>::Iterator it = mTextureMap.Find(name);
	if (it != mTextureMap.End())
	{
		it->second_.Reset();
		it->second_ = texture;
	}
	else
	{	
		mTextureMap[name] = SharedPtr<Texture>(texture);
	}


}

void Sapphire::Material::SetFillMode(FillMode mode)
{
	mFillMode = mode;
}

Sapphire::Texture*  Sapphire::Material::GetTexture(String name)
{
	HashMap<String, SharedPtr<Texture>>::Iterator it = mTextureMap.Find(name);
	if (it != mTextureMap.End())
	{
		return it->second_.Get();
	}
	return nullptr;
}

Sapphire::FillMode Sapphire::Material::GetFillMode()
{
	return mFillMode;
}

Sapphire::MaterialShaderParameter Sapphire::Material::GetShaderParameter(String name)
{
	HashMap<String, MaterialShaderParameter>::Iterator it = mShaderParameters.Find(name);
	if (it != mShaderParameters.End())
	{
		return it->second_;
	}
	return MaterialShaderParameter();
}

void Sapphire::Material::SetShader(String name, Shader * shader)
{
	HashMap<String, SharedPtr<Shader>>::Iterator it = mShaderMap.Find(name);
	if (it != mShaderMap.End())
	{
		it->second_.Reset();
		it->second_ = shader;
	}
	else
	{
		mShaderMap[name] = SharedPtr<Shader>(shader);
	}


}

Sapphire::Shader * Sapphire::Material::GetShader(String name)
{
	HashMap<String, SharedPtr<Shader>>::Iterator it = mShaderMap.Find(name);
	if (it != mShaderMap.End())
	{
		return it->second_.Get();
	}
	return NULL;
}

Sapphire::EVertexType Sapphire::Material::GetVertexType()
{
	return mVertexType;
}

void Sapphire::Material::SetVertexType(EVertexType vType)
{
	mVertexType = vType;
}

