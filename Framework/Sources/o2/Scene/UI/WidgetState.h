#pragma once
#include "o2/Animation/AnimationClip.h"
#include "o2/Animation/AnimationPlayer.h"
#include "o2/Assets/Types/AnimationAsset.h"
#include "o2/Utils/Serialization/Serializable.h"
#include "o2/Utils/Types/String.h"
#include "o2/Utils/Editor/Attributes/InvokeOnChangeAttribute.h"

namespace o2
{
	class Widget;

	// -----------------------------------------------------------------------
	// Widget state. Could be true or false, and animates widget by this state
	// -----------------------------------------------------------------------
	class WidgetState: public ISerializable
	{
	public:
		String name; // State name @SERIALIZABLE @SCRIPTABLE

		AnimationPlayer player;                        // Animation player
		float           offStateAnimationSpeed = 1.0f; // False state transition animation speed @SERIALIZABLE @SCRIPTABLE

	public:
		PROPERTIES(WidgetState);
		PROPERTY(bool, state, SetState, GetState); // State flag property @SCRIPTABLE

		PROPERTY(AnimationAssetRef, animationAsset, SetAnimationAsset, GetAnimationAsset); // Animation asset property @EDITOR_IGNORE @SCRIPTABLE
		PROPERTY(AnimationClip, animationClip, SetAnimationClip, GetAnimationClip);        // animation clip property @EDITOR_IGNORE @SCRIPTABLE

	public:
		Function<void()> onStateFullyTrue;	  // This event calls when state is completely true (at the end of animation)
		Function<void()> onStateFullyFalse;   // This event calls when state is completely false (at the end of animation)
		Function<void()> onStateBecomesTrue;  // This event calls when state becomes to true
		Function<void()> onStateBecomesFalse; // This event calls when state becomes to true

	public:
		// Default constructor @SCRIPTABLE
		WidgetState();

		// Copy-constructor
		WidgetState(const WidgetState& state);

		// Destructor
		~WidgetState();

		// Boolean cast operator
		operator bool();

		// Assign from boolean operator
		WidgetState& operator=(bool state);

		// Equals operator
		bool operator==(const WidgetState& other) const;

		// Sets owner widget @SCRIPTABLE
		void SetOwner(Widget* owner, bool errors);

		// Sets animation asset
		void SetAnimationAsset(const AnimationAssetRef& asset);

		// Returns animation asset
		const AnimationAssetRef& GetAnimationAsset() const;

		// Sets animation asset instance clip
		void SetAnimationClip(const AnimationClip& animation);

		// Returns animation asset instance clip, if exists
		AnimationClip& GetAnimationClip();

		// Sets current state @SCRIPTABLE
		void SetState(bool state);

		// Sets state immediately @SCRIPTABLE
		void SetStateForcible(bool state);

		// Returns current state @SCRIPTABLE
		bool GetState() const;

		// Updates animation
		void Update(float dt);

		SERIALIZABLE(WidgetState);

	protected:
		bool    mState = false;   // Current state @SERIALIZABLE
		Widget* mOwner = nullptr; // Owner widget pointer

		AnimationAssetRef mAnimation; // Widget animation @SERIALIZABLE @EDITOR_PROPERTY @INVOKE_ON_CHANGE(OnAnimationChanged)

	protected:
		// Called when animation changed from editor
		void OnAnimationChanged();

		// Completion deserialization callback
		void OnDeserialized(const DataValue& node) override;

		// Completion deserialization delta callback
		void OnDeserializedDelta(const DataValue& node, const IObject& origin) override;

		friend class Widget;
	};
}

CLASS_BASES_META(o2::WidgetState)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::WidgetState)
{
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().SERIALIZABLE_ATTRIBUTE().NAME(name);
	FIELD().PUBLIC().NAME(player);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(1.0f).NAME(offStateAnimationSpeed);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(state);
	FIELD().PUBLIC().EDITOR_IGNORE_ATTRIBUTE().SCRIPTABLE_ATTRIBUTE().NAME(animationAsset);
	FIELD().PUBLIC().EDITOR_IGNORE_ATTRIBUTE().SCRIPTABLE_ATTRIBUTE().NAME(animationClip);
	FIELD().PUBLIC().NAME(onStateFullyTrue);
	FIELD().PUBLIC().NAME(onStateFullyFalse);
	FIELD().PUBLIC().NAME(onStateBecomesTrue);
	FIELD().PUBLIC().NAME(onStateBecomesFalse);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(false).NAME(mState);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mOwner);
	FIELD().PROTECTED().EDITOR_PROPERTY_ATTRIBUTE().INVOKE_ON_CHANGE_ATTRIBUTE(OnAnimationChanged).SERIALIZABLE_ATTRIBUTE().NAME(mAnimation);
}
END_META;
CLASS_METHODS_META(o2::WidgetState)
{

	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const WidgetState&);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, SetOwner, Widget*, bool);
	FUNCTION().PUBLIC().SIGNATURE(void, SetAnimationAsset, const AnimationAssetRef&);
	FUNCTION().PUBLIC().SIGNATURE(const AnimationAssetRef&, GetAnimationAsset);
	FUNCTION().PUBLIC().SIGNATURE(void, SetAnimationClip, const AnimationClip&);
	FUNCTION().PUBLIC().SIGNATURE(AnimationClip&, GetAnimationClip);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, SetState, bool);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, SetStateForcible, bool);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(bool, GetState);
	FUNCTION().PUBLIC().SIGNATURE(void, Update, float);
	FUNCTION().PROTECTED().SIGNATURE(void, OnAnimationChanged);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDeserialized, const DataValue&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDeserializedDelta, const DataValue&, const IObject&);
}
END_META;
