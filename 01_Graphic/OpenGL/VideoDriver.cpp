#include "Resource\ResourceCache.h"
#include "VideoDriver.h"
#include "Context.h"
#include "Color.h"
#include "../ITexture.h"
#include "../Resource/XMLFile.h"
#include "../GraphicsEvents.h"
#include "../Geometry.h"

Sapphire::VideoDriver::VideoDriver(Context * context) :Object(context)
,m_isInitialized(false),
m_windth(800),
m_height(600),
m_fullScreen(false),
m_canResize(false),
m_clearColor(Color::BLACK),
m_windowName("testWindow"),
m_bAnisotropySupport(false)
{
}

Sapphire::VideoDriver::~VideoDriver()
{
}

bool Sapphire::VideoDriver::m_gl3support = false;

void Sapphire::VideoDriver::Initialize()
{
	//初始化
	SetShaderManager(context_->GetSubsystem<ShaderManager>());
	ResourceCache* cache = context_->GetSubsystem<ResourceCache>();
	SharedPtr<XMLFile> configFile = cache->GetTempResource<XMLFile>("config.xml");
	mShaderManager = context_->GetSubsystem<ShaderManager>();
	LoadConfig(configFile);
	//glfwInit来初始化GLFW，然后我们可以使用glfwWindowHint来配置GLFW。
	glfwInit();
	//glfwWindowHint的第一个参数告诉我们，配置哪个选项，这些选项带有GLFW_前缀。第二个参数是一个整数，它代表我们为选项所设置的值
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, SAPPHIRE_OPENGL_VERSION_MAJOR); //告诉GLFW我们使用的OpenGL版本是3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, SAPPHIRE_OPENGL_VERSION_MIRROR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  //我们同样告诉GLFW，我们希望明确地使用core-profile
	if (m_canResize)
	{
		glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	}
	else
	{
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	}
	mWindowHandle = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	//创建我们窗口的环境（glfwMakeContextCurrent），这个环境是当前线程的主环境
	//设置当前窗口
	glfwMakeContextCurrent(mWindowHandle);
	if (mWindowHandle == NULL)
	{
		SAPPHIRE_LOGDEBUG("Failed to create GLFW window");
		glfwTerminate();
		return;
	}

	//初始化GLEW
	//在初始化GLEW前我们把glewExperimental变量设置为GL_TRUE。(glew 1.13或更早的版本)
	//设置glewExperimental为true可以保证GLEW使用更多的现代技术来管理OpenGL机能。
	//如果不这么设置，它就会使用默认的GL_FALSE，这样当使用core profile的时有可能发生问题。
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) //glewInit()放在Opengl环境建立之后
	{
		//初始化失败
		SAPPHIRE_LOGDEBUG("Failed to initialize GLEW");
		return;
	}


	// 定义视口大小
	int width, height;
	glfwGetFramebufferSize(mWindowHandle, &width, &height);
	glViewport(0, 0, width, height);

	m_isInitialized = true;
	
}

void Sapphire::VideoDriver::SetWindow(WindowsHandle * window)
{
	if (window)
	{
		glfwMakeContextCurrent(window);
		if (mWindowHandle)
		{
			glfwDestroyWindow(mWindowHandle);
		}
		mWindowHandle = window;
	}
}

void Sapphire::VideoDriver::SetWindowPos(WindowsHandle * window, IntVector2 & pos)
{
	if (mWindowHandle)
	{
		glfwSetWindowPos(window, pos.x_, pos.y_);
	}
	SAPPHIRE_LOGDEBUG("Error! WindowHandle invalid!");
}

Sapphire::WindowsHandle * Sapphire::VideoDriver::GetWindow()
{
	return mWindowHandle==NULL? mWindowHandle:NULL;
}

void Sapphire::VideoDriver::SetShaderManager(Sapphire::ShaderManager * manager)
{
	if (manager)
	{
		if (mShaderManager.NotNull())
		{
			mShaderManager.Reset();
		}
		mShaderManager = manager;

	}
}

