#include "Predefined.h"

#include "File.h"
#include "Log.h"
#include "PackageFile.h"


namespace Sapphire
{

	PackageFile::PackageFile(Context* context) :
		Object(context),
		totalSize_(0),
		checksum_(0),
		compressed_(false)
	{
	}

	PackageFile::PackageFile(Context* context, const String& fileName, unsigned startOffset) :
		Object(context),
		totalSize_(0),
		checksum_(0),
		compressed_(false)
	{
		Open(fileName, startOffset);
	}

	PackageFile::~PackageFile()
	{
	}

	bool PackageFile::Open(const String& fileName, unsigned startOffset)
	{
#ifdef ANDROID
		if (URHO3D_IS_ASSET(fileName))
		{
			URHO3D_LOGERROR("Package files within the apk are not supported on Android");
			return false;
		}
#endif

		SharedPtr<File> file(new File(context_, fileName));
		if (!file->IsOpen())
			return false;

		// Check ID, then read the directory
		file->Seek(startOffset);
		String id = file->ReadFileID();
		if (id != "UPAK" && id != "ULZ4")
		{
			// If start offset has not been explicitly specified, also try to read package size from the end of file
			// to know how much we must rewind to find the package start
			if (!startOffset)
			{
				unsigned fileSize = file->GetSize();
				file->Seek((unsigned)(fileSize - sizeof(unsigned)));
				unsigned newStartOffset = fileSize - file->ReadUInt();
				if (newStartOffset < fileSize)
				{
					startOffset = newStartOffset;
					file->Seek(startOffset);
					id = file->ReadFileID();
				}
			}

			if (id != "UPAK" && id != "ULZ4")
			{
				SAPPHIRE_LOGERROR(fileName + " is not a valid package file");
				return false;
			}
		}

		fileName_ = fileName;
		nameHash_ = fileName_;
		totalSize_ = file->GetSize();
		compressed_ = id == "ULZ4";

		unsigned numFiles = file->ReadUInt();
		checksum_ = file->ReadUInt();

		for (unsigned i = 0; i < numFiles; ++i)
		{
			String entryName = file->ReadString();
			PackageEntry newEntry;
			newEntry.offset_ = file->ReadUInt() + startOffset;
			newEntry.size_ = file->ReadUInt();
			newEntry.checksum_ = file->ReadUInt();
			if (!compressed_ && newEntry.offset_ + newEntry.size_ > totalSize_)
			{
				SAPPHIRE_LOGERROR("File entry " + entryName + " outside package file");
				return false;
			}
			else
				entries_[entryName] = newEntry;
		}

		return true;
	}

	bool PackageFile::Exists(const String& fileName) const
	{
		bool found = entries_.Find(fileName) != entries_.End();

#ifdef WIN32
		// On Windows perform a fallback case-insensitive search
		if (!found)
		{
			for (HashMap<String, PackageEntry>::ConstIterator i = entries_.Begin(); i != entries_.End(); ++i)
			{
				if (!i->first_.Compare(fileName, false))
				{
					found = true;
					break;
				}
			}
		}
#endif

		return found;
	}

	const PackageEntry* PackageFile::GetEntry(const String& fileName) const
	{
		HashMap<String, PackageEntry>::ConstIterator i = entries_.Find(fileName);
		if (i != entries_.End())
			return &i->second_;

#ifdef WIN32
		// On Windows perform a fallback case-insensitive search
		else
		{
			for (HashMap<String, PackageEntry>::ConstIterator j = entries_.Begin(); j != entries_.End(); ++j)
			{
				if (!j->first_.Compare(fileName, false))
					return &j->second_;
			}
		}
#endif

		return 0;
	}
}