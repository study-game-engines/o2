#pragma once

#include "o2/Events/CursorAreaEventsListener.h"

namespace o2
{
	class DragableObject;
	class ISelectableDragableObjectsGroup;

	// --------------------------------
	// Drag objects drop area interface
	// --------------------------------
	class DragDropArea: virtual public CursorAreaEventsListener
	{
	protected:
		// Called when some DragEventsListener was dropped to this
		virtual void OnDropped(DragableObject* draggable);

		// Called when some selectable listeners was dropped to this
		virtual void OnDropped(ISelectableDragableObjectsGroup* group);

		// Called when some drag listener was entered to this area
		virtual void OnDragEnter(DragableObject* draggable);

		// Called when some drag listeners was entered to this area
		virtual void OnDragEnter(ISelectableDragableObjectsGroup* group);

		// Called when some drag listener was dragged above this area
		virtual void OnDraggedAbove(DragableObject* draggable);

		// Called when some drag listeners was dragged above this area
		virtual void OnDraggedAbove(ISelectableDragableObjectsGroup* group);

		// Called when some drag listener was exited from this area
		virtual void OnDragExit(DragableObject* draggable);

		// Called when some drag listeners was exited from this area
		virtual void OnDragExit(ISelectableDragableObjectsGroup* group);

		friend class DragableObject;
		friend class SelectableDragableObject;
	};

	// --------------------------
	// Draggable object interface
	// --------------------------
	class DragableObject: virtual public CursorAreaEventsListener
	{
	public:
		// Default constructor
		DragableObject();

		// Destructor
		virtual ~DragableObject();

		// Returns true if it's dragging
		bool IsDragging() const;

		// Called when listener was drawn
		void OnDrawn() override;

		// Returns last cursor pressed point
		Vec2F GetCursorPressedPoint() const;

	protected:
		bool  mIsDragging = false;           // True when dragging
		float mDragDistanceThreshold = 3.0f; // Drag distance threshold: object starts dragging when cursor moves more tan this distance
		Vec2F mPressedCursorPos;             // Cursor pressed position
		int   mPressedCursorId;              // Id of pressed cursor

		DragDropArea* mDragDropArea = nullptr; // Drag drop area under this when dragging

	protected:
		// Returns drag drop area under cursor
		DragDropArea* GetDropAreaUnderCursor(CursorId cursorId) const;

		// Called when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor) override;

		// Called when cursor released (only when cursor pressed this at previous time)
		void OnCursorReleased(const Input::Cursor& cursor) override;

		// Called when cursor pressing was broken (when scrolled scroll area or some other)
		void OnCursorPressBreak(const Input::Cursor& cursor) override;

		// Called when cursor moved on this (or moved outside when this was pressed)
		void OnCursorStillDown(const Input::Cursor& cursor) override;

		// Called when started dragging
		virtual void OnDragStart(const Input::Cursor& cursor);

		// Called when dragged
		virtual void OnDragged(const Input::Cursor& cursor, DragDropArea* area);

		// Called when dragging completed
		virtual void OnDragEnd(const Input::Cursor& cursor);

		// Called when this was dropped
		virtual void OnDropped(DragDropArea* area);

		friend class EventSystem;
		friend class DragDropArea;
	};

	class SelectableDragableObject;

	// --------------------------------------------
	// Selectable draggable objects group interface
	// --------------------------------------------
	class ISelectableDragableObjectsGroup: virtual public CursorAreaEventsListener
	{
	public:
		// Returns selected objects in group
		virtual Vector<SelectableDragableObject*> GetSelectedDragObjects() const = 0;

		// Returns all objects in group 
		virtual Vector<SelectableDragableObject*> GetAllObjects() const = 0;

		// Selects object
		virtual void Select(SelectableDragableObject* object) = 0;

		// Deselects object
		virtual void Deselect(SelectableDragableObject* object) = 0;

		// Adds selectable object to group
		virtual void AddSelectableObject(SelectableDragableObject* object) = 0;

		// Removes selectable object from group
		virtual void RemoveSelectableObject(SelectableDragableObject* object) = 0;

