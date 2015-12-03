#pragma once

extern Window* CreateCustomWindow(Context* context, String name, IntVector2 offset);

extern Button* CreateCloseButton(Window* window, String name);
extern Button* CreateCustomButton(Window* window, String name, String buttontext);

extern UIElement* CreateLetterBox(Context * context);
