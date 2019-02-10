#include "stdafx.h"
#include "WindowsLayout.h"

#include "Core/WindowsSystem/IEditorWindow.h"
#include "Core/WindowsSystem/DockWindowPlace.h"
#include "Core/WindowsSystem/DockableWindow.h"
#include "Core/WindowsSystem/WindowsManager.h"
#include "Scene/UI/Widget.h"
#include "Scene/UI/WidgetLayout.h"

namespace Editor
{
	void WindowsLayout::WindowDockPlace::RetrieveLayout(Widget* widget)
	{
		if (widget->GetType() == TypeOf(DockWindowPlace))
		{
			anchors.Set(widget->layout->GetAnchorLeft(), widget->layout->GetAnchorTop(),
						widget->layout->GetAnchorRight(), widget->layout->GetAnchorBottom());

			for (auto child : widget->GetChildWidgets())
			{
				if (child->GetType() == TypeOf(DockWindowPlace))
				{
					childs.Add(WindowDockPlace());
					childs[childs.Count() - 1].RetrieveLayout(child);
				}
				else if (child->GetType() == TypeOf(DockableWindow) && child->IsEnabled())
				{
					windows.Add(child->name);
				}
			}
		}
	}

	bool WindowsLayout::WindowDockPlace::operator==(const WindowDockPlace& other) const
	{
		return anchors == other.anchors && childs == other.childs && windows == other.windows;
	}

	bool WindowsLayout::operator==(const WindowsLayout& other) const
	{
		return mainDock == other.mainDock && windows == other.windows;
	}

	void WindowsLayout::RestoreDock(WindowDockPlace* dockDef, DockWindowPlace* dockWidget)
	{
		Vector<DockWindowPlace*> childDockWidgets;
		for (auto child : dockDef->childs)
		{
			dockWidget->interactable = false;

			DockWindowPlace* newDock = mnew DockWindowPlace();
			newDock->name = "dock place";
			dockWidget->AddChild(newDock);

			*newDock->layout = WidgetLayout::BothStretch();
			newDock->layout->anchorMin = child.anchors.LeftBottom();
			newDock->layout->anchorMax = child.anchors.RightTop();
			childDockWidgets.Add(newDock);
		}

		int idx = 0;
		for (auto child : dockDef->childs)
		{
			DockWindowPlace* newDock = childDockWidgets[idx++];

			if ((child.anchors.left < 1.0f && child.anchors.left > 0.0f) ||
				(child.anchors.right < 1.0f && child.anchors.right > 0.0f))
			{
				DockWindowPlace* neighborMin = childDockWidgets.FindMatch([&](DockWindowPlace* x) {
					return Math::Equals(x->layout->GetAnchorRight(), newDock->layout->GetAnchorLeft()) && x != newDock;
				});

				DockWindowPlace* neighborMax = childDockWidgets.FindMatch([&](DockWindowPlace* x) {
					return Math::Equals(x->layout->GetAnchorLeft(), newDock->layout->GetAnchorRight()) && x != newDock;
				});

				newDock->SetResizibleDir(TwoDirection::Horizontal, 1.5f, neighborMin, neighborMax);

				if (neighborMin)
					neighborMin->SetResizibleDir(TwoDirection::Horizontal, 1.5f, neighborMin->mNeighborMin, newDock);

				if (neighborMax)
					neighborMax->SetResizibleDir(TwoDirection::Horizontal, 1.5f, newDock, neighborMax->mNeighborMax);
			}

			if ((child.anchors.top < 1.0f && child.anchors.top > 0.0f) ||
				(child.anchors.bottom < 1.0f && child.anchors.bottom > 0.0f))
			{
				DockWindowPlace* neighborMin = childDockWidgets.FindMatch([&](DockWindowPlace* x) {
					return Math::Equals(x->layout->GetAnchorTop(), newDock->layout->GetAnchorBottom()) && x != newDock;
				});

				DockWindowPlace* neighborMax = childDockWidgets.FindMatch([&](DockWindowPlace* x) {
					return Math::Equals(x->layout->GetAnchorBottom(), newDock->layout->GetAnchorTop()) && x != newDock;
				});

				newDock->SetResizibleDir(TwoDirection::Vertical, 1.5f, neighborMin, neighborMax);

				if (neighborMin)
					neighborMin->SetResizibleDir(TwoDirection::Vertical, 1.5f, neighborMin->mNeighborMin, newDock);

				if (neighborMax)
					neighborMax->SetResizibleDir(TwoDirection::Vertical, 1.5f, newDock, neighborMax->mNeighborMax);
			}
		}

		idx = 0;
		for (auto child : dockDef->childs)
		{
			DockWindowPlace* newDock = childDockWidgets[idx++];
			RestoreDock(&child, newDock);
		}

		for (auto wnd : dockDef->windows)
		{
			auto window =
				o2EditorWindows.mEditorWindows.FindMatch([&](IEditorWindow* x) { return x->mWindow->GetName() == wnd; });

			if (window)
			{
				DockableWindow* dockWnd = (DockableWindow*)window->mWindow;

				dockWnd->mNonDockSize = dockWnd->layout->size;
				dockWidget->AddChild(dockWnd);
				*dockWnd->layout = WidgetLayout::BothStretch();
				dockWnd->SetDocked(true);
				dockWnd->Show();
			}
		}

		dockWidget->ArrangeChildWindows();

		CleanEmptyDocks(dockWidget);
	}

	void WindowsLayout::CleanEmptyDocks(DockWindowPlace* dockPlace)
	{
		auto childs = dockPlace->GetChildWidgets();
		for (auto child : childs)
		{
			if (auto dockChild = dynamic_cast<DockWindowPlace*>(child))
			{
				CleanEmptyDocks(dockChild);

				if (dockChild->GetChildWidgets().IsEmpty())
					dockPlace->RemoveChild(child);
			}
		}
	}

}

DECLARE_CLASS(Editor::WindowsLayout);

DECLARE_CLASS(Editor::WindowsLayout::WindowDockPlace);