		// Deselects all in group
		void DeselectAll();

		// Selects all in group
		void SelectAll();

		// Returns current dragging object or null
		const SelectableDragableObject* GetDraggingObject() const;

	protected:
		SelectableDragableObject* mDraggingObject = nullptr;

	protected:
		// Called when selectable draggable object was pressed
		virtual void OnSelectableObjectCursorPressed(SelectableDragableObject* object, const Input::Cursor& cursor) {}

		// Called when selectable draggable object was released
		virtual void OnSelectableObjectCursorReleased(SelectableDragableObject* object, const Input::Cursor& cursor) {}

		// Called when selectable object was began to drag
		virtual void OnSelectableObjectBeganDragging(SelectableDragableObject* object) {}

		friend class SelectableDragableObject;
	};

	// ----------------------------------
	// Selectable draggable objects group
	// ----------------------------------
	class SelectableDragableObjectsGroup: public ISelectableDragableObjectsGroup
	{
	public:
		// Destructor
		virtual ~SelectableDragableObjectsGroup();

		// Returns selected listeners in group
		Vector<SelectableDragableObject*> GetSelectedDragObjects() const override;

		// Returns all objects in group 
		Vector<SelectableDragableObject*> GetAllObjects() const override;

		// Selects object
		void Select(SelectableDragableObject* object) override;

		// Deselects object
		void Deselect(SelectableDragableObject* object) override;

		// Adds selectable object to group
		void AddSelectableObject(SelectableDragableObject* object) override;

		// Removes selectable object from group
		void RemoveSelectableObject(SelectableDragableObject* object) override;

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point) override;

	protected:
		Vector<SelectableDragableObject*> mObjects;         // Listeners in group
		Vector<SelectableDragableObject*> mSelectedObjects; // Selected listeners in group

	protected:
		// Called when selectable draggable object was pressed
		void OnSelectableObjectCursorPressed(SelectableDragableObject* object, const Input::Cursor& cursor) override;

		friend class SelectableDragableObject;
	};

	// -------------------------------------
	// Selectable draggable object interface
	// -------------------------------------
	class SelectableDragableObject: public DragableObject
	{
	public:
		// Virtual destructor. Removes himself from selectable group, if need
		virtual ~SelectableDragableObject();

		// Returns is this selected
		bool IsSelected() const;

		// Sets is this selected
		virtual void SetSelected(bool selected);

		// Selects this
		void Select();

		// Deselects this
		void Deselect();

		// Sets selection group
		void SetSelectionGroup(ISelectableDragableObjectsGroup* group);

		// Returns selection group
		ISelectableDragableObjectsGroup* GetSelectionGroup() const;

		// Sets dragging available only when selected
		void SetDragOnlySelected(bool value);

		// Returns dragging available only when selected
		bool IsDragOnlySelected() const;

		// Returns true when input events can be handled by down listeners
		bool IsInputTransparent() const override;

	protected:
		float mDragDelay = 0.35f;   // Delay before disabling drag
		float mPressedTime = 0.0f; // Time when cursor pressed 

		bool mDragOnlySelected = false; // Is dragging available only when selected
		bool mIsSelected  = false;      // Is this selected
		bool mIsDragAvailable = true;   // Is dragging available

		ISelectableDragableObjectsGroup* mSelectGroup = nullptr; // Selection group

	protected:
		// Called when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor) override;

		// Called when cursor moved on this (or moved outside when this was pressed)
		void OnCursorStillDown(const Input::Cursor& cursor) override;

		// Called when cursor released (only when cursor pressed this at previous time)
		void OnCursorReleased(const Input::Cursor& cursor) override;

		// Called when cursor released outside this(only when cursor pressed this at previous time)
		void OnCursorReleasedOutside(const Input::Cursor& cursor) override;

		// Called when this was selected
		virtual void OnSelected();

		// Called when this was unselected
		virtual void OnDeselected();

		friend class ISelectableDragableObjectsGroup;
		friend class SelectableDragableObjectsGroup;
	};
}
