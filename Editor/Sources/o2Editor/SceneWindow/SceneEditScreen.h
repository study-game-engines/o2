#pragma once

#include "o2/Events/DrawableCursorEventsListener.h"
#include "o2/Events/KeyboardEventsListener.h"
#include "o2/Render/Camera.h"
#include "o2/Render/IDrawable.h"
#include "o2/Render/TextureRef.h"
#include "o2/Utils/Editor/DragAndDrop.h"
#include "o2/Utils/Singleton.h"
#include "o2Editor/Core/UI/ScrollView.h"

using namespace o2;

namespace o2
{
	class Sprite;
	class SceneEditableObject;
	class DrawableComponent;
	class Tree;
}

// Editor scene screen accessor macros
#define o2EditorSceneScreen SceneEditScreen::Instance()

namespace Editor
{
	class IEditTool;
	class SceneDragHandle;
	class SceneEditorLayer;
	class SceneTree;

	// --------------------
	// Scene editing screen
	// --------------------
	class SceneEditScreen: public DragDropArea, public KeyboardEventsListener,
		public Singleton<SceneEditScreen>, public ScrollView
	{
	public:
		Function<void(const Vector<SceneEditableObject*>&)> onSelectionChanged; // Actors selection change event

		// Default constructor
		SceneEditScreen();

		// Copy-constructor
		SceneEditScreen(const SceneEditScreen& other);

		// Destructor
		~SceneEditScreen();

		// Draws widget
		void Draw() override;

		// Called when required to redraw content. Sets flag and redraws at next frame
		void NeedRedraw();

		// Updates drawables, states and widget
		void Update(float dt) override;

		// Returns is listener scrollable
		bool IsScrollable() const override;

		// Transforms point from screen space to scene space
		Vec2F ScreenToScenePoint(const Vec2F& point);

		// Transforms point from scene space to screen space
		Vec2F SceneToScreenPoint(const Vec2F& point);

		// Transforms point from screen space to scene space
		Vec2F ScreenToSceneVector(const Vec2F& point);

		// Transforms point from scene space to screen space
		Vec2F SceneToScreenVector(const Vec2F& point);

		// Draws object selection
		void DrawObjectSelection(SceneEditableObject* object, const Color4& color);

		// Selects objects
		void SelectObjects(Vector<SceneEditableObject*> objects, bool additive = true);

		// Selects object
		void SelectObject(SceneEditableObject* object, bool additive = true);

		// Selects all objects
		void SelectAllObjects();

		// Clears objects selection
		void ClearSelection();

		// Returns left top widgets container, can be used for tools additional controls
		HorizontalLayout* GetLeftTopWidgetsContainer();

		// Returns right top widgets container, can be used for tools additional controls
		HorizontalLayout* GetRightTopWidgetsContainer();

		// Returns left bottom widgets container, can be used for tools additional controls
		HorizontalLayout* GetLeftBottomWidgetsContainer();

		// Returns right bottom widgets container, can be used for tools additional controls
		HorizontalLayout* GetRightBottomWidgetsContainer();

		// Adds editable layer
		void AddEditorLayer(SceneEditorLayer* layer);

		// Removes editable layer
		void RemoveEditorLayer(SceneEditorLayer* layer);

		// Sets layers with name enabled
		void SetLayerEnabled(const String& name, bool enabled);

		// Returns is layer enabled
		bool IsLayerEnabled(const String& name) const;

		// Selects tool with type
		template<typename _type>
		void SelectTool();

		// Selects tool
		void SelectTool(const IEditTool* tool);

		// Returns selected tool
		IEditTool* GetSelectedTool() const;

		// Adds tool
		void AddTool(IEditTool* tool);

		// Removes tool
		void RemoveTool(IEditTool* tool);

		// Returns tool by type, or null if it doesn't exists
		template<typename _type>
		_type* GetTool();

		// Returns all registered tools
		const Vector<IEditTool*>& GetTools() const;

		// Returns selected objects array
		const Vector<SceneEditableObject*>& GetSelectedObjects() const;

		// Returns top selected objects in hierarchy
		const Vector<SceneEditableObject*>& GetTopSelectedObjects() const;

		// Returns color for single selected object
		const Color4& GetSingleObjectSelectionColor() const;

		// Return color for multiple selected objects
		const Color4& GetManyObjectsSelectionColor() const;

		// Called when scene was changed and needs to redraw
		void OnSceneChanged();

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point) override;

