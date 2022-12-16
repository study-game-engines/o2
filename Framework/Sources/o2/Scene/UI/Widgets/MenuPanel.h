#pragma once

#include "o2/Events/DrawableCursorEventsListener.h"
#include "o2/Scene/UI/Widgets/ContextMenu.h"
#include "o2/Scene/UI/Widget.h"
#include "o2/Utils/Function/Function.h"

namespace o2
{
	class HorizontalLayout;

	// -------------
	// UI Menu panel
	// -------------
	class MenuPanel: public Widget, public DrawableCursorEventsListener
	{
	public:
		class Item: public ISerializable
		{
		public:
			WString text; // @SERIALIZABLE

			Vector<ContextMenu::Item*> subItems; // @SERIALIZABLE

			Function<void()> onClick;

		public:
			Item();
			Item(const WString& text, const Vector<ContextMenu::Item*>& subItems);
			Item(const WString& text, const Function<void()> onClick);

			bool operator==(const Item& other) const;

			SERIALIZABLE(Item);
		};

	public:
		// Default constructor
		MenuPanel();

		// Copy-constructor
		MenuPanel(const MenuPanel& other);

		// Destructor
		~MenuPanel();

		// Copy-operator
		MenuPanel& operator=(const MenuPanel& other);

		// Updates drawables, states and widget
		void Update(float dt) override;

		// Draws widget
		void Draw() override;

		// Add item
		Widget* AddItem(const Item& item);

		// Adds item by path ("node/sub node/target")
		void AddItem(const WString& path, const Function<void()>&
					 clickFunc = Function<void()>(),
					 const ImageAssetRef& icon = ImageAssetRef(),
					 const ShortcutKeys& shortcut = ShortcutKeys());

		// Adds toggle item by path ("node/sub node/target")
		void AddToggleItem(const WString& path, bool value,
						   const Function<void(bool)>& clickFunc = Function<void(bool)>(),
						   const ImageAssetRef& icon = ImageAssetRef(),
						   const ShortcutKeys& shortcut = ShortcutKeys());

		// Inserts item at position
		void InsertItem(const Item& item, int position);

		// Adds array of items
		void AddItems(Vector<Item> items);

		// Inserts array of items at position
		void InsertItems(Vector<Item> items, int position);

		// Returns item at position
		Item GetItem(int position);

		// Returns array of all items
		Vector<Item> GetItems() const;

		// Removes item at position
		void RemoveItem(int position);

		// Removes item by path
		void RemoveItem(const WString& path);

		// Removes all items
		void RemoveAllItems();

		// Returns items vertical layout
		HorizontalLayout* GetItemsLayout() const;

		// Returns item sample
		Widget* GetItemSample() const;

		// Sets item sample
		void SetItemSample(Widget* sample);

		// Returns selection drawable
		Sprite* GetSelectionDrawable() const;

		// Sets selection drawable layout
		void SetSelectionDrawableLayout(const Layout& layout);

		// Returns selection drawable layout
		Layout GetSelectionDrawableLayout() const;

		// Returns create menu group in editor
		static String GetCreateMenuGroup();

		SERIALIZABLE(MenuPanel);

	protected:
		HorizontalLayout* mLayout = nullptr; // Items layout

		Widget* mItemSample = nullptr; // Item sample @SERIALIZABLE

		Vector<Function<void()>> mClickFunctions; // Items click functions

		Sprite* mSelectionDrawable = nullptr; // Selection sprite @SERIALIZABLE
		Layout  mSelectionLayout;             // Selection layout, result selection area depends on selected item @SERIALIZABLE

		RectF mCurrentSelectionRect;  // Current selection rectangle (for smoothing)
		RectF mTargetSelectionRect;   // Target selection rectangle (over selected item)
		Vec2F mLastSelectCheckCursor; // Last cursor position on selection check

		int   mSelectedItem = -1;            // Index of selected item
		float mSelectSubContextTime = -1.0f; // Time to appearing selected sub context

		ContextMenu* mOpenedContext = nullptr; // Last opened context in menu

	protected:
		// Called when visible was changed
		void OnEnableInHierarchyChanged() override;

		// Creates sub context menus by path
		ContextMenu* CreateSubContext(WString& path);

		// Creates item widget
		Widget* CreateItem(const Item& item);

		// Returns item info
		Item GetItemDef(int idx) const;

		// Returns item widget under point and stores index in idxPtr, if not null
		Widget* GetItemUnderPoint(const Vec2F& point, int* idxPtr);

		// Updates hover
		void UpdateHover(const Vec2F& point);

		// Called when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor) override;

		// Called when cursor stay down during frame
		void OnCursorStillDown(const Input::Cursor& cursor) override;

