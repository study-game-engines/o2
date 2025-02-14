#pragma once

#include "IAssetConverter.h"

namespace o2
{
	// ----------------
	// Folder converter
	// ----------------
	class FolderAssetConverter: public IAssetConverter
	{
	public:
		// Returns vector of processing assets types
		Vector<const Type*> GetProcessingAssetsTypes() const override;

		// Converts folder by path
		void ConvertAsset(const AssetInfo& node);

		// Removes folder by path
		void RemoveAsset(const AssetInfo& node);

		// Moves folder to new path
		void MoveAsset(const AssetInfo& nodeFrom, const AssetInfo& nodeTo);

		// Post processing folders
		Vector<UID> AssetsPostProcess();

		// Resets converter
		void Reset();

		IOBJECT(FolderAssetConverter);

	protected:
		Vector<String> mRemovedFolders; // Removes folders
	};
}

CLASS_BASES_META(o2::FolderAssetConverter)
{
	BASE_CLASS(o2::IAssetConverter);
}
END_META;
CLASS_FIELDS_META(o2::FolderAssetConverter)
{
	FIELD().PROTECTED().NAME(mRemovedFolders);
}
END_META;
CLASS_METHODS_META(o2::FolderAssetConverter)
{

	FUNCTION().PUBLIC().SIGNATURE(Vector<const Type*>, GetProcessingAssetsTypes);
	FUNCTION().PUBLIC().SIGNATURE(void, ConvertAsset, const AssetInfo&);
	FUNCTION().PUBLIC().SIGNATURE(void, RemoveAsset, const AssetInfo&);
	FUNCTION().PUBLIC().SIGNATURE(void, MoveAsset, const AssetInfo&, const AssetInfo&);
	FUNCTION().PUBLIC().SIGNATURE(Vector<UID>, AssetsPostProcess);
	FUNCTION().PUBLIC().SIGNATURE(void, Reset);
}
END_META;