		IOBJECT(SceneEditScreen);

	protected:
		Color4 mSelectedObjectColor = Color4(220, 220, 220, 255);      // Selected object color
		Color4 mMultiSelectedObjectColor = Color4(220, 220, 220, 100); // Selected object color
		float  mObjectMinimalSelectionSize = 10.0f;                    // Minimal object size on pixels

		SceneTree* mSceneTree; // Pointer to object tree widget

		Vector<SceneEditableObject*> mSelectedObjects;          // Current selected objects
		Vector<SceneEditableObject*> mTopSelectedObjects;       // Current selected objects most top in hierarchy
		bool                         mSelectedFromThis = false; // True if selection changed from this, needs to break recursive selection update

		Vector<IEditTool*> mTools;                 // Available tools
		IEditTool*         mEnabledTool = nullptr; // Current enabled tool

		Vector<SceneDragHandle*> mDragHandles; // Dragging handles array

		Vector<SceneEditorLayer*> mEditorLayers;        // List of editable layers
		Map<String, bool>         mEditorLayersEnabled; // Map of enabled or disabled layers by user

		HorizontalLayout* mLeftTopWidgetsContainer = nullptr;     // Additional controls widgets container at left top
		HorizontalLayout* mRightTopWidgetsContainer = nullptr;    // Additional controls widgets container at right top
		HorizontalLayout* mLeftBottomWidgetsContainer = nullptr;  // Additional controls widgets container at left bottom
		HorizontalLayout* mRightBottomWidgetsContainer = nullptr; // Additional controls widgets container at right bottom
		
	protected:
		// Initializes tools
		void InitializeTools();

		// Creates and configures widgets container with specified base corner
		HorizontalLayout* InitializeWidgetsContainer(BaseCorner baseCorner);

		// Returns true if some handle hovered or pressed by cursor
		bool IsHandleWorking(const Input::Cursor& cursor) const;

		// Called when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor) override;

		// Called when cursor released (only when cursor pressed this at previous time)
		void OnCursorReleased(const Input::Cursor& cursor) override;

		// Called when cursor pressing was broken (when scrolled scroll area or some other)
		void OnCursorPressBreak(const Input::Cursor& cursor) override;

		// Called when cursor stay down during frame
		void OnCursorStillDown(const Input::Cursor& cursor) override;

		// Called when cursor moved on this (or moved outside when this was pressed)
		void OnCursorMoved(const Input::Cursor& cursor) override;

		// Called when cursor enters this object
		void OnCursorEnter(const Input::Cursor& cursor) override;

		// Called when cursor exits this object
		void OnCursorExit(const Input::Cursor& cursor) override;

		// Called when right mouse button was pressed on this
		void OnCursorRightMousePressed(const Input::Cursor& cursor) override;

		// Called when right mouse button stay down on this
		void OnCursorRightMouseStayDown(const Input::Cursor& cursor) override;

		// Called when right mouse button was released (only when right mouse button pressed this at previous time)
		void OnCursorRightMouseReleased(const Input::Cursor& cursor) override;

		// Called when middle mouse button was pressed on this
		void OnCursorMiddleMousePressed(const Input::Cursor& cursor) override;

		// Called when middle mouse button stay down on this
		void OnCursorMiddleMouseStayDown(const Input::Cursor& cursor) override;

		// Called when middle mouse button was released (only when middle mouse button pressed this at previous time)
		void OnCursorMiddleMouseReleased(const Input::Cursor& cursor) override;

		// Called when scrolling
		void OnScrolled(float scroll) override;

		// Called when key was pressed
		void OnKeyPressed(const Input::Key& key) override;

		// Called when key was released
		void OnKeyReleased(const Input::Key& key) override;

		// Called when key stay down during frame
		void OnKeyStayDown(const Input::Key& key) override;

		// Called when changed selected objects from this
		void OnObjectsSelectedFromThis();

		// Redraws scene texture
		void RedrawContent() override;

		// Draws objects drawables components
		void DrawObjects();

		// Draws selection on objects
		void DrawSelection();

