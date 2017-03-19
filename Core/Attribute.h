#pragma once

#include "Ptr.h"
#include "Variant.h"

namespace Sapphire
{

	//只在编辑器中显示的属性，不能序列化
	static const unsigned AM_EDIT = 0x0;
	//用于文件序列化的属性
	static const unsigned AM_FILE = 0x1;
	//用于网络复制的属性
	static const unsigned AM_NET = 0x2;
	/// 同时用于文件序列化和网络复制的属性（默认）
	static const unsigned AM_DEFAULT = 0x3;
	/// 使用最新的数据组替代在网络复制中的增量更新
	static const unsigned AM_LATESTDATA = 0x4;
	/// 不在编辑器中显示的属性
	static const unsigned AM_NOEDIT = 0x8;
	/// 属性是一个节点ID并且可能需要重写
	static const unsigned AM_NODEID = 0x10;
	/// 属性是一个组件ID并且可能需要重写
	static const unsigned AM_COMPONENTID = 0x20;
	/// 属性是一个节点ID向量，首个元素是节点数
	static const unsigned AM_NODEIDVECTOR = 0x40;

	class Serializable;

	/// 执行属性访问器的抽象基类
	class SAPPHIRE_API AttributeAccessor : public RefCounted
	{
	public:
		/// 获取属性
		virtual void Get(const Serializable* ptr, Variant& dest) const = 0;
		/// 设置属性
		virtual void Set(Serializable* ptr, const Variant& src) = 0;
	};

	/// 自动序列化可变类型的描述信息
	struct AttributeInfo
	{
		
		AttributeInfo() :
			type_(VAR_NONE),
			offset_(0),
			enumNames_(0),
			mode_(AM_DEFAULT),
			ptr_(0)
		{
		}

		/// 构造偏移属性 
		AttributeInfo(VariantType type, const char* name, size_t offset, const Variant& defaultValue, unsigned mode) :
			type_(type),
			name_(name),
			offset_((unsigned)offset),
			enumNames_(0),
			defaultValue_(defaultValue),
			mode_(mode),
			ptr_(0)
		{
		}

		/// 构造偏移枚举属性
		AttributeInfo(const char* name, size_t offset, const char** enumNames, const Variant& defaultValue, unsigned mode) :
			type_(VAR_INT),
			name_(name),
			offset_((unsigned)offset),
			enumNames_(enumNames),
			defaultValue_(defaultValue),
			mode_(mode),
			ptr_(0)
		{
		}

		/// 构造访问器属性
		AttributeInfo(VariantType type, const char* name, AttributeAccessor* accessor, const Variant& defaultValue, unsigned mode) :
			type_(type),
			name_(name),
			offset_(0),
			enumNames_(0),
			accessor_(accessor),
			defaultValue_(defaultValue),
			mode_(mode),
			ptr_(0)
		{
		}

		 
		// 构造访问器枚举属性
		AttributeInfo(const char* name, AttributeAccessor* accessor, const char** enumNames, const Variant& defaultValue,
			unsigned mode) :
			type_(VAR_INT),
			name_(name),
			offset_(0),
			enumNames_(enumNames),
			accessor_(accessor),
			defaultValue_(defaultValue),
			mode_(mode),
			ptr_(0)
		{
		}

		//属性类型
		VariantType type_;
		//属性名
		String name_;
		 
		// 对象的起始偏移字节数
		unsigned offset_;
		/// 枚举名
		const char** enumNames_;
		/// 访问模式的帮助对象
		SharedPtr<AttributeAccessor> accessor_;
		// 网络复制的默认值
		Variant defaultValue_;
		/// 属性模式：是否使用序列化，网络复制，或一起使用
		unsigned mode_;
		// 在序列化之外的地方使用属性数据指针
		void* ptr_;
	};
}
