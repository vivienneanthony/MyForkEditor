#include "EngineStd.h"
#include "Actors/CharacterComponent/Character.h"
#include "CharacterController.h"

CharacterController::CharacterController(Context* context, WeakPtr<Character> character) : IPointerHandler(context), IKeyboardHandler(context), Object(context), m_pCharacter(character)
{
	memset(m_bIsKey, 0, sizeof(m_bIsKey));
	m_Rotation = Vector3::ZERO;
	m_pInput = GetSubsystem<Input>();
	m_bIsMouseRButtonDown = false;
	URHO3D_ATTRIBUTE("Controls Yaw", float, m_Controls.yaw_, 0.0f, AM_DEFAULT);
	URHO3D_ATTRIBUTE("Controls Pitch", float, m_Controls.pitch_, 0.0f, AM_DEFAULT);
}

CharacterController::~CharacterController()
{


}

void CharacterController::SetCharacter(WeakPtr<Character> character)
{
	m_pCharacter = character;
}

void CharacterController::OnUpdate(float timeStep)
{
	// IT IS NOT CORRECT.
	// THERE SHOULD BE SEND MESSAGE TO GAME LOGIC, INSTEAD OF UPDATE POSITION OF CHARACTER
	m_MoveDir = Vector3::ZERO;

	if (m_bIsKey['W'])
	{
		m_MoveDir += Vector3::FORWARD;
	}

	if (m_bIsKey['S'])
	{
		m_MoveDir += Vector3::BACK;
	}

	if (m_bIsKey['A'])
	{
		m_MoveDir += Vector3::LEFT;
	}

	if (m_bIsKey['D'])
	{
		m_MoveDir += Vector3::RIGHT;
	}

	if (m_bIsKey[KEY_SPACE])
	{
		m_pCharacter->SetInJump(true);
	}
	else
	{
		m_pCharacter->SetInJump(false);
	}

	// Limit pitchj
	m_Rotation.y_ = Clamp(m_Rotation.y_, -80.0f, 80.0f);

	m_pCharacter->SetRotation(m_Rotation);
	m_pCharacter->SetMoveDir(m_MoveDir);
}

bool CharacterController::VOnPointerMove(const Vector2& mousePos, const int radius)
{
	if (m_bIsMouseRButtonDown)
	{
		m_Rotation.x_ += m_pInput->GetMouseMoveX() * YAW_SENSITIVITY;
		m_Rotation.y_ += m_pInput->GetMouseMoveY() * YAW_SENSITIVITY;
	}
	return true;
}

bool CharacterController::VOnPointerButtonDown(const Vector2& mousePos, const int radius, const String& buttonName)
{
	if (buttonName == "PointerRight")
	{
		m_bIsMouseRButtonDown = true;
		return true;
	}
	return true;
}

bool CharacterController::VOnPointerButtonUp(const  Vector2& mousePos, const int radius, const String& buttonName)
{
	if (buttonName == "PointerRight")
	{
		m_bIsMouseRButtonDown = false;
		return true;
	}
	return true;

}

bool CharacterController::VOnKeyDown(const int c)
{
	m_Controls.Set(c, true);
	if (c >= 0 && c <= 255)
	{
		m_bIsKey[c] = true;
	}

	return true;
}

bool CharacterController::VOnKeyUp(const int c)
{
	m_Controls.Set(c, false);
	if (c >= 0 && c <= 255)
	{
		m_bIsKey[c] = false;
	}
	return true;
}


void CharacterController::InitializeAllDelegates()
{

}

void CharacterController::DestroyAllDelegates()
{

}
