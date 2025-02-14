#pragma once
#include "IEditorTool.h"

#include "o2Editor/SceneWindow/SceneEditorLayer.h"
#include "o2/Utils/Editor/FrameHandles.h"

namespace Editor
{
	// --------------------------------------------------------
	// Custom frame tool. Used for special frame editing things
	// --------------------------------------------------------
	struct CustomFrameTool: public IEditTool
	{
		struct SceneLayer: public SceneEditorLayer
		{
			CustomFrameTool* tool = nullptr;

		public:
			void DrawOverScene() override;
			void Update(float dt) override;

			int GetOrder() const override;

			bool IsEnabled() const override;

			const String& GetName() const override;
			const String& GetIconName() const override;
		};

		FrameHandles frameHandles;      // Frame handles 
		SceneLayer   sceneLayer;        // Scene layer for drawing spline
		bool         isEnabled = false; // Is tool enabled now

		Function<void(const Basis&)> onChanged; // Called when frame changes
		Function<Vec2F()>            getOrigin; // Returns origin of frame         

	public:
		// Default constructor
		CustomFrameTool();

		// Empty copy operator
		CustomFrameTool& operator=(const CustomFrameTool& other) { return *this; }

		// Sets frame
		void SetFrame(const Basis& frame);

		// Returns current frame
		const Basis& GetFrame() const;

		// Resets callbacks
		void Reset();

		// Returns toggle in menu panel icon name
		String GetPanelIcon() const override;

		// Called when tool was enabled
		void OnEnabled() override;

		// Called when tool was disabled
		void OnDisabled() override;

		IOBJECT(CustomFrameTool);

	private:
		Basis mBasis; // Current editing basis
	};
}

CLASS_BASES_META(Editor::CustomFrameTool)
{
	BASE_CLASS(Editor::IEditTool);
}
END_META;
CLASS_FIELDS_META(Editor::CustomFrameTool)
{
	FIELD().PUBLIC().NAME(frameHandles);
	FIELD().PUBLIC().NAME(sceneLayer);
	FIELD().PUBLIC().DEFAULT_VALUE(false).NAME(isEnabled);
	FIELD().PUBLIC().NAME(onChanged);
	FIELD().PUBLIC().NAME(getOrigin);
	FIELD().PRIVATE().NAME(mBasis);
}
END_META;
CLASS_METHODS_META(Editor::CustomFrameTool)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().SIGNATURE(void, SetFrame, const Basis&);
	FUNCTION().PUBLIC().SIGNATURE(const Basis&, GetFrame);
	FUNCTION().PUBLIC().SIGNATURE(void, Reset);
	FUNCTION().PUBLIC().SIGNATURE(String, GetPanelIcon);
	FUNCTION().PUBLIC().SIGNATURE(void, OnEnabled);
	FUNCTION().PUBLIC().SIGNATURE(void, OnDisabled);
}
END_META;
