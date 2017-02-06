#include "LayerProperty.h"

#include "Animation/AnimatedFloat.h"
#include "Animation/AnimatedVector.h"
#include "Animation/Animation.h"
#include "Render/Sprite.h"
#include "SceneWindow/SceneEditScreen.h"
#include "UI/DropDown.h"
#include "UI/UIManager.h"

namespace Editor
{

	LayerProperty::LayerProperty(UIWidget* widget /*= nullptr*/)
	{
		if (widget)
			mDropDown = dynamic_cast<UIDropDown*>(widget);
		else
			mDropDown = o2UI.CreateWidget<UIDropDown>("editor property");

		mDropDown->onSelectedText = Function<void(const WString&)>(this, &LayerProperty::SelectLayer);
	}

	LayerProperty::~LayerProperty()
	{
		delete mDropDown;
	}

	void LayerProperty::Setup(const Vector<void*>& targets, bool isProperty)
	{
		if (isProperty)
		{
			mAssignFunc = [](void* ptr, Scene::Layer* value) { *((Property<Scene::Layer*>*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return ((Property<Scene::Layer*>*)(ptr))->Get(); };
		}
		else
		{
			mAssignFunc = [](void* ptr, Scene::Layer* value) { *((Scene::Layer**)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return *((Scene::Layer**)(ptr)); };
		}

		mValuesPointers = targets;

		Refresh();
	}

	void LayerProperty::Refresh()
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
				SetUnknownValue();
		}
		else if (newCommonValue != lastCommonValue || lastDifferent)
			SetCommonValue(newCommonValue);
	}

	UIWidget* LayerProperty::GetWidget() const
	{
		return mDropDown;
	}

	Scene::Layer* LayerProperty::GetCommonValue() const
	{
		return mCommonValue;
	}

	bool LayerProperty::IsValuesDifferent() const
	{
		return mValuesDifferent;
	}

	void LayerProperty::SetValue(Scene::Layer* value)
	{
		for (auto ptr : mValuesPointers)
			mAssignFunc(ptr, value);

		SetCommonValue(value);
	}

	void LayerProperty::SetUnknownValue()
	{
		mCommonValue = nullptr;
		mValuesDifferent = true;

		mUpdatingValue = true;
		mDropDown->SelectItemAt(-1);
		mUpdatingValue = false;

		mDropDown->SetState("undefined", true);

		onChanged();
		o2EditorSceneScreen.OnSceneChanged();
	}

	const Type* LayerProperty::GetFieldType() const
	{
		return &TypeOf(Scene::Layer*);
	}

	void LayerProperty::SetCommonValue(Scene::Layer* value)
	{
		mCommonValue = value;
		mValuesDifferent = false;

		mUpdatingValue = true;
		if (!mCommonValue)
		{
			mDropDown->SelectItemAt(-1);
			mDropDown->SetState("undefined", false);
		}
		else
		{
			mDropDown->value = mCommonValue->name;
			mDropDown->SetState("undefined", false);
		}
		mUpdatingValue = false;

		onChanged();
		o2EditorSceneScreen.OnSceneChanged();
	}

	void LayerProperty::UpdateLayersList()
	{
		auto layers = o2Scene.GetLayers();
		auto dropdownLayers = mDropDown->GetAllItemsText();
		for (auto itemName : dropdownLayers)
		{
			if (!layers.ContainsPred([&](Scene::Layer* x) { return x->name == itemName; }))
				mDropDown->RemoveItem(itemName);
		}

		for (auto layer : layers)
		{
			if (!dropdownLayers.Contains(layer->name))
				mDropDown->AddItem(layer->name);
		}
	}

	void LayerProperty::SelectLayer(const WString& name)
	{
		if (mUpdatingValue)
			return;

		SetValue(o2Scene.GetLayer(name));
	}
}

CLASS_META(Editor::LayerProperty)
{
	BASE_CLASS(Editor::IPropertyField);

	PROTECTED_FIELD(mAssignFunc);
	PROTECTED_FIELD(mGetFunc);
	PROTECTED_FIELD(mValuesPointers);
	PROTECTED_FIELD(mCommonValue);
	PROTECTED_FIELD(mValuesDifferent);
	PROTECTED_FIELD(mDropDown);
	PROTECTED_FIELD(mUpdatingValue);

	PUBLIC_FUNCTION(void, Setup, const Vector<void*>&, bool);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(Scene::Layer*, GetCommonValue);
	PUBLIC_FUNCTION(bool, IsValuesDifferent);
	PUBLIC_FUNCTION(void, SetValue, Scene::Layer*);
	PUBLIC_FUNCTION(void, SetUnknownValue);
	PUBLIC_FUNCTION(const Type*, GetFieldType);
	PROTECTED_FUNCTION(void, SetCommonValue, Scene::Layer*);
	PROTECTED_FUNCTION(void, UpdateLayersList);
	PROTECTED_FUNCTION(void, SelectLayer, const WString&);
}
END_META;
 