Sapphire::ShaderManager * Sapphire::VideoDriver::GetShaderManager()
{
	return mShaderManager.NotNull()?mShaderManager.Get():NULL;
}

Sapphire::ETextureFilterMode Sapphire::VideoDriver::GetDefaultTextureFilterMode()
{
	return ETextureFilterMode::EFILTER_NEAREST;
}

void Sapphire::VideoDriver::SetTextureParametersDirty()
{
	MutexLock lock(m_gpuobjectsMutex);
	for (PODVector<GPUObject*>::Iterator it = m_gpuobjects.Begin(); it != m_gpuobjects.End(); ++it)
	{
		Texture* texture = dynamic_cast<Texture*>(*it);
		if (texture)
			texture->SetParametersDirty();   //设置纹理参数发生变更
	}
}

void Sapphire::VideoDriver::SetTextureAnisotropy(unsigned level)
{
	if (level != m_textureAnisotropy)
	{
		m_textureAnisotropy = level;
		SetTextureParametersDirty();
	}
	
}

void Sapphire::VideoDriver::SetDefaultTextureFilterMode(ETextureFilterMode mode)
{
	if (mode != m_defaultfilterMode)
	{
		m_defaultfilterMode = mode;
		SetTextureParametersDirty();
	}
}

void Sapphire::VideoDriver::SetTextureUnit()
{
	m_textureUnits["DiffMap"] = TU_DIFFUSE;
	m_textureUnits["DiffCubeMap"] = TU_DIFFUSE;
	m_textureUnits["AlbedoBuffer"] = TU_ALBEDOBUFFER;
	m_textureUnits["NormalMap"] = TU_NORMAL;
	m_textureUnits["NormalBuffer"] = TU_NORMALBUFFER;
	m_textureUnits["SpecMap"] = TU_SPECULAR;
	m_textureUnits["EmissiveMap"] = TU_EMISSIVE;
	m_textureUnits["EnvMap"] = TU_ENVIRONMENT;
	m_textureUnits["EnvCubeMap"] = TU_ENVIRONMENT;
	m_textureUnits["LightRampMap"] = TU_LIGHTRAMP;
	m_textureUnits["LightSpotMap"] = TU_LIGHTSHAPE;
	m_textureUnits["LightCubeMap"] = TU_LIGHTSHAPE;
	m_textureUnits["ShadowMap"] = TU_SHADOWMAP;
#ifdef SAPPHIRE_DESKTOP_GRAPHICS
	m_textureUnits["VolumeMap"] = TU_VOLUMEMAP;
	m_textureUnits["FaceSelectCubeMap"] = TU_FACESELECT;
	m_textureUnits["IndirectionCubeMap"] = TU_INDIRECTION;
	m_textureUnits["DepthBuffer"] = TU_DEPTHBUFFER;
	m_textureUnits["LightBuffer"] = TU_LIGHTBUFFER;
	m_textureUnits["ZoneCubeMap"] = TU_ZONE;
	m_textureUnits["ZoneVolumeMap"] = TU_ZONE;
#endif
}

void Sapphire::VideoDriver::Close()
{
}

void Sapphire::VideoDriver::Clear()
{

}

void Sapphire::VideoDriver::BeginFrame()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	for (int i = 0; i < SAPPHIRE_MAX_TEXTURE_UNIT; i++)
	{
		setTexture(i, 0);
	}
	prepareDraw();

	SendEvent(E_BEGINRENDERING);
	
}

void Sapphire::VideoDriver::render()
{
}

void Sapphire::VideoDriver::createWindow()
{
}

void Sapphire::VideoDriver::EndFrame()
{
	SendEvent(E_ENDRENDERING);
}

void Sapphire::VideoDriver::prepareDraw()
{
}

void Sapphire::VideoDriver::Draw(EPRIMITIVE_TYPE type, Geometry * geo)
{
	if (!geo)
	{
		return;
	}
	Material* material = geo->getMaterial();
	
	
	
}

