#pragma once

#include "../Context.h"
#include "JSONValue.h"


namespace Sapphire
{

	/// %Localization subsystem. Stores all the strings in all languages.
	class SAPPHIRE_API Localization : public Object
	{
		SAPPHIRE_OBJECT(Localization, Object);

	public:
		/// Construct.
		Localization(Context* context);
		/// Destruct. Free all resources.
		virtual ~Localization();

		/// Return the number of languages.
		int GetNumLanguages() const { return (int)languages_.Size(); }

		/// Return the index number of current language. The index is determined by the order of loading.
		int GetLanguageIndex() const { return languageIndex_; }

		/// Return the index number of language. The index is determined by the order of loading.
		int GetLanguageIndex(const String& language);
		/// Return the name of current language.
		String GetLanguage();
		/// Return the name of language.
		String GetLanguage(int index);
		/// Set current language.
		void SetLanguage(int index);
		/// Set current language.
		void SetLanguage(const String& language);
		/// Return a string in the current language. Returns String::EMPTY if id is empty. Returns id if translation is not found and logs a warning.
		String Get(const String& id);
		/// Clear all loaded strings.
		void Reset();
		/// Load strings from JSONValue.
		void LoadJSON(const JSONValue& source);
		/// Load strings from JSONFile. The file should be UTF8 without BOM.
		void LoadJSONFile(const String& name);

	private:
		/// Language names.
		Vector<String> languages_;
		/// Index of current language.
		int languageIndex_;
		/// Storage strings: <Language <StringId, Value> >.
		HashMap<StringHash, HashMap<StringHash, String> > strings_;
	};
}