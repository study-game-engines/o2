#include "ActorsTransform.h"

#include "Scene/Actor.h"
#include "SceneWindow/SceneEditScreen.h"

namespace Editor
{
	EditorActorsTransformAction::EditorActorsTransformAction()
	{}

	EditorActorsTransformAction::EditorActorsTransformAction(const Vector<Actor*>& actors,
															 const Vector<ActorTransform>& beforeTransforms):
		beforeTransforms(beforeTransforms)
	{
		actorsIds = actors.Select<UInt64>([](Actor* x) { return x->GetID(); });
		doneTransforms = actors.Select<ActorTransform>([](Actor* x) { return x->transform; });
	}

	String EditorActorsTransformAction::GetName() const
	{
		return "Actors transformation";
	}

	void EditorActorsTransformAction::Redo()
	{
		for (int i = 0; i < actorsIds.Count(); i++)
		{
			Actor* actor = o2Scene.GetActorByID(actorsIds[i]);
			if (actor)
				actor->transform = doneTransforms[i];
		}
	}

	void EditorActorsTransformAction::Undo()
	{
		for (int i = 0; i < actorsIds.Count(); i++)
		{
			Actor* actor = o2Scene.GetActorByID(actorsIds[i]);
			if (actor)
				actor->transform = beforeTransforms[i];
		}
	}

}

CLASS_META(Editor::EditorActorsTransformAction)
{
	BASE_CLASS(Editor::IAction);

	PUBLIC_FIELD(actorsIds);
	PUBLIC_FIELD(beforeTransforms);
	PUBLIC_FIELD(doneTransforms);

	PUBLIC_FUNCTION(String, GetName);
	PUBLIC_FUNCTION(void, Redo);
	PUBLIC_FUNCTION(void, Undo);
}
END_META;
