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

	class SAPPHIRE_API Material : public Resource
	{
		SAPPHIRE_OBJECT(Material, Resource);

	public:

		Material(Context* context);
		virtual ~Material();

		virtual void SetShaderParameter(String name, Variant parameter);
		virtual void SetTexture(String name, Texture* texture);
		virtual void SetFillMode(FillMode mode);
		virtual Texture*   GetTexture(String name);
		virtual FillMode    GetFillMode();
		virtual MaterialShaderParameter  GetShaderParameter(String name);
		virtual void  SetShader(String name, Shader* shader);
		virtual Shader*   GetShader(String name);
		virtual EVertexType  GetVertexType();
		virtual void         SetVertexType(EVertexType vType);

	private:
		HashMap<String, SharedPtr<Shader>> mShaderMap;
		HashMap<String, MaterialShaderParameter> mShaderParameters;
		HashMap<String, SharedPtr<Texture>>   mTextureMap;
		FillMode                     mFillMode;
		EVertexType                  mVertexType;
	};
}