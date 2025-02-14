#pragma once

#include "o2/Utils/Basic/IObject.h"
#include "o2/Utils/Reflection/Reflection.h"
#include "o2/Utils/Reflection/Type.h"
#include "o2/Utils/Types/Containers/Vector.h"

using namespace o2;

namespace o2
{
	class Widget;
	class WidgetLayer;
}

namespace Editor
{
	class SpoilerWithHead;

	// -------------------------------------------
	// Editor widget layer layout viewer interface
	// -------------------------------------------
	class IWidgetLayerLayoutViewer : public IObject
	{
	public:
		// Default constructor. Initializes data widget
		IWidgetLayerLayoutViewer();

		// Virtual destructor
		virtual ~IWidgetLayerLayoutViewer();

		// Sets target actors
		virtual void SetTargetLayers(const Vector<WidgetLayer*>& layers) {}

		// Returns data widget
		virtual Widget* GetWidget() const;

		// Expands view
		void Expand();

		// Collapse view
		void Collapse();

		// Updates properties values
		virtual void Refresh();

		// Sets viewer enabled
		void SetEnabled(bool enabled);

		// Returns is viewer enabled
		bool IsEnabled() const;

		IOBJECT(IWidgetLayerLayoutViewer);

	protected:
		SpoilerWithHead* mSpoiler = nullptr;

		bool mEnabled = false; // Is viewer enabled 

	protected:
		// Enable viewer event function
		virtual void OnEnabled() {}

		// Disable viewer event function
		virtual void OnDisabled() {}
	};
}

CLASS_BASES_META(Editor::IWidgetLayerLayoutViewer)
{
	BASE_CLASS(o2::IObject);
}
END_META;
CLASS_FIELDS_META(Editor::IWidgetLayerLayoutViewer)
{
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mSpoiler);
	FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mEnabled);
}
END_META;
CLASS_METHODS_META(Editor::IWidgetLayerLayoutViewer)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().SIGNATURE(void, SetTargetLayers, const Vector<WidgetLayer*>&);
	FUNCTION().PUBLIC().SIGNATURE(Widget*, GetWidget);
	FUNCTION().PUBLIC().SIGNATURE(void, Expand);
	FUNCTION().PUBLIC().SIGNATURE(void, Collapse);
	FUNCTION().PUBLIC().SIGNATURE(void, Refresh);
	FUNCTION().PUBLIC().SIGNATURE(void, SetEnabled, bool);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsEnabled);
	FUNCTION().PROTECTED().SIGNATURE(void, OnEnabled);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDisabled);
}
END_META;
