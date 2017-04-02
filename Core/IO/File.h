#pragma once

#include "ArrayPtr.h"
#include "Serializer.h"
#include "Deserializer.h"
#include "../Object.h"


#ifdef ANDROID
#include <SDL/SDL_rwops.h>
#endif

namespace Sapphire
{

#ifdef ANDROID
	extern const char* APK;

	//检查给的的路径名是否在APK的资源目录中
#define SAPPHIRE_IS_ASSET(p) p.StartsWith(APK)
	// 截取APK前缀的宏
#ifdef ASSET_DIR_INDICATOR
#define SAPPHIRE_ASSET(p) p.Substring(5).Replaced("/", ASSET_DIR_INDICATOR "/").CString()
#else
#define SAPPHIRE_ASSET(p) p.Substring(5).CString()
#endif
#endif

	//文件的打开模式
	enum FileMode
	{
		FILE_READ = 0,
		FILE_WRITE,
		FILE_READWRITE
	};

	class PackageFile;

	//文件通过文件系统或者package文件打开
	class SAPPHIRE_API File : public Object, public Deserializer, public Serializer
	{
		SAPPHIRE_OBJECT(File, Object);

	public:
		 
		File(Context* context);
		/// 打开一个文件系统文件
		File(Context* context, const String& fileName, FileMode mode = FILE_READ);
		/// 从包文件打开 
		File(Context* context, PackageFile* package, const String& fileName);
		/// 析构
		virtual ~File();

		//从文件读取自己。返回实际读取的字节数
		virtual unsigned Read(void* dest, unsigned size);
		//  设置文件开始的偏移位置
		virtual unsigned Seek(unsigned position);
		// 写字节到文件，返回实际写入的字节数
		virtual unsigned Write(const void* data, unsigned size);

		// 返回文件名
		virtual const String& GetName() const { return fileName_; }

		//检查文件校验和
		virtual unsigned GetChecksum();

	   //打开一个文件系统文件。如果成功返回true
		bool Open(const String& fileName, FileMode mode = FILE_READ);
		//打开一个Package。如果成功返回true
		bool Open(PackageFile* package, const String& fileName);
		 
		void Close();
		// 刷新所有已缓冲的内容输出到文件
		void Flush();
	 
		//改变文件名。用于资源系统
		void SetName(const String& name);

		/// 返回打开模式
		FileMode GetMode() const { return mode_; }

		/// 返回是否打开
		bool IsOpen() const;

		/// 返回文件句柄
		void* GetHandle() const { return handle_; }

		/// 返回这个文件是否从一个创建
		bool IsPackaged() const { return offset_ != 0; }

	private:
		/// 文件名
		String fileName_;
		/// 打开模式.
		FileMode mode_;
		/// 文件句柄
		void* handle_;
#ifdef ANDROID
		// 安卓资源加载的SDL RWops 环境
		SDL_RWops* assetHandle_;
#endif
		 
		//读取安卓资源或加载压缩文件的缓冲区
		SharedArrayPtr<unsigned char> readBuffer_;
		///加载压缩文件的解压缩输入缓冲区
		SharedArrayPtr<unsigned char> inputBuffer_;
		// 读取缓冲区的偏移位置
		unsigned readBufferOffset_;
		/// 当前读取缓冲区的大小
		unsigned readBufferSize_;
		/// 一个package文件的起始位置，0表示常规文件
		unsigned offset_;
		/// 校验和内容
		unsigned checksum_;
		/// 压缩标志
		bool compressed_;
		///  是否同步读取
		bool readSyncNeeded_;
		///  是否同步写入
		bool writeSyncNeeded_;
	};

}