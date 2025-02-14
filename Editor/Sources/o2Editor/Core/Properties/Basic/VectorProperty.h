#pragma once

#include "o2/Utils/Property.h"
#include "o2Editor/Core/Properties/IPropertyField.h"
#include "o2Editor/Core/Properties/PropertiesContext.h"

using namespace o2;

namespace o2
{
	class Button;
	class Spoiler;
	class Widget;
}

namespace Editor
{
	class IntegerProperty;

	// ----------------------
	// Editor vector property
	// ----------------------
	class VectorProperty: public IPropertyField
	{
	public:
		// Default constructor
		VectorProperty();

		// Copy constructor
		VectorProperty(const VectorProperty& other);

		// Copy operator
		VectorProperty& operator=(const VectorProperty& other);

		// Destructor
		~VectorProperty();

		// Sets fields
		void SetValueAndPrototypeProxy(const TargetsVec& targets) override;

		// Updates and checks value
		void Refresh() override;

		// Returns editing by this field type
		const Type* GetValueType() const override;

		// Sets property caption
		void SetCaption(const WString& text) override;

		// Returns property caption
		WString GetCaption() const override;

		// Adds remove button
		Button* GetRemoveButton() override;

		// Specializes vector type
		void SpecializeType(const Type* type);

		// Specializes field info, processing attributes
		void SetFieldInfo(const FieldInfo* fieldInfo) override;

		// Returns specialized vector type
		const Type* GetSpecializedType() const;

		// Expands property fields
		void Expand();

		// Collapses property field
		void Collapse();

		// Sets properties expanding
		void SetExpanded(bool expanded);

		// Returns is properties expanded
		bool IsExpanded() const;

		IOBJECT(VectorProperty);

	protected:
		struct TargetObjectData
		{
			IAbstractValueProxy* proxy = nullptr;
			void* data = nullptr;
			bool isCreated = false;

			bool operator==(const TargetObjectData& other) const { return proxy == other.proxy; }

			void Refresh();
			void SetValue();
		};

	protected:
		Spoiler* mSpoiler = nullptr; // Properties spoiler

		const VectorType* mVectorType = nullptr; // Vector type

		Vector<Pair<TargetObjectData, TargetObjectData>>  mTargetObjects; // Target objects
						 						    
		Vector<IPropertyField*> mValueProperties;     // Values properties
		Vector<IPropertyField*> mValuePropertiesPool; // Unused value properties pool

		IntegerProperty* mCountProperty = nullptr; // Vector count property

		bool mCountDifferents = false; // Is targets counts of elements are different
		int  mCountOfElements = 0;     // Common count of elements

		HorizontalLayout* mHeaderContainer = nullptr; // Count property and other controls container

		Button* mAddButton = nullptr; // Add button, adds new element at end

		bool mIsRefreshing = false; // Is currently refreshing content. Need to prevent cycled size changing

	protected:
		// Searches controls widgets and layers and initializes them
		void InitializeControls();

		// Returns value pointer from proxy when proxy is pointer proxy
		void* GetProxyValuePointer(IAbstractValueProxy* proxy) const;

		// Returns free element property
		IPropertyField* GetFreeValueProperty();

		// Frees element property
		void FreeValueProperty(IPropertyField* def);

		// Updates element caption
		void UpdateElementCaption(IPropertyField* propertyDef) const;

		// Called when count property changing
		void OnCountChanged(IPropertyField* def);

		// Sets new count of elements in vector
		void Resize(int newCount);

		// Removes element from vector
		void Remove(int idx);

		// Called when add button has pressed
		void OnAddPressed();

		// Called when expanding spoiler, refreshing array properties
		void OnExpand();

		// Returns object target data from proxy. Creates copy of object when it is property proxy, or gets pointer from pointer proxy
		TargetObjectData GetObjectFromProxy(IAbstractValueProxy* proxy);

		// Called when some property changed, sets value via proxy
		void OnPropertyChanged(const String& path, const Vector<DataDocument>& before, 
							   const Vector<DataDocument>& after);
	};
}

CLASS_BASES_META(Editor::VectorProperty)
{
	BASE_CLASS(Editor::IPropertyField);
}
END_META;
CLASS_FIELDS_META(Editor::VectorProperty)
{
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mSpoiler);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mVectorType);
	FIELD().PROTECTED().NAME(mTargetObjects);
	FIELD().PROTECTED().NAME(mValueProperties);
	FIELD().PROTECTED().NAME(mValuePropertiesPool);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mCountProperty);
	FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mCountDifferents);
	FIELD().PROTECTED().DEFAULT_VALUE(0).NAME(mCountOfElements);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mHeaderContainer);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mAddButton);
	FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mIsRefreshing);
}
END_META;
CLASS_METHODS_META(Editor::VectorProperty)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const VectorProperty&);
	FUNCTION().PUBLIC().SIGNATURE(void, SetValueAndPrototypeProxy, const TargetsVec&);
	FUNCTION().PUBLIC().SIGNATURE(void, Refresh);
	FUNCTION().PUBLIC().SIGNATURE(const Type*, GetValueType);
	FUNCTION().PUBLIC().SIGNATURE(void, SetCaption, const WString&);
	FUNCTION().PUBLIC().SIGNATURE(WString, GetCaption);
	FUNCTION().PUBLIC().SIGNATURE(Button*, GetRemoveButton);
	FUNCTION().PUBLIC().SIGNATURE(void, SpecializeType, const Type*);
	FUNCTION().PUBLIC().SIGNATURE(void, SetFieldInfo, const FieldInfo*);
	FUNCTION().PUBLIC().SIGNATURE(const Type*, GetSpecializedType);
	FUNCTION().PUBLIC().SIGNATURE(void, Expand);
	FUNCTION().PUBLIC().SIGNATURE(void, Collapse);
	FUNCTION().PUBLIC().SIGNATURE(void, SetExpanded, bool);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsExpanded);
	FUNCTION().PROTECTED().SIGNATURE(void, InitializeControls);
	FUNCTION().PROTECTED().SIGNATURE(void*, GetProxyValuePointer, IAbstractValueProxy*);
	FUNCTION().PROTECTED().SIGNATURE(IPropertyField*, GetFreeValueProperty);
	FUNCTION().PROTECTED().SIGNATURE(void, FreeValueProperty, IPropertyField*);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateElementCaption, IPropertyField*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCountChanged, IPropertyField*);
	FUNCTION().PROTECTED().SIGNATURE(void, Resize, int);
	FUNCTION().PROTECTED().SIGNATURE(void, Remove, int);
	FUNCTION().PROTECTED().SIGNATURE(void, OnAddPressed);
	FUNCTION().PROTECTED().SIGNATURE(void, OnExpand);
	FUNCTION().PROTECTED().SIGNATURE(TargetObjectData, GetObjectFromProxy, IAbstractValueProxy*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnPropertyChanged, const String&, const Vector<DataDocument>&, const Vector<DataDocument>&);
}
END_META;
