#pragma once

#include "o2/Assets/Asset.h"
#include "o2/Assets/Types/BitmapFontAsset.h"
#include "o2/Assets/Types/VectorFontAsset.h"
#include "o2/Render/FontRef.h"
#include "o2/Render/RectDrawable.h"
#include "o2/Utils/Types/String.h"

namespace o2
{
	class Mesh;
	class Render;

	// ------------------------------------------------------------------------------------------
	// Text renderer class. Using font, basis and many style parameters. Caching text into meshes
	// ------------------------------------------------------------------------------------------
	class Text: public IRectDrawable
	{
	public:
		struct SymbolsSet;

	public:
		PROPERTIES(Text);
		PROPERTY(FontRef, font, SetFont, GetFont);                     // Font reference property
		PROPERTY(FontAssetRef, fontAsset, SetFontAsset, GetFontAsset); // Font asset reference property
		
		PROPERTY(WString, text, SetText, GetText); // Text property, wstring
		
		PROPERTY(int, height, SetHeight, GetHeight); // Text height
		
		PROPERTY(VerAlign, verAlign, SetVerAlign, GetVerAlign); // vertical align property
		PROPERTY(HorAlign, horAlign, SetHorAlign, GetHorAlign); // Horizontal align property
		
		PROPERTY(bool, wordWrap, SetWordWrap, GetWordWrap);         // Words wrapping flag property
		PROPERTY(bool, dotsEngings, SetDotsEngings, IsDotsEngings); // Dots endings when overflow property
		
		PROPERTY(float, symbolsDistanceCoef, SetSymbolsDistanceCoef, GetSymbolsDistanceCoef); // Characters distance coef, 1 is standard
		PROPERTY(float, linesDistanceCoef, SetLinesDistanceCoef, GetLinesDistanceCoef);       // Lines distance coef, 1 is standard

	public:
		// Default constructor
		Text();

		// Constructor
		Text(FontRef font);

		// Constructor
		Text(const String& fontFileName);

		// Constructor
		Text(const UID& fontAssetId);

		// Constructor
		Text(const BitmapFontAssetRef& fontAsset);

		// Constructor
		Text(const VectorFontAssetRef& fontAsset);

		// Copy-constructor
		Text(const Text& text);

		// Destructor
		~Text();

		// Assign operator
		Text& operator=(const Text& other);

		// Draw text
		void Draw() override;

		// Sets using font
		void SetFont(FontRef font);

		// Returns using font
		FontRef GetFont() const;

		// Sets bitmap font asset 
		void SetFontAsset(const FontAssetRef& asset);

		// Returns asset by font asset id
		FontAssetRef GetFontAsset() const;

		// Sets font height
		void SetHeight(int height);

		// Returns font height
		int GetHeight() const;

		// Sets text
		void SetText(const WString& text);

		// Returns text
		const WString& GetText() const;

		// Sets horizontal align
		void SetHorAlign(HorAlign align);

		// Returns horizontal align
		HorAlign GetHorAlign() const;

		// Sets vertical align
		void SetVerAlign(VerAlign align);

		// returns vertical align
		VerAlign GetVerAlign() const;

		// Sets word wrapping
		void SetWordWrap(bool flag);

		// Returns word wrapping
		bool GetWordWrap() const;

		// Sets dots endings
		void SetDotsEngings(bool flag);

		// Returns dots endings
		bool IsDotsEngings() const;

		// Sets characters distance coefficient
		void SetSymbolsDistanceCoef(float coef = 1);

		// Returns characters distance coef
		float GetSymbolsDistanceCoef() const;

		// Sets lines distance coefficient
		void SetLinesDistanceCoef(float coef = 1);

		// Returns lines distance coefficient
		float GetLinesDistanceCoef() const;

		// Returns symbol set structure pointer
		SymbolsSet& GetSymbolsSet();

		// Returns real text size
		Vec2F GetRealSize();

		// Returns real text rectangle
		RectF GetRealRect();

		// Returns text size
		static Vec2F GetTextSize(const WString& text, Font* font, int height = 11,
								 const Vec2F& areaSize = Vec2F(),
								 HorAlign horAlign = HorAlign::Left, VerAlign verAlign = VerAlign::Top,
								 bool wordWrap = true, bool dotsEngings = false, float charsDistCoef = 1.0f,
								 float linesDistCoef = 1.0f);

		SERIALIZABLE(Text);

