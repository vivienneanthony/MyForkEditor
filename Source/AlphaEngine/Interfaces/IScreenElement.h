#ifndef ISCREEN_ELEMENT_H
#define ISCREEN_ELEMENT_H

class IScreenElement : public Object
{
public:
	IScreenElement(Context* context);
	virtual ~IScreenElement();

	virtual bool VOnRestore() = 0;
	virtual bool VOnLostDevice() = 0;
	virtual void VOnUpdate(float timeStep) = 0;
	virtual void VOnShutdown() = 0;

	virtual bool VOnMsgProc(AppMsg message) = 0;

	virtual int VGetZOrder() const = 0;
	virtual void VSetZOrder(int const zOrder) = 0;

	virtual bool VIsVisible() const = 0;
	virtual void VSetVisible(bool visible) = 0;

	virtual bool const operator < (IScreenElement const &other) { return VGetZOrder() < other.VGetZOrder(); }
};

#endif
