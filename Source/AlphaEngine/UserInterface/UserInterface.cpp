#include "AlphaEngineStd.h"
#include "UserInterface.h"

BaseUI::BaseUI(Context* context) : IScreenElement(context)
{ 
	m_bIsVisible = true;
	m_PosX = m_PosY = 0; 
	m_Width = 100; 
	m_Height = 100; 
}

BaseUI::~BaseUI()
{


}

void BaseUI::VOnUpdate(float timeStep)
{ 

}

bool BaseUI::VIsVisible() const
{
	return m_bIsVisible;
}


void BaseUI::VSetVisible(bool visible)
{ 
	m_bIsVisible = visible; 
}