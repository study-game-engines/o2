#include "o2Editor/stdafx.h"
#include "TreeWindow.h"

#include "o2/Animation/Tracks/AnimationFloatTrack.h"
#include "o2/Scene/Actor.h"
#include "o2/Scene/Components/AnimationComponent.h"
#include "o2/Scene/Components/EditorTestComponent.h"
#include "o2/Scene/Components/ImageComponent.h"
#include "o2/Scene/Components/ParticlesEmitterComponent.h"
#include "o2/Scene/Scene.h"
#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Scene/UI/WidgetState.h"
#include "o2/Scene/UI/Widgets/Button.h"
#include "o2/Scene/UI/Widgets/ContextMenu.h"
#include "o2/Scene/UI/Widgets/EditBox.h"
#include "o2/Scene/UI/Widgets/Toggle.h"
#include "o2/Scene/UI/Widgets/Tree.h"
#include "o2/Utils/Editor/SceneEditableObject.h"
#include "o2/Utils/System/Clipboard.h"
#include "o2Editor/Core/Actions/Create.h"
#include "o2Editor/Core/Actions/Delete.h"
#include "o2Editor/Core/Actions/Enable.h"
#include "o2Editor/Core/Actions/Lock.h"
#include "o2Editor/Core/Actions/Reparent.h"
#include "o2Editor/Core/EditorApplication.h"
#include "o2Editor/PropertiesWindow/PropertiesWindow.h"
#include "o2Editor/SceneWindow/SceneEditScreen.h"
#include "o2Editor/TreeWindow/SceneTree.h"

DECLARE_SINGLETON(Editor::TreeWindow);

namespace Editor
{
	TreeWindow::TreeWindow()
	{
		InitializeWindow();
	}

	TreeWindow::TreeWindow(const TreeWindow& other) :
		IEditorWindow(other)
	{
		InitializeWindow();
	}

	TreeWindow::~TreeWindow()
	{}

	SceneTree* TreeWindow::GetSceneTree() const
	{
		return mSceneTree;
	}

	void TreeWindow::HighlightObjectTreeNode(SceneEditableObject* targetObject)
	{
		mSceneTree->ScrollToAndHighlight(targetObject);
	}

	void TreeWindow::SetWidgetsLayersVisible(bool visible)
	{
		Widget::isEditorLayersVisible = visible;
		mSceneTree->UpdateNodesView();
	}

	bool TreeWindow::IsWidgetsLayersVisible() const
	{
		return Widget::isEditorLayersVisible;
	}

	void TreeWindow::SetWidgetsInternalChildrenVisible(bool visible)
	{
		Widget::isEditorInternalChildrenVisible = visible;
		mSceneTree->UpdateNodesView();
	}

	bool TreeWindow::IsWidgetsInternalChildrenVisible() const
	{
		return Widget::isEditorInternalChildrenVisible;
	}

	void TreeWindow::InitializeWindow()
	{
		mWindow->caption = "Tree";
		mWindow->name = "tree window";
		mWindow->SetIcon(mnew Sprite("ui/UI4_tree_wnd_icon.png"));
		mWindow->SetIconLayout(Layout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(0, 1)));
		mWindow->SetViewLayout(Layout::BothStretch(0, -2, 0, 18));

		InitializeTopPanel();
		InitializeSceneTree();
		InitializeContextMenu();
		InitializeTestScene();

