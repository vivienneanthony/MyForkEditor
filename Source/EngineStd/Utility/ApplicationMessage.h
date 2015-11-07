#ifndef APPLICATION_MESSAGE_H
#define APPLICATION_MESSAGE_H

enum APP_EVENT
{
	KEY_UP, KEY_DOWN,
	TEXT_INPUT,
	MOUSE_MOVE, MOUSE_LBUTTONDOWN, MOUSE_RBUTTONDOWN, MOUSE_LBUTTONUP, MOUSE_RBUTTONUP, MOUSE_MIDDLEDOWN, MOUSE_MIDDLEUP
};

enum APP_SYSCOMMAND { SYS_MAXIMIZE, SYS_CLOSE, SYS_ALTLEFT, SYS_ALTRIGHT, SYS_UNKNOWN };
enum SPECIFIC_MSG { SPC_QUITNOPROMT };


struct APP_WPARAM
{
	APP_SYSCOMMAND m_syscommand;
	SPECIFIC_MSG m_specificMsg;
};

struct APP_LPARAM
{
	Vector2 m_position;
	int m_keycode;
	int m_scancode;
	int m_mode;
	unsigned int m_unicode;
};


struct AppMsg
{
	APP_EVENT uEvent;
	APP_LPARAM lParam;
	APP_WPARAM wParam;
};

#endif // APPLICATION_MESSAGE_H
