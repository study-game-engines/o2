#pragma once
#include "IEditorTool.h"

#include "o2/Scene/Components/SkinningMeshBoneComponent.h"
#include "o2/Scene/Components/SkinningMeshComponent.h"
#include "o2/Utils/Editor/FrameHandles.h"
#include "o2Editor/SceneWindow/SceneDragHandle.h"
#include "o2Editor/SceneWindow/SceneEditorLayer.h"

namespace Editor
{
	// -------------------------------------------------
	// Skeleton tool. Used for editing bones of skeleton
	// -------------------------------------------------
	struct SkeletonTool: public IEditTool
	{
		// ---------------------------------------------------------------------
		// Scene layer. Used to draw and update stuff of bones handles and other
		// ---------------------------------------------------------------------
		struct SceneLayer: public SceneEditorLayer
		{
			SkeletonTool* tool = nullptr;

		public:
			void DrawOverScene() override;
			void Update(float dt) override;

			int GetOrder() const override;

			bool IsEnabled() const override;

			const String& GetName() const override;
			const String& GetIconName() const override;
		};

		// ---------------------------------------------------------
		// Bone handle container. Keeps handle and reference to bone
		// ---------------------------------------------------------
		struct BoneHandle
		{
			SceneDragHandle* handle = nullptr;            // Bone handle in scene
			Ref<SkinningMeshBoneComponent> boneComponent; // Bone component

			Basis pressedTransform; // Stored bone transform when pressed

		public:
			// Default constructor, creates handle
			BoneHandle();

			// Destructor, destroys handle
			~BoneHandle();

			// Updates handle position by bone transform
			void UpdatePosition();

		private:
			// Called when handle is moved, changes bone transformation
			void OnHandleChangedPos(const Vec2F& pos);
		};

		// ---------------------------------------------------------------------------------
		// Skeleton edit instance. Contains handles and reference to skinning mesh component
		// ---------------------------------------------------------------------------------
		struct SkeletonInstance
		{
			Ref<SkinningMeshComponent> skinningMeshComponent; // Reference to component
			Vector<BoneHandle*>        boneHandles;           // List of bones handles

		public:
			// Updates handles positions by bones transforms
			void UpdateHandlesPositions();
		};

	public:
		SceneLayer   sceneLayer;        // Scene layer for drawing spline
		bool         isEnabled = false; // Is tool enabled now     

	public:
		// Default constructor
		SkeletonTool();

		// Returns toggle in menu panel icon name
		String GetPanelIcon() const override;

		// Called when tool was enabled
		void OnEnabled() override;

		// Called when tool was disabled
		void OnDisabled() override;

		// Adds and enables handles for editing skeleton bones
		void AddSkeletonInstance(const Ref<SkinningMeshComponent>& skinningMeshComponent);

		// Removing skeleton from editing
		void RemoveSkeletonInstance(const Ref<SkinningMeshComponent>& skinningMeshComponent);

		// Returns whether the given skeleton is already being edited
		bool IsEditingSkeleton(const Ref<SkinningMeshComponent>& skinningMeshComponent) const;

		// Returns list of editing skeletons
		const Vector<SkeletonInstance*>& GetEditingSkeletons() const;

		IOBJECT(SkeletonTool);

	private:
		Vector<SkeletonInstance*> mSkeletons; // List of editing skeletons
	};
}

CLASS_BASES_META(Editor::SkeletonTool)
{
	BASE_CLASS(Editor::IEditTool);
}
END_META;
CLASS_FIELDS_META(Editor::SkeletonTool)
{
	FIELD().PUBLIC().NAME(sceneLayer);
	FIELD().PUBLIC().DEFAULT_VALUE(false).NAME(isEnabled);
	FIELD().PRIVATE().NAME(mSkeletons);
}
END_META;
CLASS_METHODS_META(Editor::SkeletonTool)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().SIGNATURE(String, GetPanelIcon);
	FUNCTION().PUBLIC().SIGNATURE(void, OnEnabled);
	FUNCTION().PUBLIC().SIGNATURE(void, OnDisabled);
	FUNCTION().PUBLIC().SIGNATURE(void, AddSkeletonInstance, const Ref<SkinningMeshComponent>&);
	FUNCTION().PUBLIC().SIGNATURE(void, RemoveSkeletonInstance, const Ref<SkinningMeshComponent>&);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsEditingSkeleton, const Ref<SkinningMeshComponent>&);
	FUNCTION().PUBLIC().SIGNATURE(const Vector<SkeletonInstance*>&, GetEditingSkeletons);
}
END_META;