		mSceneTree->UpdateNodesView();
	}

	void TreeWindow::InitializeTestScene()
	{
		for (int i = 0; i < 10; i++)
			o2Scene.AddTag(String::Format("Tag_#%i", i + 1));

		// test actors
		for (int i = 0; i < 10; i++)
		{
			Actor* actor = mnew Actor(ActorCreateMode::InScene);
			actor->name = String::Format("Actor #%i", i + 1);
			actor->layer = o2Scene.AddLayer(String::Format("Layer #%i", i + 1));

			for (int j = 0; j < 10; j++)
			{
				Actor* childActor = mnew Actor(ActorCreateMode::InScene);
				childActor->name = String::Format("%i Child actor #%i", i + 1, j + 1);
				actor->AddChild(childActor);

				for (int k = 0; k < 10; k++)
				{
					Actor* childActor2 = mnew Actor({ mnew ImageComponent("ui/UI4_Background.png"),
													  mnew EditorTestComponent(),
													  mnew AnimationComponent(),
													  mnew ParticlesEmitterComponent() }, ActorCreateMode::InScene);
					childActor2->name = String::Format("%i %i Sub Child actor #%i", i + 1, j + 1, k + 1);
					//childActor2->transform->position = Vec2F(Math::Random(-500.0f, 500.0f), Math::Random(-500.0f, 500.0f));
					childActor2->transform->position = Vec2F((float)(k*100), (float)((i*2 + j)*100));
					childActor->AddChild(childActor2);
				}
			}
		}
	}

	void TreeWindow::InitializeSceneTree()
	{
		mSceneTree = o2UI.CreateWidget<SceneTree>("standard");
		*mSceneTree->layout = WidgetLayout::BothStretch(2, 0, 0, 18);

		mSceneTree->onNodeRightButtonClicked = THIS_FUNC(OnTreeRBPressed);

		mWindow->AddChild(mSceneTree);
	}

	void TreeWindow::InitializeContextMenu()
	{
		// Context menu
		mTreeContextMenu = o2UI.CreateWidget<ContextMenu>("standard");
		mTreeContextMenu->AddItem("Debug update view", [&]() { mSceneTree->UpdateNodesView(); });

		mTreeContextMenu->AddItem("---");

		mTreeContextMenu->AddItem("Create empty actor", [&]() { OnContextCreateNewPressed(); }, ImageAssetRef(),
								  ShortcutKeys('N', true, true));

		mTreeContextMenu->AddItem("Create UI/Widget", [&]() { CreateObject<Widget>("Widget"); });

		mTreeContextMenu->AddItem("Create UI/---");

		mTreeContextMenu->AddItem("Create UI/Empty layer", [&]() { CreateObject<WidgetLayer>("Layer"); });
		mTreeContextMenu->AddItem("Create UI/Sprite layer", [&]()
		{
			WidgetLayer* newLayer = mnew WidgetLayer();
			newLayer->SetDrawable(mnew Sprite());
			newLayer->name = "Sprite";
			OnCreateObject(newLayer);
		});

		mTreeContextMenu->AddItem("Create UI/Text layer", [&]()
		{
			WidgetLayer* newLayer = mnew WidgetLayer();
			newLayer->SetDrawable(mnew Text());
			newLayer->name = "Text";
			OnCreateObject(newLayer);
		});

		mTreeContextMenu->AddItem("Create UI/---");

		mTreeContextMenu->AddItem("Create UI/Horizontal layout", [&]() { CreateObject<HorizontalLayout>("Horizontal layout"); });
		mTreeContextMenu->AddItem("Create UI/Vertical layout", [&]() { CreateObject<VerticalLayout>("Vertical layout"); });
		mTreeContextMenu->AddItem("Create UI/Grid layout", [&]() { CreateObject<GridLayout>("Grid layout"); });

		mTreeContextMenu->AddItem("Create UI/---");

		mTreeContextMenu->AddItem("Create UI/Scroll area", [&]() { CreateObject<ScrollArea>("Scroll area"); });
		mTreeContextMenu->AddItem("Create UI/Button", [&]() { CreateObject<Button>("Button"); });
		mTreeContextMenu->AddItem("Create UI/Label", [&]() { CreateObject<Label>("Label"); });
		mTreeContextMenu->AddItem("Create UI/Image", [&]() { CreateObject<Image>("Image"); });
		mTreeContextMenu->AddItem("Create UI/Edit box", [&]() { CreateObject<EditBox>("Edit box"); });
		mTreeContextMenu->AddItem("Create UI/Toggle", [&]() { CreateObject<Toggle>("Toggle"); });

		mTreeContextMenu->AddItem("Create UI/---");

		mTreeContextMenu->AddItem("Create UI/Horizontal progress", [&]() { CreateObject<HorizontalProgress>("Progress"); });
		mTreeContextMenu->AddItem("Create UI/Vertical progress", [&]() { CreateObject<VerticalProgress>("Progress"); });
		mTreeContextMenu->AddItem("Create UI/Horizontal scroll bar", [&]() { CreateObject<HorizontalScrollBar>("Scroll bar"); });
		mTreeContextMenu->AddItem("Create UI/Vertical scroll bar", [&]() { CreateObject<VerticalScrollBar>("Scroll bar"); });

		mTreeContextMenu->AddItem("Create UI/---");

		mTreeContextMenu->AddItem("Create UI/Context menu", [&]() { CreateObject<ContextMenu>("Context menu"); });
		mTreeContextMenu->AddItem("Create UI/Menu panel", [&]() { CreateObject<o2::MenuPanel>("Menu panel"); });

		mTreeContextMenu->AddItem("Create UI/---");
		mTreeContextMenu->AddItem("Create UI/Custom drop down", [&]() { CreateObject<CustomDropDown>("Dropdown"); });
		mTreeContextMenu->AddItem("Create UI/Custom list", [&]() { CreateObject<CustomList>("List"); });
		mTreeContextMenu->AddItem("Create UI/Drop down", [&]() { CreateObject<DropDown>("Dropdown"); });
		mTreeContextMenu->AddItem("Create UI/List", [&]() { CreateObject<List>("List"); });
		mTreeContextMenu->AddItem("Create UI/Long list", [&]() { CreateObject<LongList>("List"); });

		mTreeContextMenu->AddItem("Create UI/---");

		mTreeContextMenu->AddItem("Create UI/Spoiler", [&]() { CreateObject<Spoiler>("Spoiler"); });
		mTreeContextMenu->AddItem("Create UI/Tree", [&]() { CreateObject<Tree>("Tree"); });

		mTreeContextMenu->AddItem("Create UI/---");

		mTreeContextMenu->AddItem("Create UI/Window", [&]() { CreateObject<Window>("Window"); });

		InitializeOtherCreateMenu();

		mTreeContextMenu->AddItem("---");

		mTreeContextMenu->AddItem("Copy", [&]() { OnContextCopyPressed(); }, ImageAssetRef(), ShortcutKeys('C', true));
		mTreeContextMenu->AddItem("Cut", [&]() { OnContextCutPressed(); }, ImageAssetRef(), ShortcutKeys('X', true));
		mTreeContextMenu->AddItem("Paste", [&]() { OnContextPastePressed(); }, ImageAssetRef(), ShortcutKeys('V', true));
		mTreeContextMenu->AddItem("Delete", [&]() { OnContextDeletePressed(); }, ImageAssetRef(), ShortcutKeys(VK_DELETE));
		mTreeContextMenu->AddItem("Duplicate", [&]() { OnContextDuplicatePressed(); }, ImageAssetRef(), ShortcutKeys('D', true));

		mTreeContextMenu->AddItem("---");

		mTreeContextMenu->AddItem("Collapse all", [&]() { OnContextCollapseAllPressed(); });
		mTreeContextMenu->AddItem("Expand all", [&]() { OnContextExpandAllPressed(); });

		mTreeContextMenu->AddItem("---");

		mTreeContextMenu->AddItem("Enable\\disable", [&]() { OnContextEnablePressed(); }, ImageAssetRef(), ShortcutKeys('L', true));
		mTreeContextMenu->AddItem("Lock\\unlock", [&]() { OnContextLockPressed(); }, ImageAssetRef(), ShortcutKeys('O', true));

		mTreeContextMenu->AddItem("---");

		mTreeContextMenu->AddItem("Save prototype", [&]() { OnContextCopyPressed(); });
		mTreeContextMenu->AddItem("Reset to prototype", [&]() { OnContextCopyPressed(); });
		mTreeContextMenu->AddItem("Break link to prototype", [&]() { OnContextCopyPressed(); });

		mTreeContextMenu->AddItem("---");

		mTreeContextMenu->AddToggleItem("View widgets layers", true, THIS_FUNC(OnViewLayersToggled));
		mTreeContextMenu->AddToggleItem("View widgets internal children", true, THIS_FUNC(OnViewInternalChildrenToggled));

		mTreeContextMenu->AddItem("---");

		InitUIStyleCreateMenu();

		mWindow->AddChild(mTreeContextMenu);

		mSceneTree->onFocused = [&]() { mTreeContextMenu->SetItemsMaxPriority(); };
		mSceneTree->onUnfocused = [&]() { mTreeContextMenu->SetItemsMinPriority(); };
	}

	void TreeWindow::InitializeOtherCreateMenu()
	{
		auto subTypes = TypeOf(Actor).GetDerivedTypes();

		for (auto subType : subTypes)
		{
			String path = subType->GetName().ReplacedAll("o2::", "").ReplacedAll("::", "/");
			mTreeContextMenu->AddItem("Create other/" + path, [=]() {
				auto objectType = dynamic_cast<const ObjectType*>(subType);
				Actor* newActor = dynamic_cast<Actor*>(objectType->DynamicCastToIObject(subType->CreateSample()));
				newActor->name = path;
				OnCreateObject(newActor);
			});
		}
	}

	void TreeWindow::InitUIStyleCreateMenu()
	{
		auto styleWidgets = o2UI.GetWidgetStyles();

		for (auto styleWidget : styleWidgets)
		{
			auto path = styleWidget->GetType().GetName() + "/" + styleWidget->GetName();
			path.ReplaceAll("o2::", "");
			path.ReplaceAll("::", "/");

			mTreeContextMenu->AddItem(String("Create UI style/") + path, [=]()
			{
				Widget* newWidget = styleWidget->CloneAs<Widget>();
				newWidget->SetEnableForcible(true);
				OnCreateObject(newWidget);
			});
		}
	}

	void TreeWindow::InitUILayersCreateMenu()
	{
		mTreeContextMenu->AddItem("Create/UI layer/Empty", [&]()
		{
			WidgetLayer* newLayer = mnew WidgetLayer();
			newLayer->name = "empty";
			OnCreateObject(newLayer);
		});

		mTreeContextMenu->AddItem("Create/UI layer/Sprite", [&]()
		{
			WidgetLayer* newLayer = mnew WidgetLayer();
			newLayer->SetDrawable(mnew Sprite());
			newLayer->name = "sprite";
			OnCreateObject(newLayer);
		});

		mTreeContextMenu->AddItem("Create/UI layer/Text", [&]()
		{
			WidgetLayer* newLayer = mnew WidgetLayer();
			newLayer->SetDrawable(mnew Text());
			newLayer->name = "text";
			OnCreateObject(newLayer);
		});
	}

	void TreeWindow::PostInitializeWindow()
	{
		mSceneTree->AttachToSceneEvents();
	}

	void TreeWindow::OnSearchPressed()
	{}

	void TreeWindow::OnListTreeToggled(bool value)
	{}

	void TreeWindow::OnSearchEdited(const WString& searchStr)
	{
		mInSearch = searchStr.Length() > 0;

		if (mInSearch)
		{
			mSearchObjects.Clear();

			for (auto actor : o2Scene.GetRootActors())
				SearchObjectsRecursive(actor, (String)searchStr);
		}

		mSceneTree->UpdateNodesView();
	}

	void TreeWindow::SearchObjectsRecursive(SceneEditableObject* object, const String& searchStr)
	{
		if (object->GetName().CountOf(searchStr) > 0)
			mSearchObjects.Add(object);

		for (auto child : object->GetEditablesChildren())
			SearchObjectsRecursive(child, searchStr);
	}

	void TreeWindow::OnTreeRBPressed(TreeNode* node)
	{
		bool canCreateUILayers = false;
		if (node)
		{
			SceneEditableObject* object = (SceneEditableObject*)node->GetObject();
			canCreateUILayers = dynamic_cast<WidgetLayer*>(object) != nullptr ||
				dynamic_cast<Widget::LayersEditable*>(object) != nullptr ||
				dynamic_cast<Widget*>(object) != nullptr;
		}

		mTreeContextMenu->FindItemWidget("Create UI/Empty layer")->SetEnabled(canCreateUILayers);
		mTreeContextMenu->FindItemWidget("Create UI/Sprite layer")->SetEnabled(canCreateUILayers);
		mTreeContextMenu->FindItemWidget("Create UI/Text layer")->SetEnabled(canCreateUILayers);

		mTreeContextMenu->Show();
	}

	void TreeWindow::OnCreateObject(SceneEditableObject* newObject)
	{
		auto selectedObjects = mSceneTree->GetSelectedObjects();

		if (newObject->IsSupportsLayout())
		{
			if (!selectedObjects.IsEmpty())
				newObject->SetLayout(Layout::BothStretch());
			else
				newObject->SetLayout(Layout::Based(BaseCorner::Center, Vec2F(100, 100)));
		}
		else
			newObject->SetTransform(Basis(Vec2F(), Vec2F(100, 0), Vec2F(0, 100)));

		if (selectedObjects.Count() > 0)
		{
			auto obj = selectedObjects.Last();
			auto node = mSceneTree->GetNode(obj);

			SceneEditableObject* parentObject = obj;
			parentObject->AddEditableChild(newObject);

			auto parentChilds = parentObject->GetEditablesChildren();
			auto action = mnew CreateAction({ newObject }, parentObject,
											parentChilds.Count() > 1 ? parentChilds[parentChilds.Count() - 2] : nullptr);

			o2EditorApplication.DoneAction(action);
		}
		else
		{
			auto sceneObjects = o2Scene.GetRootEditableObjects();
			auto action = mnew CreateAction({ newObject }, nullptr,
											sceneObjects.Count() > 1 ? sceneObjects[sceneObjects.Count() - 2] : nullptr);

			o2EditorApplication.DoneAction(action);
		}

		newObject->UpdateTransform();

		mSceneTree->ScrollToAndHighlight(newObject);
		o2EditorSceneScreen.SelectObjectWithoutAction(newObject, false);
	}

	void TreeWindow::OnContextCreateNewPressed()
	{
		if (!mSceneTree->IsFocused())
			return;

		Actor* newActor = mnew Actor();
		newActor->name = "Empty";
		OnCreateObject(newActor);
	}

	void TreeWindow::OnContextCreateSprite()
	{
		if (!mSceneTree->IsFocused())
			return;

		Actor* newActor = mnew Actor({ mnew ImageComponent() });
		newActor->name = "Sprite";
		newActor->transform->size = Vec2F(10, 10);
		OnCreateObject(newActor);
	}

	void TreeWindow::OnContextCreateButton()
	{
		if (!mSceneTree->IsFocused())
			return;
	}

	void TreeWindow::OnContextCopyPressed()
	{
		if (!mSceneTree->IsFocused())
			return;

		Vector<SceneEditableObject*> selectedObjects =
			mSceneTree->GetSelectedObjects().Select<SceneEditableObject*>(
				[](auto x) { return dynamic_cast<SceneEditableObject*>(x); });

		DataValue data;
		data.SetValue(selectedObjects);

		WString clipboardData = data.SaveAsWString();

		Clipboard::SetText(clipboardData);
	}

	void TreeWindow::OnContextCutPressed()
	{
		if (!mSceneTree->IsFocused())
			return;

		OnContextCopyPressed();
		OnContextDeletePressed();
	}

	void TreeWindow::OnContextPastePressed()
	{
		if (!mSceneTree->IsFocused())
			return;

		auto selectedObjects = mSceneTree->GetSelectedObjects();

		SceneEditableObject* parent = selectedObjects.Count() > 0 ? selectedObjects.Last() : nullptr;
		auto parentChilds = parent ? parent->GetEditablesChildren() : o2Scene.GetRootEditableObjects();
		SceneEditableObject* prevObject = parentChilds.Count() > 0 ? parentChilds.Last() : nullptr;

		WString clipboardData = Clipboard::GetText();
		DataValue data;
		data.LoadFromData(clipboardData);

		Vector<SceneEditableObject*> objects;
		data.GetValue(objects);

		for (auto object : objects)
			object->GenerateNewID();

		for (auto object : objects)
			object->SetEditableParent(parent);

		mSceneTree->UpdateNodesView();

		auto action = mnew CreateAction(objects, parent, prevObject);
		o2EditorApplication.DoneAction(action);

		mSceneTree->SetSelectedObjects(objects);
	}

	void TreeWindow::OnContextDeletePressed()
	{
		if (!mSceneTree->IsFocused())
			return;

		o2EditorPropertiesWindow.SetTarget(nullptr);

		auto selectedObjects = o2EditorSceneScreen.GetTopSelectedObjects();
		o2EditorSceneScreen.ClearSelectionWithoutAction();

		auto action = mnew DeleteAction(selectedObjects);
		o2EditorApplication.DoneAction(action);

		for (auto object : selectedObjects)
		{
			if (object->IsSupportsDeleting())
				delete object;
		}

		mSceneTree->UpdateNodesView();
	}

	void TreeWindow::OnContextDuplicatePressed()
	{
		if (!mSceneTree->IsFocused())
			return;

		auto selectedObjects = o2EditorSceneScreen.GetTopSelectedObjects();

		for (auto object : selectedObjects)
		{
			SceneEditableObject* copy = object->CloneAs<SceneEditableObject>();
			copy->SetEditableParent(object->GetEditableParent());
		}

		mSceneTree->UpdateNodesView();
		mSceneTree->SetSelectedObjects(selectedObjects);
	}

	void TreeWindow::OnContextExpandAllPressed()
	{
		if (!mSceneTree->IsFocused())
			return;

		auto selectedObjects = mSceneTree->GetSelectedObjects();

		for (auto object : selectedObjects)
		{
			auto node = mSceneTree->GetNode(object);

			if (node)
				node->Expand();
		}
	}

	void TreeWindow::OnContextCollapseAllPressed()
	{
		if (!mSceneTree->IsFocused())
			return;

		auto selectedObject = mSceneTree->GetSelectedObjects();

		for (auto object : selectedObject)
		{
			auto node = mSceneTree->GetNode(object);

			if (node)
				node->Collapse();
		}
	}

	void TreeWindow::OnContextLockPressed()
	{
		if (!mSceneTree->IsFocused())
			return;

		auto selectedObjects = mSceneTree->GetSelectedObjects();

		bool value = selectedObjects.Count() > 0 ? !selectedObjects.Last()->IsLocked() : true;
		auto action = mnew LockAction(selectedObjects, value);
		o2EditorApplication.DoneAction(action);

		for (auto object : selectedObjects)
		{
			object->SetLocked(value);

			// 			auto node = mActorsTree->GetNode(actor);
			// 			if (node)
			// 				node->UpdateView();
		}
	}

	void TreeWindow::OnContextEnablePressed()
	{
		if (!mSceneTree->IsFocused())
			return;

		auto selectedObjects = mSceneTree->GetSelectedObjects();

		bool value = selectedObjects.Count() > 0 ? !selectedObjects.Last()->IsEnabled() : true;
		auto action = mnew LockAction(selectedObjects, value);
		o2EditorApplication.DoneAction(action);

		for (auto object : selectedObjects)
		{
			object->SetEnabled(value);

			// 			auto node = mActorsTree->GetNode(actor);
			// 			if (node)
			// 				node->UpdateView();
		}
	}

	void TreeWindow::OnViewLayersToggled(bool view)
	{
		SetWidgetsLayersVisible(view);
	}

	void TreeWindow::OnViewInternalChildrenToggled(bool view)
	{
		SetWidgetsInternalChildrenVisible(view);
	}

	void TreeWindow::OnActorCreated(SceneEditableObject* actor)
	{
		mSceneTree->UpdateNodesView();
	}

	void TreeWindow::OnActorDestroyed(SceneEditableObject* actor)
	{
		mSceneTree->UpdateNodesView();
	}

	void TreeWindow::InitializeTopPanel()
	{
		Widget* upPanel = mnew Widget();
		upPanel->name = "up panel";
		*upPanel->layout = WidgetLayout::HorStretch(VerAlign::Top, 0, 0, 20, 0);
		upPanel->AddLayer("back", mnew Sprite("ui/UI4_square_field.png"), Layout::BothStretch(-4, -4, -5, -5));

		Button* searchButton = o2UI.CreateWidget<Button>("search");
		*searchButton->layout = WidgetLayout::Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(-1, 1));
		searchButton->onClick += THIS_FUNC(OnSearchPressed);
		upPanel->AddChild(searchButton);

		mListTreeToggle = o2UI.CreateWidget<Toggle>("list-tree");
		*mListTreeToggle->layout = WidgetLayout::Based(BaseCorner::Right, Vec2F(20, 20), Vec2F(0, 0));
		mListTreeToggle->onToggle += THIS_FUNC(OnListTreeToggled);
		upPanel->AddChild(mListTreeToggle);

		mSearchEditBox = o2UI.CreateWidget<EditBox>("backless");
		*mSearchEditBox->layout = WidgetLayout::BothStretch(19, 2, 21, -2);
		mSearchEditBox->onChanged += THIS_FUNC(OnSearchEdited);
		upPanel->AddChild(mSearchEditBox);

		mWindow->AddChild(upPanel);
	}

}

DECLARE_CLASS(Editor::TreeWindow);
