#pragma once
#include "../Container/Vector.h"
#include "../Container/HashMap.h"
#include "Str.h"
#include "../Variant.h"
#include "../Context.h"

namespace Sapphire
{
	/// JSON 值类型
	enum JSONValueType
	{
		 
		JSON_NULL = 0,
	 
		JSON_BOOL,
	 
		JSON_NUMBER,
		 
		JSON_STRING,
	 
		JSON_ARRAY,
		//无序集合键值对
		JSON_OBJECT,
	};

	/// JSON数字类型
	enum JSONNumberType
	{
		/// 不是一个数字
		JSONNT_NAN = 0,
		/// 整形.
		JSONNT_INT,
		/// 无符号整形
		JSONNT_UINT,
		/// 单精度或双精度浮点
		JSONNT_FLOAT_DOUBLE,
	};

	class JSONValue;

	/// JSON 数组类型
	typedef Vector<JSONValue> JSONArray;
	/// JSON Object
	typedef HashMap<String, JSONValue> JSONObject;
	/// JSON object iterator.
	typedef JSONObject::Iterator JSONObjectIterator;
	/// Constant JSON object iterator.
	typedef JSONObject::ConstIterator ConstJSONObjectIterator;

	/// JSON value 
	class SAPPHIRE_API JSONValue
	{
	public:
		 
		JSONValue() :
			type_(0)
		{
		}
		 
		JSONValue(bool value) :
			type_(0)
		{
			*this = value;
		}
		 
		JSONValue(int value) :
			type_(0)
		{
			*this = value;
		}
		 
		JSONValue(unsigned value) :
			type_(0)
		{
			*this = value;
		}
		 
		JSONValue(float value) :
			type_(0)
		{
			*this = value;
		}
		 
		JSONValue(double value) :
			type_(0)
		{
			*this = value;
		}
		 
		JSONValue(const String& value) :
			type_(0)
		{
			*this = value;
		}
		 
		JSONValue(const char* value) :
			type_(0)
		{
			*this = value;
		}
		 
		JSONValue(const JSONArray& value) :
			type_(0)
		{
			*this = value;
		}
		 
		JSONValue(const JSONObject& value) :
			type_(0)
		{
			*this = value;
		}
		 
		JSONValue(const JSONValue& value) :
			type_(0)
		{
			*this = value;
		}
		 
		~JSONValue()
		{
			SetType(JSON_NULL);
		}

	 
		JSONValue& operator =(bool rhs);
		 
		JSONValue& operator =(int rhs);
		 
		JSONValue& operator =(unsigned rhs);
		 
		JSONValue& operator =(float rhs);
		 
		JSONValue& operator =(double rhs);
		 
		JSONValue& operator =(const String& rhs);
		 
		JSONValue& operator =(const char* rhs);
		 
		JSONValue& operator =(const JSONArray& rhs);
		 
		JSONValue& operator =(const JSONObject& rhs);
		 
		JSONValue& operator =(const JSONValue& rhs);

		 
		JSONValueType GetValueType() const;
		 
		JSONNumberType GetNumberType() const;
		 
		bool IsNull() const { return GetValueType() == JSON_NULL; }
	 
		bool IsBool() const { return GetValueType() == JSON_BOOL; }
		 
		bool IsNumber() const { return GetValueType() == JSON_NUMBER; }
		 
		bool IsString() const { return GetValueType() == JSON_STRING; }
		 
		bool IsArray() const { return GetValueType() == JSON_ARRAY; }
		 
		bool IsObject() const { return GetValueType() == JSON_OBJECT; }

		 
		bool GetBool() const { return IsBool() ? boolValue_ : false; }
		 
		int GetInt() const { return IsNumber() ? (int)numberValue_ : 0; }
		 
		unsigned GetUInt() const { return IsNumber() ? (unsigned)numberValue_ : 0; }
		 
		float GetFloat() const { return IsNumber() ? (float)numberValue_ : 0.0f; }
		 
		double GetDouble() const { return IsNumber() ? numberValue_ : 0.0; }
		 
		const String& GetString() const { return IsString() ? *stringValue_ : String::EMPTY; }
	 
		const char* GetCString() const { return IsString() ? stringValue_->CString() : 0; }
		 
		const JSONArray& GetArray() const { return IsArray() ? *arrayValue_ : emptyArray; }
		 
		const JSONObject& GetObject() const { return IsObject() ? *objectValue_ : emptyObject; }

		// JSON 函数
		/// 返回指定索引JSON值
		JSONValue& operator [](unsigned index);
		/// 返回指定索引JSON值
		const JSONValue& operator [](unsigned index) const;
		/// 添加JSON 值
		void Push(const JSONValue& value);
		/// 移除最后添加的JSON 值
		void Pop();
		/// 在指定位置插入一个JSON值
		void Insert(unsigned pos, const JSONValue& value);
		/// 删除一定范围的JSON值
		void Erase(unsigned pos, unsigned length = 1);
		/// 重设数值大小
		void Resize(unsigned newSize);
		/// 返回大小
		unsigned Size() const;

		// JSON 对象函数
		/// 返回key对应的JSON 值
		JSONValue& operator [](const String& key);
		/// 返回Key对应的JSON值
		const JSONValue& operator [](const String& key) const;
		/// 设置JSON值
		void Set(const String& key, const JSONValue& value);
		/// 返回JSON值
		const JSONValue& Get(const String& key) const;
		/// 删除一个pair
		bool Erase(const String& key);
		/// 返回是否包含key
		bool Contains(const String& key) const;
		/// 返回起始迭代器
		JSONObjectIterator Begin();
	 
		ConstJSONObjectIterator Begin() const;
	 
		JSONObjectIterator End();
		 
		ConstJSONObjectIterator End() const;

		/// 清空数组
		void Clear();

		///设置值类型和数值类型 
		void SetType(JSONValueType valueType, JSONNumberType numberType = JSONNT_NAN);

		/// 设置variant context必须提供资源引用
		void SetVariant(const Variant& variant, Context* context = 0);
		/// 返回variant
		Variant GetVariant() const;
		/// 设置variant，context必须提供资源引用
		void SetVariantValue(const Variant& variant, Context* context = 0);
		/// 返回variant类型
		Variant GetVariantValue(VariantType type) const;
		/// 设置 variant map.
		void SetVariantMap(const VariantMap& variantMap, Context* context = 0);
		/// 返回variant map
		VariantMap GetVariantMap() const;
		/// 设置variant 数组， context必须提供资源引用
		void SetVariantVector(const VariantVector& variantVector, Context* context = 0);
		/// 返回variant 数组
		VariantVector GetVariantVector() const;

		/// 空JSON
		static const JSONValue EMPTY;
		/// 空JSON数组
		static const JSONArray emptyArray;
		/// 空JSON对象
		static const JSONObject emptyObject;

	private:
		/// 类型.
		unsigned type_;
		union
		{
			/// Boolean.
			bool boolValue_;
			/// 数值
			double numberValue_;
			/// 字符串
			String* stringValue_;
			/// 数组
			JSONArray* arrayValue_;
			/// 对象
			JSONObject* objectValue_;
		};
	};
}