#pragma once

#include "Str.h"
#include "Variant.h"
#include "GraphicDefs.h"
#include "Shader.h"
#include "ITexture.h"



namespace Sapphire
{
	struct MaterialShaderParameter
	{
		String name_;
		Variant value_;
	};

	/// 纹理单元的hash方法
	 inline unsigned MakeHash(const ETextureUnit& value)
	{
		return (unsigned)value;
	}

	class SAPPHIRE_API Material : public Resource
	{
		SAPPHIRE_OBJECT(Material, Resource);

	public:

		Material(Context* context);
		virtual ~Material();

		virtual void SetShaderParameter(String name, Variant parameter);
		virtual void SetTexture(ETextureUnit unit, Texture* texture);
		virtual void SetFillMode(FillMode mode);
		virtual Texture*   GetTexture(ETextureUnit name);
		virtual FillMode    GetFillMode();
		virtual MaterialShaderParameter  GetShaderParameter(String name);
		virtual void  SetShader(String name, Shader* shader);
		virtual Shader*   GetShader(String name);
		virtual EVertexType  GetVertexType();
		virtual void         SetVertexType(EVertexType vType);

	private:

		HashMap<String, SharedPtr<Shader>> mShaderMap;
		HashMap<String, MaterialShaderParameter> mShaderParameters;
		//纹理单元与纹理的映射
		HashMap<ETextureUnit, SharedPtr<Texture>>   mTextureMap;
		FillMode                     mFillMode;
		EVertexType                  mVertexType;
	};
}