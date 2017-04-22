#pragma once
#include <GLFW\glfw3.h>
#include "Object.h"

namespace Sapphire
{
	typedef GLFWwindow WindowsHandle ;
	class ShaderManager;
	class Texture;


	class  SAPPHIRE_API  VideoDriver : public Object
	{
		SAPPHIRE_OBJECT(VideoDriver, Object);

	public:

		VideoDriver(Context* context);
		~VideoDriver();

		void  Initialize();
		void  SetWindow(WindowsHandle* window);
		WindowsHandle*  GetWindow();
		void   SetShaderManager(Sapphire::ShaderManager* manager);
		ShaderManager*  GetShaderManager();

		void   BeginFrame();
		void   render();
		void   EndFrame();

		void   prepareDraw();

		void   release();
		int    getWidth();
		void   setWidth(int width);
		void   setClearColor(Color c);
		Color  getClearColor();
		void   setWindowName(String name);
		String getWindowName();
		//是否支持OpenGL3.0
		static bool   getGL3Support(){ return m_gl3support; };

	private:

		void createWindow();
		//设置纹理 
		void setTexture(int index, Texture* tex);

		SharedPtr<ShaderManager>  mShaderManager;
		SharedPtr<WindowsHandle>  mWindowHandle;
		int  m_windth;
		int  m_height;
		bool      m_canResize;
		Color     m_clearColor;
		String    m_windowName;
		//渲染使用的纹理
		Texture*  m_textures[SAPPHIRE_MAX_TEXTURE_UNIT];
		//激活的纹理索引
		int       m_activeTexture;
		//纹理类型
		unsigned  m_textrueTypes[SAPPHIRE_MAX_TEXTURE_UNIT];
		//OpenGL3.0支持标志
		static bool      m_gl3support;

	};
}
