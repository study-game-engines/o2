#pragma once

#include "o2/Assets/Types/ImageAsset.h"
#include "o2/Render/Camera.h"
#include "o2/Render/FontRef.h"
#include "o2/Render/Mesh.h"
#include "o2/Utils/Editor/DragHandle.h"
#include "o2/Utils/Editor/FrameHandles.h"
#include "o2/Utils/Math/Curve.h"
#include "o2Editor/Core/Actions/ActionsList.h"
#include "o2Editor/Core/Actions/IAction.h"
#include "o2Editor/Core/UI/FrameScrollView.h"

using namespace o2;

namespace o2
{
	class ContextMenu;
	class EditBox;
	class Window;
}

namespace Editor
{
	// ---------------------
	// Curves editing widget
	// ---------------------
	class CurvesEditor: public FrameScrollView, public SelectableDragHandlesGroup
	{
	public:
		ActionsList* actionsListDelegate = nullptr; // Actions fall down list. When it is null, editor uses local actions list

	public:
		// Default constructor
		CurvesEditor();

		// Copy-constructor
		CurvesEditor(const CurvesEditor& other);

		// Destructor
		~CurvesEditor();

		// Copy-operator
		CurvesEditor& operator=(const CurvesEditor& other);

		// Draws widget, updates render target 
		void Draw() override;

		// Updates drawables, states and widget
		void Update(float dt) override;

		// Returns list of curves
		Map<String, Curve*> GetCurves() const;

		// Adds editing curve with color. If color is default it will be randomized
		void AddCurve(const String& id, Curve* curve, const Color4& color = Color4(44, 62, 80));

		// Removed curve from editing
		void RemoveCurve(Curve* curve);

		// Removed curve from editing
		void RemoveCurve(const String& id);

		// Removes all editing curves
		void RemoveAllCurves();

		// Adds curves range with color. It can't be edited, just a solid color between curves
		void AddCurvesRange(Curve* curveA, Curve* curveB, const Color4& color = Color4::Green());

		// Removes curve range
		void RemoveCurvesRange(Curve* curveA, Curve* curveB);

		// Adds curves range with color. It can't be edited, just a solid color between curves
		void AddCurvesRange(const String& idA, const String& idB, const Color4& color = Color4::Green());

		// Sets curve color
		void SetCurveColor(Curve* curve, const Color4& color);

		// Removes curve range
		void RemoveCurvesRange(const String& idA, const String& idB);

		// Sets selection rectange sprite image
		void SetSelectionSpriteImage(const ImageAssetRef& image);

		// Sets captions text font
		void SetTextFont(const FontRef& font);

		// Sets captions offsets 
		void SetTextBorder(const BorderF& border);

		// Sets main key handle images
		void SetMainHandleImages(const ImageAssetRef& regular, const ImageAssetRef& hover, const ImageAssetRef& pressed,
								 const ImageAssetRef& selected);

		// Sets support key handle images
		void SetSupportHandleImages(const ImageAssetRef& regular, const ImageAssetRef& hover, const ImageAssetRef& pressed,
									const ImageAssetRef& selected);

		// Enables curves scale adjusting. When it is true, all curves adopts their size to be in the same view range
		void SetAdjustCurvesScale(bool enable);

		// Updates layout
		void UpdateSelfTransform() override;

		// Returns context menu
		ContextMenu* GetContextMenu() const;

		SERIALIZABLE(CurvesEditor);

	protected:
		struct CurveInfo;

		struct CurveHandle : public DragHandle
		{
			CurveInfo* curveInfo = nullptr;

		public:
			// Default constructor
			CurveHandle();

			// Constructor with views
			CurveHandle(Sprite* regular, Sprite* hover = nullptr, Sprite* pressed = nullptr,
					    Sprite* selected = nullptr, Sprite* selectedHovered = nullptr, Sprite* selectedPressed = nullptr);

			// Copy-constructor
			CurveHandle(const DragHandle& other);

			// Destructor
			~CurveHandle() override;

			// Copy-operator
			CurveHandle& operator=(const CurveHandle& other);

			// Returns handle position with curve view transformations
			Vec2F GetLocalPosition() const;