		// Binds to scene tree selection window
		void BindSceneTree();

		// Called when scene tree selection changed
		void OnTreeSelectionChanged(Vector<SceneEditableObject*> selectedObjects);

		// Updates top selected objects
		void UpdateTopSelectedObjects();

		// Called when objects was changed
		void OnSceneChanged(Vector<SceneEditableObject*> objects);

		// Clears objects selection
		void ClearSelectionWithoutAction(bool sendSelectedMessage = true);

		// Selects objects
		void SelectObjectsWithoutAction(Vector<SceneEditableObject*> objects, bool additive = true);

		// Selects object
		void SelectObjectWithoutAction(SceneEditableObject* object, bool additive = true);

		// Called when some selectable listeners was dropped to this
		void OnDropped(ISelectableDragableObjectsGroup* group) override;

		// Called when some drag listeners was entered to this area
		void OnDragEnter(ISelectableDragableObjectsGroup* group) override;

		// Called when some drag listeners was dragged above this area
		void OnDraggedAbove(ISelectableDragableObjectsGroup* group) override;

		// Called when some drag listeners was exited from this area
		void OnDragExit(ISelectableDragableObjectsGroup* group) override;

		// Returns that this has transparent input
		bool IsInputTransparent() const override;

		friend class DeleteAction;
		friend class SelectAction;
		friend class SelectionTool;
		friend class SceneDragHandle;
		friend class SceneWindow;
		friend class TreeWindow;
		friend class CreateAction;
	};
}

#include "o2Editor/Core/Tools/IEditorTool.h"

namespace Editor
{
	template<typename _type>
	void SceneEditScreen::SelectTool()
	{
		SelectTool(mTools.FindOrDefault([&](auto x) { return x->GetType() == TypeOf(_type); }));
	}

	template<typename _type>
	_type* SceneEditScreen::GetTool()
	{
		for (auto tool : mTools)
		{
			if (auto typedTool = dynamic_cast<_type*>(tool))
				return typedTool;
		}

		return nullptr;
	}

}

