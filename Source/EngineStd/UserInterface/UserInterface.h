#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

// This was factored to create a common class that
// implements some of the IScreenElement class common
// to modal/modeless dialogs

class BaseUI : public IScreenElement
{
	URHO3D_OBJECT(BaseUI,IScreenElement);

public:
	BaseUI(Context* context);
	virtual ~BaseUI();

	virtual void VOnUpdate(float timeStep);
	virtual bool VOnLostDevice() { return true; };
	virtual bool VIsVisible() const;
	virtual void VSetVisible(bool visible);

protected:
	int	m_PosX;
	int m_PosY;
	int	m_Width;
	int m_Height;
	int	m_Result;
	bool m_bIsVisible;


};

#endif //USER_INTERFACE_H
