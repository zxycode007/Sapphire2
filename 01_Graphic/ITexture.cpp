#include "ITexture.h"
#include "Context.h"
#include "IVideoDriver.h"
#include "GraphicDefs.h"
#include "Resource\XMLFile.h"
#include "Resource\XMLElement.h"

static const char* addressModeNames[] =
{
	"wrap",
	"mirror",
	"clamp",
	"border",
	0
};

static const char* filterModeNames[] =
{
	"nearest",
	"bilinear",
	"trilinear",
	"anisotropic",
	"default",
	0
};

Sapphire::Texture::Texture(Context * ctx) : Resource(ctx), GPUObject(ctx->GetSubsystem<VideoDriver>())
, m_parametersDirty(true),
m_filterMode(FILTER_DEFAULT),
m_width(0),
m_height(0),
m_depth(0)
{
	for (int i = 0; i < MAX_COORDS; ++i)
		m_addressMode[i] = EAD_WRAP;
}

Sapphire::Texture::~Texture()
{
}

void Sapphire::Texture::SetMipLevel(unsigned level)
{
	m_mipLevel = level;
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


void Sapphire::Texture::SetParametersDirty()
{
	m_parametersDirty = true;
}

void Sapphire::Texture::SetFilterMode(ETextureFilterMode filterMode)
{
	m_filterMode = filterMode;
	//改了参数，需要更新
	m_parametersDirty = true;
}

void Sapphire::Texture::SetAddressMode(ETextureCoordinate coord, ETextureAddressingMode addressMode)
{
	m_addressMode[coord] = addressMode;
	m_parametersDirty = true;
}

 

void  Sapphire::Texture::UpdateParameters()
{
	if (!GetGPUHandle() && !GetVideoDiver())
		return;
	//设置纹理参数
	glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GetWrapMode(m_addressMode[ECOORD_U]));
	glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GetWrapMode(m_addressMode[ECOORD_V]));
#ifndef GL_ES_VERSION_2_0
	glTexParameteri(m_target, GL_TEXTURE_WRAP_R, GetWrapMode(m_addressMode[ECOORD_W]));
#endif

	ETextureFilterMode filterMode = m_filterMode;
	if (filterMode == FILTER_DEFAULT)
		filterMode = GetVideoDiver()->GetDefaultTextureFilterMode();  //获取驱动默认的滤波模式

	// Filtering
	switch (filterMode)
	{
	case EFILTER_NEAREST:
		glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		break;

	case FILTER_BILINEAR:
		if (m_mipLevel < 2)
			glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		else
			glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;

	case FILTER_ANISOTROPIC:
	case FILTER_TRILINEAR:
		if (m_mipLevel < 2)
			glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		else
			glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;

	default:
		break;
	}


#ifndef GL_ES_VERSION_2_0
	// 各项异性滤波
	if (GetVideoDiver()->GetAnisotropySupport())
	{
		glTexParameterf(m_target, GL_TEXTURE_MAX_ANISOTROPY_EXT,
			filterMode == FILTER_ANISOTROPIC ? (float)GetVideoDiver()->GetTextureAnisotropy() : 1.0f);
	}

#endif

	m_parametersDirty = false;
}

void Sapphire::Texture::SetParameters(XMLFile * xml)
{
	if (!xml)
		return;

	XMLElement rootElem = xml->GetRoot();
	SetParameters(rootElem);
}

void Sapphire::Texture::SetParameters(const XMLElement & element)
{
	XMLElement paramElem = element.GetChild();
	while (paramElem)
	{
		String name = paramElem.GetName();

		if (name == "address")
		{
			String coord = paramElem.GetAttributeLower("coord");
			if (coord.Length() >= 1)
			{
				ETextureCoordinate coordIndex = (ETextureCoordinate)(coord[0] - 'u');
				String mode = paramElem.GetAttributeLower("mode");
				SetAddressMode(coordIndex, (ETextureAddressingMode)GetStringListIndex(mode.CString(), addressModeNames, EAD_WRAP));
			}
		}		 

		if (name == "filter")
		{
			String mode = paramElem.GetAttributeLower("mode");
			SetFilterMode((ETextureFilterMode)GetStringListIndex(mode.CString(), filterModeNames, FILTER_DEFAULT));
		}

		if (name == "mipmap")
			SetMipLevel(paramElem.GetBool("enable") ? 0 : 1);

		 

		paramElem = paramElem.GetNext();
	}
}

GLenum Sapphire::GetWrapMode(Sapphire::ETextureAddressingMode mode)
{
#ifndef GL_ES_VERSION_2_0
	return Sapphire::VideoDriver::getGL3Support() ? Sapphire::gl3WrapModes[mode] : Sapphire::glWrapModes[mode];
#else
	return glWrapModes[mode];
#endif
}