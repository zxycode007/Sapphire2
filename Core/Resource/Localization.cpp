#include "../Sapphire/Predefined.h"

#include "../Resource/Localization.h"
#include "../Resource/ResourceCache.h"
#include "../Resource/JSONFile.h"
#include "../Resource/ResourceEvents.h"
#include "../IO/Log.h"

#include "../DebugNew.h"


namespace Sapphire
{


	Localization::Localization(Context* context) :
		Object(context),
		languageIndex_(-1)
	{
	}

	Localization::~Localization()
	{
	}

	int Localization::GetLanguageIndex(const String& language)
	{
		if (language.Empty())
		{
			SAPPHIRE_LOGWARNING("Localization::GetLanguageIndex(language): language name is empty");
			return -1;
		}
		if (GetNumLanguages() == 0)
		{
			SAPPHIRE_LOGWARNING("Localization::GetLanguageIndex(language): no loaded languages");
			return -1;
		}
		for (int i = 0; i < GetNumLanguages(); i++)
		{
			if (languages_[i] == language)
				return i;
		}
		return -1;
	}

	String Localization::GetLanguage()
	{
		if (languageIndex_ == -1)
		{
			SAPPHIRE_LOGWARNING("Localization::GetLanguage(): no loaded languages");
			return String::EMPTY;
		}
		return languages_[languageIndex_];
	}

	String Localization::GetLanguage(int index)
	{
		if (GetNumLanguages() == 0)
		{
			SAPPHIRE_LOGWARNING("Localization::GetLanguage(index): no loaded languages");
			return String::EMPTY;
		}
		if (index < 0 || index >= GetNumLanguages())
		{
			SAPPHIRE_LOGWARNING("Localization::GetLanguage(index): index out of range");
			return String::EMPTY;
		}
		return languages_[index];
	}

	void Localization::SetLanguage(int index)
	{
		if (GetNumLanguages() == 0)
		{
			SAPPHIRE_LOGWARNING("Localization::SetLanguage(index): no loaded languages");
			return;
		}
		if (index < 0 || index >= GetNumLanguages())
		{
			SAPPHIRE_LOGWARNING("Localization::SetLanguage(index): index out of range");
			return;
		}
		if (index != languageIndex_)
		{
			//设置当前语言索引
			languageIndex_ = index;
			//通知语言改变
			VariantMap& eventData = GetEventDataMap();
			SendEvent(E_CHANGELANGUAGE, eventData);
		}
	}

	void Localization::SetLanguage(const String& language)
	{
		if (language.Empty())
		{
			SAPPHIRE_LOGWARNING("Localization::SetLanguage(language): language name is empty");
			return;
		}
		if (GetNumLanguages() == 0)
		{
			SAPPHIRE_LOGWARNING("Localization::SetLanguage(language): no loaded languages");
			return;
		}
		int index = GetLanguageIndex(language);
		if (index == -1)
		{
			SAPPHIRE_LOGWARNING("Localization::SetLanguage(language): language not found");
			return;
		}
		SetLanguage(index);
	}

	String Localization::Get(const String& id)
	{
		if (id.Empty())
			return String::EMPTY;
		if (GetNumLanguages() == 0)
		{
			SAPPHIRE_LOGWARNING("Localization::Get(id): no loaded languages");
			return id;
		}
		//语言字符串
		String result = strings_[StringHash(GetLanguage())][StringHash(id)];
		if (result.Empty())
		{
			SAPPHIRE_LOGWARNING("Localization::Get(\"" + id + "\") not found translation, language=\"" + GetLanguage() + "\"");
			return id;
		}
		return result;
	}

	void Localization::Reset()
	{
		languages_.Clear();
		languageIndex_ = -1;
		strings_.Clear();
	}

	//加载JSON
	void Localization::LoadJSON(const JSONValue& source)
	{
		for (JSONObject::ConstIterator i = source.Begin(); i != source.End(); ++i)
		{
			//取id
			String id = i->first_;
			if (id.Empty())
			{
				SAPPHIRE_LOGWARNING("Localization::LoadJSON(source): string ID is empty");
				continue;
			}
			const JSONValue& langs = i->second_;
			//遍历JSON
			for (JSONObject::ConstIterator j = langs.Begin(); j != langs.End(); ++j)
			{
				//语言名
				const String& lang = j->first_;
				if (lang.Empty())
				{
					SAPPHIRE_LOGWARNING("Localization::LoadJSON(source): language name is empty, string ID=\"" + id + "\"");
					continue;
				}
				//值
				const String& string = j->second_.GetString();
				if (string.Empty())
				{
					SAPPHIRE_LOGWARNING(
						"Localization::LoadJSON(source): translation is empty, string ID=\"" + id + "\", language=\"" + lang + "\"");
					continue;
				}
				if (strings_[StringHash(lang)][StringHash(id)] != String::EMPTY)
				{
					SAPPHIRE_LOGWARNING(
						"Localization::LoadJSON(source): override translation, string ID=\"" + id + "\", language=\"" + lang + "\"");
				}
				strings_[StringHash(lang)][StringHash(id)] = string;
				if (!languages_.Contains(lang))
					languages_.Push(lang);
				if (languageIndex_ == -1)
					languageIndex_ = 0;
			}
		}
	}

	void Localization::LoadJSONFile(const String& name)
	{
		ResourceCache* cache = GetSubsystem<ResourceCache>();
		JSONFile* jsonFile = cache->GetResource<JSONFile>(name);
		if (jsonFile)
			LoadJSON(jsonFile->GetRoot());   //从根文件开始读取JSON
	}

}