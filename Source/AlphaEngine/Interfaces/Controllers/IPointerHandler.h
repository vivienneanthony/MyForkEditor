#ifndef IPOINTER_HANDLER_H
#define IPOINTER_HANDLER_H

class IPointerHandler : public virtual Object
{
public:
	IPointerHandler(Context* context);
	virtual ~IPointerHandler();
	virtual bool VOnPointerMove(const Vector2& pos, const int radius) = 0;
	virtual bool VOnPointerButtonDown(const Vector2& pos, const int radius, const String& buttonName) = 0;
	virtual bool VOnPointerButtonUp(const Vector2& pos, const int radius, const String& buttonName) = 0;
};

#endif
