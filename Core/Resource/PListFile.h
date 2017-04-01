#pragma once


#include "../Resource/Resource.h"

namespace Sapphire
{

	class XMLElement;

	/// PList value 类型.
	enum PListValueType
	{
		PLVT_NONE = 0,
		PLVT_INT,
		PLVT_BOOL,
		PLVT_FLOAT,
		PLVT_STRING,
		PLVT_VALUEMAP,
		PLVT_VALUEVECTOR,
	};

	class PListValue;

	/// PList value map.
	typedef HashMap<String, PListValue> PListValueMap;

	/// PList value数组
	typedef Vector<PListValue> PListValueVector;

	/// PList value.
	class SAPPHIRE_API PListValue
	{
	public:
		 
		PListValue();
		 
		PListValue(int value);
		 
		PListValue(bool value);
		 
		PListValue(float value);
		 
		PListValue(const String& value);
		 
		PListValue(PListValueMap& valueMap);
		 
		PListValue(PListValueVector& valueVector);
		 
		PListValue(const PListValue& value);
		 
		~PListValue();

		 
		PListValue& operator =(const PListValue& rhs);

		/// 返回是否有效
		operator bool() const { return type_ != PLVT_NONE; }

	 
		void SetInt(int value);
	 
		void SetBool(bool value);
		 
		void SetFloat(float value);
		 
		void SetString(const String& value);
		 
		void SetValueMap(const PListValueMap& valueMap);
	 
		void SetValueVector(const PListValueVector& valueVector);

		/// 返回类型
		PListValueType GetType() const { return type_; }

		 
		int GetInt() const;
		 
		bool GetBool() const;
		 
		float GetFloat() const;
		 
		const String& GetString() const;
		 
		//从一个字符串转到IntRect
		IntRect GetIntRect() const;
		//从一个字符串转到IntVector
		IntVector2 GetIntVector2() const;
		 
		const PListValueMap& GetValueMap() const;
		 
		const PListValueVector& GetValueVector() const;

		/// 转换到value map  
		PListValueMap& ConvertToValueMap();
		/// 转换到value vector  
		PListValueVector& ConvertToValueVector();

	private:
		/// 重载.
		void Reset();

		 
		PListValueType type_;
	 
		union
		{
			int int_;
			bool bool_;
			float float_;
			String* string_;
			PListValueMap* valueMap_;
			PListValueVector* valueVector_;
		};
	};

	/// Property list (plist).
	class SAPPHIRE_API PListFile : public Resource
	{
		SAPPHIRE_OBJECT(PListFile, Resource);

	public:
		 
		PListFile(Context* context);
		 
		virtual ~PListFile();
		/// 注册对象工厂
		static void RegisterObject(Context* context);

		/// 从流中加载资源。可以从工作线程中调用，如果成功返回true
		virtual bool BeginLoad(Deserializer& source);

		/// 返回根
		const PListValueMap& GetRoot() const { return root_; }

	private:
		/// 加载字典
		bool LoadDict(PListValueMap& dict, const XMLElement& dictElem);
		/// 加载数组
		bool LoadArray(PListValueVector& array, const XMLElement& arrayElem);
		/// 加载值
		bool LoadValue(PListValue& value, const XMLElement& valueElem);

		/// 根字典 
		PListValueMap root_;
	};
}