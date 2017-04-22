#include "ITexture.h"
#include "Context.h"
#include "IVideoDriver.h"
#include "GraphicDefs.h"

Sapphire::Texture::Texture(Context * ctx) : Resource(ctx), GPUObject(ctx->GetSubsystem<VideoDriver>())
{
	
}

Sapphire::Texture::~Texture()
{
}

void Sapphire::Texture::SetSize(int width, int height, int depth)
{
	m_width = width;
	m_height = height;
	m_depth = depth;
}

void Sapphire::Texture::SetWidth(int width)
{
	m_width = width;
}

void Sapphire::Texture::SetHeight(int height)
{
	m_height = height;
}

void Sapphire::Texture::SetDepth(int depth)
{
	m_depth = depth;
}

int Sapphire::Texture::GetDepth()
{
	return m_depth;
}

int Sapphire::Texture::GetWidth()
{
	return m_width;
}

int Sapphire::Texture::GetHeight()
{
	return m_height;
}

unsigned Sapphire::Texture::GetTextureTarget()
{
	return m_target;
}


void  Sapphire::Texture::UpdateParameters()
{
	if (!GetGPUHandle() && !GetVideoDiver())
		return;
	glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GetWrapMode(m_addressMode[COORD_U]));
	glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GetWrapMode(m_addressMode[COORD_V]));
#ifndef GL_ES_VERSION_2_0
	glTexParameteri(m_target, GL_TEXTURE_WRAP_R, GetWrapMode(m_addressMode[COORD_W]));
#endif
}

static GLenum GetWrapMode(Sapphire::TextureAddressingMode mode)
{
#ifndef GL_ES_VERSION_2_0
	return Sapphire::VideoDriver::getGL3Support() ? Sapphire::gl3WrapModes[mode] : Sapphire::glWrapModes[mode];
#else
	return glWrapModes[mode];
#endif
}