#pragma once

#include "o2/Render/RectDrawable.h"
#include "o2/Scene/UI/WidgetLayerLayout.h"
#include "o2/Utils/Basic/ITree.h"
#include "o2/Utils/Editor/Attributes/ExpandedByDefaultAttribute.h"
#include "o2/Utils/Editor/SceneEditableObject.h"
#include "o2/Utils/Math/Layout.h"
#include "o2/Utils/Serialization/Serializable.h"
#include "o2/Utils/Types/String.h"

namespace o2
{
	class Widget;

#if IS_EDITOR
	typedef SceneEditableObject WidgetLayerBase;
#else
	typedef ISerializable WidgetLayerBase;
#endif

	// ---------------------
	// Widget drawable layer
	// ---------------------
	class WidgetLayer: public WidgetLayerBase
	{
	public:
		PROPERTIES(WidgetLayer);
		PROPERTY(bool, enabled, SetEnabled, IsEnabled);                  // Enable property
		PROPERTY(float, depth, SetDepth, GetDepth);                      // Drawing depth (higher depths will draw later)
		PROPERTY(float, transparency, SetTransparency, GetTransparency); // Drawable transparency property
		PROPERTY(IRectDrawable*, drawable, SetDrawable, GetDrawable);    // Drawable property @EXPANDED_BY_DEFAULT

		ACCESSOR(WidgetLayer*, child, String, GetChild, GetAllChildLayers); // Child layer accessor

	public:
		WidgetLayerLayout layout; // Drawable layout @SERIALIZABLE

		String name;                // Name of layer @SERIALIZABLE
		Layout interactableLayout;  // Interactable area layout @SERIALIZABLE

	public:
		// Default constructor
		WidgetLayer();

		// Copy-constructor
		WidgetLayer(const WidgetLayer& other);

		// Destructor
		~WidgetLayer();

		// Copy-operator
		WidgetLayer& operator=(const WidgetLayer& other);

		// Returns pointer to owner widget
		Widget* GetOwnerWidget() const;

		// Returns link to prototype
		const WidgetLayer* GetPrototypeLink() const;

		// Sets layer drawable
		void SetDrawable(IRectDrawable* drawable);

		// Returns layer drawable
		IRectDrawable* GetDrawable() const;

		// Draws drawable
		void Draw();

		// Returns is layer enabled
		bool IsEnabled() const override;

		// Returns is layer enabled and all parent are enabled too
		bool IsEnabledInHierarchy() const override;

		// Sets enabling of layer
		void SetEnabled(bool enabled) override;

		// Sets parent layer
		void SetParent(WidgetLayer* parent);

		// Returns parent layer
		WidgetLayer* GetParent() const;

		// Adds new child layer and returns him
		WidgetLayer* AddChild(WidgetLayer* node);

		// Remove child layer and releases him if needs
		void RemoveChild(WidgetLayer* node, bool release = true);

		// Removes and releases all children nodes
		void RemoveAllChildren();

		// Return child layers
		Vector<WidgetLayer*>& GetChildren();

		// Returns constant child layers
		const Vector<WidgetLayer*>& GetChildren() const;

		// Adds child layer
		WidgetLayer* AddChildLayer(const String& name, IRectDrawable* drawable, const Layout& layout = Layout::BothStretch(),
								   float depth = 0.0f);

		// Returns child layer by path
		WidgetLayer* GetChild(const String& path);

		// Returns child layer by name
		WidgetLayer* FindChild(const String& name);

		// Returns child layer with type
		template<typename _type>
		_type* FindChild() const;

		// Returns all child layers
		Vector<WidgetLayer*> GetAllChilds() const;

		// Sets depth
		void SetDepth(float depth);

		// Returns depth
		float GetDepth() const;

		// Sets transparency and updates this and children result transparencies
		void SetTransparency(float transparency);

		// Returns transparency
		float GetTransparency();

		// Returns result transparency
		float GetResTransparency() const;

		// Returns true if layer is under point
		bool IsUnderPoint(const Vec2F& point);

		// Returns layout rectangle
		const RectF& GetRect() const;

		SERIALIZABLE(WidgetLayer);

	protected:
		struct ICopyVisitor
		{
			virtual ~ICopyVisitor() {}
			virtual void OnCopy(const WidgetLayer* source, WidgetLayer* target) = 0;
		};

		struct MakePrototypeCloneVisitor: public ICopyVisitor
		{
			void OnCopy(const WidgetLayer* source, WidgetLayer* target) override;
		};

