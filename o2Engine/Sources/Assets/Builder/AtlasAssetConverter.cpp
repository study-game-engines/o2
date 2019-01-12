#include "stdafx.h"
#include "AtlasAssetConverter.h"

#include "Assets/Assets.h"
#include "Assets/AtlasAsset.h"
#include "Assets/ImageAsset.h"
#include "Utils/Bitmap/Bitmap.h"
#include "Utils/FileSystem/FileSystem.h"
#include "Utils/Debug/Log/LogStream.h"

namespace o2
{
	Vector<const Type*> AtlasAssetConverter::GetProcessingAssetsTypes() const
	{
		Vector<const Type*> res;
		res.Add(&TypeOf(AtlasAsset));
		return res;
	}

	void AtlasAssetConverter::ConvertAsset(const AssetTree::AssetNode& node)
	{
		String sourceAssetPath = o2Assets.GetAssetsPath() + node.path;
		String buildedAssetPath = o2Assets.GetDataPath() + node.path;
		String sourceAssetMetaPath = sourceAssetPath + ".meta";
		String buildedAssetMetaPath = buildedAssetPath + ".meta";

		if (!o2FileSystem.IsFileExist(buildedAssetPath))
			o2FileSystem.WriteFile(buildedAssetPath, "");

		o2FileSystem.FileCopy(sourceAssetMetaPath, buildedAssetMetaPath);
	}

	void AtlasAssetConverter::RemoveAsset(const AssetTree::AssetNode& node)
	{
		String buildedAssetPath = o2Assets.GetDataPath() + node.path;
		String buildedAssetMetaPath = buildedAssetPath + ".meta";

		DataNode atlasData;
		atlasData.LoadFromFile(buildedAssetPath);
		int pagesCount = atlasData["mPages"].GetChildNodes().Count();

		for (int i = 0; i < pagesCount; i++)
			o2FileSystem.FileDelete(buildedAssetPath + (String)i + ".png");

		o2FileSystem.FileDelete(buildedAssetPath);
		o2FileSystem.FileDelete(buildedAssetMetaPath);
	}

	void AtlasAssetConverter::MoveAsset(const AssetTree::AssetNode& nodeFrom, const AssetTree::AssetNode& nodeTo)
	{
		String fullPathFrom = o2Assets.GetDataPath() + nodeFrom.path;
		String fullPathTo = o2Assets.GetDataPath() + nodeTo.path;
		String fullMetaPathFrom = fullPathFrom + ".meta";
		String fullMetaPathTo = fullPathTo + ".meta";

		DataNode atlasData;
		atlasData.LoadFromFile(fullPathFrom);
		int pagesCount = atlasData["mPages"].GetChildNodes().Count();

		for (int i = 0; i < pagesCount; i++)
			o2FileSystem.FileMove(fullPathFrom + (String)i + ".png",
								  fullPathTo + (String)i + ".png");

		o2FileSystem.FileMove(fullPathFrom, fullPathTo);
		o2FileSystem.FileMove(fullMetaPathFrom, fullMetaPathTo);
	}

	Vector<UID> AtlasAssetConverter::AssetsPostProcess()
	{
		CheckBasicAtlas();
		return CheckRebuildingAtlases();
	}

	void AtlasAssetConverter::Reset()
	{}

	void AtlasAssetConverter::CheckBasicAtlas()
	{
		const Type* imageType = &TypeOf(ImageAsset);
		const Type* atlasType = &TypeOf(AtlasAsset);

		AssetTree::AssetNode* basicAtlasInfo = nullptr;

		Vector<UID> availableAtlasesIds;
		for (auto assetInfo : mAssetsBuilder->mBuiltAssetsTree.mAllAssets)
		{
			if (assetInfo->assetType == atlasType)
			{
				availableAtlasesIds.Add(assetInfo->meta->ID());
			}

			if (assetInfo->path == GetBasicAtlasPath())
				basicAtlasInfo = assetInfo;
		}

		for (auto assetInfo : mAssetsBuilder->mBuiltAssetsTree.mAllAssets)
		{
			if (assetInfo->assetType == imageType)
			{
				ImageAsset::MetaInfo* imageMeta = (ImageAsset::MetaInfo*)assetInfo->meta;

				if (!availableAtlasesIds.Contains(imageMeta->mAtlasId))
					imageMeta->mAtlasId = basicAtlasInfo->meta->ID();
			}
		}
	}

