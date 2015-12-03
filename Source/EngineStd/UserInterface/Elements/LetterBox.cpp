#include "EngineStd.h"
#include "../UserInterface.h"

#include "LetterBox.h"

LetterBox::LetterBox(Context* context) : BaseUI(context)
{
	m_bIsVisible = true;
	m_PosX = m_PosY = 0;
	m_Width = 100;
	m_Height = 100;

	// Create letterbox
    m_pLetterBox = CreateLetterBox(context_);

    // Set Opacity test
    m_pLetterBox->SetOpacity(.5);

}

LetterBox::~LetterBox()
{


}

void LetterBox::VOnUpdate(float timeStep)
{

}

bool LetterBox::VIsVisible() const
{
	return m_bIsVisible;
}

void LetterBox::VSetVisible(bool visible)
{
	m_bIsVisible = visible;
}

void LetterBox::VSetZOrder(int const zOrder)
{
    return;
}

int LetterBox::VGetZOrder() const
{
    return 1;
}

void LetterBox::VOnShutdown()
{
    return;
}

bool LetterBox::VOnLostDevice()
{
    return true;
}

bool LetterBox::VOnRestore()
{

    return true;
}

bool LetterBox::VOnMsgProc(AppMsg message)
{
    return true;
}