		struct InstantiatePrototypeCloneVisitor: public ICopyVisitor
		{
			void OnCopy(const WidgetLayer* source, WidgetLayer* target) override;
		};

	protected:
		mutable ICopyVisitor* mCopyVisitor = nullptr; // Copy visitor. Used when instantiating prefab

		const WidgetLayer* mPrototypeLink = nullptr; // Linked prototype layer

		IRectDrawable* mDrawable = nullptr; // Drawable @SERIALIZABLE

		SceneUID mUID = 0; // Scene editor uid @SERIALIZABLE @IGNORE_DELTA_SEARCH

		bool mEnabled = true; // Is layer enabled

		float mTransparency = 1.0f;    // Layer transparency @SERIALIZABLE
		float mResTransparency = 1.0f; // Result drawable transparency, depends on parent transparency

		float mDepth = 0.0f; // Depth of drawable @SERIALIZABLE

		RectF mAbsolutePosition; // Result absolute drawable position
		RectF mInteractableArea; // Interactable area, depends on interactableLayout

		Widget* mOwnerWidget = nullptr; // Owner widget pointer 

		WidgetLayer*         mParent = nullptr; // Pointer to parent layer 
		Vector<WidgetLayer*> mChildren;         // Children layers

		bool mUpdatingLayout = false; // It is true when updating layout now, prevents recursive layout updating 

	protected:
		// Regular serializing without prototype
		void SerializeRaw(DataValue& node) const;

		// Regular deserializing without prototype
		void DeserializeRaw(const DataValue& node);

		// Regular serializing with prototype
		void SerializeWithProto(DataValue& node) const;

		// Regular deserializing with prototype
		void DeserializeWithProto(const DataValue& node);

		// Completion deserialization callback
		void OnDeserialized(const DataValue& node) override;

		// Completion deserialization delta callback
		void OnDeserializedDelta(const DataValue& node, const IObject& origin) override;

		// Sets owner widget for this and children
		void SetOwnerWidget(Widget* owner);

		// It is called when layout was changed, calls owner widget layout updating
		void OnLayoutChanged();

		// Updates drawable rect, calling when widget's layout was changed
		void UpdateLayout();

		// It is called when transparency was changed and updates children transparencies
		void UpdateResTransparency();

		// It is called when owner widget including in scene. Registers editable object and it's children
		void OnAddToScene();

		// It is called when owner widget excluding in scene. Unregisters editable object and it's children
		void OnRemoveFromScene();

		// Returns dictionary with all child layers
		Map<String, WidgetLayer*> GetAllChildLayers();

#if IS_EDITOR
	public:
		PROPERTY(bool, locked, SetLocked, IsLocked); // Is locked property

	public:
		// Returns true when object is on scene
		bool IsOnScene() const override;

		// Returns unique id
		SceneUID GetID() const override;

		// Generates new random id 
		void GenerateNewID(bool childs = true) override;

		// Returns name of object
		const String& GetName() const override;

		// Sets name of object
		void SetName(const String& name) override;

		// Returns object's link to prototype
		const SceneEditableObject* GetEditableLink() const override;

		// Returns list of object's children
		Vector<SceneEditableObject*> GetEditableChildren() const override;

		// Returns object's parent object. Return nullptr when it is a root scene object
		SceneEditableObject* GetEditableParent() const override;

		// Sets parent object. nullptr means make this object as root. idx is place in parent children. idx == -1 means last
		void SetEditableParent(SceneEditableObject* object) override;

		// Adds child. idx is place in parent children. idx == -1 means last
		void AddEditableChild(SceneEditableObject* object, int idx = -1) override;

		// Sets index in siblings - children of parent
		void SetIndexInSiblings(int idx) override;

		// Checks that this object can be added as child to another object
		bool CanBeParentedTo(const Type& parentType) override;

		// Returns is that type of object can be enabled and disabled
		bool IsSupportsDisabling() const override;

		// Returns is that type of object can be locked
		bool IsSupportsLocking() const override;

		// Returns is object locked, override when it's supports
		bool IsLocked() const override;

		// Returns is object locked and all parent are locked too
		bool IsLockedInHierarchy() const override;

		// Sets locking of object, override when it's supports
		void SetLocked(bool locked) override;

		// Returns is that type of object can be transformed
		bool IsSupportsTransforming() const override;

		// Returns transform, override when it's supports
		Basis GetTransform() const override;

		// Sets transform of object, override when it's supports
		void SetTransform(const Basis& transform) override;