			// Converts local to view position (transformed by curve view)
			Vec2F LocalToCurveView(const Vec2F& point) const;

			// Converts view position (transformed by curve view) to local
			Vec2F CurveViewToLocal(const Vec2F& point) const;

			// Converts point from screen to local space
			Vec2F ScreenToLocal(const Vec2F& point) override;

			// Converts point from local to screen space
			Vec2F LocalToScreen(const Vec2F& point) override;

			SERIALIZABLE(CurveHandle);
		};

		struct KeyHandles
		{
			CurvesEditor* curveEditor = nullptr;
			CurveHandle   mainHandle;
			CurveHandle   leftSupportHandle;
			CurveHandle   rightSupportHandle;
				            
			int    curveKeyIdx;
			UInt64 curveKeyUid;

		public:
			KeyHandles() {}
			KeyHandles(const CurveHandle& mainSample, const CurveHandle& supportSample, CurvesEditor* editor, const Color4& color);

			void Draw(const RectF& camRect);
			bool IsSomeHandleSelected() const;
		};

		struct CurveInfo
		{
			CurvesEditor* editor = nullptr;

			String curveId;
			Curve* curve = nullptr;

			Vector<KeyHandles*> handles;

			Vector<Vec2F> approximatedPoints;

			Color4 color;
			Vec2F viewScale;
			Vec2F viewOffset;

			bool disableChangesHandling = false;

		public:
			CurveInfo();
			~CurveInfo();

			void UpdateHandles();
			void UpdateApproximatedPoints();
			void AdjustScale();
			void OnCurveChanged();

			void BeginCurveManualChange();
			void CompleteCurveManualChange();
		};

		class CurveCopyInfo: public ISerializable
		{
		public:
			String             curveId; // @SERIALIZABLE
			Vector<Curve::Key> keys;    // @SERIALIZABLE

			SERIALIZABLE(CurveCopyInfo);
		};

		struct RangeInfo
		{
			CurveInfo* curveA = nullptr;
			CurveInfo* curveB = nullptr;
			Color4     color;
			Mesh*      mesh;

			RangeInfo();
			~RangeInfo();

			void UpdateMesh();
		};

		struct SelectedHandlesInfo
		{
			int  index;
			bool mainHandle;
			bool leftSupportHandle;
			bool rightSupportHandle;

			bool operator==(const SelectedHandlesInfo& other) const;
		};

		struct CurveKeysInfo
		{
			String                      curveId;
			Vector<Curve::Key>          keys;
			Vector<SelectedHandlesInfo> selectedHandles;

			bool operator==(const CurveKeysInfo& other) const;
		};

	protected:
		ContextMenu* mContextMenu = nullptr; // Context menu for editing keys properties, copying, pasting and other
							    								    
		CurveHandle mMainHandleSample;      // Main handle sample, uses to copy sprites @SERIALIZABLE
		CurveHandle mSupportHandleSample;   // Support handle sample, uses to copy sprites @SERIALIZABLE
		CurveInfo   mHandleSamplesStubInfo; // Empty curve info, used int handles samples
							    								    
		Vector<CurveInfo*> mCurves; // Editing curves infos list 
		Vector<RangeInfo*> mRanges; // Curves ranges list
							    								    
		Vector<CurveHandle*>       mSupportHandles;      // Support points handles list
		SelectableDragHandlesGroup mSupportHandlesGroup; // Support points handles selection group. They are must be selectable separately from main handles
		
		Vector<CurveHandle*> mSelectingHandlesBuf; // Potentially selecting handles while selecting
							    								    
		Sprite* mSelectionSprite = nullptr; // Selection sprite @SERIALIZABLE

		bool mAdjustCurvesScale = true; // When it is true, all curves adopts their size to be in the same view range

		FontRef mTextFont;             // Captions text font @SERIALIZABLE
		Text*   mTextLeft = nullptr;   // Captions text drawable at left border
		Text*   mTextRight = nullptr;  // Captions text drawable at right border
		Text*   mTextTop = nullptr;    // Captions text drawable at top border
		Text*   mTextBottom = nullptr; // Captions text drawable at bottom border
		BorderF mTextBorder;           // Captions offsets from border
							    								    
