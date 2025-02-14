#pragma once

#include "ImageComponent.h"
#include "o2/Assets/Asset.h"
#include "o2/Assets/Types/ActorAsset.h"
#include "o2/Assets/Types/AnimationAsset.h"
#include "o2/Assets/Types/DataAsset.h"
#include "o2/Assets/Types/ImageAsset.h"
#include "o2/Render/Sprite.h"
#include "o2/Scene/Component.h"
#include "o2/Scene/Components/ParticlesEmitterComponent.h"
#include "o2/Scene/Physics/RigidBody.h"
#include "o2/Scene/Tags.h"
#include "o2/Utils/Editor/Attributes/DontDeleteAttribute.h"
#include "o2/Utils/Editor/Attributes/InvokeOnChangeAttribute.h"
#include "o2/Utils/Math/Curve.h"

namespace o2
{
	class EditorTestComponent: public Component
	{
	public:
		Vector<Vec2I> mVecs; // @SERIALIZABLE @INVOKE_ON_CHANGE(Test)

		enum class TestEnum { A, B, C, D, E, F, G, H, K, L, M, N };
		 
		class TestInside: public ISerializable
		{
		public:
			float mFloat = 1.2f;        // @SERIALIZABLE @SCRIPTABLE
			String mString = String("bla bla"); // @SERIALIZABLE @SCRIPTABLE
			WString mWString;           // @SERIALIZABLE @SCRIPTABLE
			bool mBool = true;          // @SERIALIZABLE @SCRIPTABLE
			 
			ComponentRef mComponent;   // @SERIALIZABLE @SCRIPTABLE
			Ref<RigidBody> mRigidBody; // @SERIALIZABLE @SCRIPTABLE

			bool operator==(const TestInside& other) const { return false; }

			// @SCRIPTABLE
			TestInside() {}

			SERIALIZABLE(TestInside);
		};

		void SetSpritePtr(Sprite* sprite) { mSprite = sprite; }
		Sprite* GetSpritePtr() const { return mSprite; }

		void SetSprite(const Sprite& sprite) { *mSprite = sprite; }
		Sprite GetSprite() const { return *mSprite; }

		void SetArray(const Vector<Vec2I>& arr) { mVecs = arr; }
		const Vector<Vec2I>& GetArray() const { return mVecs; }

		PROPERTIES(EditorTestComponent);
		PROPERTY(Sprite*, spritePropPtr, SetSpritePtr, GetSpritePtr);
		PROPERTY(Sprite, spriteProp, SetSprite, GetSprite);
		PROPERTY(Vector<Vec2I>, arr, SetArray, GetArray);

		int mInteger;					                    // @SERIALIZABLE @INVOKE_ON_CHANGE(Test)
		float mFloat;					                    // @SERIALIZABLE
		String mString;					                    // @SERIALIZABLE
		WString mWString;				                    // @SERIALIZABLE
		bool mBool;						                    // @SERIALIZABLE
		ImageAssetRef mImageAsset;			                // @SERIALIZABLE
		ActorAssetRef mActorAsset;			                // @SERIALIZABLE
		DataAssetRef mDataAsset;                            // @SERIALIZABLE
		AnimationAssetRef mAnimationAsset;                  // @SERIALIZABLE
		Sprite* mSprite = mnew Sprite();                    // @SERIALIZABLE @DONT_DELETE
		ActorRef mActor;                                    // @SERIALIZABLE
		TagGroup mTags;                                     // @SERIALIZABLE
		SceneLayer* mLayer;                               
		ComponentRef mComponent;                            // @SERIALIZABLE
		Ref<RigidBody> mRigidBody;                          // @SERIALIZABLE
		Ref<ImageComponent> mImageComponent;                // @SERIALIZABLE
		Ref<ParticlesEmitterComponent> mParticlesComponent; // @SERIALIZABLE
		Color4 mColor;					                    // @SERIALIZABLE
		Vec2F mVec2F;					                    // @SERIALIZABLE
		Vec2I mVec2I;					                    // @SERIALIZABLE
		Vertex mVertex;				                    // @SERIALIZABLE
		RectF mRectF;					                    // @SERIALIZABLE
		RectI mRectI;					                    // @SERIALIZABLE
		BorderF mBorderF;					                // @SERIALIZABLE
		BorderI mBorderI;					                // @SERIALIZABLE
		Curve mCurve = Curve::EaseInOut();                  // @SERIALIZABLE
		TestInside mTestInside;                             // @SERIALIZABLE
		TestEnum mTestEnum;                                 // @SERIALIZABLE
		TestInside* mTestInsidePtr = nullptr;               // @SERIALIZABLE

