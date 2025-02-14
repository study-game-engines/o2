#pragma once

#include "o2/Scene/UI/Widgets/CustomList.h"

namespace o2
{
	// --------------------
	// Text items list view
	// --------------------
	class List: public CustomList
	{
	public:
		PROPERTIES(List);
		PROPERTY(WString, value, SelectItemText, GetSelectedItemText);             // Current selected item text
		PROPERTY(Vector<WString>, values, SetSelectedItems, GetSelectedItemsText); // Current selected items texts

	public:
		Function<void(const WString&)> onSelectedText; // Change text selected event

	public:
		// Default constructor
		List();

		// Copy-constructor
		List(const List& other);

		// Destructor
		~List();

		// Copy operator
		List& operator=(const List& other);

		// Adds new text item and returns position
		int AddItem(const WString& text);

		// Add new text item at position and returns this position
		int AddItem(const WString& text, int position);

		// Adds array of text items
		void AddItems(const Vector<WString>& data);

		// Removes item, if exist
		void RemoveItem(const WString& text);

		// Returns position of item. Returns -1 if can't find item
		int FindItem(const WString& text);

		// Returns item text by position
		WString GetItemText(int position) const;

		// Returns array of all text items
		Vector<WString> GetAllItemsText() const;

		// Returns current selected text item
		WString GetSelectedItemText();

		// Selects item text
		void SelectItemText(const WString& text);

		// Selects items texts
		void SetSelectedItems(const Vector<WString>& items);

		// Returns selected items texts
		Vector<WString> GetSelectedItemsText() const;

		// Returns create menu group in editor
		static String GetCreateMenuGroup();

		SERIALIZABLE(List);

	protected:
		// Called when selected item index was changed
		void OnSelectionChanged() override;
	};
}

CLASS_BASES_META(o2::List)
{
	BASE_CLASS(o2::CustomList);
}
END_META;
CLASS_FIELDS_META(o2::List)
{
	FIELD().PUBLIC().NAME(value);
	FIELD().PUBLIC().NAME(values);
	FIELD().PUBLIC().NAME(onSelectedText);
}
END_META;
CLASS_METHODS_META(o2::List)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const List&);
	FUNCTION().PUBLIC().SIGNATURE(int, AddItem, const WString&);
	FUNCTION().PUBLIC().SIGNATURE(int, AddItem, const WString&, int);
	FUNCTION().PUBLIC().SIGNATURE(void, AddItems, const Vector<WString>&);
	FUNCTION().PUBLIC().SIGNATURE(void, RemoveItem, const WString&);
	FUNCTION().PUBLIC().SIGNATURE(int, FindItem, const WString&);
	FUNCTION().PUBLIC().SIGNATURE(WString, GetItemText, int);
	FUNCTION().PUBLIC().SIGNATURE(Vector<WString>, GetAllItemsText);
	FUNCTION().PUBLIC().SIGNATURE(WString, GetSelectedItemText);
	FUNCTION().PUBLIC().SIGNATURE(void, SelectItemText, const WString&);
	FUNCTION().PUBLIC().SIGNATURE(void, SetSelectedItems, const Vector<WString>&);
	FUNCTION().PUBLIC().SIGNATURE(Vector<WString>, GetSelectedItemsText);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCreateMenuGroup);
	FUNCTION().PROTECTED().SIGNATURE(void, OnSelectionChanged);
}
END_META;
