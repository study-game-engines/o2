#pragma once

#include "o2/Assets/Asset.h"
#include "o2/Render/TextureRef.h"
#include "o2/Utils/Types/Ref.h"
#include "o2/Assets/Types/ImageAsset.h"

namespace o2
{
	// -----------
	// Atlas asset
	// -----------
	class AtlasAsset: public Asset
	{
	public: 
		class Meta;
		class Page;

	public:
		PROPERTIES(AtlasAsset);
		GETTER(Meta*, meta, GetMeta);                     // Meta information getter
		GETTER(Vector<ImageAssetRef>, images, GetImages); // Images assets getter
		GETTER(Vector<Page>, pages, GetPages);            // Pages getter

	public:
		// Default constructor
		AtlasAsset();

		// Copy-constructor
		AtlasAsset(const AtlasAsset& asset);

		// Check equals operator
		AtlasAsset& operator=(const AtlasAsset& asset);

		// Returns containing images assets
		const Vector<ImageAssetRef>& GetImages() const;

		// Returns pages array
		const Vector<Page>& GetPages() const;

		// Is contains image
		bool ContainsImage(const ImageAssetRef& image);

		// Adds image to atlas
		void AddImage(const ImageAssetRef& image);

		// Removes image from atlas
		void RemoveImage(const ImageAssetRef& image);

		// Removes all images from atlas
		void RemoveAllImages();

		// Returns meta information
		Meta* GetMeta() const;

		// Returns extensions string
		static const char* GetFileExtensions();

		// Returns atlas page's texture file name
		static String GetPageTextureFileName(const AssetInfo& atlasInfo, UInt pageIdx);

		// Returns atlas page's texture reference
		static TextureRef GetPageTextureRef(const AssetInfo& atlasInfo, UInt pageIdx);

		// Returns editor sorting weight
		static int GetEditorSorting() { return 97; }

		// Is this asset type is available to create from editor's assets window
		static bool IsAvailableToCreateFromEditor() { return true; }

		ASSET_TYPE(AtlasAsset, Meta);

	public:
		// -----------------------------------
		// Platform specified meta information
		// -----------------------------------
		struct PlatformMeta: public ISerializable
		{
			Vec2I  maxSize = Vec2I(2048, 2048); // Maximal atlas size @SERIALIZABLE
			String format;                      // Atlas format @SERIALIZABLE

			bool operator==(const PlatformMeta& other) const;

			SERIALIZABLE(PlatformMeta);
		};

		// ----------------
		// Meta information
		// ----------------
		class Meta: public DefaultAssetMeta<AtlasAsset>
		{
		public:
			PlatformMeta ios;     // IOS specified meta @SERIALIZABLE
			PlatformMeta android; // Android specified meta @SERIALIZABLE
			PlatformMeta macOS;   // MacOS specified meta @SERIALIZABLE
			PlatformMeta windows; // Windows specified meta @SERIALIZABLE
			int          border;  // Images pack border @SERIALIZABLE

		public:
			// Returns true if other meta is equal to this
			bool IsEqual(AssetMeta* other) const override;

			SERIALIZABLE(Meta);
		};

		// ----------
		// Atlas page
		// ----------
		class Page: public ISerializable
		{
		public:
			// Returns number
			UInt ID() const;

			// Returns size
			Vec2I Size() const;

			// Returns texture reference
			TextureRef GetTextureRef() const;

			// Returns texture file name
			String GetTextureFileName() const;

			// Returns images rectangles
			const Map<UID, RectI>& ImagesRects() const;

			// Check equal operator
			bool operator==(const Page& other) const;

			SERIALIZABLE(Page);

		private:
			UInt            mId;          // Page number @SERIALIZABLE
			Vec2I           mSize;        // Size of page @SERIALIZABLE
			Map<UID, RectI> mImagesRects; // Images source rectangles @SERIALIZABLE
			AtlasAsset*     mOwner;       // Owner atlas

