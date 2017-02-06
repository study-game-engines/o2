#include "BooleanProperty.h"

#include "SceneWindow/SceneEditScreen.h"
#include "UI/Toggle.h"
#include "UI/UIManager.h"

namespace Editor
{
	BooleanProperty::BooleanProperty(UIWidget* widget /*= nullptr*/)
	{
		if (widget)
			mToggle = dynamic_cast<UIToggle*>(widget);
		else
			mToggle = o2UI.CreateWidget<UIToggle>("without caption");

		mToggle->layout.minHeight = 10;
		mToggle->onToggleByUser = Function<void(bool)>(this, &BooleanProperty::OnEdited);
	}

	BooleanProperty::~BooleanProperty()
	{
		delete mToggle;
	}

	void BooleanProperty::Setup(const Vector<void*>& targets, bool isProperty)
	{
		if (isProperty)
		{
			mAssignFunc = [](void* ptr, const bool& value) { *((Property<bool>*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return ((Property<bool>*)(ptr))->Get(); };
		}
		else
		{
			mAssignFunc = [](void* ptr, const bool& value) { *((bool*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return *((bool*)(ptr)); };
		}

		mValuesPointers = targets;

		Refresh();
	}

	void BooleanProperty::Refresh()
	{
		if (mValuesPointers.IsEmpty())
			return;

		auto lastCommonValue = mCommonValue;
		auto lastDifferent = mValuesDifferent;

		auto newCommonValue = mGetFunc(mValuesPointers[0]);
		auto newDifferent = false;

		for (int i = 1; i < mValuesPointers.Count(); i++)
		{
			if (newDifferent != mGetFunc(mValuesPointers[i]))
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
		else if (lastCommonValue != newCommonValue || lastDifferent)
			SetCommonValue(newCommonValue);
	}

	UIWidget* BooleanProperty::GetWidget() const
	{
		return mToggle;
	}

	bool BooleanProperty::GetCommonValue() const
	{
		return mCommonValue;
	}

	bool BooleanProperty::IsValuesDifferent() const
	{
		return mValuesDifferent;
	}

	void BooleanProperty::SetValue(bool value)
	{
		for (auto ptr : mValuesPointers)
			mAssignFunc(ptr, value);

		SetCommonValue(value);
	}

	void BooleanProperty::SetUnknownValue(bool defaultValue /*= false*/)
	{
		mValuesDifferent = true;
		mToggle->value = defaultValue;
		mToggle->SetValueUnknown();
		mCommonValue = defaultValue;

		onChanged();
		o2EditorSceneScreen.OnSceneChanged();
	}

	const Type* BooleanProperty::GetFieldType() const
	{
		return &TypeOf(bool);
	}

	void BooleanProperty::SetCommonValue(bool value)
	{
		mCommonValue = value;
		mValuesDifferent = false;
		mToggle->value = value;

		onChanged();
		o2EditorSceneScreen.OnSceneChanged();
	}

	void BooleanProperty::OnEdited(bool value)
	{
		SetValue(value);
	}
}

CLASS_META(Editor::BooleanProperty)
{
	BASE_CLASS(Editor::IPropertyField);

	PROTECTED_FIELD(mAssignFunc);
	PROTECTED_FIELD(mGetFunc);
	PROTECTED_FIELD(mValuesPointers);
	PROTECTED_FIELD(mCommonValue);
	PROTECTED_FIELD(mValuesDifferent);
	PROTECTED_FIELD(mToggle);

	PUBLIC_FUNCTION(void, Setup, const Vector<void*>&, bool);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(bool, GetCommonValue);
	PUBLIC_FUNCTION(bool, IsValuesDifferent);
	PUBLIC_FUNCTION(void, SetValue, bool);
	PUBLIC_FUNCTION(void, SetUnknownValue, bool);
	PUBLIC_FUNCTION(const Type*, GetFieldType);
	PROTECTED_FUNCTION(void, SetCommonValue, bool);
	PROTECTED_FUNCTION(void, OnEdited, bool);
}
END_META;
 