		Vec2F mSelectingPressedPoint; // Point, where cursor was pressed, selection starts here, in local space
							    								    
		FrameHandles mTransformFrame;                // Keys transformation frame
		bool         mTransformFrameVisible = false; // Is transform frame visible. it visible when 2 or more main handles was selected
		Basis        mTransformFrameBasis;           // Basis of transform frame in screen space
								 							   	    
		bool mIsViewScrolling = false; // Is scrolling view at this time

		bool mNeedAdjustView = false; // True when need to adjust view scale. This works in update
							    
		Vector<CurveKeysInfo> mBeforeTransformKeys; // Stored selected keys before handles transformed

		ActionsList mActionsList; // Local actions list. It uses when actionFallDown is null 

	protected:
		// Called when visible was changed. Sets context menu items priority
		void OnEnableInHierarchyChanged() override;

		// Called when scrolling
		void OnScrolled(float scroll) override;

		// Searches curve by id
		Curve* FindCurve(const String& id);

		// Initializes context menu items
		void InitializeContextMenu();

		// Initializes text drawables by font and sets aligning
		void InitializeTextDrawables();

		// Recalculates view area by curves approximated points
		void RecalculateViewArea();

		// Redraws content into render target
		void RedrawContent() override;

		// Draws grid and captions
		void DrawGrid() override;

		// Draws curves
		void DrawCurves();

		// Draws handles
		void DrawHandles();

		// Draw selection sprite
		void DrawSelection();

		// Draws transformation frame
		void DrawTransformFrame();

		// Adds curve key main and support handles and initializes them
		void AddCurveKeyHandles(CurveInfo* info, int keyId);

		// Removes curve key handles
		void RemoveCurveKeyHandles(CurveInfo* info, int keyId);

		// Called when one of main curve key handles was moved. Updates graphics and handles
		void OnCurveKeyMainHandleDragged(CurveInfo* info, KeyHandles* handles, const Vec2F& position);

		// Called when one of left support curve key handles was moved. Updates graphics and handles
		void OnCurveKeyLeftSupportHandleDragged(CurveInfo* info, KeyHandles* handles, const Vec2F& position);

		// Called when one of right support curve key handles was moved. Updates graphics and handles
		void OnCurveKeyRightSupportHandleDragged(CurveInfo* info, KeyHandles* handles, const Vec2F& position);

		// Checks left support handle constrains and returns filtered position
		Vec2F CheckLeftSupportHandlePosition(CurveInfo* info, KeyHandles* handles, const Vec2F& position);

		// Checks right support handle constrains and returns filtered position
		Vec2F CheckRightSupportHandlePosition(CurveInfo* info, KeyHandles* handles, const Vec2F& position);

		// Smooths key support points and updates handles
		void SmoothKey(CurveInfo* info, int idx);

		// Called when cursor double clicked, adds new point in curve
		void OnCursorDblClicked(const Input::Cursor& cursor) override;

		// Called when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor) override;

		// Called when cursor released (only when cursor pressed this at previous time)
		void OnCursorReleased(const Input::Cursor& cursor) override;

		// Called when cursor stay down during frame
		void OnCursorStillDown(const Input::Cursor& cursor) override;

		// Called when right mouse button stay down on this, overriding from scroll view to call context menu
		void OnCursorRightMouseStayDown(const Input::Cursor& cursor) override;

		// Called when right mouse button was released (only when right mouse button pressed this at previous time), overriding from scroll view to call context menu
		void OnCursorRightMouseReleased(const Input::Cursor& cursor) override;

		// Called when selectable draggable handle was released
		void OnHandleCursorReleased(DragHandle* handle, const Input::Cursor& cursor) override;

		// Called when selectable handle was began to drag
		void OnHandleBeganDragging(DragHandle* handle) override;

		// Called when selectable handle moved, moves all selected handles position
		void OnHandleMoved(DragHandle* handle, const Vec2F& cursorPos) override;

		// Called when selectable handle completed changing
		void OnHandleCompletedChange(DragHandle* handle) override;

		// Called when selection is changed - some handle was added or removed from selection
		void OnSelectionChanged() override;