bool Sapphire::VideoDriver::IsInitialzed()
{
	return m_isInitialized;
}

void Sapphire::VideoDriver::release()
{
	glfwTerminate();
}

int Sapphire::VideoDriver::getWidth()
{
	return m_windth;
}

void Sapphire::VideoDriver::setWidth(int width)
{
	m_windth = width;
}

void Sapphire::VideoDriver::setClearColor(Color c)
{
	m_clearColor = c;
}

Sapphire::Color Sapphire::VideoDriver::getClearColor()
{
	return m_clearColor;
}

void Sapphire::VideoDriver::setWindowName(String name)
{
	m_windowName = name;
}

Sapphire::String Sapphire::VideoDriver::getWindowName()
{
	return m_windowName;
}

void Sapphire::VideoDriver::setVBO(unsigned obj)
{
	if (m_Vbo != obj)
	{
		if (obj)
		{
			glBindBuffer(GL_ARRAY_BUFFER, obj);
			m_Vbo = obj;
		}
	}
}

void Sapphire::VideoDriver::setVAO(unsigned obj)
{
	if (m_Vao != obj)
	{
		if (obj)
		{
			glBindVertexArray(obj);
			m_Vao = obj;

		}
	}
}

void Sapphire::VideoDriver::setEBO(unsigned obj)
{
	if (m_Ebo != obj)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj);
		m_Ebo = obj;
	}
}

void Sapphire::VideoDriver::setTexture(int index, Texture* tex)
{
	if (index > SAPPHIRE_MAX_TEXTURE_UNIT)
	{
		SAPPHIRE_LOGGING("texture index exceeded!");
		return;
	}
	
	//纹理已存在
	if (m_textures[index] != tex)
	{
		if (m_activeTexture != index)
		{
			//激活该纹理
			glActiveTexture(GL_TEXTURE0 + index);
			m_activeTexture = index;
		}

		if (tex)
		{
			//如果纹理有效
			unsigned glType = tex->GetTextureTarget();
			if (m_textrueTypes[index] && m_textrueTypes[index] != glType)
			{
				//取消原绑定的纹理
				glBindTexture(m_textrueTypes[index], 0);
			}
			//绑定现在的纹理
			glBindTexture(glType, tex->GetGPUHandle());
			m_textrueTypes[index] = glType;

			if (tex->GetTextureParametersDirty())
				tex->UpdateParameters();

		}
		else if (m_textrueTypes[index])
		{
			//解除原理的绑定
			glBindTexture(m_textrueTypes[index], 0);
			m_textrueTypes[index] = 0;
		}

	}
	else
	{
		if (tex && tex->GetTextureParametersDirty())
		{
			if (m_activeTexture != index)
			{    
				//激活新的纹理
				glActiveTexture(GL_TEXTURE0 + index);
				m_activeTexture = index;
			}
			glBindTexture(m_textrueTypes[index], tex->GetGPUHandle());
			tex->UpdateParameters();
		}
	}

}

void Sapphire::VideoDriver::LoadConfig(XMLFile * xmlFile)
{
	if (!xmlFile)
	{
		SAPPHIRE_LOGDEBUG("load config.xml faild!");
		return;
	}
	XMLElement root = xmlFile->GetRoot();
	LoadConfigElement(&root);
}

void Sapphire::VideoDriver::LoadConfigElement(XMLElement * element)
{
	XMLElement paramElem = element->GetChild();
	while (paramElem)
	{
		String name = paramElem.GetName();
		if (name == "display")
		{
			String width = paramElem.GetAttributeLower("width");
			String height = paramElem.GetAttributeLower("height");
			String fullscreen = paramElem.GetAttribute("fullscreen");
			m_windth = ToInt(width);
			m_height = ToInt(height);
			m_fullScreen = ToBool(fullscreen);
		}
		if (name == "application")
		{
			String appName = paramElem.GetAttribute("name");
			m_windowName = appName;
		}
		paramElem = paramElem.GetNext();
	}
}
