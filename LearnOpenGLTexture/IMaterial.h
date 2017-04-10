#pragma once
#include "ShaderManager.h"
#include "Vertex.h"



namespace Sapphire
{
	class ITexture;

	struct MaterialShaderParameter
	{
		String name_;
		Variant value_;
	};

	//���ģʽ
	enum FillMode
	{
		FILL_SOLID = 0,
		FILL_WIREFRAME,
		FILL_POINT
	};

	

	class IMaterial
	{
		
	public:
		

		virtual void SetShaderParameter(String name, Variant parameter) = 0;
		virtual void SetTexture(String name, ITexture* texture) = 0;
		virtual void SetFillMode(FillMode mode) = 0;
		virtual ITexture*   GetTexture(String name) = 0;
		virtual FillMode    GetFillMode() = 0;
		virtual MaterialShaderParameter  GetShaderParameter(String name) = 0;
		virtual void  SetShader(String name, Shader* shader) = 0;
		virtual Shader*      GetShader(String name) = 0;
		virtual EVertexType  GetVertexType() = 0;
		virtual void         SetVertexType(EVertexType vType) = 0;

	};
}