	public:
		// ----------------
		// Text symbols set
		// ----------------
		struct SymbolsSet
		{
			// ----------------------------------
			// Simple symbol definition structure
			// ----------------------------------
			struct Symbol
			{
				RectF  mFrame;   // Frame of symbol layout
				RectF  mTexSrc;  // Texture source rect
				UInt16 mCharId;  // Character id
				Vec2F  mOrigin;  // Character offset
				float  mAdvance; // Character advance

			public:
				// Default constructor
				Symbol();

				// Constructor
				Symbol(const Vec2F& position, const Vec2F& size, const RectF& texSrc, UInt16 charId,
					   const Vec2F& origin, float advance);

			 // Equals operator
				bool operator==(const Symbol& other) const;
			};

			// -------------------------
			// Line definition structure
			// -------------------------
			struct Line
			{
				Vector<Symbol> mSymbols;       // Symbols in line
				WString       mString;        // Line string
				Vec2F         mSize;          // Size of line in pixels
				Vec2F         mPosition;      // Position of line
				int           mLineBegSymbol; // Index of line beginning symbol
				int           mSpacesCount;   // Spaces count at line
				bool          mEndedNewLine;  // True, if line ended by new line character

			public:
				// Default constructor
				Line();

				// Equals operator
				bool operator==(const Line& other) const;
			};

		public:
			FontRef  mFont;            // Font
			int      mHeight;          // Text height
			WString  mText;            // Text string
			Vec2F    mPosition;        // Position, in pixels
			Vec2F    mAreaSize;        // Area size, in pixels
			Vec2F    mRealSize;        // Real text size
			HorAlign mHorAlign;        // Horizontal align
			VerAlign mVerAlign;        // Vertical align
			bool     mWordWrap;        // True, when words wrapping
			bool     mDotsEndings;     // Dots ending when overflow
			float    mSymbolsDistCoef; // Characters distance coefficient, 1 is standard
			float    mLinesDistCoef;   // Lines distance coefficient, 1 is standard

			Vector<Line> mLines; // Lines definitions

		public:
			// Calculating characters layout by parameters
			void Initialize(FontRef font, const WString& text, int height, const Vec2F& position, const Vec2F& areaSize,
							HorAlign horAlign, VerAlign verAlign, bool wordWrap, bool dotsEngings, float charsDistCoef,
							float linesDistCoef);

			// Moves symbols 
			void Move(const Vec2F& offs);
		};

	protected:
		static const char* mBasicSymbolsPreset;
		const UInt mMeshMaxPolyCount = 4096;

		WString  mText;              // Wide char string, containing rendering text @SERIALIZABLE
		UID      mFontAssetId;       // Font asset id @SERIALIZABLE
		FontRef  mFont;              // Using font
		int      mHeight;            // Text height @SERIALIZABLE
		float    mSymbolsDistCoef;   // Characters distance coef, 1 is standard @SERIALIZABLE
		float    mLinesDistanceCoef; // Lines distance coef, 1 is standard @SERIALIZABLE
		VerAlign mVerAlign;          // Vertical align @SERIALIZABLE
		HorAlign mHorAlign;          // Horizontal align @SERIALIZABLE
		bool     mWordWrap;          // True, when words wrapping @SERIALIZABLE
		bool     mDotsEndings;       // If true, text will end on '...' @SERIALIZABLE

		Vector<Mesh*> mMeshes;        // Meshes vector
		Basis         mLastTransform; // Last mesh update transformation

		SymbolsSet mSymbolsSet; // Symbols set definition

		bool mUpdatingMesh; // True, when mesh is already updating

	protected:
		// Updating meshes
		void UpdateMesh();

		// Checks test's characters in font and rebuilds mesh. Used when fond is resetting
		void CheckCharactersAndRebuildMesh();

		// Transforming meshes by basis
		void TransformMesh(const Basis& bas);

		// Preparing meshes for characters count
		void PrepareMesh(int charactersCount);

		// Calculates and returns text basis
		Basis CalculateTextBasis() const;

		// It is called when color was changed
		void ColorChanged() override;

		// It is called when basis was changed
		void BasisChanged() override;

		// Calling when deserializing
		void OnDeserialized(const DataValue& node) override;
	};
}

