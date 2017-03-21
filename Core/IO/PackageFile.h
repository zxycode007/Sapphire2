#pragma once

#include "../Object.h"


namespace  Sapphire
{

	/// %File entry within the package file.
	struct PackageEntry
	{
		/// Offset from the beginning.
		unsigned offset_;
		/// File size.
		unsigned size_;
		/// File checksum.
		unsigned checksum_;
	};

	/// Stores files of a directory tree sequentially for convenient access.
	class SAPPHIRE_API PackageFile : public Object
	{
		SAPPHIRE_OBJECT(PackageFile, Object);

	public:
		/// Construct.
		PackageFile(Context* context);
		/// Construct and open.
		PackageFile(Context* context, const String& fileName, unsigned startOffset = 0);
		/// Destruct.
		virtual ~PackageFile();

		/// Open the package file. Return true if successful.
		bool Open(const String& fileName, unsigned startOffset = 0);
		/// Check if a file exists within the package file. This will be case-insensitive on Windows and case-sensitive on other platforms.
		bool Exists(const String& fileName) const;
		/// Return the file entry corresponding to the name, or null if not found. This will be case-insensitive on Windows and case-sensitive on other platforms.
		const PackageEntry* GetEntry(const String& fileName) const;

		/// Return all file entries.
		const HashMap<String, PackageEntry>& GetEntries() const { return entries_; }

		/// Return the package file name.
		const String& GetName() const { return fileName_; }

		/// Return hash of the package file name.
		StringHash GetNameHash() const { return nameHash_; }

		/// Return number of files.
		unsigned GetNumFiles() const { return entries_.Size(); }

		/// Return total size of the package file.
		unsigned GetTotalSize() const { return totalSize_; }

		/// Return checksum of the package file contents.
		unsigned GetChecksum() const { return checksum_; }

		/// Return whether the files are compressed.
		bool IsCompressed() const { return compressed_; }

		/// Return list of file names in the package.
		const Vector<String> GetEntryNames() const { return entries_.Keys(); }

	private:
		/// File entries.
		HashMap<String, PackageEntry> entries_;
		/// File name.
		String fileName_;
		/// Package file name hash.
		StringHash nameHash_;
		/// Package file total size.
		unsigned totalSize_;
		/// Package file checksum.
		unsigned checksum_;
		/// Compressed flag.
		bool compressed_;
	};
}