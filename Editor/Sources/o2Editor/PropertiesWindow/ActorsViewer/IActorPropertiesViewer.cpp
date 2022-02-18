#include "o2Editor/stdafx.h"
#include "IActorPropertiesViewer.h"

#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/Widgets/Image.h"
#include "o2/Utils/Editor/EditorScope.h"
#include "o2Editor/Core/UI/SpoilerWithHead.h"

namespace Editor
{
	IActorPropertiesViewer::IActorPropertiesViewer()
	{
		PushEditorScopeOnStack scope;

		mSpoiler = o2UI.CreateWidget<SpoilerWithHead>();

		mSpoiler->expandHeight = false;
		mSpoiler->expandWidth = true;
		mSpoiler->fitByChildren = true;
		mSpoiler->borderBottom = 5;
		mSpoiler->SetCaption("Transform");
		mSpoiler->GetIcon()->SetImageName("ui/UI4_component_icon.png");
		mSpoiler->GetIcon()->layout->center -= Vec2F(2, 0);

		mSpoiler->SetExpanded(true);
	}

	IActorPropertiesViewer::~IActorPropertiesViewer()
	{
		if (mSpoiler)
			delete mSpoiler;
	}

	Widget* IActorPropertiesViewer::GetWidget() const
	{
		return mSpoiler;
	}

	void IActorPropertiesViewer::Expand()
	{
		mSpoiler->Expand();
	}

	void IActorPropertiesViewer::Collapse()
	{
		mSpoiler->Collapse();
	}

	void IActorPropertiesViewer::Refresh()
	{}

	bool IActorPropertiesViewer::IsEmpty() const
	{
		return true;
	}

	void IActorPropertiesViewer::SetEnabled(bool enabled)
	{
		if (mEnabled == enabled)
			return;

		mEnabled = enabled;

		if (mEnabled)
			OnEnabled();
		else
			OnDisabled();
	}

	bool IActorPropertiesViewer::IsEnabled() const
	{
		return mEnabled;
	}

}

DECLARE_CLASS(Editor::IActorPropertiesViewer);
