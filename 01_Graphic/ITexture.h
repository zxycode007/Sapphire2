#pragma once
#include "Resource\Resource.h"
#include "IGPUObject.h"
#include <GL\glew.h>
#include "GraphicDefs.h"

namespace Sapphire
{
	class Image;

	static GLenum glWrapModes[] =
	{
		GL_REPEAT,
		GL_MIRRORED_REPEAT,
		GL_CLAMP_TO_EDGE,
#ifndef GL_ES_VERSION_2_0
		GL_CLAMP
#else
		GL_CLAMP_TO_EDGE
#endif
	};

#ifndef GL_ES_VERSION_2_0
	static GLenum gl3WrapModes[] =
	{
		GL_REPEAT,
		GL_MIRRORED_REPEAT,
		GL_CLAMP_TO_EDGE,
		GL_CLAMP_TO_BORDER
	};
#endif

	//获取纹理寻址模式
	static GLenum GetWrapMode(TextureAddressingMode mode);
	

	class SAPPHIRE_API Texture : public Resource, public GPUObject
	{
		SAPPHIRE_OBJECT(Texture, Resource);

	public:

		Texture(Context* ctx);
		virtual ~Texture();

		void SetSize(int width, int height, int depth = 32);
		void SetWidth(int width);
		void SetHeight(int height);
		void SetDepth(int depth);
		int GetDepth();
	    int GetWidth();
		int GetHeight();
		unsigned  GetTextureTarget();
		bool     GetTextureParametersDirty(){ return m_parametersDirty; }
		//更新纹理参数
		void   UpdateParameters();

	protected:
		//纹理目标GL_TEXTURE_2D/3D
		unsigned   m_target;
		int m_width;
		int m_height;
		int m_depth;
		//纹理参数脏标志
		bool  m_parametersDirty;
		//u/v/w纹理寻址模式
		TextureAddressingMode m_addressMode[TextureCoordinate::MAX_COORDS];

	private:

		
		


	};

}

