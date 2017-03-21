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
		if (SAPPHIRE_IS_ASSET(fileName))
		{
			SAPPHIRE_LOGERROR("Package files within the apk are not supported on Android");
			return false;
		}
#endif

		SharedPtr<File> file(new File(context_, fileName));
		if (!file->IsOpen())
			return false;

		// 检查ID，并读取目录
		file->Seek(startOffset);
		//读取文件ID (文件头0-4字节)
		String id = file->ReadFileID();
		if (id != "UPAK" && id != "ULZ4")
		{
			// 如果起始偏移不能明确指定，那么尝试从文件末尾读取包大小来知道需要倒回到包起始要多少字节
			if (!startOffset)
			{
				unsigned fileSize = file->GetSize();
				file->Seek((unsigned)(fileSize - sizeof(unsigned)));
				//从末尾读取包大小，来确定新起始偏移地址
				unsigned newStartOffset = fileSize - file->ReadUInt();
				
				if (newStartOffset < fileSize)
				{
					//合法
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
		//依次读取文件入口
		for (unsigned i = 0; i < numFiles; ++i)
		{
			String entryName = file->ReadString();
			PackageEntry newEntry;
			//文件在包中偏移量
			newEntry.offset_ = file->ReadUInt() + startOffset;
			//文件大小
			newEntry.size_ = file->ReadUInt();
			//文件校验和
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