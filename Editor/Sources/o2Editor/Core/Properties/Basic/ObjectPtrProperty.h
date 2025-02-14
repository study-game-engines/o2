#pragma once

#include "o2/Utils/Property.h"
#include "o2Editor/Core/Properties/IPropertyField.h"
#include "o2Editor/Core/Properties/PropertiesContext.h"

using namespace o2;

namespace o2
{
	class Button;
	class ContextMenu;
	class Label;
	class Spoiler;
	class Widget;
}

namespace Editor
{
	class IObjectPropertiesViewer;

	// ------------------------------
	// Editor object pointer property
	// ------------------------------
	class ObjectPtrProperty: public IPropertyField
	{
	public:
		// Default constructor
		ObjectPtrProperty();

		// Copy constructor
		ObjectPtrProperty(const ObjectPtrProperty& other);

		// Copy operator
		ObjectPtrProperty& operator=(const ObjectPtrProperty& other);

		// Sets fields
		void SetValueAndPrototypeProxy(const TargetsVec& targets) override;

		// Updates and checks value
		void Refresh() override;

		// Returns editing by this field type
		const Type* GetValueType() const override;

		// Returns editing by this field type by static function, can't be changed during runtime
		static const Type* GetValueTypeStatic();

		// Specializes field info, processing attributes
		void SetFieldInfo(const FieldInfo* fieldInfo) override;

		// Sets property caption
		void SetCaption(const WString& text) override;

		// Returns property caption
		WString GetCaption() const override;

		// Adds remove button
		Button* GetRemoveButton() override;

		// Sets basic object type, used in create
		void SetBasicType(const ObjectType* type);

		// Expands property fields
		void Expand();

		// Collapses property field
		void Collapse();

		// Sets properties expanding
		void SetExpanded(bool expanded);

		// Returns is properties expanded
		bool IsExpanded() const;

		IOBJECT(ObjectPtrProperty);

	protected:
		const ObjectType* mBasicObjectType = nullptr;   // Base object type, used for create
		const ObjectType* mCurrentObjectType = nullptr; // Type of target objects
		const ObjectType* mBuiltObjectType = nullptr;   // Type of built object properties

		bool mAvailableMultipleTypes = false; // Is multiple types creation available

		bool mDontDeleteEnabled = false; // When it is true, delete button is disabled
		bool mNoHeader = false;          // Is no header attribute exists
		bool mExpanded = false;          // True when must be expanded after creating object viewer

		IObjectPropertiesViewer* mObjectViewer = nullptr; // Object viewer

		Label*            mCaption = nullptr;            // Property caption, used when object is empty and there are no spoiler
		HorizontalLayout* mHeaderContainer = nullptr;    // Type caption and create/delete button container widget, placed on spoiler head
		Label*            mTypeCaption = nullptr;        // Caption that shows type of object or nullptr
		Button*           mCreateDeleteButton = nullptr; // Create and delete button. Create - when value is nullptr, delete - when not
		ContextMenu*      mCreateMenu = nullptr;         // Create object context menu. Initializes with types derived from mObjectType 
													     // when this type changing and create button were pressed

		bool mContextInitialized = false;    // True when context menu initialized with available types of objects. 
		                                     // Context menu initializes when type changed and create button pressed

		bool mImmediateCreateObject = false; // True when no reason to show context menu, because there is only one available type

	protected:
		// Called when property puts in buffer. Here you can release your shared resources
		void OnFreeProperty() override;

		// Searches controls widgets and layers and initializes them
		void InitializeControls();

		// Updates viewer header caption and header container 
		void UpdateViewerHeader();

		// Called when create button pressed and shows create object menu
		void OnCreateOrDeletePressed();

		// Creates object by type
		void CreateObject(const ObjectType* type);

		// Stores values to data
		void StoreValues(Vector<DataDocument>& data) const override;

		// Converts proxy to IObject property, gets value and returns
		IObject* GetProxy(IAbstractValueProxy* proxy) const;

		// Converts proxy to IObject proxy, then sets value via proxy
		void SetProxy(IAbstractValueProxy* proxy, IObject* object);
	};
}

CLASS_BASES_META(Editor::ObjectPtrProperty)
{
	BASE_CLASS(Editor::IPropertyField);
}
END_META;
CLASS_FIELDS_META(Editor::ObjectPtrProperty)
{
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mBasicObjectType);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mCurrentObjectType);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mBuiltObjectType);
	FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mAvailableMultipleTypes);
	FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mDontDeleteEnabled);
	FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mNoHeader);
	FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mExpanded);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mObjectViewer);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mCaption);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mHeaderContainer);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mTypeCaption);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mCreateDeleteButton);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mCreateMenu);
	FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mContextInitialized);
	FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mImmediateCreateObject);
}
END_META;
CLASS_METHODS_META(Editor::ObjectPtrProperty)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const ObjectPtrProperty&);
	FUNCTION().PUBLIC().SIGNATURE(void, SetValueAndPrototypeProxy, const TargetsVec&);
	FUNCTION().PUBLIC().SIGNATURE(void, Refresh);
	FUNCTION().PUBLIC().SIGNATURE(const Type*, GetValueType);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(const Type*, GetValueTypeStatic);
	FUNCTION().PUBLIC().SIGNATURE(void, SetFieldInfo, const FieldInfo*);
	FUNCTION().PUBLIC().SIGNATURE(void, SetCaption, const WString&);
	FUNCTION().PUBLIC().SIGNATURE(WString, GetCaption);
	FUNCTION().PUBLIC().SIGNATURE(Button*, GetRemoveButton);
	FUNCTION().PUBLIC().SIGNATURE(void, SetBasicType, const ObjectType*);
	FUNCTION().PUBLIC().SIGNATURE(void, Expand);
	FUNCTION().PUBLIC().SIGNATURE(void, Collapse);
	FUNCTION().PUBLIC().SIGNATURE(void, SetExpanded, bool);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsExpanded);
	FUNCTION().PROTECTED().SIGNATURE(void, OnFreeProperty);
	FUNCTION().PROTECTED().SIGNATURE(void, InitializeControls);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateViewerHeader);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCreateOrDeletePressed);
	FUNCTION().PROTECTED().SIGNATURE(void, CreateObject, const ObjectType*);
	FUNCTION().PROTECTED().SIGNATURE(void, StoreValues, Vector<DataDocument>&);
	FUNCTION().PROTECTED().SIGNATURE(IObject*, GetProxy, IAbstractValueProxy*);
	FUNCTION().PROTECTED().SIGNATURE(void, SetProxy, IAbstractValueProxy*, IObject*);
}
END_META;
