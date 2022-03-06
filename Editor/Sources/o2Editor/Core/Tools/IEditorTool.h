#pragma once

#include "o2/Application/Input.h"
#include "o2/Utils/Basic/IObject.h"
#include "o2/Utils/Reflection/Type.h"
#include "o2/Utils/Serialization/DataValue.h"

using namespace o2;

namespace o2
{
	class SceneEditableObject;
	class Toggle;
}

namespace Editor
{
	// ---------------------
	// Editor tool interface
	// ---------------------
	class IEditTool: public IObject
	{
	public:
		virtual ~IEditTool() {}

		// Creates if required and retuns toggle button for menu panel
		virtual Toggle* GetPanelToggle() const;

		IOBJECT(IEditTool);

	protected:
		bool mNeedRedraw = false; // True when need redraw scene

		mutable Toggle* mPanelToggle = nullptr; // Toggle button in menu panel

	protected:
		// Creates toggle button for menu panel
		virtual Toggle* CreatePanelToggle() const;

		// Returns toggle in menu panel icon name
		virtual String GetPanelIcon() const;

		// Returns shortcut keys for toggle
		virtual ShortcutKeys GetShortcut() const;

		// Draws tool
		virtual void DrawScene() {}

		// Draws tool
		virtual void DrawScreen() {}

		// It is called when scene actors was changed
		virtual void OnSceneChanged(Vector<SceneEditableObject*> changedObjects) {}

		// Updates tool
		virtual void Update(float dt) {}

		// It is called when tool was enabled
		virtual void OnEnabled() {}

		// It is called when tool was disabled
		virtual void OnDisabled() {}

		// It is called when objects selection was changed
		virtual void OnObjectsSelectionChanged(Vector<SceneEditableObject*> objects) {}

		// It is called when cursor pressed on this
		virtual void OnCursorPressed(const Input::Cursor& cursor) {}

		// It is called when cursor released (only when cursor pressed this at previous time)
		virtual void OnCursorReleased(const Input::Cursor& cursor) {}

		// It is called when cursor pressing was broken (when scrolled scroll area or some other)
		virtual void OnCursorPressBreak(const Input::Cursor& cursor) {}

		// It is called when cursor stay down during frame
		virtual void OnCursorStillDown(const Input::Cursor& cursor) {}

		// It is called when cursor moved on this (or moved outside when this was pressed)
		virtual void OnCursorMoved(const Input::Cursor& cursor) {}

		// It is called when cursor enters this object
		virtual void OnCursorEnter(const Input::Cursor& cursor) {}

		// It is called when cursor exits this object
		virtual void OnCursorExit(const Input::Cursor& cursor) {}

		// It is called when right mouse button was pressed on this
		virtual void OnCursorRightMousePressed(const Input::Cursor& cursor) {}

		// It is called when right mouse button stay down on this
		virtual void OnCursorRightMouseStayDown(const Input::Cursor& cursor) {}

		// It is called when right mouse button was released (only when right mouse button pressed this at previous time)
		virtual void OnCursorRightMouseReleased(const Input::Cursor& cursor) {}

		// It is called when middle mouse button was pressed on this
		virtual void OnCursorMiddleMousePressed(const Input::Cursor& cursor) {}

		// It is called when middle mouse button stay down on this
		virtual void OnCursorMiddleMouseStayDown(const Input::Cursor& cursor) {}

		// It is called when middle mouse button was released (only when middle mouse button pressed this at previous time)
		virtual void OnCursorMiddleMouseReleased(const Input::Cursor& cursor) {}

		// It is called when scrolling
		virtual void OnScrolled(float scroll) {}

		// It is called when key was pressed
		virtual void OnKeyPressed(const Input::Key& key) {}

		// It is called when key was released
		virtual void OnKeyReleased(const Input::Key& key) {}

		// It is called when key stay down during frame
		virtual void OnKeyStayDown(const Input::Key& key) {}

		friend class SceneEditScreen;
	};
}

CLASS_BASES_META(Editor::IEditTool)
{
	BASE_CLASS(o2::IObject);
}
END_META;
CLASS_FIELDS_META(Editor::IEditTool)
{
	FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mNeedRedraw);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mPanelToggle);
}
END_META;
CLASS_METHODS_META(Editor::IEditTool)
{

	FUNCTION().PUBLIC().SIGNATURE(Toggle*, GetPanelToggle);
	FUNCTION().PROTECTED().SIGNATURE(Toggle*, CreatePanelToggle);
	FUNCTION().PROTECTED().SIGNATURE(String, GetPanelIcon);
	FUNCTION().PROTECTED().SIGNATURE(ShortcutKeys, GetShortcut);
	FUNCTION().PROTECTED().SIGNATURE(void, DrawScene);
	FUNCTION().PROTECTED().SIGNATURE(void, DrawScreen);
	FUNCTION().PROTECTED().SIGNATURE(void, OnSceneChanged, Vector<SceneEditableObject*>);
	FUNCTION().PROTECTED().SIGNATURE(void, Update, float);
	FUNCTION().PROTECTED().SIGNATURE(void, OnEnabled);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDisabled);
	FUNCTION().PROTECTED().SIGNATURE(void, OnObjectsSelectionChanged, Vector<SceneEditableObject*>);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorPressed, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorReleased, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorPressBreak, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorStillDown, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorMoved, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorEnter, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorExit, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorRightMousePressed, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorRightMouseStayDown, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorRightMouseReleased, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorMiddleMousePressed, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorMiddleMouseStayDown, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorMiddleMouseReleased, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnScrolled, float);
	FUNCTION().PROTECTED().SIGNATURE(void, OnKeyPressed, const Input::Key&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnKeyReleased, const Input::Key&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnKeyStayDown, const Input::Key&);
}
END_META;
