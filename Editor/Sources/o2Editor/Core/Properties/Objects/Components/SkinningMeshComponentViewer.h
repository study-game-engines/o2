#pragma once

#include "o2/Scene/Components/SkinningMeshBoneComponent.h"
#include "o2/Scene/Components/SkinningMeshComponent.h"
#include "o2Editor/Core/Properties/IObjectPropertiesViewer.h"
#include "o2Editor/Core/Tools/CustomFrameTool.h"
#include "o2Editor/Core/Tools/MeshTopologyTool.h"
#include "o2Editor/Core/Tools/SplineTool.h"
#include "o2Editor/Core/UI/SplineEditor/SplineEditor.h"
#include "o2Editor/SceneWindow/SceneEditorLayer.h"

using namespace o2;

namespace o2
{
	class Spoiler;
	class Button;
}

namespace Editor
{
	class SceneDragHandle;
	struct SkeletonTool;

	// ------------------------------
	// Skinning Mesh component viewer
	// ------------------------------
	class SkinningMeshComponentViewer: public TObjectPropertiesViewer<SkinningMeshComponent>
	{
	public:
		// Default constructor
		SkinningMeshComponentViewer();

		// Destructor
		~SkinningMeshComponentViewer();

		// Copy operator
		SkinningMeshComponentViewer& operator=(const SkinningMeshComponentViewer& other);

		IOBJECT(SkinningMeshComponentViewer);

	protected:
		struct SceneLayer : public SceneEditorLayer
		{
			SkinningMeshComponentViewer* viewer = nullptr;
			Sprite               textureSprite;

		public:
			void DrawOverScene() override;
			void Update(float dt) override;

			int GetOrder() const override;

			bool IsEnabled() const override;

			const String& GetName() const override;
			const String& GetIconName() const override;

			void DrawMeshWire();
		};

	protected:

	protected:
		SplineTool       mSplineTool;             // Spline tool
		CustomFrameTool  mFrameTool;              // Mapping frame tool
		MeshTopologyTool mTopologyTool;           // Mesh topology tool
		SkeletonTool*    mSkeletonTool = nullptr; // Shared skeleton tool
		SceneLayer       mFrameTetxureLayer;      // Frame texture drawing layer

		Button* mFitAndCenterButton = nullptr; // Fit and centerize button
		Button* mEditSkeletonButton = nullptr; // Enable/disable skeleton editing button
		Button* mReskinButton = nullptr;       // Resets mesh skinning and updates bones transforms

		bool mEditingSkeleton = false; // It editing skeleton enabled

	protected:
		// Called when the viewer is refreshed, builds properties, and places them in mPropertiesContext
		void RebuildProperties(const Vector<Pair<IObject*, IObject*>>& targetObjets) override;

		// Called when viewer is refreshed
		void OnRefreshed(const Vector<Pair<IObject*, IObject*>>& targetObjets) override;

		// Enable viewer event function
		void OnEnabled() override;

		// Disable viewer event function
		void OnDisabled() override;

		// Called when button pressed
		void FitAndCenterize();

		// Called when edit skeleton pressed
		void OnEditSkeletonPressed();

		// Called when reskin pressed
		void OnReskinPressed();

		// Updates the caption on the skeleton edit button depending on whether it is enabled or not
		void UpdateEditSkeletonButtonCaption();
	};
}

CLASS_BASES_META(Editor::SkinningMeshComponentViewer)
{
	BASE_CLASS(Editor::TObjectPropertiesViewer<SkinningMeshComponent>);
}
END_META;
CLASS_FIELDS_META(Editor::SkinningMeshComponentViewer)
{
	FIELD().PROTECTED().NAME(mSplineTool);
	FIELD().PROTECTED().NAME(mFrameTool);
	FIELD().PROTECTED().NAME(mTopologyTool);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mSkeletonTool);
	FIELD().PROTECTED().NAME(mFrameTetxureLayer);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mFitAndCenterButton);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mEditSkeletonButton);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mReskinButton);
	FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mEditingSkeleton);
}
END_META;
CLASS_METHODS_META(Editor::SkinningMeshComponentViewer)
{

	typedef const Vector<Pair<IObject*, IObject*>>& _tmp1;
	typedef const Vector<Pair<IObject*, IObject*>>& _tmp2;

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PROTECTED().SIGNATURE(void, RebuildProperties, _tmp1);
	FUNCTION().PROTECTED().SIGNATURE(void, OnRefreshed, _tmp2);
	FUNCTION().PROTECTED().SIGNATURE(void, OnEnabled);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDisabled);
	FUNCTION().PROTECTED().SIGNATURE(void, FitAndCenterize);
	FUNCTION().PROTECTED().SIGNATURE(void, OnEditSkeletonPressed);
	FUNCTION().PROTECTED().SIGNATURE(void, OnReskinPressed);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateEditSkeletonButtonCaption);
}
END_META;
