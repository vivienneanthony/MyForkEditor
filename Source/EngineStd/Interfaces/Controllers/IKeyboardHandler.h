#ifndef IKEYBOARD_HANDLER_H
#define IKEYBOARD_HANDLER_H

class IKeyboardHandler : public virtual Object
{
public:
	IKeyboardHandler(Context* context);
	virtual ~IKeyboardHandler();
	virtual bool VOnKeyDown(const int c) = 0;
	virtual bool VOnKeyUp(const int c) = 0;
};

#endif // IKEYBOARD_HANDLER_H
