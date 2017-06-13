#include "EnableAction.h"

#include "Scene/Actor.h"
#include "SceneWindow/SceneEditScreen.h"

namespace Editor
{
	EnableAction::EnableAction()
	{}

	EnableAction::EnableAction(const Vector<Actor*>& actors, bool enable):
		enable(enable)
	{
		actorsIds = actors.Select<UInt64>([](Actor* x) { return x->GetID(); });
	}

	String EnableAction::GetName() const
	{
		return enable ? "Enable actors" : "Disable actors";
	}

	void EnableAction::Redo()
	{
		for (auto actorId : actorsIds)
		{
			auto actor = o2Scene.GetActorByID(actorId);
			if (actor)
				actor->SetEnabled(enable);
		}
	}

	void EnableAction::Undo()
	{
		for (auto actorId : actorsIds)
		{
			auto actor = o2Scene.GetActorByID(actorId);
			if (actor)
				actor->SetEnabled(!enable);
		}
	}

}

CLASS_META(Editor::EnableAction)
{
	BASE_CLASS(Editor::IAction);

	PUBLIC_FIELD(actorsIds);
	PUBLIC_FIELD(enable);

	PUBLIC_FUNCTION(String, GetName);
	PUBLIC_FUNCTION(void, Redo);
	PUBLIC_FUNCTION(void, Undo);
}
END_META;
