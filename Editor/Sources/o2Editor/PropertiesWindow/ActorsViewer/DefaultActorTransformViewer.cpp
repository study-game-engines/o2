#include "o2Editor/stdafx.h"
#include "DefaultActorTransformViewer.h"

#include "o2/Scene/Actor.h"
#include "o2/Scene/DrawableComponent.h"
#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/Widget.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Scene/UI/Widgets/Image.h"
#include "o2/Scene/UI/Widgets/Label.h"
#include "o2/Scene/UI/Widgets/Spoiler.h"
#include "o2/Scene/UI/Widgets/VerticalLayout.h"
#include "o2Editor/Core/Actions/PropertyChange.h"
#include "o2Editor/Core/EditorApplication.h"
#include "o2/Utils/Editor/EditorScope.h"
#include "o2Editor/Core/Properties/Basic/FloatProperty.h"
#include "o2Editor/Core/Properties/Basic/Vector2FloatProperty.h"
#include "o2Editor/Core/UI/SpoilerWithHead.h"
#include "o2Editor/SceneWindow/SceneEditScreen.h"

namespace Editor
{
	DefaultActorTransformViewer::DefaultActorTransformViewer()
	{
		PushEditorScopeOnStack scope;

		// Position
		auto positionPropertyContainer = mnew Widget();
		positionPropertyContainer->name = "position";
		positionPropertyContainer->layout->minHeight = 20;
		mSpoiler->AddChild(positionPropertyContainer);

		auto positionIcon = o2UI.CreateImage("ui/UI4_position_icon.png");
		*positionIcon->layout = WidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(0, 0));
		positionPropertyContainer->AddChild(positionIcon);

		mPositionProperty = o2UI.CreateWidget<Vec2FProperty>("colored");
		*mPositionProperty->layout = WidgetLayout::HorStretch(VerAlign::Top, 20, 0, 20, 0);
		mPositionProperty->SetValuePath("transform/position");
		mPositionProperty->onChangeCompleted = THIS_FUNC(OnPropertyChangeCompleted);
		positionPropertyContainer->AddChild(mPositionProperty);

		// Pivot
		auto pivotPropertyContainer = mnew Widget();
		pivotPropertyContainer->name = "pivot";
		pivotPropertyContainer->layout->minHeight = 20;
		mSpoiler->AddChild(pivotPropertyContainer);

