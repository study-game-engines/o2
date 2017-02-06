#include "ColorProperty.h"

#include "Application/Application.h"
#include "Core/Dialogs/ColorPickerDlg.h"
#include "SceneWindow/SceneEditScreen.h"
#include "UI/EditBox.h"
#include "UI/Image.h"
#include "UI/UIManager.h"

namespace Editor
{
	ColorProperty::ColorProperty(UIWidget* widget /*= nullptr*/)
	{
		if (widget)
			mEditBox = widget;
		else
			mEditBox = o2UI.CreateWidget<UIWidget>("colorProperty");

		mEditBox->layout.minHeight = 10;

		Color4 color1(1.0f, 1.0f, 1.0f, 1.0f), color2(0.7f, 0.7f, 0.7f, 1.0f);
		Bitmap backLayerBitmap(Bitmap::Format::R8G8B8A8, Vec2I(20, 20));
		backLayerBitmap.Fill(color1);
		backLayerBitmap.FillRect(0, 0, 10, 10, color2);
		backLayerBitmap.FillRect(10, 10, 20, 20, color2);

		UIImage* backImage = mnew UIImage();
		backImage->image = mnew Sprite(&backLayerBitmap);
		backImage->GetImage()->mode = SpriteMode::Tiled;
		backImage->layout = UIWidgetLayout::BothStretch(1, 1, 1, 1);
		mEditBox->AddChild(backImage);

		Bitmap colorLayerBitmap(Bitmap::Format::R8G8B8A8, Vec2I(20, 20));
		colorLayerBitmap.Fill(color1);
		mColorSprite = mnew UIImage();
		mColorSprite->image = mnew Sprite(&colorLayerBitmap);
		mColorSprite->layout = UIWidgetLayout::BothStretch(1, 1, 1, 1);
		mEditBox->AddChild(mColorSprite);

		mEditBox->onDraw += [&]() { mClickArea.OnDrawn(); };
		mClickArea.isUnderPoint = [&](const Vec2F& point) { return mEditBox->IsUnderPoint(point); };
		mClickArea.onCursorReleased = [&](const Input::Cursor& cursor) { if (mEditBox->IsUnderPoint(cursor.position)) OnClicked(); };
	}

	ColorProperty::~ColorProperty()
	{
		delete mEditBox;
	}

	void ColorProperty::Setup(const Vector<void*>& targets, bool isProperty)
	{
		if (isProperty)
		{
			mAssignFunc = [](void* ptr, const Color4& value) { *((Property<Color4>*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return ((Property<Color4>*)(ptr))->Get(); };
		}
		else
		{
			mAssignFunc = [](void* ptr, const Color4& value) { *((Color4*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return *((Color4*)(ptr)); };
		}

		mValuesPointers = targets;

		Refresh();
	}

	void ColorProperty::Refresh()
	{
		if (mValuesPointers.IsEmpty())
			return;

		auto lastCommonValue = mCommonValue;
		auto lastDifferent = mValuesDifferent;

		auto newCommonValue = mGetFunc(mValuesPointers[0]);
		auto newDifferent = false;

		for (int i = 1; i < mValuesPointers.Count(); i++)
		{
			if (newCommonValue != mGetFunc(mValuesPointers[i]))
			{
				newDifferent = true;
				break;
			}
		}

		if (newDifferent)
		{
			if (!lastDifferent)
				SetUnknownValue(newCommonValue);
		}
		else if (!Math::Equals(lastCommonValue, newCommonValue) || lastDifferent)
			SetCommonValue(newCommonValue);
	}

	UIWidget* ColorProperty::GetWidget() const
	{
		return mEditBox;
	}

	Color4 ColorProperty::GetCommonValue() const
	{
		return mCommonValue;
	}

	bool ColorProperty::IsValuesDifferent() const
	{
		return mValuesDifferent;
	}

	void ColorProperty::SetValue(const Color4& value)
	{
		for (auto ptr : mValuesPointers)
			mAssignFunc(ptr, value);

		SetCommonValue(value);
	}

	void ColorProperty::SetUnknownValue(const Color4& defaultValue /*= Color4()*/)
	{
		mCommonValue = defaultValue;
		mValuesDifferent = true;
		mColorSprite->GetImage()->SetColor(mCommonValue);
		mColorSprite->transparency = mCommonValue.AF();

		onChanged();
		o2EditorSceneScreen.OnSceneChanged();
	}

	const Type* ColorProperty::GetFieldType() const
	{
		return &TypeOf(Color4);
	}

	void ColorProperty::SetCommonValue(const Color4& value)
	{
		mCommonValue = value;
		mValuesDifferent = false;
		mColorSprite->GetImage()->SetColor(mCommonValue);
		mColorSprite->transparency = mCommonValue.AF();

		onChanged();
		o2EditorSceneScreen.OnSceneChanged();
	}

	void ColorProperty::OnClicked()
	{
// 		CHOOSECOLOR cc;                 // common dialog box structure 
// 		static COLORREF acrCustClr[16]; // array of custom colors 
// 		HBRUSH hbrush;                  // brush handle
// 		static DWORD rgbCurrent;        // initial color selection
// 
// 										// Initialize CHOOSECOLOR 
// 		ZeroMemory(&cc, sizeof(cc));
// 		cc.lStructSize = sizeof(cc);
// 		cc.hwndOwner = o2Application.GetWindowHandle();
// 		cc.lpCustColors = (LPDWORD)acrCustClr;
// 		cc.rgbResult = rgbCurrent;
// 		cc.Flags = CC_FULLOPEN | CC_RGBINIT;
// 
// 		if (ChooseColor(&cc)==TRUE)
// 		{
// 			SetValue(Color4(GetRValue(cc.rgbResult), GetGValue(cc.rgbResult), GetBValue(cc.rgbResult), 255));
// 		}

		ColorPickerDlg::Show(mCommonValue, [&](const Color4& color) { SetValue(color); });
	}

}

CLASS_META(Editor::ColorProperty)
{
	BASE_CLASS(Editor::IPropertyField);

	PROTECTED_FIELD(mAssignFunc);
	PROTECTED_FIELD(mGetFunc);
	PROTECTED_FIELD(mValuesPointers);
	PROTECTED_FIELD(mCommonValue);
	PROTECTED_FIELD(mValuesDifferent);
	PROTECTED_FIELD(mEditBox);
	PROTECTED_FIELD(mColorSprite);
	PROTECTED_FIELD(mClickArea);

	PUBLIC_FUNCTION(void, Setup, const Vector<void*>&, bool);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(Color4, GetCommonValue);
	PUBLIC_FUNCTION(bool, IsValuesDifferent);
	PUBLIC_FUNCTION(void, SetValue, const Color4&);
	PUBLIC_FUNCTION(void, SetUnknownValue, const Color4&);
	PUBLIC_FUNCTION(const Type*, GetFieldType);
	PROTECTED_FUNCTION(void, SetCommonValue, const Color4&);
	PROTECTED_FUNCTION(void, OnClicked);
}
END_META;
 