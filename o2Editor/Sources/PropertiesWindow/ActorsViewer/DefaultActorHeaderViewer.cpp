#include "DefaultActorHeaderViewer.h"

#include "PropertiesWindow/Properties/AssetProperty.h"
#include "PropertiesWindow/Properties/LayerProperty.h"
#include "PropertiesWindow/Properties/TagProperty.h"
#include "UI/Button.h"
#include "UI/Toggle.h"
#include "UI/Widget.h"

namespace Editor
{
	DefaultActorHeaderViewer::DefaultActorHeaderViewer()
	{
		mDataView = mnew UIWidget();
		mDataView->layout.minHeight = 60;
	}

	DefaultActorHeaderViewer::~DefaultActorHeaderViewer()
	{
		delete mDataView;
	}

	void DefaultActorHeaderViewer::SetTargetActors(const Vector<Actor*>& actors)
	{

	}

	UIWidget* DefaultActorHeaderViewer::GetWidget() const
	{
		return mDataView;
	}
}
 
CLASS_META(Editor::DefaultActorHeaderViewer)
{
	BASE_CLASS(Editor::IActorHeaderViewer);

	PUBLIC_FIELD(mDataView);
	PUBLIC_FIELD(mEnableToggle);
	PUBLIC_FIELD(mLockToggle);
	PUBLIC_FIELD(mPrefabProperty);
	PUBLIC_FIELD(mPrefabApplyBtn);
	PUBLIC_FIELD(mPrefabRevertBtn);
	PUBLIC_FIELD(mPrefabBreakBtn);
	PUBLIC_FIELD(mTagsProperty);
	PUBLIC_FIELD(mLayerProperty);

	PUBLIC_FUNCTION(void, SetTargetActors, const Vector<Actor*>&);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
}
END_META;
 