		// Checks supports handles visibility
		void CheckHandlesVisible();

		// Updates transformation frame by selected handles
		void UpdateTransformFrame();

		// Returns is transform frame visible. it will be visible when 2 or more main handles was selected
		bool IsTransformFrameVisible() const;

		// Sets all selected keys supports type
		void SetSelectedKeysSupportsType(Curve::Key::Type type);

		// Called when transform frame was transformed
		void OnTransformFrameTransformed(const Basis& basis);

		// Called when beginning transforming some handles. Stores selected keys before changing
		void OnTransformBegin();

		// Called when transform completed. Checks that selected keys changed, creates action for undo/redo
		void OnTransformCompleted();

		// Transforms point from local to curve view coords
		Vec2F LocalToCurveView(const Vec2F& point, const Vec2F& viewScale, const Vec2F& viewOffset) const;

		// Transforms point from curve view to local coords
		Vec2F CurveViewToLocal(const Vec2F& point, const Vec2F& viewScale, const Vec2F& viewOffset) const;

		// Stores undo action, clears redo actions
		void DoneAction(IAction* action);

	
		// On context menu edit pressed. Shows key edit window
		void OnEditPressed();

		// On context menu auto smooth pressed. Enables auto smoothing for selected keys
		void OnAutoSmoothChecked(bool checked);

		// On context menu flat pressed. Enables flat support handles for selected keys
		void OnFlatChecked(bool checked);

		// On context menu free pressed. Free handles transform for selected keys
		void OnFreeChecked(bool checked);

		// On context menu linear pressed. Free handles transform for selected keys
		void OnLinearChecked(bool checked);

		// On context menu broken pressed. Breaks connection between left and right supports
		void OnBrokenChecked(bool checked);

		// On context menu discrete pressed. Sets discrete keys transition for selected
		void OnDiscreteChecked(bool checked);

		// On context menu copy pressed. Stores keys into buffer
		void OnCopyPressed();

		// On context menu cut pressed. Stores keys into buffer and removes them
		void OnCutPressed();

		// On context menu paste pressed. Inserts keys from buffer at cursor position
		void OnPastePressed();

		// On context menu delete pressed. Deletes selected keys
		void OnDeletePressed();

		// On context menu insert pressed. Inserts keys from buffer and moves keys to right
		void OnInsertPressed();

		// On context menu undo pressed. Reverts previous action and puts into actions stack
		void OnUndoPressed();

		// On context menu redo pressed. Restores action from stack
		void OnRedoPressed();

		friend class CurveAddKeysAction;
		friend class CurveDeleteKeysAction;
		friend class CurveKeysChangeAction;
	};
}