	Vector<UID> AtlasAssetConverter::CheckRebuildingAtlases()
	{
		Vector<UID> res;
		const Type* atlasAssetType = &TypeOf(AtlasAsset);

		for (auto info : mAssetsBuilder->mBuiltAssetsTree.mAllAssets)
		{
			if (info->assetType == atlasAssetType)
				if (CheckAtlasRebuilding(info))
					res.Add(info->id);
		}

		return res;
	}

	bool AtlasAssetConverter::CheckAtlasRebuilding(AssetTree::AssetNode* atlasInfo)
	{
		DataNode atlasData;
		atlasData.LoadFromFile(mAssetsBuilder->mBuiltAssetsPath + atlasInfo->path);

		ImagesVec lastImages;
		lastImages = atlasData["AllImages"];

		ImagesVec currentImages;
		const Type* imageType = &TypeOf(ImageAsset);
		UID atlasId = atlasInfo->meta->ID();
		for (auto assetInfo : mAssetsBuilder->mBuiltAssetsTree.mAllAssets)
		{
			if (assetInfo->assetType == imageType)
			{
				ImageAsset::MetaInfo* imageMeta = (ImageAsset::MetaInfo*)assetInfo->meta;
				if (imageMeta->mAtlasId == atlasId)
				{
					currentImages.Add(Image(imageMeta->ID(), assetInfo->time));
				}
			}
		}

		if (IsAtlasNeedRebuild(currentImages, lastImages))
		{
			RebuildAtlas(atlasInfo, currentImages);
			return true;
		}

		return false;
	}

	bool AtlasAssetConverter::IsAtlasNeedRebuild(ImagesVec& currentImages, ImagesVec& lastImages)
	{
		if (currentImages.Count() != lastImages.Count())
			return true;

		for (auto lastImg : lastImages)
		{
			bool found = false;
			for (auto curImg : currentImages)
			{
				if (lastImg.id == curImg.id)
				{
					if (lastImg.time != curImg.time)
					{
						return true;
					}
					else
					{
						found = true;
						break;
					}
				}
			}

			if (!found)
				return true;
		}

		for (auto curImg : currentImages)
		{
			if (mAssetsBuilder->mModifiedAssets.ContainsPred([&](auto x) { return x->id == curImg.id; }))
				return true;
		}

		return false;
	}