		// Called when cursor released (only when cursor pressed this at previous time)
		void OnCursorReleased(const Input::Cursor& cursor) override;

		// Called when cursor pressing was broken (when scrolled scroll area or some other)
		void OnCursorPressBreak(const Input::Cursor& cursor) override;

		// Called when cursor moved on this (or moved outside when this was pressed)
		void OnCursorMoved(const Input::Cursor& cursor) override;

		// Called when cursor exits this object
		void OnCursorExit(const Input::Cursor& cursor) override;
	};
}

CLASS_BASES_META(o2::MenuPanel)
{
	BASE_CLASS(o2::Widget);
	BASE_CLASS(o2::DrawableCursorEventsListener);
}
END_META;
CLASS_FIELDS_META(o2::MenuPanel)
{
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mLayout);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(nullptr).NAME(mItemSample);
	FIELD().PROTECTED().NAME(mClickFunctions);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(nullptr).NAME(mSelectionDrawable);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().NAME(mSelectionLayout);
	FIELD().PROTECTED().NAME(mCurrentSelectionRect);
	FIELD().PROTECTED().NAME(mTargetSelectionRect);
	FIELD().PROTECTED().NAME(mLastSelectCheckCursor);
	FIELD().PROTECTED().DEFAULT_VALUE(-1).NAME(mSelectedItem);
	FIELD().PROTECTED().DEFAULT_VALUE(-1.0f).NAME(mSelectSubContextTime);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mOpenedContext);
}
END_META;
CLASS_METHODS_META(o2::MenuPanel)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const MenuPanel&);
	FUNCTION().PUBLIC().SIGNATURE(void, Update, float);
	FUNCTION().PUBLIC().SIGNATURE(void, Draw);
	FUNCTION().PUBLIC().SIGNATURE(Widget*, AddItem, const Item&);
	FUNCTION().PUBLIC().SIGNATURE(void, AddItem, const WString&, const Function<void()>&, const ImageAssetRef&, const ShortcutKeys&);
	FUNCTION().PUBLIC().SIGNATURE(void, AddToggleItem, const WString&, bool, const Function<void(bool)>&, const ImageAssetRef&, const ShortcutKeys&);
	FUNCTION().PUBLIC().SIGNATURE(void, InsertItem, const Item&, int);
	FUNCTION().PUBLIC().SIGNATURE(void, AddItems, Vector<Item>);
	FUNCTION().PUBLIC().SIGNATURE(void, InsertItems, Vector<Item>, int);
	FUNCTION().PUBLIC().SIGNATURE(Item, GetItem, int);
	FUNCTION().PUBLIC().SIGNATURE(Vector<Item>, GetItems);
	FUNCTION().PUBLIC().SIGNATURE(void, RemoveItem, int);
	FUNCTION().PUBLIC().SIGNATURE(void, RemoveItem, const WString&);
	FUNCTION().PUBLIC().SIGNATURE(void, RemoveAllItems);
	FUNCTION().PUBLIC().SIGNATURE(HorizontalLayout*, GetItemsLayout);
	FUNCTION().PUBLIC().SIGNATURE(Widget*, GetItemSample);
	FUNCTION().PUBLIC().SIGNATURE(void, SetItemSample, Widget*);
	FUNCTION().PUBLIC().SIGNATURE(Sprite*, GetSelectionDrawable);
	FUNCTION().PUBLIC().SIGNATURE(void, SetSelectionDrawableLayout, const Layout&);
	FUNCTION().PUBLIC().SIGNATURE(Layout, GetSelectionDrawableLayout);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCreateMenuGroup);
	FUNCTION().PROTECTED().SIGNATURE(void, OnEnableInHierarchyChanged);
	FUNCTION().PROTECTED().SIGNATURE(ContextMenu*, CreateSubContext, WString&);
	FUNCTION().PROTECTED().SIGNATURE(Widget*, CreateItem, const Item&);
	FUNCTION().PROTECTED().SIGNATURE(Item, GetItemDef, int);
	FUNCTION().PROTECTED().SIGNATURE(Widget*, GetItemUnderPoint, const Vec2F&, int*);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateHover, const Vec2F&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorPressed, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorStillDown, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorReleased, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorPressBreak, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorMoved, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorExit, const Input::Cursor&);
}
END_META;

CLASS_BASES_META(o2::MenuPanel::Item)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::MenuPanel::Item)
{
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(text);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(subItems);
	FIELD().PUBLIC().NAME(onClick);
}
END_META;
CLASS_METHODS_META(o2::MenuPanel::Item)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const WString&, const Vector<ContextMenu::Item*>&);
	FUNCTION().PUBLIC().CONSTRUCTOR(const WString&, const Function<void()>);
}
END_META;
