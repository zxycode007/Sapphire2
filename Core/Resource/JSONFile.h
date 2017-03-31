#pragma once

#include "Resource.h"
#include "JSONValue.h"

namespace Sapphire
{
	/// JSON document resource.
	class SAPPHIRE_API JSONFile : public Resource
	{
		SAPPHIRE_OBJECT(JSONFile, Resource);

	public:
		/// Construct.
		JSONFile(Context* context);
		/// Destruct.
		virtual ~JSONFile();
		/// Register object factory.
		static void RegisterObject(Context* context);

		/// Load resource from stream. May be called from a worker thread. Return true if successful.
		virtual bool BeginLoad(Deserializer& source);
		/// Save resource with default indentation (one tab). Return true if successful.
		virtual bool Save(Serializer& dest) const;
		/// Save resource with user-defined indentation, only the first character (if any) of the string is used and the length of the string defines the character count. Return true if successful.
		bool Save(Serializer& dest, const String& indendation) const;

		/// Return root value.
		JSONValue& GetRoot() { return root_; }
		/// Return root value.
		const JSONValue& GetRoot() const { return root_; }

	private:
		/// JSON root value.
		JSONValue root_;
	};

}