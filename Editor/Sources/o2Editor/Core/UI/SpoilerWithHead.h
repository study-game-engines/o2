#pragma once

#include "o2/Scene/UI/Widgets/Spoiler.h"

using namespace o2;

namespace o2
{
	class Button;
	class Image;
}

namespace Editor
{
	// -----------------------------------------------
	// Editor properties spoiler with head and caption
	// -----------------------------------------------
	class SpoilerWithHead : public Spoiler
	{
	public:
		Function<void()> onOptionsPressed; // Options button pressed event
		Function<void()> onSavePressed;    // Save button pressed event

	public:
		// Default constructor
		SpoilerWithHead();

		// Copy constructor
		SpoilerWithHead(const SpoilerWithHead& other);

		// Copy operator
		SpoilerWithHead& operator=(const SpoilerWithHead& other);

		// Returns icon
		Image* GetIcon() const;

		// Returns create menu category in editor
		static String GetCreateMenuCategory();

		IOBJECT(SpoilerWithHead);

	private:
		Button* mOptionsBtn = nullptr; // Component option button
		Button* mSaveBtn = nullptr;    // Save prototype button
		Image*  mIcon = nullptr;       // Component icon

	private:
		// Initializes controls: options, save button and icon
		void InitializeControls();
	};
}

CLASS_BASES_META(Editor::SpoilerWithHead)
{
	BASE_CLASS(o2::Spoiler);
}
END_META;
CLASS_FIELDS_META(Editor::SpoilerWithHead)
{
	FIELD().PUBLIC().NAME(onOptionsPressed);
	FIELD().PUBLIC().NAME(onSavePressed);
	FIELD().PRIVATE().DEFAULT_VALUE(nullptr).NAME(mOptionsBtn);
	FIELD().PRIVATE().DEFAULT_VALUE(nullptr).NAME(mSaveBtn);
	FIELD().PRIVATE().DEFAULT_VALUE(nullptr).NAME(mIcon);
}
END_META;
CLASS_METHODS_META(Editor::SpoilerWithHead)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const SpoilerWithHead&);
	FUNCTION().PUBLIC().SIGNATURE(Image*, GetIcon);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCreateMenuCategory);
	FUNCTION().PRIVATE().SIGNATURE(void, InitializeControls);
}
END_META;
