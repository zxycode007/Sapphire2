#pragma once

#include "Resource.h"
#include "JSONValue.h"

namespace Sapphire
{
	/// JSON 文档资源
	class SAPPHIRE_API JSONFile : public Resource
	{
		SAPPHIRE_OBJECT(JSONFile, Resource);

	public:
		 
		JSONFile(Context* context);
		 
		virtual ~JSONFile();
		/// 注册对象工厂
		static void RegisterObject(Context* context);

		/// 从流中加载资源。可以从工作线程调用。如果成功返回true
		virtual bool BeginLoad(Deserializer& source);
		/// 用默认的缩进保存资源。如果成功返回true
		virtual bool Save(Serializer& dest) const;
		/// 用用户定义的缩进保存资源
		bool Save(Serializer& dest, const String& indendation) const;

		/// 返回root
		JSONValue& GetRoot() { return root_; }
		/// 返回root
		const JSONValue& GetRoot() const { return root_; }

	private:
		/// JSON root value.
		JSONValue root_;
	};

}