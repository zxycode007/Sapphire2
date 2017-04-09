#pragma once

#include "IMaterial.h"


namespace Sapphire
{
	class Material : public IMaterial
	{
	public:

		Material();
		virtual ~Material();

		virtual void SetShaderParameter(String name, Variant parameter);
		virtual void SetTexture(String name, ITexture* texture);
		virtual void SetFillMode(FillMode mode) ;
		virtual ITexture*   GetTexture(String name);
		virtual FillMode    GetFillMode();
		virtual MaterialShaderParameter  GetShaderParameter(String name);
		virtual void  SetShader(String name, Shader* shader);
		virtual Shader*   GetShader(String name);

	private:
		HashMap<String, Shader*> mShaderMap;
		HashMap<String, MaterialShaderParameter> mShaderParameters;
		HashMap<String, ITexture*>   mTextureMap;
		FillMode                     mFillMode;
	};
}