#include "ITexture.h"

Sapphire::Texture::Texture(Context * ctx): Resource(ctx)
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
