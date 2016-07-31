#include "AnimatedValue.h"

#include "Animation/Animatable.h"

namespace o2
{
	void AnimatedValue<RectF>::RegInAnimatable(AnimationState* state, const String& path)
	{
		state->mOwner->RegAnimatedValue<RectF>(this, path, state);
	}

	void AnimatedValue<bool>::RegInAnimatable(AnimationState* state, const String& path)
	{
		state->mOwner->RegAnimatedValue<bool>(this, path, state);
	}

	void AnimatedValue<Color4>::RegInAnimatable(AnimationState* state, const String& path)
	{
		state->mOwner->RegAnimatedValue<Color4>(this, path, state);
	}
}