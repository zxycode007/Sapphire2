#pragma once


#include "ArrayPtr.h"
#include "Resource.h"

struct SDL_Surface;

namespace Sapphire
{
	static const int COLOR_LUT_SIZE = 16;

	/// 支持的压缩图像格式
	enum CompressedFormat
	{
		CF_NONE = 0,
		CF_RGBA,
		CF_DXT1,
		CF_DXT3,
		CF_DXT5,
		CF_ETC1,
		CF_PVRTC_RGB_2BPP,
		CF_PVRTC_RGBA_2BPP,
		CF_PVRTC_RGB_4BPP,
		CF_PVRTC_RGBA_4BPP,
	};

	/// 压缩图像mip级别
	struct CompressedLevel
	{
		 
		CompressedLevel() :
			data_(0),
			width_(0),
			height_(0),
			depth_(0),
			blockSize_(0),
			dataSize_(0),
			rowSize_(0),
			rows_(0)
		{
		}

		/// 解压到RGBA。 目标缓冲区必须是width * height * 4 bytes. 如果成功返回true
		bool Decompress(unsigned char* dest);

		/// 压缩的图像数据
		unsigned char* data_;
		/// 压缩格式
		CompressedFormat format_;
		 
		int width_;
		 
		int height_;
	 
		int depth_;
		/// 块大小
		unsigned blockSize_;
		/// 总的块大小
		unsigned dataSize_;
		/// 行大小
		unsigned rowSize_;
		/// 行数
		unsigned rows_;
	};

	/// %Image 资源.
	class SAPPHIRE_API Image : public Resource
	{
		SAPPHIRE_OBJECT(Image, Resource);

	public:
		 
		Image(Context* context);
		 
		virtual ~Image();
		/// 注册对象工厂
		static void RegisterObject(Context* context);

		/// 从流中加载资源。 
		virtual bool BeginLoad(Deserializer& source);
		/// 保存图像到一个流中。 (不管原来的格式，图片保存为png，不支持压缩图像数据）
		virtual bool Save(Serializer& dest) const;

		/// 设置2d大小和颜色分类的数量.旧的图像数据会被破坏并且新的图像数据会是未定义。
		bool SetSize(int width, int height, unsigned components);
		///  设置3d大小，深度和颜色分类的数量.旧的图像数据会被破坏并且新的图像数据会是未定义。
		bool SetSize(int width, int height, int depth, unsigned components);
		///  设置新的图像数据
		void SetData(const unsigned char* pixelData);
		/// 设置一个2d像素的颜色
		void SetPixel(int x, int y, const Color& color);
		/// 设置一个3d像素的颜色
		void SetPixel(int x, int y, int z, const Color& color);
		/// 设置一个2d像素的整形颜色，R分类在最低8位
		void SetPixelInt(int x, int y, unsigned uintColor);
		/// 设置一个3d像素的整形颜色，R分类在最低8位
		void SetPixelInt(int x, int y, int z, unsigned uintColor);
		/// 加载颜色查找表。
		bool LoadColorLUT(Deserializer& source);
		/// 水平翻转图像
		bool FlipHorizontal();
		/// 垂直翻转图像
		bool FlipVertical();
		///  用双线性重新采样改变图像大小
		bool Resize(int width, int height);
		/// 用一个颜色清空图像
		void Clear(const Color& color);
		/// 用一个整形颜色清空图像.R分量在最低8位
		void ClearInt(unsigned uintColor);
		 
		bool SaveBMP(const String& fileName) const;
		 
		bool SavePNG(const String& fileName) const;
		 
		bool SaveTGA(const String& fileName) const;
		 
		bool SaveJPG(const String& fileName, int quality) const;
		/// 该纹理是否用作立方体贴图。只针对DDS
		bool IsCubemap() const { return cubemap_; }
		/// 该纹理是否用作volume texture。 只针对DDS
		bool IsArray() const { return array_; }
		/// 该纹理是否在sRGB颜色空间，只针对DDS
		bool IsSRGB() const { return sRGB_; }

		/// 返回一个2d像素颜色
		Color GetPixel(int x, int y) const;
		/// 返回一个2d像素颜色
		Color GetPixel(int x, int y, int z) const;
		/// 返回一个整形2d像素颜色  R分量在最低8位
		unsigned GetPixelInt(int x, int y) const;
		/// 返回一个整形3d像素颜色  R分量在最低8位
		unsigned GetPixelInt(int x, int y, int z) const;
		/// 返回一个双线性采样的2d像素颜色。 X和Y的范围0-1
		Color GetPixelBilinear(float x, float y) const;
		/// 返回一个三线性采样的3d像素颜色。 X和Y的范围0-1
		Color GetPixelTrilinear(float x, float y, float z) const;

		int GetWidth() const { return width_; }

		int GetHeight() const { return height_; }

		int GetDepth() const { return depth_; }

		/// 返回颜色分量数
		unsigned GetComponents() const { return components_; }

		/// 返现像素数据指针
		unsigned char* GetData() const { return data_; }

		/// 返回是否压缩
		bool IsCompressed() const { return compressedFormat_ != CF_NONE; }

		/// 返回压缩格式 
		CompressedFormat GetCompressedFormat() const { return compressedFormat_; }

		/// 返回压缩mip 级别数
		unsigned GetNumCompressedLevels() const { return numCompressedLevels_; }

		/// 返回下一个双线性滤波的mip级别
		SharedPtr<Image> GetNextLevel() const;
		/// 返回立方体贴图数组下一个临近的兄弟图像
		SharedPtr<Image> GetNextSibling() const { return nextSibling_; }
		/// 返回图像转换到4分量RGBA.
		SharedPtr<Image> ConvertToRGBA() const;
		/// 返回一个压缩的mip级别
		CompressedLevel GetCompressedLevel(unsigned index) const;
		/// 获取从一个图像中定义的区域的子图像，如果失败返回null.不支持3d图像。必须手动释放子图像
		Image* GetSubimage(const IntRect& rect) const;
		/// 从一个图像返回SDL surface。如果失败返回null。 只支持RGB图像。 指定区域将返回部分图像，必须手动释放内存
		SDL_Surface* GetSDLSurface(const IntRect& rect = IntRect::ZERO) const;
		/// 预先计数mip级别， 用于异步纹理加载
		void PrecalculateLevels();

	private:
		/// 用stb_image 解码图像
		static unsigned char* GetImageData(Deserializer& source, int& width, int& height, unsigned& components);
		/// 释放一个图像文件的像素数据
		static void FreeImageData(unsigned char* pixelData);

		int width_;

		int height_;

		int depth_;
		/// 颜色分量数
		unsigned components_;
		/// 压缩mip级别数
		unsigned numCompressedLevels_;
		/// 如果是dds，立方体状态
		bool cubemap_;
		/// 纹理数组状态 DDS
		bool array_;
		/// 数据是sRGB
		bool sRGB_;
		/// 压缩格式
		CompressedFormat compressedFormat_;
		/// 像素数据
		SharedArrayPtr<unsigned char> data_;
		/// 预计算mip级别图像
		SharedPtr<Image> nextLevel_;
		/// 下一个纹理数据或立方体纹理图像
		SharedPtr<Image> nextSibling_;
	};
}