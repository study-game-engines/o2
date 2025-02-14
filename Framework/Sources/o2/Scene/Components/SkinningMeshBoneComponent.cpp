#include "o2/stdafx.h"
#include "SkinningMeshBoneComponent.h"

#include "o2/Scene/Actor.h"
#include "o2/Scene/Components/SkinningMeshComponent.h"

namespace o2
{

	SkinningMeshBoneComponent::SkinningMeshBoneComponent()
	{}

	SkinningMeshBoneComponent::SkinningMeshBoneComponent(const SkinningMeshBoneComponent& other):
		DrawableComponent(other)
	{}

	SkinningMeshBoneComponent::~SkinningMeshBoneComponent()
	{}

	SkinningMeshBoneComponent& SkinningMeshBoneComponent::operator=(const SkinningMeshBoneComponent& other)
	{
		DrawableComponent::operator=(other);
		return *this;
	}

	void SkinningMeshBoneComponent::Draw()
	{
// 		o2Render.DrawArrow(mOwner->transform->GetWorldPivot(),
// 						   mOwner->transform->Local2WorldPoint(Vec2F(length, 0) + mOwner->transform->GetSizePivot()));

		DrawableComponent::OnDrawn();
		ISceneDrawable::Draw();
	}

	void SkinningMeshBoneComponent::Update(float dt)
	{
	}

	String SkinningMeshBoneComponent::GetName()
	{
		return "Skinning mesh bone";
	}

	String SkinningMeshBoneComponent::GetCategory()
	{
		return "Animation";
	}

	String SkinningMeshBoneComponent::GetIcon()
	{
		return "ui/UI4_emitter_component.png";
	}

	void SkinningMeshBoneComponent::OnTransformUpdated()
	{
	}

	void SkinningMeshBoneComponent::OnAddToScene()
	{
		if (auto skinningMesh = FindSkinningMesh())
			skinningMesh->NeedUpdateBones(true);
	}

	void SkinningMeshBoneComponent::OnRemoveFromScene()
	{
		if (auto skinningMesh = FindSkinningMesh())
			skinningMesh->NeedUpdateBones(true);
	}

	SkinningMeshComponent* SkinningMeshBoneComponent::FindSkinningMesh() const
	{
		auto itActor = mOwner;
		while (itActor)
		{
			if (auto mesh = itActor->GetComponent<SkinningMeshComponent>())
				return mesh;

			itActor = itActor->GetParent();
		}

		return nullptr;
	}
}

DECLARE_TEMPLATE_CLASS(o2::Ref<o2::SkinningMeshBoneComponent>);

DECLARE_CLASS(o2::SkinningMeshBoneComponent);
