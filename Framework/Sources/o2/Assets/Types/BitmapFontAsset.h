#pragma once

#include "o2/Assets/Types/FontAsset.h"

namespace o2
{
	class Font;

	// -----------------
	// Bitmap font asset
	// -----------------
	class BitmapFontAsset: public FontAsset
	{
	public:
		class Meta;

	public:
		PROPERTIES(BitmapFontAsset);
		GETTER(Meta*, meta, GetMeta); // Meta information getter

	public:
		// Default constructor
		BitmapFontAsset();

		// Copy-constructor
		BitmapFontAsset(const BitmapFontAsset& asset);

		// Check equals operator
		BitmapFontAsset& operator=(const BitmapFontAsset& asset);

		// Returns meta information
		Meta* GetMeta() const;

		// Returns extensions string
		static const char* GetFileExtensions();

		// Returns editor sorting weight
		static int GetEditorSorting() { return 95; }

		ASSET_TYPE(BitmapFontAsset, Meta);

	public:
		// ----------------
		// Meta information
		// ----------------
		class Meta: public DefaultAssetMeta<BitmapFontAsset>
		{
		public:
			SERIALIZABLE(Meta);
		};

	protected:
		// Loads data
		void LoadData(const String& path) override;

		friend class Assets;
	};

	typedef Ref<BitmapFontAsset> BitmapFontAssetRef;
}

CLASS_BASES_META(o2::BitmapFontAsset)
{
	BASE_CLASS(o2::FontAsset);
}
END_META;
CLASS_FIELDS_META(o2::BitmapFontAsset)
{
	FIELD().PUBLIC().NAME(meta);
}
END_META;
CLASS_METHODS_META(o2::BitmapFontAsset)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const BitmapFontAsset&);
	FUNCTION().PUBLIC().SIGNATURE(Meta*, GetMeta);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(const char*, GetFileExtensions);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(int, GetEditorSorting);
	FUNCTION().PROTECTED().SIGNATURE(void, LoadData, const String&);
}
END_META;

CLASS_BASES_META(o2::BitmapFontAsset::Meta)
{
	BASE_CLASS(o2::DefaultAssetMeta<BitmapFontAsset>);
}
END_META;
CLASS_FIELDS_META(o2::BitmapFontAsset::Meta)
{
}
END_META;
CLASS_METHODS_META(o2::BitmapFontAsset::Meta)
{
}
END_META;
