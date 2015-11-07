#ifndef CHARACTER_CONTROLLER_H
#define CHARACTER_CONTROLLER_H

class Character;

class CharacterController : public IPointerHandler, public IKeyboardHandler
{
	URHO3D_OBJECT(CharacterController,IKeyboardHandler)
public:
	CharacterController(Context* context, WeakPtr<Character> character);
	virtual ~CharacterController();

	void SetCharacter(WeakPtr<Character> character);
	void OnUpdate(float timeStep);

	bool VOnPointerMove(const Vector2& mousePos, const int radius);
	bool VOnPointerButtonDown(const Vector2& mousePos, const int radius, const String& buttonName);
	bool VOnPointerButtonUp(const  Vector2& mousePos, const int radius, const String& buttonName);

	inline bool VOnKeyDown(const int c);
	inline bool VOnKeyUp(const int c);

private:
	void InitializeAllDelegates();				// Register all delegates
	void DestroyAllDelegates();					// Unsubscribe from all events

protected:
	Controls m_Controls;
	Input* m_pInput;

	WeakPtr<Character> m_pCharacter;
	Vector3 m_MoveDir;
	Vector3 m_Rotation;
	bool m_bIsKey[256];			// Which keys are up and down
	bool m_bIsMouseRButtonDown;
};

#endif
