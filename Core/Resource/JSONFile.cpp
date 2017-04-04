#include "../Sapphire/Predefined.h"

#include "../Container/ArrayPtr.h"
#include "../Core/Profiler.h"
#include "../Core/Context.h"
#include "../IO/Deserializer.h"
#include "../IO/Log.h"
#include "../Resource/JSONFile.h"
#include "../Resource/ResourceCache.h"

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>

#include "../DebugNew.h"

namespace Sapphire
{

	JSONFile::JSONFile(Context* context) :
		Resource(context)
	{
	}

	JSONFile::~JSONFile()
	{
	}

	void JSONFile::RegisterObject(Context* context)
	{
		context->RegisterFactory<JSONFile>();
	}

	// 转换rapidjsonValue到JSONValue
	static void ToJSONValue(JSONValue& jsonValue, const rapidjson::Value& rapidjsonValue)
	{
		using namespace rapidjson;
		switch (rapidjsonValue.GetType())
		{
		case kNullType:
			//null类型
			jsonValue.SetType(JSON_NULL);
			break;

		case kFalseType:
			jsonValue = false;
			break;

		case kTrueType:
			jsonValue = true;
			break;

		case kNumberType:
			if (rapidjsonValue.IsInt())
				jsonValue = rapidjsonValue.GetInt();
			else if (rapidjsonValue.IsUint())
				jsonValue = rapidjsonValue.GetUint();
			else
				jsonValue = rapidjsonValue.GetDouble();
			break;

		case kStringType:
			jsonValue = rapidjsonValue.GetString();
			break;

		case kArrayType:
		{
			jsonValue.Resize(rapidjsonValue.Size());  //设置数组空间
			for (unsigned i = 0; i < rapidjsonValue.Size(); ++i)
			{
				ToJSONValue(jsonValue[i], rapidjsonValue[i]);  //依次转换
			}
		}
		break;

		case kObjectType:   //对象类型
		{
			jsonValue.SetType(JSON_OBJECT);
			//rapidJson遍历成员
			for (rapidjson::Value::ConstMemberIterator i = rapidjsonValue.MemberBegin(); i != rapidjsonValue.MemberEnd(); ++i)
			{
				//依次转换
				JSONValue& value = jsonValue[String(i->name.GetString())];  //创建
				ToJSONValue(value, i->value);
			}
		}
		break;

		default:
			break;
		}
	}

	//开始从流中加载
	bool JSONFile::BeginLoad(Deserializer& source)
	{
		//获取数据大小
		unsigned dataSize = source.GetSize();
		if (!dataSize && !source.GetName().Empty())
		{
			SAPPHIRE_LOGERROR("Zero sized JSON data in " + source.GetName());
			return false;
		}
		//创建缓冲区
		SharedArrayPtr<char> buffer(new char[dataSize + 1]); 
		if (source.Read(buffer.Get(), dataSize) != dataSize) //读取流中数据到缓冲区中
			return false;
		buffer[dataSize] = '\0';

		rapidjson::Document document;       //对buffer进行解析
		if (document.Parse<0>(buffer).HasParseError())
		{
			SAPPHIRE_LOGERROR("Could not parse JSON data from " + source.GetName());
			return false;
		}
		//从根节点开始解析
		ToJSONValue(root_, document); 
		//设置内存使用量
		SetMemoryUse(dataSize);

		return true;
	}

	//将JSONValue转到rapidjsonValue
	static void ToRapidjsonValue(rapidjson::Value& rapidjsonValue, const JSONValue& jsonValue, rapidjson::MemoryPoolAllocator<>& allocator)
	{
		switch (jsonValue.GetValueType())
		{
		case JSON_NULL:
			rapidjsonValue.SetNull();
			break;

		case JSON_BOOL:
			rapidjsonValue.SetBool(jsonValue.GetBool());
			break;

		case JSON_NUMBER:
		{
			switch (jsonValue.GetNumberType())
			{
			case JSONNT_INT:
				rapidjsonValue.SetInt(jsonValue.GetInt());
				break;

			case JSONNT_UINT:
				rapidjsonValue.SetUint(jsonValue.GetUInt());
				break;

			default:
				rapidjsonValue.SetDouble(jsonValue.GetDouble());
				break;
			}
		}
		break;

		case JSON_STRING:
			rapidjsonValue.SetString(jsonValue.GetCString(), allocator);
			break;

		case JSON_ARRAY:
		{
			const JSONArray& jsonArray = jsonValue.GetArray();

			rapidjsonValue.SetArray();
			rapidjsonValue.Reserve(jsonArray.Size(), allocator);

			for (unsigned i = 0; i < jsonArray.Size(); ++i)
			{
				rapidjson::Value value;
				rapidjsonValue.PushBack(value, allocator);
				ToRapidjsonValue(rapidjsonValue[i], jsonArray[i], allocator);
			}
		}
		break;

		case JSON_OBJECT:
		{
			const JSONObject& jsonObject = jsonValue.GetObject();

			rapidjsonValue.SetObject();  //设置rapidjson对象为Object类型
			for (JSONObject::ConstIterator i = jsonObject.Begin(); i != jsonObject.End(); ++i)
			{
				const char* name = i->first_.CString();
				rapidjson::Value value;
				rapidjsonValue.AddMember(name, value, allocator);
				ToRapidjsonValue(rapidjsonValue[name], i->second_, allocator);
			}
		}
		break;

		default:
			break;
		}
	}
	//保存到流
	bool JSONFile::Save(Serializer& dest) const
	{
		return Save(dest, "\t");
	}

	bool JSONFile::Save(Serializer& dest, const String& indendation) const
	{
		using namespace rapidjson;
		rapidjson::Document document;
		ToRapidjsonValue(document, root_, document.GetAllocator());

		StringBuffer buffer;
		PrettyWriter<StringBuffer> writer(buffer, &(document.GetAllocator()));
		writer.SetIndent(!indendation.Empty() ? indendation.Front() : '\0', indendation.Length());

		document.Accept(writer);
		unsigned size = (unsigned)buffer.GetSize();
		return dest.Write(buffer.GetString(), size) == size;
	}
}