CLASS_BASES_META(o2::Text)
{
	BASE_CLASS(o2::IRectDrawable);
}
END_META;
CLASS_FIELDS_META(o2::Text)
{
	FIELD().PUBLIC().NAME(font);
	FIELD().PUBLIC().NAME(fontAsset);
	FIELD().PUBLIC().NAME(text);
	FIELD().PUBLIC().NAME(height);
	FIELD().PUBLIC().NAME(verAlign);
	FIELD().PUBLIC().NAME(horAlign);
	FIELD().PUBLIC().NAME(wordWrap);
	FIELD().PUBLIC().NAME(dotsEngings);
	FIELD().PUBLIC().NAME(symbolsDistanceCoef);
	FIELD().PUBLIC().NAME(linesDistanceCoef);
	FIELD().PROTECTED().DEFAULT_VALUE(4096).NAME(mMeshMaxPolyCount);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().NAME(mText);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().NAME(mFontAssetId);
	FIELD().PROTECTED().NAME(mFont);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().NAME(mHeight);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().NAME(mSymbolsDistCoef);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().NAME(mLinesDistanceCoef);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().NAME(mVerAlign);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().NAME(mHorAlign);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().NAME(mWordWrap);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().NAME(mDotsEndings);
	FIELD().PROTECTED().NAME(mMeshes);
	FIELD().PROTECTED().NAME(mLastTransform);
	FIELD().PROTECTED().NAME(mSymbolsSet);
	FIELD().PROTECTED().NAME(mUpdatingMesh);
}
END_META;
CLASS_METHODS_META(o2::Text)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(FontRef);
	FUNCTION().PUBLIC().CONSTRUCTOR(const String&);
	FUNCTION().PUBLIC().CONSTRUCTOR(const UID&);
	FUNCTION().PUBLIC().CONSTRUCTOR(const BitmapFontAssetRef&);
	FUNCTION().PUBLIC().CONSTRUCTOR(const VectorFontAssetRef&);
	FUNCTION().PUBLIC().CONSTRUCTOR(const Text&);
	FUNCTION().PUBLIC().SIGNATURE(void, Draw);
	FUNCTION().PUBLIC().SIGNATURE(void, SetFont, FontRef);
	FUNCTION().PUBLIC().SIGNATURE(FontRef, GetFont);
	FUNCTION().PUBLIC().SIGNATURE(void, SetFontAsset, const FontAssetRef&);
	FUNCTION().PUBLIC().SIGNATURE(FontAssetRef, GetFontAsset);
	FUNCTION().PUBLIC().SIGNATURE(void, SetHeight, int);
	FUNCTION().PUBLIC().SIGNATURE(int, GetHeight);
	FUNCTION().PUBLIC().SIGNATURE(void, SetText, const WString&);
	FUNCTION().PUBLIC().SIGNATURE(const WString&, GetText);
	FUNCTION().PUBLIC().SIGNATURE(void, SetHorAlign, HorAlign);
	FUNCTION().PUBLIC().SIGNATURE(HorAlign, GetHorAlign);
	FUNCTION().PUBLIC().SIGNATURE(void, SetVerAlign, VerAlign);
	FUNCTION().PUBLIC().SIGNATURE(VerAlign, GetVerAlign);
	FUNCTION().PUBLIC().SIGNATURE(void, SetWordWrap, bool);
	FUNCTION().PUBLIC().SIGNATURE(bool, GetWordWrap);
	FUNCTION().PUBLIC().SIGNATURE(void, SetDotsEngings, bool);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsDotsEngings);
	FUNCTION().PUBLIC().SIGNATURE(void, SetSymbolsDistanceCoef, float);
	FUNCTION().PUBLIC().SIGNATURE(float, GetSymbolsDistanceCoef);
	FUNCTION().PUBLIC().SIGNATURE(void, SetLinesDistanceCoef, float);
	FUNCTION().PUBLIC().SIGNATURE(float, GetLinesDistanceCoef);
	FUNCTION().PUBLIC().SIGNATURE(SymbolsSet&, GetSymbolsSet);
	FUNCTION().PUBLIC().SIGNATURE(Vec2F, GetRealSize);
	FUNCTION().PUBLIC().SIGNATURE(RectF, GetRealRect);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(Vec2F, GetTextSize, const WString&, Font*, int, const Vec2F&, HorAlign, VerAlign, bool, bool, float, float);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateMesh);
	FUNCTION().PROTECTED().SIGNATURE(void, CheckCharactersAndRebuildMesh);
	FUNCTION().PROTECTED().SIGNATURE(void, TransformMesh, const Basis&);
	FUNCTION().PROTECTED().SIGNATURE(void, PrepareMesh, int);
	FUNCTION().PROTECTED().SIGNATURE(Basis, CalculateTextBasis);
	FUNCTION().PROTECTED().SIGNATURE(void, ColorChanged);
	FUNCTION().PROTECTED().SIGNATURE(void, BasisChanged);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDeserialized, const DataValue&);
}
END_META;