CLASS_BASES_META(Editor::CurvesEditor)
{
	BASE_CLASS(Editor::FrameScrollView);
	BASE_CLASS(o2::SelectableDragHandlesGroup);
}
END_META;
CLASS_FIELDS_META(Editor::CurvesEditor)
{
	FIELD().PUBLIC().DEFAULT_VALUE(nullptr).NAME(actionsListDelegate);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mContextMenu);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().NAME(mMainHandleSample);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().NAME(mSupportHandleSample);
	FIELD().PROTECTED().NAME(mHandleSamplesStubInfo);
	FIELD().PROTECTED().NAME(mCurves);
	FIELD().PROTECTED().NAME(mRanges);
	FIELD().PROTECTED().NAME(mSupportHandles);
	FIELD().PROTECTED().NAME(mSupportHandlesGroup);
	FIELD().PROTECTED().NAME(mSelectingHandlesBuf);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(nullptr).NAME(mSelectionSprite);
	FIELD().PROTECTED().DEFAULT_VALUE(true).NAME(mAdjustCurvesScale);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().NAME(mTextFont);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mTextLeft);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mTextRight);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mTextTop);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mTextBottom);
	FIELD().PROTECTED().NAME(mTextBorder);
	FIELD().PROTECTED().NAME(mSelectingPressedPoint);
	FIELD().PROTECTED().NAME(mTransformFrame);
	FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mTransformFrameVisible);
	FIELD().PROTECTED().NAME(mTransformFrameBasis);
	FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mIsViewScrolling);
	FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mNeedAdjustView);
	FIELD().PROTECTED().NAME(mBeforeTransformKeys);
	FIELD().PROTECTED().NAME(mActionsList);
}
END_META;
CLASS_METHODS_META(Editor::CurvesEditor)
{

	typedef Map<String, Curve*> _tmp1;

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const CurvesEditor&);
	FUNCTION().PUBLIC().SIGNATURE(void, Draw);
	FUNCTION().PUBLIC().SIGNATURE(void, Update, float);
	FUNCTION().PUBLIC().SIGNATURE(_tmp1, GetCurves);
	FUNCTION().PUBLIC().SIGNATURE(void, AddCurve, const String&, Curve*, const Color4&);
	FUNCTION().PUBLIC().SIGNATURE(void, RemoveCurve, Curve*);
	FUNCTION().PUBLIC().SIGNATURE(void, RemoveCurve, const String&);
	FUNCTION().PUBLIC().SIGNATURE(void, RemoveAllCurves);
	FUNCTION().PUBLIC().SIGNATURE(void, AddCurvesRange, Curve*, Curve*, const Color4&);
	FUNCTION().PUBLIC().SIGNATURE(void, RemoveCurvesRange, Curve*, Curve*);
	FUNCTION().PUBLIC().SIGNATURE(void, AddCurvesRange, const String&, const String&, const Color4&);
	FUNCTION().PUBLIC().SIGNATURE(void, SetCurveColor, Curve*, const Color4&);
	FUNCTION().PUBLIC().SIGNATURE(void, RemoveCurvesRange, const String&, const String&);
	FUNCTION().PUBLIC().SIGNATURE(void, SetSelectionSpriteImage, const ImageAssetRef&);
	FUNCTION().PUBLIC().SIGNATURE(void, SetTextFont, const FontRef&);
	FUNCTION().PUBLIC().SIGNATURE(void, SetTextBorder, const BorderF&);
	FUNCTION().PUBLIC().SIGNATURE(void, SetMainHandleImages, const ImageAssetRef&, const ImageAssetRef&, const ImageAssetRef&, const ImageAssetRef&);
	FUNCTION().PUBLIC().SIGNATURE(void, SetSupportHandleImages, const ImageAssetRef&, const ImageAssetRef&, const ImageAssetRef&, const ImageAssetRef&);
	FUNCTION().PUBLIC().SIGNATURE(void, SetAdjustCurvesScale, bool);
	FUNCTION().PUBLIC().SIGNATURE(void, UpdateSelfTransform);
	FUNCTION().PUBLIC().SIGNATURE(ContextMenu*, GetContextMenu);
	FUNCTION().PROTECTED().SIGNATURE(void, OnEnableInHierarchyChanged);
	FUNCTION().PROTECTED().SIGNATURE(void, OnScrolled, float);
	FUNCTION().PROTECTED().SIGNATURE(Curve*, FindCurve, const String&);
	FUNCTION().PROTECTED().SIGNATURE(void, InitializeContextMenu);
	FUNCTION().PROTECTED().SIGNATURE(void, InitializeTextDrawables);
	FUNCTION().PROTECTED().SIGNATURE(void, RecalculateViewArea);
	FUNCTION().PROTECTED().SIGNATURE(void, RedrawContent);
	FUNCTION().PROTECTED().SIGNATURE(void, DrawGrid);
	FUNCTION().PROTECTED().SIGNATURE(void, DrawCurves);
	FUNCTION().PROTECTED().SIGNATURE(void, DrawHandles);
	FUNCTION().PROTECTED().SIGNATURE(void, DrawSelection);
	FUNCTION().PROTECTED().SIGNATURE(void, DrawTransformFrame);
	FUNCTION().PROTECTED().SIGNATURE(void, AddCurveKeyHandles, CurveInfo*, int);
	FUNCTION().PROTECTED().SIGNATURE(void, RemoveCurveKeyHandles, CurveInfo*, int);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCurveKeyMainHandleDragged, CurveInfo*, KeyHandles*, const Vec2F&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCurveKeyLeftSupportHandleDragged, CurveInfo*, KeyHandles*, const Vec2F&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCurveKeyRightSupportHandleDragged, CurveInfo*, KeyHandles*, const Vec2F&);
	FUNCTION().PROTECTED().SIGNATURE(Vec2F, CheckLeftSupportHandlePosition, CurveInfo*, KeyHandles*, const Vec2F&);
	FUNCTION().PROTECTED().SIGNATURE(Vec2F, CheckRightSupportHandlePosition, CurveInfo*, KeyHandles*, const Vec2F&);
	FUNCTION().PROTECTED().SIGNATURE(void, SmoothKey, CurveInfo*, int);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorDblClicked, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorPressed, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorReleased, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorStillDown, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorRightMouseStayDown, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorRightMouseReleased, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnHandleCursorReleased, DragHandle*, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnHandleBeganDragging, DragHandle*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnHandleMoved, DragHandle*, const Vec2F&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnHandleCompletedChange, DragHandle*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnSelectionChanged);
	FUNCTION().PROTECTED().SIGNATURE(void, CheckHandlesVisible);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateTransformFrame);
	FUNCTION().PROTECTED().SIGNATURE(bool, IsTransformFrameVisible);
	FUNCTION().PROTECTED().SIGNATURE(void, SetSelectedKeysSupportsType, Curve::Key::Type);
	FUNCTION().PROTECTED().SIGNATURE(void, OnTransformFrameTransformed, const Basis&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnTransformBegin);
	FUNCTION().PROTECTED().SIGNATURE(void, OnTransformCompleted);
	FUNCTION().PROTECTED().SIGNATURE(Vec2F, LocalToCurveView, const Vec2F&, const Vec2F&, const Vec2F&);
	FUNCTION().PROTECTED().SIGNATURE(Vec2F, CurveViewToLocal, const Vec2F&, const Vec2F&, const Vec2F&);
	FUNCTION().PROTECTED().SIGNATURE(void, DoneAction, IAction*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnEditPressed);
	FUNCTION().PROTECTED().SIGNATURE(void, OnAutoSmoothChecked, bool);
	FUNCTION().PROTECTED().SIGNATURE(void, OnFlatChecked, bool);
	FUNCTION().PROTECTED().SIGNATURE(void, OnFreeChecked, bool);
	FUNCTION().PROTECTED().SIGNATURE(void, OnLinearChecked, bool);
	FUNCTION().PROTECTED().SIGNATURE(void, OnBrokenChecked, bool);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDiscreteChecked, bool);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCopyPressed);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCutPressed);
	FUNCTION().PROTECTED().SIGNATURE(void, OnPastePressed);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDeletePressed);
	FUNCTION().PROTECTED().SIGNATURE(void, OnInsertPressed);
	FUNCTION().PROTECTED().SIGNATURE(void, OnUndoPressed);
	FUNCTION().PROTECTED().SIGNATURE(void, OnRedoPressed);
}
END_META;

CLASS_BASES_META(Editor::CurvesEditor::CurveHandle)
{
	BASE_CLASS(o2::DragHandle);
}
END_META;
CLASS_FIELDS_META(Editor::CurvesEditor::CurveHandle)
{
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(curveInfo);
}
END_META;
CLASS_METHODS_META(Editor::CurvesEditor::CurveHandle)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(Sprite*, Sprite*, Sprite*, Sprite*, Sprite*, Sprite*);
	FUNCTION().PUBLIC().CONSTRUCTOR(const DragHandle&);
	FUNCTION().PUBLIC().SIGNATURE(Vec2F, GetLocalPosition);
	FUNCTION().PUBLIC().SIGNATURE(Vec2F, LocalToCurveView, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE(Vec2F, CurveViewToLocal, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE(Vec2F, ScreenToLocal, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE(Vec2F, LocalToScreen, const Vec2F&);
}
END_META;

CLASS_BASES_META(Editor::CurvesEditor::CurveCopyInfo)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(Editor::CurvesEditor::CurveCopyInfo)
{
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(curveId);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(keys);
}
END_META;
CLASS_METHODS_META(Editor::CurvesEditor::CurveCopyInfo)
{
}
END_META;