		// Updates transform immediately
		void UpdateTransform() OPTIONAL_OVERRIDE;

		// Returns is object supports pivot 
		bool IsSupportsPivot() const override;

		// Sets transform pivot point
		void SetPivot(const Vec2F& pivot) override;

		// Returns transform pivot
		Vec2F GetPivot() const override;

		// Returns is that type of object can be transformed with layout
		bool IsSupportsLayout() const override;

		// Returns layout, override when it's supports
		Layout GetLayout() const override;

		// Sets layout of object, override when it's supports
		void SetLayout(const Layout& layout) override;

		// It is called when something changed in this object
		void OnChanged() override;

		// Beginning serialization callback
		void SerializeBasicOverride(DataValue& node) const;

		// Completion deserialization callback
		void DeserializeBasicOverride(const DataValue& node);

	protected:
		bool mIsLocked = false; // Is locked

	protected:
		// It is called before making prototype from this object
		void BeginMakePrototype() const override;

		// It is called before instantiate from this object
		void BeginInstantiatePrototype() const override;

#endif // IS_EDITOR

		friend class Widget;
		friend class WidgetLayerLayout;
	};

	template<typename _type>
	_type* WidgetLayer::FindChild() const
	{
		for (auto child : mChildren)
			if (child->mDrawable && child->mDrawable->GetType() == TypeOf(_type))
				return (_type*)(child->mDrawable);

		for (auto child : mChildren)
		{
			auto res = child->FindChild<_type>();
			if (res)
				return res;
		}

		return nullptr;
	}
}

