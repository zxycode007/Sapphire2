#pragma once

#include "../Context.h"
#include "JSONValue.h"


namespace Sapphire
{

	/// %本地化子系统。保存所有语言的所有字符串
	class SAPPHIRE_API Localization : public Object
	{
		SAPPHIRE_OBJECT(Localization, Object);

	public:
		/// 构造器
		Localization(Context* context);
		/// 析构。释放所有资源
		virtual ~Localization();

		/// 返回语言的数量
		int GetNumLanguages() const { return (int)languages_.Size(); }

		/// 取得当前语言的索引号。 
		int GetLanguageIndex() const { return languageIndex_; }

		/// 取得当前语言的索引号。
		int GetLanguageIndex(const String& language);
		/// 返回当前语言的名字
		String GetLanguage();
		/// 返回当前语言名
		String GetLanguage(int index);
		/// 设置当前语言
		void SetLanguage(int index);
		/// 设置当前语言
		void SetLanguage(const String& language);
		/// 返回一个当前语言的字符。 返回String::Empty为空
		String Get(const String& id);
		/// 清空所有加载的字符串
		void Reset();
		/// 从JSONValue中加载字符串
		void LoadJSON(const JSONValue& source);
		/// 从JSON文件中加载字符串。文件应该是UTF8不带bom编码
		void LoadJSONFile(const String& name);

	private:
		/// 语言名
		Vector<String> languages_;
		/// 当前语言索引
		int languageIndex_;
		/// 存储字符串 <Language <StringId, Value> >.
		HashMap<StringHash, HashMap<StringHash, String> > strings_;
	};
}