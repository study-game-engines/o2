#include "DefaultActorComponentViewer.h"

#include "PropertiesWindow/PropertiesWindow.h"
#include "Scene/Component.h"
#include "UI/Label.h"
#include "UI/Spoiler.h"
#include "UI/VerticalLayout.h"
#include "UI/Widget.h"

namespace Editor
{
	DefaultActorComponentViewer::DefaultActorComponentViewer()
	{}

	DefaultActorComponentViewer::~DefaultActorComponentViewer()
	{}

	void DefaultActorComponentViewer::SetTargetComponents(const Vector<Component*>& components)
	{
		if (!components.IsEmpty())
		{
			String caption = components[0]->GetName();
			if (caption.IsEmpty())
				caption = o2EditorProperties.MakeSmartFieldName(mComponentType->GetName());
				
			mNameCaption->text = caption;
		}

		mTargetComponents = components;
		mFieldProperties.Set(components.Select<IObject*>([](auto x) { return (IObject*)x; }));
	}

	const Type* DefaultActorComponentViewer::GetComponentType() const
	{
		return mComponentType;
	}

	void DefaultActorComponentViewer::SepcializeComponentType(const Type* type)
	{
		mDataView->name = "component " + type->GetName();
		mComponentType = type;
		o2EditorProperties.BuildTypeViewer((UIVerticalLayout*)mPropertiesLayout, type, mFieldProperties);

		mSpoiler->name = "spoiler " + type->GetName();
		mPropertiesLayout->name = "properties " + type->GetName();
	}

	void DefaultActorComponentViewer::Refresh()
	{
		mFieldProperties.Set(mTargetComponents.Select<IObject*>([](auto x) { return (IObject*)x; }));
	}

}

CLASS_META(Editor::DefaultActorComponentViewer)
{
	BASE_CLASS(Editor::IActorComponentViewer);

	PROTECTED_FIELD(mTargetComponents);
	PROTECTED_FIELD(mFieldProperties);
	PROTECTED_FIELD(mComponentType);

	PUBLIC_FUNCTION(void, SetTargetComponents, const Vector<Component*>&);
	PUBLIC_FUNCTION(const Type*, GetComponentType);
	PUBLIC_FUNCTION(void, SepcializeComponentType, const Type*);
	PUBLIC_FUNCTION(void, Refresh);
}
END_META;
 