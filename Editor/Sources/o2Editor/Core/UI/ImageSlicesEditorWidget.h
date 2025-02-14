#pragma once

#include "o2/Scene/UI/Widget.h"
#include "o2/Scene/UI/Widgets/Image.h"

using namespace o2;

namespace o2
{
	class WidgetDragHandle;
}

namespace Editor
{
	class BorderIProperty;

	// -----------------------------
	// Editor of image slices widget
	// -----------------------------
	class ImageSlicesEditorWidget: public Widget
	{
	public:
		// Default constructor
		ImageSlicesEditorWidget();

		// Default copy-constructor
		ImageSlicesEditorWidget(const ImageSlicesEditorWidget& other);

		// Copy operator
		ImageSlicesEditorWidget& operator=(const ImageSlicesEditorWidget& other);

		// Sets image and property
		void Setup(const ImageAssetRef& image, BorderIProperty* borderProperty);

		// Returns create menu category in editor
		static String GetCreateMenuCategory();

		SERIALIZABLE(ImageSlicesEditorWidget);

	protected:
		class PreviewImage: public Image
		{
		public:
			// Sets texture filter to nearest and draws it
			void Draw() override;

			// Returns create menu category in editor
			static String GetCreateMenuCategory();

			SERIALIZABLE(PreviewImage);
		};

	private:
		PreviewImage* mPreviewImage = nullptr;
		Image*        mPreviewImageBack = nullptr;

		WidgetDragHandle* mBorderLeftHandle;
		WidgetDragHandle* mBorderRightHandle;
		WidgetDragHandle* mBorderTopHandle;
		WidgetDragHandle* mBorderBottomHandle;
		BorderF           mBordersSmoothValue;

		BorderIProperty* mBorderProperty = nullptr;

	protected:
		// Initializes image preview widgets and border handles
		void InitializeImagePreview();

		// Initializes image slices handles
		void InitializeSliceHandles();

		// Fits image by size
		void FitImage();

		// Updates borders anchors layouts
		void UpdateBordersAnchors();

		// Updates targets assets borders values by floored mBordersSmoothValue
		void UpdateBordersValue();

		// Creates grid sprite
		Sprite* CreateGridSprite();
	};
}

CLASS_BASES_META(Editor::ImageSlicesEditorWidget)
{
	BASE_CLASS(o2::Widget);
}
END_META;
CLASS_FIELDS_META(Editor::ImageSlicesEditorWidget)
{
	FIELD().PRIVATE().DEFAULT_VALUE(nullptr).NAME(mPreviewImage);
	FIELD().PRIVATE().DEFAULT_VALUE(nullptr).NAME(mPreviewImageBack);
	FIELD().PRIVATE().NAME(mBorderLeftHandle);
	FIELD().PRIVATE().NAME(mBorderRightHandle);
	FIELD().PRIVATE().NAME(mBorderTopHandle);
	FIELD().PRIVATE().NAME(mBorderBottomHandle);
	FIELD().PRIVATE().NAME(mBordersSmoothValue);
	FIELD().PRIVATE().DEFAULT_VALUE(nullptr).NAME(mBorderProperty);
}
END_META;
CLASS_METHODS_META(Editor::ImageSlicesEditorWidget)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const ImageSlicesEditorWidget&);
	FUNCTION().PUBLIC().SIGNATURE(void, Setup, const ImageAssetRef&, BorderIProperty*);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCreateMenuCategory);
	FUNCTION().PROTECTED().SIGNATURE(void, InitializeImagePreview);
	FUNCTION().PROTECTED().SIGNATURE(void, InitializeSliceHandles);
	FUNCTION().PROTECTED().SIGNATURE(void, FitImage);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateBordersAnchors);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateBordersValue);
	FUNCTION().PROTECTED().SIGNATURE(Sprite*, CreateGridSprite);
}
END_META;

CLASS_BASES_META(Editor::ImageSlicesEditorWidget::PreviewImage)
{
	BASE_CLASS(o2::Image);
}
END_META;
CLASS_FIELDS_META(Editor::ImageSlicesEditorWidget::PreviewImage)
{
}
END_META;
CLASS_METHODS_META(Editor::ImageSlicesEditorWidget::PreviewImage)
{

	FUNCTION().PUBLIC().SIGNATURE(void, Draw);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCreateMenuCategory);
}
END_META;
