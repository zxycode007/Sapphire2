#include "FileSystem.h"
#include "Material.h"
#include "ShaderManager.h"
#include "ITexture.h"
#include "Textrue2D.h"
#include "Resource/XMLFile.h"
#include "Resource/ResourceCache.h"
#include "IVideoDriver.h"

namespace Sapphire
{



	//纹理单元命名
	static const char* textureUnitNames[] =
	{
		"diffuse",
		"normal",
		"specular",
		"emissive",
		"environment",
#ifdef SAPPHIRE_DESKTOP_GRAPHICS
		"volume",
		"custom1",
		"custom2",
		"lightramp",
		"lightshape",
		"shadowmap",
		"faceselect",
		"indirection",
		"depth",
		"light",
		"zone",
		0
#else
		"lightramp",
		"lightshape",
		"shadowmap",
		0
#endif
	};

	static const char*  fillModeNames[] =
	{
		"solid",
		"wireframe",
		"point",
		0

	};

	static const char*  vertexTypeNames[] =
	{
		"standrd",
		"color",
		"normal",
		"texture",
		"texture2",
		"tangent",
		0
	};

	EVertexType  ParseEVertexType(String name)
	{
		name.ToLower().Trimmed();
		EVertexType vType = (EVertexType)GetStringListIndex(name.CString(), vertexTypeNames, MAX_EVT);
		return vType;
	}

	FillMode     ParseFillMode(String name)
	{
		name.ToLower().Trimmed();
		FillMode  mode = (FillMode)GetStringListIndex(name.CString(), fillModeNames, MAX_FILLMODE);
		return mode;
	}

	ETextureUnit ParseTextureUnitName(String name)
	{
		name = name.ToLower().Trimmed();
		//通过字符串找到对应纹理单元
		ETextureUnit unit = (ETextureUnit)GetStringListIndex(name.CString(), textureUnitNames, MAX_TEXTURE_UNITS);
		if (unit == MAX_TEXTURE_UNITS)
		{
            //没有匹配到有效的纹理单元
			//特别处理
			if (name == "diff")
				unit = TU_DIFFUSE;
			else if (name == "albedo")
				unit = TU_DIFFUSE;
			else if (name == "norm")
				unit = TU_NORMAL;
			else if (name == "spec")
				unit = TU_SPECULAR;
			else if (name == "env")
				unit = TU_ENVIRONMENT;
			// 检查合法性
			else if (name.Length() < 3)
				unit = (ETextureUnit)Clamp(ToInt(name), 0, MAX_TEXTURE_UNITS - 1);
		}

		if (unit == MAX_TEXTURE_UNITS)
			SAPPHIRE_LOGERROR("Unknown texture unit name " + name);

		return unit;
	}
}

Sapphire::Material::Material(Context* context) :Resource(context)
{
	mFillMode = FILL_SOLID;
}

Sapphire::Material::~Material()
{
}

bool Sapphire::Material::BeginLoad(Deserializer& source)
{
	VideoDriver* driver = context_->GetSubsystem<VideoDriver>();
	if (!driver)
	{
		return true;
	}
	//读取配置，将所用到的纹理加入后台下载
	mLoadXMLFile = new XMLFile(context_);
	mLoadXMLFile->Load(source);
	if (GetAsyncLoadState() == ASYNC_LOADING)  //异步读取
	{
		ResourceCache* cache = GetSubsystem<ResourceCache>();
		XMLElement root = mLoadXMLFile->GetRoot();
		XMLElement techniqueElem = root.GetChild("technique");

		while (techniqueElem)
		{
			techniqueElem = techniqueElem.GetNext("technique");
		}
		XMLElement textureElem = root.GetChild("texture");
		//在后台加载纹理
		while (textureElem)
		{
			String name = textureElem.GetAttribute("name");
			//获取扩展名是否是xml
			if (GetExtension(name) == ".xml")
			{
#ifdef SAPPHIRE_DESKTOP_GRAPHICS
				ETextureUnit unit = TU_DIFFUSE;
				if (textureElem.HasAttribute("unit"))
					unit = ParseTextureUnitName(textureElem.GetAttribute("unit"));
				//if (unit == TU_VOLUMEMAP)
				//	cache->BackgroundLoadResource<Texture3D>(name, true, this);
				//else
				 
#endif
				//cache->BackgroundLoadResource<TextureCube>(name, true, this);
			}
			else
				cache->BackgroundLoadResource<Texture2D>(name, true, this);  //后台加载该纹理
			//取得下一个纹理
			textureElem = textureElem.GetNext("texture");
		}

	}
	else
	{
		ResetToDefault();
		mLoadXMLFile.Reset();
		return false;
	}
	

}

bool Sapphire::Material::EndLoad()
{
	VideoDriver* driver = context_->GetSubsystem<VideoDriver>();
	if (!driver)
	{
		return true;
	}

	bool success = false;
	ResetToDefault();
	if (mLoadXMLFile)
	{
		XMLElement root = mLoadXMLFile->GetRoot();
		//加载材质配置
		Load(root);
	}
	mLoadXMLFile.Reset();
	return success;
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

void Sapphire::Material::SetTexture(ETextureUnit unit, Texture* texture)
{
	HashMap<ETextureUnit, SharedPtr<Texture>>::Iterator it = mTextureMap.Find(unit);
	if (it != mTextureMap.End())
	{
		it->second_.Reset();
		it->second_ = texture;
	}
	else
	{	
		mTextureMap[unit] = SharedPtr<Texture>(texture);
	}


}

void Sapphire::Material::SetFillMode(FillMode mode)
{
	mFillMode = mode;
}

Sapphire::Texture*  Sapphire::Material::GetTexture(ETextureUnit unit)
{
	HashMap<ETextureUnit, SharedPtr<Texture>>::ConstIterator it = mTextureMap.Find(unit);
	if (it != mTextureMap.End())
	{
		return it->second_.Get();
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

void Sapphire::Material::ResetToDefault()
{

}

void Sapphire::Material::Load(XMLElement& root)
{
	
	if (root.IsNull())
	{
		return;
	}
	ResourceCache* cache = context_->GetSubsystem<ResourceCache>();
	XMLElement techniqueElem = root.GetChild("technique");
	if (techniqueElem.NotNull())
	{
		XMLElement shaderElem = techniqueElem.GetChild("shader");
		if (shaderElem.NotNull())
		{
			
			mShaderName = shaderElem.GetName();
			String fillmode = shaderElem.GetAttribute("fillmode");
			mFillMode = ParseFillMode(fillmode);
			String vertexType = shaderElem.GetAttribute("vertexType");
			mVertexType = ParseEVertexType(vertexType);
		}

	}


}

