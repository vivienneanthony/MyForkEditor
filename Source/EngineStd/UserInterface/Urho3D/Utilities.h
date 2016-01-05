#pragma once

extern Window* CreateCustomWindow(Context* context, String name, int width, int height, IntVector2 offset);
extern Text* CreateCustomText(Window* window, String text);

extern Button* CreateCloseButton(Window* window, String name);
extern Button* CreateCustomButton(Window* window, String name, String buttontext);

extern UIElement* CreateLetterBox(Context * context);
