#ifndef LETTERBOX_ELEMENT_H
#define LETTERBOX_ELEMENT_H

#include "UserInterface/UserInterface.h"

class LetterBox : public BaseUI
{
	URHO3D_OBJECT(LetterBox, BaseUI);

public:
	LetterBox(Context* context);
	virtual ~LetterBox();

    // Default virtuals must be defined
	virtual bool VOnRestore();
	virtual bool VOnLostDevice();
	virtual void VOnUpdate(float timeStep);
	virtual void VOnShutdown();

	virtual bool VOnMsgProc(AppMsg message);

	virtual int VGetZOrder() const;
	virtual void VSetZOrder(int const zOrder);

    // Visibility
    void VSetVisible(bool visible);
    bool VIsVisible() const;

private:
    UIElement * m_pLetterBox;           // Letter Box UIElement
};

#endif


