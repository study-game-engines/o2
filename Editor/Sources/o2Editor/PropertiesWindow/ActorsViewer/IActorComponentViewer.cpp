#include "o2Editor/stdafx.h"
#include "IActorComponentViewer.h"

#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/Widget.h"
#include "o2/Scene/UI/Widgets/Button.h"
#include "o2/Scene/UI/Widgets/Image.h"
#include "o2/Utils/Editor/EditorScope.h"
#include "o2/Utils/StringUtils.h"
#include "o2Editor/Core/Properties/Properties.h"
#include "o2Editor/Core/UI/SpoilerWithHead.h"

namespace Editor
{
	IActorComponentViewer::IActorComponentViewer()
	{
		PushEditorScopeOnStack scope;

		mSpoiler = o2UI.CreateWidget<SpoilerWithHead>();

		mSpoiler->expandHeight = false;
		mSpoiler->expandWidth = true;
		mSpoiler->fitByChildren = true;
		mSpoiler->borderBottom = 5;
		mSpoiler->SetCaption("Component");
		mSpoiler->GetIcon()->SetImageName("ui/UI4_component_icon.png");
		mSpoiler->GetIcon()->layout->center -= Vec2F(2, 0);
		mSpoiler->GetIcon()->GetImage()->SetColor(Color4(235, 255, 253));

		mRemoveButton = o2UI.CreateButton("", THIS_FUNC(RemoveTargetComponents), "close");
		*mRemoveButton->layout = WidgetLayout::Based(BaseCorner::RightTop, Vec2F(20, 20), Vec2F(1, 0));
		mSpoiler->AddInternalWidget(mRemoveButton);

		mSpoiler->SetExpanded(true);
	}

	IActorComponentViewer::~IActorComponentViewer()
	{
		if (mSpoiler)
			delete mSpoiler;
	}

	void IActorComponentViewer::SetTargetComponents(const Vector<Component*>& components)
	{
		mTargetComponents = components;

		if (!components.IsEmpty())
		{
			String caption = components[0]->GetType().InvokeStatic<String>("GetName");
			if (caption.IsEmpty())
				caption = GetSmartName(components[0]->GetType().GetName());

			mSpoiler->SetCaption(caption);
			mSpoiler->GetIcon()->SetImageName(components[0]->GetType().InvokeStatic<String>("GetIcon"));
		}
	}

	Widget* IActorComponentViewer::GetWidget() const
	{
		return mSpoiler;
	}

	void IActorComponentViewer::Expand()
	{
		mSpoiler->Expand();
	}

	void IActorComponentViewer::Collapse()
	{
		mSpoiler->Collapse();
	}

	void IActorComponentViewer::Refresh()
	{	}

	void IActorComponentViewer::SetEnabled(bool enabled)
	{
		if (mEnabled == enabled)
			return;

		mEnabled = enabled;

		if (mEnabled)
			OnEnabled();
		else
			OnDisabled();
	}

	bool IActorComponentViewer::IsEnabled() const
	{
		return mEnabled;
	}

	void IActorComponentViewer::RemoveTargetComponents()
	{
		for (auto comp : mTargetComponents)
			delete comp;

		mTargetComponents.Clear();

	}

}

DECLARE_CLASS(Editor::IActorComponentViewer);