CLASS_BASES_META(Editor::SceneEditScreen)
{
	BASE_CLASS(o2::DragDropArea);
	BASE_CLASS(o2::KeyboardEventsListener);
	BASE_CLASS(o2::Singleton<SceneEditScreen>);
	BASE_CLASS(Editor::ScrollView);
}
END_META;
CLASS_FIELDS_META(Editor::SceneEditScreen)
{
	FIELD().PUBLIC().NAME(onSelectionChanged);
	FIELD().PROTECTED().DEFAULT_VALUE(Color4(220, 220, 220, 255)).NAME(mSelectedObjectColor);
	FIELD().PROTECTED().DEFAULT_VALUE(Color4(220, 220, 220, 100)).NAME(mMultiSelectedObjectColor);
	FIELD().PROTECTED().DEFAULT_VALUE(10.0f).NAME(mObjectMinimalSelectionSize);
	FIELD().PROTECTED().NAME(mSceneTree);
	FIELD().PROTECTED().NAME(mSelectedObjects);
	FIELD().PROTECTED().NAME(mTopSelectedObjects);
	FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mSelectedFromThis);
	FIELD().PROTECTED().NAME(mTools);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mEnabledTool);
	FIELD().PROTECTED().NAME(mDragHandles);
	FIELD().PROTECTED().NAME(mEditorLayers);
	FIELD().PROTECTED().NAME(mEditorLayersEnabled);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mLeftTopWidgetsContainer);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mRightTopWidgetsContainer);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mLeftBottomWidgetsContainer);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mRightBottomWidgetsContainer);
}
END_META;
CLASS_METHODS_META(Editor::SceneEditScreen)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const SceneEditScreen&);
	FUNCTION().PUBLIC().SIGNATURE(void, Draw);
	FUNCTION().PUBLIC().SIGNATURE(void, NeedRedraw);
	FUNCTION().PUBLIC().SIGNATURE(void, Update, float);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsScrollable);
	FUNCTION().PUBLIC().SIGNATURE(Vec2F, ScreenToScenePoint, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE(Vec2F, SceneToScreenPoint, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE(Vec2F, ScreenToSceneVector, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE(Vec2F, SceneToScreenVector, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE(void, DrawObjectSelection, SceneEditableObject*, const Color4&);
	FUNCTION().PUBLIC().SIGNATURE(void, SelectObjects, Vector<SceneEditableObject*>, bool);
	FUNCTION().PUBLIC().SIGNATURE(void, SelectObject, SceneEditableObject*, bool);
	FUNCTION().PUBLIC().SIGNATURE(void, SelectAllObjects);
	FUNCTION().PUBLIC().SIGNATURE(void, ClearSelection);
	FUNCTION().PUBLIC().SIGNATURE(HorizontalLayout*, GetLeftTopWidgetsContainer);
	FUNCTION().PUBLIC().SIGNATURE(HorizontalLayout*, GetRightTopWidgetsContainer);
	FUNCTION().PUBLIC().SIGNATURE(HorizontalLayout*, GetLeftBottomWidgetsContainer);
	FUNCTION().PUBLIC().SIGNATURE(HorizontalLayout*, GetRightBottomWidgetsContainer);
	FUNCTION().PUBLIC().SIGNATURE(void, AddEditorLayer, SceneEditorLayer*);
	FUNCTION().PUBLIC().SIGNATURE(void, RemoveEditorLayer, SceneEditorLayer*);
	FUNCTION().PUBLIC().SIGNATURE(void, SetLayerEnabled, const String&, bool);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsLayerEnabled, const String&);
	FUNCTION().PUBLIC().SIGNATURE(void, SelectTool, const IEditTool*);
	FUNCTION().PUBLIC().SIGNATURE(IEditTool*, GetSelectedTool);
	FUNCTION().PUBLIC().SIGNATURE(void, AddTool, IEditTool*);
	FUNCTION().PUBLIC().SIGNATURE(void, RemoveTool, IEditTool*);
	FUNCTION().PUBLIC().SIGNATURE(const Vector<IEditTool*>&, GetTools);
	FUNCTION().PUBLIC().SIGNATURE(const Vector<SceneEditableObject*>&, GetSelectedObjects);
	FUNCTION().PUBLIC().SIGNATURE(const Vector<SceneEditableObject*>&, GetTopSelectedObjects);
	FUNCTION().PUBLIC().SIGNATURE(const Color4&, GetSingleObjectSelectionColor);
	FUNCTION().PUBLIC().SIGNATURE(const Color4&, GetManyObjectsSelectionColor);
	FUNCTION().PUBLIC().SIGNATURE(void, OnSceneChanged);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsUnderPoint, const Vec2F&);
	FUNCTION().PROTECTED().SIGNATURE(void, InitializeTools);
	FUNCTION().PROTECTED().SIGNATURE(HorizontalLayout*, InitializeWidgetsContainer, BaseCorner);
	FUNCTION().PROTECTED().SIGNATURE(bool, IsHandleWorking, const Input::Cursor&);
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
	FUNCTION().PROTECTED().SIGNATURE(void, OnObjectsSelectedFromThis);
	FUNCTION().PROTECTED().SIGNATURE(void, RedrawContent);
	FUNCTION().PROTECTED().SIGNATURE(void, DrawObjects);
	FUNCTION().PROTECTED().SIGNATURE(void, DrawSelection);
	FUNCTION().PROTECTED().SIGNATURE(void, BindSceneTree);
	FUNCTION().PROTECTED().SIGNATURE(void, OnTreeSelectionChanged, Vector<SceneEditableObject*>);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateTopSelectedObjects);
	FUNCTION().PROTECTED().SIGNATURE(void, OnSceneChanged, Vector<SceneEditableObject*>);
	FUNCTION().PROTECTED().SIGNATURE(void, ClearSelectionWithoutAction, bool);
	FUNCTION().PROTECTED().SIGNATURE(void, SelectObjectsWithoutAction, Vector<SceneEditableObject*>, bool);
	FUNCTION().PROTECTED().SIGNATURE(void, SelectObjectWithoutAction, SceneEditableObject*, bool);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDropped, ISelectableDragableObjectsGroup*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDragEnter, ISelectableDragableObjectsGroup*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDraggedAbove, ISelectableDragableObjectsGroup*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDragExit, ISelectableDragableObjectsGroup*);
	FUNCTION().PROTECTED().SIGNATURE(bool, IsInputTransparent);
}
END_META;