			friend class AtlasAssetConverter;
			friend class AtlasAsset;
		};

	protected:
		Vector<ImageAssetRef> mImages; // Loaded image infos @SERIALIZABLE
		Vector<Page>          mPages;  // Pages @SERIALIZABLE

	protected:
		// Completion deserialization callback
		void OnDeserialized(const DataValue& node) override;

		friend class Assets;
		friend class ImageAsset;
	};

	typedef Ref<AtlasAsset> AtlasAssetRef;
}

CLASS_BASES_META(o2::AtlasAsset)
{
	BASE_CLASS(o2::Asset);
}
END_META;
CLASS_FIELDS_META(o2::AtlasAsset)
{
	FIELD().PUBLIC().NAME(meta);
	FIELD().PUBLIC().NAME(images);
	FIELD().PUBLIC().NAME(pages);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().NAME(mImages);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().NAME(mPages);
}
END_META;
CLASS_METHODS_META(o2::AtlasAsset)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const AtlasAsset&);
	FUNCTION().PUBLIC().SIGNATURE(const Vector<ImageAssetRef>&, GetImages);
	FUNCTION().PUBLIC().SIGNATURE(const Vector<Page>&, GetPages);
	FUNCTION().PUBLIC().SIGNATURE(bool, ContainsImage, const ImageAssetRef&);
	FUNCTION().PUBLIC().SIGNATURE(void, AddImage, const ImageAssetRef&);
	FUNCTION().PUBLIC().SIGNATURE(void, RemoveImage, const ImageAssetRef&);
	FUNCTION().PUBLIC().SIGNATURE(void, RemoveAllImages);
	FUNCTION().PUBLIC().SIGNATURE(Meta*, GetMeta);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(const char*, GetFileExtensions);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetPageTextureFileName, const AssetInfo&, UInt);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(TextureRef, GetPageTextureRef, const AssetInfo&, UInt);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(int, GetEditorSorting);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(bool, IsAvailableToCreateFromEditor);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDeserialized, const DataValue&);
}
END_META;

CLASS_BASES_META(o2::AtlasAsset::PlatformMeta)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::AtlasAsset::PlatformMeta)
{
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(Vec2I(2048, 2048)).NAME(maxSize);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(format);
}
END_META;
CLASS_METHODS_META(o2::AtlasAsset::PlatformMeta)
{
}
END_META;

CLASS_BASES_META(o2::AtlasAsset::Meta)
{
	BASE_CLASS(o2::DefaultAssetMeta<AtlasAsset>);
}
END_META;
CLASS_FIELDS_META(o2::AtlasAsset::Meta)
{
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(ios);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(android);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(macOS);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(windows);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(border);
}
END_META;
CLASS_METHODS_META(o2::AtlasAsset::Meta)
{

	FUNCTION().PUBLIC().SIGNATURE(bool, IsEqual, AssetMeta*);
}
END_META;

CLASS_BASES_META(o2::AtlasAsset::Page)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::AtlasAsset::Page)
{
	FIELD().PRIVATE().SERIALIZABLE_ATTRIBUTE().NAME(mId);
	FIELD().PRIVATE().SERIALIZABLE_ATTRIBUTE().NAME(mSize);
	FIELD().PRIVATE().SERIALIZABLE_ATTRIBUTE().NAME(mImagesRects);
	FIELD().PRIVATE().NAME(mOwner);
}
END_META;
CLASS_METHODS_META(o2::AtlasAsset::Page)
{

	typedef const Map<UID, RectI>& _tmp1;

	FUNCTION().PUBLIC().SIGNATURE(UInt, ID);
	FUNCTION().PUBLIC().SIGNATURE(Vec2I, Size);
	FUNCTION().PUBLIC().SIGNATURE(TextureRef, GetTextureRef);
	FUNCTION().PUBLIC().SIGNATURE(String, GetTextureFileName);
	FUNCTION().PUBLIC().SIGNATURE(_tmp1, ImagesRects);
}
END_META;