CLASS_BASES_META(o2::WidgetLayer)
{
	BASE_CLASS(o2::SceneEditableObject);
}
END_META;
CLASS_FIELDS_META(o2::WidgetLayer)
{
	FIELD().PUBLIC().NAME(enabled);
	FIELD().PUBLIC().NAME(depth);
	FIELD().PUBLIC().NAME(transparency);
	FIELD().PUBLIC().EXPANDED_BY_DEFAULT_ATTRIBUTE().NAME(drawable);
	FIELD().PUBLIC().NAME(child);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(layout);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(name);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(interactableLayout);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mCopyVisitor);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mPrototypeLink);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(nullptr).NAME(mDrawable);
	FIELD().PROTECTED().IGNORE_DELTA_SEARCH_ATTRIBUTE().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(0).NAME(mUID);
	FIELD().PROTECTED().DEFAULT_VALUE(true).NAME(mEnabled);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(1.0f).NAME(mTransparency);
	FIELD().PROTECTED().DEFAULT_VALUE(1.0f).NAME(mResTransparency);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(0.0f).NAME(mDepth);
	FIELD().PROTECTED().NAME(mAbsolutePosition);
	FIELD().PROTECTED().NAME(mInteractableArea);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mOwnerWidget);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mParent);
	FIELD().PROTECTED().NAME(mChildren);
	FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mUpdatingLayout);
	FIELD().PUBLIC().NAME(locked);
	FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mIsLocked);
}
END_META;
CLASS_METHODS_META(o2::WidgetLayer)
{

	typedef Map<String, WidgetLayer*> _tmp1;

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const WidgetLayer&);
	FUNCTION().PUBLIC().SIGNATURE(Widget*, GetOwnerWidget);
	FUNCTION().PUBLIC().SIGNATURE(const WidgetLayer*, GetPrototypeLink);
	FUNCTION().PUBLIC().SIGNATURE(void, SetDrawable, IRectDrawable*);
	FUNCTION().PUBLIC().SIGNATURE(IRectDrawable*, GetDrawable);
	FUNCTION().PUBLIC().SIGNATURE(void, Draw);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsEnabled);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsEnabledInHierarchy);
	FUNCTION().PUBLIC().SIGNATURE(void, SetEnabled, bool);
	FUNCTION().PUBLIC().SIGNATURE(void, SetParent, WidgetLayer*);
	FUNCTION().PUBLIC().SIGNATURE(WidgetLayer*, GetParent);
	FUNCTION().PUBLIC().SIGNATURE(WidgetLayer*, AddChild, WidgetLayer*);
	FUNCTION().PUBLIC().SIGNATURE(void, RemoveChild, WidgetLayer*, bool);
	FUNCTION().PUBLIC().SIGNATURE(void, RemoveAllChildren);
	FUNCTION().PUBLIC().SIGNATURE(Vector<WidgetLayer*>&, GetChildren);
	FUNCTION().PUBLIC().SIGNATURE(const Vector<WidgetLayer*>&, GetChildren);
	FUNCTION().PUBLIC().SIGNATURE(WidgetLayer*, AddChildLayer, const String&, IRectDrawable*, const Layout&, float);
	FUNCTION().PUBLIC().SIGNATURE(WidgetLayer*, GetChild, const String&);
	FUNCTION().PUBLIC().SIGNATURE(WidgetLayer*, FindChild, const String&);
	FUNCTION().PUBLIC().SIGNATURE(Vector<WidgetLayer*>, GetAllChilds);
	FUNCTION().PUBLIC().SIGNATURE(void, SetDepth, float);
	FUNCTION().PUBLIC().SIGNATURE(float, GetDepth);
	FUNCTION().PUBLIC().SIGNATURE(void, SetTransparency, float);
	FUNCTION().PUBLIC().SIGNATURE(float, GetTransparency);
	FUNCTION().PUBLIC().SIGNATURE(float, GetResTransparency);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsUnderPoint, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE(const RectF&, GetRect);
	FUNCTION().PROTECTED().SIGNATURE(void, SerializeRaw, DataValue&);
	FUNCTION().PROTECTED().SIGNATURE(void, DeserializeRaw, const DataValue&);
	FUNCTION().PROTECTED().SIGNATURE(void, SerializeWithProto, DataValue&);
	FUNCTION().PROTECTED().SIGNATURE(void, DeserializeWithProto, const DataValue&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDeserialized, const DataValue&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDeserializedDelta, const DataValue&, const IObject&);
	FUNCTION().PROTECTED().SIGNATURE(void, SetOwnerWidget, Widget*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnLayoutChanged);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateLayout);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateResTransparency);
	FUNCTION().PROTECTED().SIGNATURE(void, OnAddToScene);
	FUNCTION().PROTECTED().SIGNATURE(void, OnRemoveFromScene);
	FUNCTION().PROTECTED().SIGNATURE(_tmp1, GetAllChildLayers);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsOnScene);
	FUNCTION().PUBLIC().SIGNATURE(SceneUID, GetID);
	FUNCTION().PUBLIC().SIGNATURE(void, GenerateNewID, bool);
	FUNCTION().PUBLIC().SIGNATURE(const String&, GetName);
	FUNCTION().PUBLIC().SIGNATURE(void, SetName, const String&);
	FUNCTION().PUBLIC().SIGNATURE(const SceneEditableObject*, GetEditableLink);
	FUNCTION().PUBLIC().SIGNATURE(Vector<SceneEditableObject*>, GetEditableChildren);
	FUNCTION().PUBLIC().SIGNATURE(SceneEditableObject*, GetEditableParent);
	FUNCTION().PUBLIC().SIGNATURE(void, SetEditableParent, SceneEditableObject*);
	FUNCTION().PUBLIC().SIGNATURE(void, AddEditableChild, SceneEditableObject*, int);
	FUNCTION().PUBLIC().SIGNATURE(void, SetIndexInSiblings, int);
	FUNCTION().PUBLIC().SIGNATURE(bool, CanBeParentedTo, const Type&);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsSupportsDisabling);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsSupportsLocking);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsLocked);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsLockedInHierarchy);
	FUNCTION().PUBLIC().SIGNATURE(void, SetLocked, bool);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsSupportsTransforming);
	FUNCTION().PUBLIC().SIGNATURE(Basis, GetTransform);
	FUNCTION().PUBLIC().SIGNATURE(void, SetTransform, const Basis&);
	FUNCTION().PUBLIC().SIGNATURE(void, UpdateTransform);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsSupportsPivot);
	FUNCTION().PUBLIC().SIGNATURE(void, SetPivot, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE(Vec2F, GetPivot);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsSupportsLayout);
	FUNCTION().PUBLIC().SIGNATURE(Layout, GetLayout);
	FUNCTION().PUBLIC().SIGNATURE(void, SetLayout, const Layout&);
	FUNCTION().PUBLIC().SIGNATURE(void, OnChanged);
	FUNCTION().PUBLIC().SIGNATURE(void, SerializeBasicOverride, DataValue&);
	FUNCTION().PUBLIC().SIGNATURE(void, DeserializeBasicOverride, const DataValue&);
	FUNCTION().PROTECTED().SIGNATURE(void, BeginMakePrototype);
	FUNCTION().PROTECTED().SIGNATURE(void, BeginInstantiatePrototype);
}
END_META;