		auto pivotIcon = o2UI.CreateImage("ui/UI4_pivot_icon.png");
		*pivotIcon->layout = WidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(0, 0));
		pivotPropertyContainer->AddChild(pivotIcon);

		mPivotProperty = o2UI.CreateWidget<Vec2FProperty>("colored");
		*mPivotProperty->layout = WidgetLayout::HorStretch(VerAlign::Top, 20, 0, 20, 0);
		mPivotProperty->SetValuePath("transform/pivot");
		mPivotProperty->onChangeCompleted = THIS_FUNC(OnPropertyChangeCompleted);
		pivotPropertyContainer->AddChild(mPivotProperty);

		// Size
		auto sizePropertyContainer = mnew Widget();
		sizePropertyContainer->name = "size";
		sizePropertyContainer->layout->minHeight = 20;
		mSpoiler->AddChild(sizePropertyContainer);

		auto sizeIcon = o2UI.CreateImage("ui/UI4_icon_size.png");
		*sizeIcon->layout = WidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(-1, 0));
		sizePropertyContainer->AddChild(sizeIcon);

		mSizeProperty = o2UI.CreateWidget<Vec2FProperty>("colored");
		*mSizeProperty->layout = WidgetLayout::HorStretch(VerAlign::Top, 20, 0, 20, 0);
		mSizeProperty->SetValuePath("transform/size");
		mSizeProperty->onChangeCompleted = THIS_FUNC(OnPropertyChangeCompleted);
		sizePropertyContainer->AddChild(mSizeProperty);

		// Scale
		auto scalePropertyContainer = mnew Widget();
		scalePropertyContainer->name = "scale";
		scalePropertyContainer->layout->minHeight = 20;
		mSpoiler->AddChild(scalePropertyContainer);

		auto scaleIcon = o2UI.CreateImage("ui/UI4_scale_icon.png");
		*scaleIcon->layout = WidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(0, 0));
		scalePropertyContainer->AddChild(scaleIcon);

		mScaleProperty = o2UI.CreateWidget<Vec2FProperty>("colored");
		*mScaleProperty->layout = WidgetLayout::HorStretch(VerAlign::Top, 20, 0, 20, 0);
		mScaleProperty->SetValuePath("transform/scale");
		mScaleProperty->onChangeCompleted = THIS_FUNC(OnPropertyChangeCompleted);
		scalePropertyContainer->AddChild(mScaleProperty);

		// Rotation
		auto rotationAndDepthPropertyContainer = mnew Widget();
		rotationAndDepthPropertyContainer->name = "rotation and depth";
		rotationAndDepthPropertyContainer->layout->minHeight = 20;
		mSpoiler->AddChild(rotationAndDepthPropertyContainer);

		auto rotateIcon = o2UI.CreateImage("ui/UI4_rotate_icon.png");
		*rotateIcon->layout = WidgetLayout(Vec2F(0, 0), Vec2F(0.0f, 1.0f), Vec2F(0, 0), Vec2F(20, 0));
		rotationAndDepthPropertyContainer->AddChild(rotateIcon);

		mRotationProperty = o2UI.CreateWidget<FloatProperty>();
		*mRotationProperty->layout = WidgetLayout(Vec2F(0, 0), Vec2F(0.5f, 1.0f), Vec2F(40, 0), Vec2F(10, 0));
		mRotationProperty->SetValuePath("transform/angleDegree");
		mRotationProperty->onChangeCompleted = THIS_FUNC(OnPropertyChangeCompleted);
		rotationAndDepthPropertyContainer->AddChild(mRotationProperty);

		// Depth
		auto depthIcon = o2UI.CreateImage("ui/UI4_layer_icon_t.png");
		*depthIcon->layout = WidgetLayout(Vec2F(0.5f, 0), Vec2F(0.5f, 1.0f), Vec2F(10, 0), Vec2F(30, 0));
		rotationAndDepthPropertyContainer->AddChild(depthIcon);

		mDepthProperty = o2UI.CreateWidget<FloatProperty>();
		*mDepthProperty->layout = WidgetLayout(Vec2F(0.5f, 0), Vec2F(1, 1.0f), Vec2F(30, 0), Vec2F());
		mDepthProperty->SetValuePath("drawDepth");
		auto depthEditBox = dynamic_cast<EditBox*>(mDepthProperty->GetChildWidget("container/layout/editBox"));
		depthEditBox->GetLayer("arrows")->layout = Layout::Based(BaseCorner::Right, Vec2F(10, 20), Vec2F(-20, 0));
		depthEditBox->SetClippingLayout(Layout::BothStretch(0, 0, 30, 0));
		depthEditBox->SetViewLayout(Layout::BothStretch(3, 1, 30, -1));

		mDepthProperty->onChangeCompleted = THIS_FUNC(OnPropertyChangeCompleted);
		mDepthProperty->onChanged = [&](IPropertyField* field) { OnDepthPropertyChanged(); };
		rotationAndDepthPropertyContainer->AddChild(mDepthProperty);

		mDepthInheritanceMenu = o2UI.CreateWidget<ContextMenu>();
		mDepthInheritanceMenu->AddToggleItem("Custom", true, [&](bool value) { OnDepthInheritanceSelected(false); });
		mDepthInheritanceMenu->AddToggleItem("Inherit from parent", false, [&](bool value) { OnDepthInheritanceSelected(true); });
		rotationAndDepthPropertyContainer->AddChild(mDepthInheritanceMenu);

		mDepthInheritanceMenuButton = o2UI.CreateWidget<Button>("expand down");
		*mDepthInheritanceMenuButton->layout = WidgetLayout::Based(BaseCorner::Right, Vec2F(20, 20));
		rotationAndDepthPropertyContainer->AddChild(mDepthInheritanceMenuButton);
		mDepthInheritanceMenuButton->onClick = [=]() { mDepthInheritanceMenu->Show(); };

		// Layout
		mLayoutSpoiler = o2UI.CreateWidget<Spoiler>("expand with caption");
		mLayoutSpoiler->name = "Layout";
		mLayoutSpoiler->caption = "Layout";
		mLayoutSpoiler->spacing = 5;
		mLayoutSpoiler->fitByChildren = true;
		mLayoutSpoiler->expandWidth = true;
		mLayoutSpoiler->expandHeight = false;
		mSpoiler->AddChild(mLayoutSpoiler);

		// Anchors
		// Right top
		auto rightTopAnchorPropertyContainer = mnew Widget();
		rightTopAnchorPropertyContainer->name = "right top anchor";
		rightTopAnchorPropertyContainer->layout->minHeight = 20;
		mLayoutSpoiler->AddChild(rightTopAnchorPropertyContainer);

		auto anchorIcon = o2UI.CreateImage("ui/UI4_icon_anchor.png");
		*anchorIcon->layout = WidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(0, 0));
		rightTopAnchorPropertyContainer->AddChild(anchorIcon);

		mAnchorRightTopProperty = o2UI.CreateWidget<Vec2FProperty>("colored");
		*mAnchorRightTopProperty->layout = WidgetLayout::HorStretch(VerAlign::Top, 20, 0, 20, 0);
		mAnchorRightTopProperty->GetChildByType<Label>("container/layout/properties/x label")->text = "R";
		mAnchorRightTopProperty->GetChildByType<Label>("container/layout/properties/y label")->text = "T";
		mAnchorRightTopProperty->SetValuePath("layout/anchorMax");
		mAnchorRightTopProperty->onChangeCompleted = THIS_FUNC(OnPropertyChangeCompleted);
		rightTopAnchorPropertyContainer->AddChild(mAnchorRightTopProperty);

		// Left bottom
		auto leftBottomAnchorPropertyContainer = mnew Widget();
		leftBottomAnchorPropertyContainer->name = "left bottom anchor";
		leftBottomAnchorPropertyContainer->layout->minHeight = 20;
		mLayoutSpoiler->AddChild(leftBottomAnchorPropertyContainer);

		mAnchorLeftBottomProperty = o2UI.CreateWidget<Vec2FProperty>("colored");
		*mAnchorLeftBottomProperty->layout = WidgetLayout::HorStretch(VerAlign::Top, 20, 0, 20, 0);
		mAnchorLeftBottomProperty->GetChildByType<Label>("container/layout/properties/x label")->text = "L";
		mAnchorLeftBottomProperty->GetChildByType<Label>("container/layout/properties/y label")->text = "B";
		mAnchorLeftBottomProperty->SetValuePath("layout/anchorMin");
		mAnchorLeftBottomProperty->onChangeCompleted = THIS_FUNC(OnPropertyChangeCompleted);
		leftBottomAnchorPropertyContainer->AddChild(mAnchorLeftBottomProperty);

		// Offsets
		// Right top
		auto rightTopOffsetPropertyContainer = mnew Widget();
		rightTopOffsetPropertyContainer->name = "right top offset";
		rightTopOffsetPropertyContainer->layout->minHeight = 20;
		mLayoutSpoiler->AddChild(rightTopOffsetPropertyContainer);

		auto offsetIcon = o2UI.CreateImage("ui/UI4_icon_offsets.png");
		*offsetIcon->layout = WidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(0, 0));
		rightTopOffsetPropertyContainer->AddChild(offsetIcon);

		moffsetRightTopProperty = o2UI.CreateWidget<Vec2FProperty>("colored");
		*moffsetRightTopProperty->layout = WidgetLayout::HorStretch(VerAlign::Top, 20, 0, 20, 0);
		moffsetRightTopProperty->GetChildByType<Label>("container/layout/properties/x label")->text = "R";
		moffsetRightTopProperty->GetChildByType<Label>("container/layout/properties/y label")->text = "T";
		moffsetRightTopProperty->SetValuePath("layout/offsetMax");
		moffsetRightTopProperty->onChangeCompleted = THIS_FUNC(OnPropertyChangeCompleted);
		rightTopOffsetPropertyContainer->AddChild(moffsetRightTopProperty);

		// Left bottom
		auto leftBottomOffsetPropertyContainer = mnew Widget();
		leftBottomOffsetPropertyContainer->name = "left bottom offset";
		leftBottomOffsetPropertyContainer->layout->minHeight = 20;
		mLayoutSpoiler->AddChild(leftBottomOffsetPropertyContainer);

		mOffsetLeftBottomProperty = o2UI.CreateWidget<Vec2FProperty>("colored");
		*mOffsetLeftBottomProperty->layout = WidgetLayout::HorStretch(VerAlign::Top, 20, 0, 20, 0);
		mOffsetLeftBottomProperty->GetChildByType<Label>("container/layout/properties/x label")->text = "L";
		mOffsetLeftBottomProperty->GetChildByType<Label>("container/layout/properties/y label")->text = "B";
		mOffsetLeftBottomProperty->SetValuePath("layout/offsetMin");
		mOffsetLeftBottomProperty->onChangeCompleted = THIS_FUNC(OnPropertyChangeCompleted);
		leftBottomOffsetPropertyContainer->AddChild(mOffsetLeftBottomProperty);

		// Min size
		auto minSizePropertyContainer = mnew Widget();
		minSizePropertyContainer->name = "right top anchor";
		minSizePropertyContainer->layout->minHeight = 20;
		mLayoutSpoiler->AddChild(minSizePropertyContainer);

		auto minSizeIcon = o2UI.CreateImage("ui/UI4_icon_min_size.png");
		*minSizeIcon->layout = WidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(0, 0));
		minSizePropertyContainer->AddChild(minSizeIcon);

		mMinSizeProperty = o2UI.CreateWidget<Vec2FProperty>("colored");
		*mMinSizeProperty->layout = WidgetLayout::HorStretch(VerAlign::Top, 20, 0, 20, 0);
		mMinSizeProperty->SetValuePath("layout/minSize");
		mMinSizeProperty->onChangeCompleted = THIS_FUNC(OnPropertyChangeCompleted);
		minSizePropertyContainer->AddChild(mMinSizeProperty);

		// Max size
		auto maxSizePropertyContainer = mnew Widget();
		maxSizePropertyContainer->name = "right top anchor";
		maxSizePropertyContainer->layout->minHeight = 20;
		mLayoutSpoiler->AddChild(maxSizePropertyContainer);

		auto maxSizeIcon = o2UI.CreateImage("ui/UI4_icon_max_size.png");
		*maxSizeIcon->layout = WidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(0, 0));
		maxSizePropertyContainer->AddChild(maxSizeIcon);

		mMaxSizeProperty = o2UI.CreateWidget<Vec2FProperty>("colored");
		*mMaxSizeProperty->layout = WidgetLayout::HorStretch(VerAlign::Top, 20, 0, 20, 0);
		mMaxSizeProperty->SetValuePath("layout/maxSize");
		mMaxSizeProperty->onChangeCompleted = THIS_FUNC(OnPropertyChangeCompleted);
		maxSizePropertyContainer->AddChild(mMaxSizeProperty);

		// Weight
		auto weightPropertyContainer = mnew Widget();
		weightPropertyContainer->name = "right top anchor";
		weightPropertyContainer->layout->minHeight = 20;
		mLayoutSpoiler->AddChild(weightPropertyContainer);

		auto weightIcon = o2UI.CreateImage("ui/UI4_icon_weight.png");
		*weightIcon->layout = WidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(0, 0));
		weightPropertyContainer->AddChild(weightIcon);

		mWeightProperty = o2UI.CreateWidget<Vec2FProperty>("colored");
		*mWeightProperty->layout = WidgetLayout::HorStretch(VerAlign::Top, 20, 0, 20, 0);
		mWeightProperty->SetValuePath("layout/weight");
		mWeightProperty->onChangeCompleted = THIS_FUNC(OnPropertyChangeCompleted);
		weightPropertyContainer->AddChild(mWeightProperty);
	}

	DefaultActorTransformViewer::~DefaultActorTransformViewer()
	{
		delete mSpoiler;
	}

	void DefaultActorTransformViewer::SetTargetActors(const Vector<Actor*>& actors)
	{
		mTargetActors = actors;

		auto prototypes = actors.Convert<Actor*>([](Actor* x) { return x->GetPrototypeLink().Get(); });

		mPositionProperty->SelectValueAndPrototypeProperties<Actor, decltype(ActorTransform::position)>(
			actors, prototypes, [](Actor* x) { return &x->transform->position; });

		mPivotProperty->SelectValueAndPrototypeProperties<Actor, decltype(ActorTransform::pivot)>(
			actors, prototypes, [](Actor* x) { return &x->transform->pivot; });

		mScaleProperty->SelectValueAndPrototypeProperties<Actor, decltype(ActorTransform::scale)>(
			actors, prototypes, [](Actor* x) { return &x->transform->scale; });

		mSizeProperty->SelectValueAndPrototypeProperties<Actor, decltype(ActorTransform::size)>(
			actors, prototypes, [](Actor* x) { return &x->transform->size; });

		mRotationProperty->SelectValueAndPrototypeProperties<Actor, decltype(ActorTransform::angleDegree)>(
			actors, prototypes, [](Actor* x) { return &x->transform->angleDegree; });

		Vector<Pair<IAbstractValueProxy*, IAbstractValueProxy*>> depthTargets;
		for (auto actor : mTargetActors)
		{
			if (auto comp = actor->GetComponent<DrawableComponent>())
			{
				DrawableComponent* proto = dynamic_cast<DrawableComponent*>(comp->GetPrototypeLink());
				depthTargets.Add(Pair<IAbstractValueProxy*, IAbstractValueProxy*>(
					mnew PropertyValueProxy<float, decltype(comp->drawDepth)>(&comp->drawDepth),
					proto ? mnew PropertyValueProxy<float, decltype(comp->drawDepth)>(&proto->drawDepth) : nullptr));
			}

			if (auto drawable = dynamic_cast<ISceneDrawable*>(actor))
			{
				ISceneDrawable* proto = dynamic_cast<ISceneDrawable*>(actor->GetPrototypeLink().Get());
				depthTargets.Add(Pair<IAbstractValueProxy*, IAbstractValueProxy*>(
					mnew PropertyValueProxy<float, decltype(drawable->drawDepth)>(&drawable->drawDepth),
					proto ? mnew PropertyValueProxy<float, decltype(drawable->drawDepth)>(&proto->drawDepth) : nullptr));
			}
		}

		mDepthProperty->SetValueAndPrototypeProxy(depthTargets);

		Vector<Widget*> targetWidgets = mTargetActors
			.FindAll([](Actor* x) { return dynamic_cast<Widget*>(x) != nullptr; })
			.Convert<Widget*>([](Actor* x) { return dynamic_cast<Widget*>(x); });

		mLayoutEnabled = !targetWidgets.IsEmpty();
		mLayoutSpoiler->enabled = mLayoutEnabled;

		if (mLayoutEnabled)
		{
			auto widgetPrototypes = targetWidgets.Convert<Widget*>(
				[](Actor* x) { return dynamic_cast<Widget*>(x->GetPrototypeLink().Get()); });

			mAnchorRightTopProperty->SelectValueAndPrototypeProperties<Widget, decltype(WidgetLayout::anchorMax)>(
				targetWidgets, widgetPrototypes, [](Widget* x) { return &x->layout->anchorMax; });

			mAnchorLeftBottomProperty->SelectValueAndPrototypeProperties<Widget, decltype(WidgetLayout::anchorMin)>(
				targetWidgets, widgetPrototypes, [](Widget* x) { return &x->layout->anchorMin; });

			moffsetRightTopProperty->SelectValueAndPrototypeProperties<Widget, decltype(WidgetLayout::offsetMax)>(
				targetWidgets, widgetPrototypes, [](Widget* x) { return &x->layout->offsetMax; });

			mOffsetLeftBottomProperty->SelectValueAndPrototypeProperties<Widget, decltype(WidgetLayout::offsetMin)>(
				targetWidgets, widgetPrototypes, [](Widget* x) { return &x->layout->offsetMin; });

			mMinSizeProperty->SelectValueAndPrototypeProperties<Widget, decltype(WidgetLayout::minSize)>(
				targetWidgets, widgetPrototypes, [](Widget* x) { return &x->layout->minSize; });

			mMaxSizeProperty->SelectValueAndPrototypeProperties<Widget, decltype(WidgetLayout::maxSize)>(
				targetWidgets, widgetPrototypes, [](Widget* x) { return &x->layout->maxSize; });

			mWeightProperty->SelectValueAndPrototypeProperties<Widget, decltype(WidgetLayout::weight)>(
				targetWidgets, widgetPrototypes, [](Widget* x) { return &x->layout->weight; });
		}
	}

	void DefaultActorTransformViewer::Refresh()
	{

		mPositionProperty->Refresh();
		mPivotProperty->Refresh();
		mScaleProperty->Refresh();
		mSizeProperty->Refresh();
		mRotationProperty->Refresh();

		enum class DepthSelectedType { Custom, Inherited, Different, Unknown };

		DepthSelectedType depthSelected = DepthSelectedType::Unknown;
		Vector<Pair<IAbstractValueProxy*, IAbstractValueProxy*>> depthTargets;
		for (auto actor : mTargetActors)
		{
			ISceneDrawable* drawable = nullptr;
			if (auto comp = actor->GetComponent<DrawableComponent>())
				drawable = comp;
			else
				drawable = dynamic_cast<ISceneDrawable*>(actor);

			if (drawable)
			{
				DepthSelectedType drawableDepthType = drawable->IsDrawingDepthInheritedFromParent() ?
					DepthSelectedType::Inherited : DepthSelectedType::Custom;

				if (depthSelected == DepthSelectedType::Unknown)
					depthSelected = drawableDepthType;
				else if (depthSelected != drawableDepthType)
					depthSelected = DepthSelectedType::Different;
			}
		}

		mDepthInheritanceMenu->SetItemChecked(0, depthSelected == DepthSelectedType::Custom);
		mDepthInheritanceMenu->SetItemChecked(1, depthSelected == DepthSelectedType::Inherited);

		if (depthSelected == DepthSelectedType::Different || depthSelected == DepthSelectedType::Unknown)
			mDepthProperty->SetUnknownValue(0);
		else if (depthSelected == DepthSelectedType::Inherited)
		{
			mDepthProperty->GetEditBox()->SetFilterNames();
			mDepthProperty->GetEditBox()->SetText("Inherited");
		}
		else
		{
			mDepthProperty->GetEditBox()->SetFilterFloat();
			mDepthProperty->Refresh();
		}

		if (mLayoutEnabled)
		{
			mAnchorRightTopProperty->Refresh();
			mAnchorLeftBottomProperty->Refresh();
			moffsetRightTopProperty->Refresh();
			mOffsetLeftBottomProperty->Refresh();
			mMinSizeProperty->Refresh();
			mMaxSizeProperty->Refresh();
			mWeightProperty->Refresh();
		}
	}

	void DefaultActorTransformViewer::OnPropertyChangeCompleted(const String& path,
																const Vector<DataDocument>& prevValue,
																const Vector<DataDocument>& newValue)
	{
		PropertyChangeAction* action = mnew PropertyChangeAction(
			o2EditorSceneScreen.GetSelectedObjects(), path, prevValue, newValue);

		o2EditorApplication.DoneAction(action);
	}

	void DefaultActorTransformViewer::OnDepthPropertyChanged()
	{
		//OnDepthInheritanceSelected(false);
	}

	void DefaultActorTransformViewer::OnDepthInheritanceSelected(bool inherited)
	{
		for (auto actor : mTargetActors)
		{
			ISceneDrawable* drawable = actor->GetComponent<ISceneDrawable>();
			if (!drawable)
				drawable = dynamic_cast<ISceneDrawable*>(actor);

			if (drawable)
				drawable->SetDrawingDepthInheritFromParent(inherited);
		}

		Refresh();
	}
}

DECLARE_CLASS(Editor::DefaultActorTransformViewer);
