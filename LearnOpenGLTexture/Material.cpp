#include "Material.h"
#include "ShaderManager.h"
#include "ITexture.h"

Sapphire::Material::Material()
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

void Sapphire::Material::SetTexture(String name, ITexture * texture)
{
	HashMap<String, ITexture*>::Iterator it = mTextureMap.Find(name);
	if (it != mTextureMap.End())
	{
		ITexture* pTexture = it->second_;
		delete pTexture;
	}
	it->second_ = texture;
}

void Sapphire::Material::SetFillMode(FillMode mode)
{
	mFillMode = mode;
}

Sapphire::ITexture * Sapphire::Material::GetTexture(String name)
{
	HashMap<String, ITexture*>::Iterator it = mTextureMap.Find(name);
	if (it != mTextureMap.End())
	{
		return it->second_;
	}
	return NULL;
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
	HashMap<String, Shader*>::Iterator it = mShaderMap.Find(name);
	if (it != mShaderMap.End())
	{
		Shader* pShader = it->second_;
		delete pShader;
	}
	it->second_ = shader;

}

Sapphire::Shader * Sapphire::Material::GetShader(String name)
{
	return mShaderMap[name];
}

Sapphire::EVertexType Sapphire::Material::GetVertexType()
{
	return mVertexType;
}

void Sapphire::Material::SetVertexType(EVertexType vType)
{
	mVertexType = vType;
}

