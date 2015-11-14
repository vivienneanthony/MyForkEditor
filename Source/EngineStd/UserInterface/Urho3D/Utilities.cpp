#include "EngineStd.h"
#include "Utilities.h"

Window* CreateCustomWindow(Context* context, String name, IntVector2 offset)
{
	// Create the Window and add it to the UI's root node
	Window* window = new Window(context);
	context->GetSubsystem<UI>()->GetRoot()->AddChild(window);

	// Set Window size and layout settings
	window->SetMinSize(692, 692);
	window->SetLayout(LM_VERTICAL, 8);
	window->SetAlignment(HA_CENTER, VA_CENTER);
	window->SetName(name);
	window->SetFixedHeight(692);
	window->SetFixedHeightResizing(true);
	window->SetMovable(true);

	window->SetStyleAuto();
	window->SetChildOffset(offset);

	// Create Window 'titlebar' container
	UIElement* titleBar = new UIElement(context);
	titleBar->SetMinSize(0, 32);
	titleBar->SetVerticalAlignment(VA_TOP);
	titleBar->SetHorizontalAlignment(HA_RIGHT);
	titleBar->SetLayoutMode(LM_HORIZONTAL);

	// Add the title bar to the Window
	window->AddChild(titleBar);

	return window;
}

Button* CreateCloseButton(Window* window, String name)
{
	Text* closeText = new Text(window->GetContext());
	closeText->SetName(String("CloseText_") + name);
	closeText->SetText("Close");
	closeText->SetAlignment(HA_CENTER, VA_TOP);

	Button* buttonClose = new Button(window->GetContext());
	buttonClose->SetName(name);
	buttonClose->SetOpacity(0.2f);
	buttonClose->AddChild(closeText);
	window->AddChild(buttonClose);

	buttonClose->SetStyleAuto();
	buttonClose->SetFixedWidth(50);
	buttonClose->SetFixedHeight(20);
	buttonClose->SetAlignment(HorizontalAlignment::HA_CENTER, VA_TOP);
	closeText->SetStyleAuto();

	return buttonClose;
}

Button* CreateCustomButton(Window* window, String name, String buttontext)
{
	Text* text = new Text(window->GetContext());
	text->SetName(name);
	text->SetText(buttontext);
	text->SetAlignment(HA_CENTER, VA_TOP);

	Button* button = new Button(window->GetContext());
	button->SetName(name);
	button->SetOpacity(0.2f);
	button->AddChild(text);
	window->AddChild(button);

	button->SetStyleAuto();
	button->SetFixedWidth(50);
	button->SetFixedHeight(20);
	button->SetAlignment(HorizontalAlignment::HA_CENTER, VA_TOP);
	text->SetStyleAuto();

	return button;
}