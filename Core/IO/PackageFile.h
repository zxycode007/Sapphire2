#pragma once

#include "../Object.h"


namespace  Sapphire
{

	/// 打包文件入口
	struct PackageEntry
	{
		/// 起始偏移地址
		unsigned offset_;
		/// 文件大小
		unsigned size_;
		/// 文件校验和
		unsigned checksum_;
	};

	/// 保存目录树到一个文件包
	class SAPPHIRE_API PackageFile : public Object
	{
		SAPPHIRE_OBJECT(PackageFile, Object);

	public:
		 
		PackageFile(Context* context);
		/// 构造并打开
		PackageFile(Context* context, const String& fileName, unsigned startOffset = 0);
		///  析构
		virtual ~PackageFile();

		/// 打开包文件。 如果为成功true
		bool Open(const String& fileName, unsigned startOffset = 0);
		/// 检查包文件是否存在。 文件名大小写敏感取决于平台
		bool Exists(const String& fileName) const;
		/// 找到符合名字的文件入口。 如果为空没找到
		const PackageEntry* GetEntry(const String& fileName) const;

		/// 返回所有文件入口
		const HashMap<String, PackageEntry>& GetEntries() const { return entries_; }

		/// 返回包文件名
		const String& GetName() const { return fileName_; }

		/// 返回包文件名的hash值
		StringHash GetNameHash() const { return nameHash_; }

		/// 返回文件数
		unsigned GetNumFiles() const { return entries_.Size(); }

		/// 返回包文件的总大小
		unsigned GetTotalSize() const { return totalSize_; }

		/// 返回包文件内容的校验和
		unsigned GetChecksum() const { return checksum_; }

		/// 返回文件是否压缩
		bool IsCompressed() const { return compressed_; }

		/// 返回在包中的文件列表
		const Vector<String> GetEntryNames() const { return entries_.Keys(); }

	private:
		/// 文件入口表
		HashMap<String, PackageEntry> entries_;
		/// 文件名
		String fileName_;
		/// 包文件名hash
		StringHash nameHash_;
		/// 包文件总大小
		unsigned totalSize_;
		/// 包文件校验和
		unsigned checksum_;
		/// 压缩标志
		bool compressed_;
	};
}