	void AtlasAssetConverter::RebuildAtlas(AssetTree::AssetNode* atlasInfo, ImagesVec& images)
	{
		auto meta = (AtlasAsset::MetaInfo*)atlasInfo->meta;

		RectsPacker packer(meta->mWindows.mMaxSize);
		float imagesBorder = (float)meta->mBorder;

		// Initialize pack images
		ImagePackDefsVec packImages;
		for (auto img : images)
		{
			// Find image info
			AssetTree::AssetNode* imgInfo =
				mAssetsBuilder->mBuiltAssetsTree.mAllAssets.FindMatch([&](auto info) { return info->meta->ID() == img.id; });

			if (!imgInfo)
			{
				mAssetsBuilder->mLog->Error("Can't find asset info by id: " + (String)img.id);
				continue;
			}

			String assetFullPath = mAssetsBuilder->mSourceAssetsPath + imgInfo->path;

			// Load bitmap
			Bitmap* bitmap = mnew Bitmap();
			if (!bitmap->Load(assetFullPath))
			{
				mAssetsBuilder->mLog->Error("Can't load bitmap for image asset: " + imgInfo->path);
				delete bitmap;
				continue;
			}

			// Create packing rect
			RectsPacker::Rect* packRect = packer.AddRect(bitmap->GetSize() +
														 Vec2F(imagesBorder*2.0f, imagesBorder*2.0f));

			ImagePackDef imagePackDef;
			imagePackDef.mAssetInfo = imgInfo;
			imagePackDef.mBitmap = bitmap;
			imagePackDef.mPackRect = packRect;

			packImages.Add(imagePackDef);
		}

		// Try to pack
		if (!packer.Pack())
		{
			mAssetsBuilder->mLog->Error("Atlas " + atlasInfo->path + " packing failed");
			return;
		}
		else
			mAssetsBuilder->mLog->Out("Atlas " + atlasInfo->path + " successfully packed");

		// Initialize bitmaps and pages
		int pagesCount = packer.GetPagesCount();
		Vector<Bitmap*> resAtlasBitmaps;
		Vector<AtlasAsset::Page> resAtlasPages;
		for (int i = 0; i < pagesCount; i++)
		{
			AtlasAsset::Page atlasPage;
			atlasPage.mId = 0;
			atlasPage.mSize = packer.GetMaxSize();
			resAtlasPages.Add(atlasPage);

			Bitmap* newBitmap = mnew Bitmap(PixelFormat::R8G8B8A8, packer.GetMaxSize());
			newBitmap->Fill(Color4(255, 255, 255, 0));

			resAtlasBitmaps.Add(newBitmap);
		}

		// Save image assets data and fill pages
		AssetInfosVec atlasImagesInfos;
		for (auto imgDef : packImages)
		{
			imgDef.mPackRect->mRect.left += imagesBorder;
			imgDef.mPackRect->mRect.right -= imagesBorder;
			imgDef.mPackRect->mRect.top -= imagesBorder;
			imgDef.mPackRect->mRect.bottom += imagesBorder;

			atlasImagesInfos.Add(AssetInfo(imgDef.mAssetInfo->path, imgDef.mAssetInfo->meta->ID(), &TypeOf(ImageAsset)));

			resAtlasBitmaps[imgDef.mPackRect->mPage]->CopyImage(imgDef.mBitmap,
																imgDef.mPackRect->mRect.LeftBottom());

			resAtlasPages[imgDef.mPackRect->mPage].mImagesRects.Add(imgDef.mAssetInfo->meta->ID(),
																	imgDef.mPackRect->mRect);

			SaveImageAsset(imgDef);
		}

		// Save pages bitmaps
		for (int i = 0; i < pagesCount; i++)
		{
			resAtlasBitmaps[i]->Save(mAssetsBuilder->mBuiltAssetsPath + atlasInfo->path + (String)i + ".png",
									 Bitmap::ImageType::Png);

			delete resAtlasBitmaps[i];
		}

		// Save atlas data
		DataNode atlasData;
		atlasData["mPages"] = resAtlasPages;
		atlasData["AllImages"] = images;
		atlasData["mImagesAssetsInfos"] = atlasImagesInfos;

		String atlasFullPath = mAssetsBuilder->mBuiltAssetsPath + atlasInfo->path;
		atlasData.SaveToFile(atlasFullPath);
		o2FileSystem.SetFileEditDate(atlasFullPath, atlasInfo->time);
	}

	void AtlasAssetConverter::SaveImageAsset(ImagePackDef& imgDef)
	{
		DataNode imgData;
		imgData["mAtlasPage"] = 0;
		imgData["mAtlasRect"] = (RectI)(imgDef.mPackRect->mRect);
		String imageFullPath = mAssetsBuilder->mBuiltAssetsPath + imgDef.mAssetInfo->path;
		imgData.SaveToFile(imageFullPath);
		o2FileSystem.SetFileEditDate(imageFullPath, imgDef.mAssetInfo->time);

		DataNode metaData;
		metaData = imgDef.mAssetInfo->meta;
		metaData.SaveToFile(mAssetsBuilder->mBuiltAssetsPath + imgDef.mAssetInfo->path + ".meta");
		metaData.SaveToFile(mAssetsBuilder->mSourceAssetsPath + imgDef.mAssetInfo->path + ".meta");
	}

	AtlasAssetConverter::Image::Image(UID id, const TimeStamp& time):
		id(id), time(time)
	{}

	bool AtlasAssetConverter::Image::operator==(const Image& other) const
	{
		return id == other.id;
	}

	bool AtlasAssetConverter::ImagePackDef::operator==(const ImagePackDef& other) const
	{
		return mAssetInfo == other.mAssetInfo && mBitmap == other.mBitmap && mPackRect == other.mPackRect;
	}
}

DECLARE_CLASS(o2::AtlasAssetConverter);

DECLARE_CLASS(o2::AtlasAssetConverter::Image);