		Vector<int> mIntVector;                    // @SERIALIZABLE
		Vector<TestInside> mTestInsideVector;      // @SERIALIZABLE @INVOKE_ON_CHANGE(Test)
		Vector<TestInside*> mTestInsideptrsVector; // @SERIALIZABLE @INVOKE_ON_CHANGE(Test)
		Vector<ActorRef> mActorVector;             // @SERIALIZABLE
		Vector<AnimationAssetRef> mAssetsVector;   // @SERIALIZABLE

		Vector<Vector<TestInside*>> mVectorOfVector; // @SERIALIZABLE

		Map<String, String> mDictionary;    // @SERIALIZABLE
		float mFloat2;					    // @SERIALIZABLE
		float mFloat3;					    // @SERIALIZABLE
		float mFloat4;					    // @SERIALIZABLE
		float mFloat5;					    // @SERIALIZABLE
		float mFloat6;					    // @SERIALIZABLE
		float mFloat7;					    // @SERIALIZABLE
		float mFloat8;					    // @SERIALIZABLE
		float mFloat9;					    // @SERIALIZABLE

		static String GetCategory();

		void Test();

		SERIALIZABLE(EditorTestComponent);
	};
}

PRE_ENUM_META(o2::EditorTestComponent::TestEnum);

CLASS_BASES_META(o2::EditorTestComponent)
{
	BASE_CLASS(o2::Component);
}
END_META;
CLASS_FIELDS_META(o2::EditorTestComponent)
{
	FIELD().PUBLIC().INVOKE_ON_CHANGE_ATTRIBUTE(Test).SERIALIZABLE_ATTRIBUTE().NAME(mVecs);
	FIELD().PUBLIC().NAME(spritePropPtr);
	FIELD().PUBLIC().NAME(spriteProp);
	FIELD().PUBLIC().NAME(arr);
	FIELD().PUBLIC().INVOKE_ON_CHANGE_ATTRIBUTE(Test).SERIALIZABLE_ATTRIBUTE().NAME(mInteger);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(mFloat);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(mString);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(mWString);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(mBool);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(mImageAsset);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(mActorAsset);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(mDataAsset);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(mAnimationAsset);
	FIELD().PUBLIC().DONT_DELETE_ATTRIBUTE().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(mnew Sprite()).NAME(mSprite);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(mActor);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(mTags);
	FIELD().PUBLIC().NAME(mLayer);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(mComponent);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(mRigidBody);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(mImageComponent);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(mParticlesComponent);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(mColor);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(mVec2F);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(mVec2I);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(mVertex);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(mRectF);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(mRectI);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(mBorderF);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(mBorderI);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(Curve::EaseInOut()).NAME(mCurve);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(mTestInside);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(mTestEnum);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(nullptr).NAME(mTestInsidePtr);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(mIntVector);
	FIELD().PUBLIC().INVOKE_ON_CHANGE_ATTRIBUTE(Test).SERIALIZABLE_ATTRIBUTE().NAME(mTestInsideVector);
	FIELD().PUBLIC().INVOKE_ON_CHANGE_ATTRIBUTE(Test).SERIALIZABLE_ATTRIBUTE().NAME(mTestInsideptrsVector);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(mActorVector);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(mAssetsVector);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(mVectorOfVector);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(mDictionary);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(mFloat2);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(mFloat3);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(mFloat4);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(mFloat5);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(mFloat6);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(mFloat7);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(mFloat8);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(mFloat9);
}
END_META;
CLASS_METHODS_META(o2::EditorTestComponent)
{

	FUNCTION().PUBLIC().SIGNATURE(void, SetSpritePtr, Sprite*);
	FUNCTION().PUBLIC().SIGNATURE(Sprite*, GetSpritePtr);
	FUNCTION().PUBLIC().SIGNATURE(void, SetSprite, const Sprite&);
	FUNCTION().PUBLIC().SIGNATURE(Sprite, GetSprite);
	FUNCTION().PUBLIC().SIGNATURE(void, SetArray, const Vector<Vec2I>&);
	FUNCTION().PUBLIC().SIGNATURE(const Vector<Vec2I>&, GetArray);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCategory);
	FUNCTION().PUBLIC().SIGNATURE(void, Test);
}
END_META;

CLASS_BASES_META(o2::EditorTestComponent::TestInside)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::EditorTestComponent::TestInside)
{
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(1.2f).NAME(mFloat);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(String("bla bla")).NAME(mString);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().SERIALIZABLE_ATTRIBUTE().NAME(mWString);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(true).NAME(mBool);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().SERIALIZABLE_ATTRIBUTE().NAME(mComponent);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().SERIALIZABLE_ATTRIBUTE().NAME(mRigidBody);
}
END_META;
CLASS_METHODS_META(o2::EditorTestComponent::TestInside)
{

	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().CONSTRUCTOR();
}
END_META;
