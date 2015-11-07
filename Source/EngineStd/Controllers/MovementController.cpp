#include "EngineStd.h"
#include "MovementController.h"

#define MAX(a, b) ((a) >= (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

MovementController::MovementController(Context* context, SharedPtr<Node> object, float initialYaw, float initialPitch, bool rotateWhenLButtonDown) : IPointerHandler(context), IKeyboardHandler(context), Object(context), m_Object(object)
{
	m_MatToWorld = m_Object->GetTransform();
	m_MatFromWorld = m_MatToWorld.Inverse();

	m_fTargetYaw = m_fYaw = static_cast<float>(DEGREES_TO_RADIANS(-initialYaw));
	m_fTargetPitch = m_fPitch = static_cast<float>(DEGREES_TO_RADIANS(initialPitch));

	m_MaxSpeed = 30.0f;	// 30 meters per second
	m_CurrentSpeed = 0.0f;

	Vector3 pos = m_MatToWorld.Translation();
	m_MatPosition.SetTranslation(pos);

	Cursor* cursor = GetSubsystem<UI>()->GetCursor();
	IntVector2 temp = cursor->GetPosition();
	m_LastMousePos.x_ = static_cast<float>(temp.x_);
	m_LastMousePos.y_ = static_cast<float>(temp.y_);

	memset(m_bIsKey, 0x00, sizeof(m_bIsKey));

	m_bIsMouseLButtonDown = false;
	m_bIsRotateWhenLButtonDown = rotateWhenLButtonDown;
}

MovementController::~MovementController()
{


}

void MovementController::SetObject(SharedPtr<Node> newObject)
{
	m_Object = newObject;
	m_MatToWorld = m_Object->GetTransform();
	m_MatFromWorld = m_MatToWorld.Inverse();
}

void MovementController::OnUpdate(float timeStep)
{
	//if (m_bIsKey['Q'])
	//{
	//	// This code is a cheat to position the camera exactly in a given
	//	// spot so I can take screen shots!


	//	m_fTargetYaw = m_fYaw += -64.35f;
	//	m_fTargetPitch = m_fPitch = 28.57f;

	//	// Calculate the new rotation matrix from the camera
	//	// yaw and pitch.
	//	
	//	
	//	return;
	//}

	bool bTranslating = false;
	Vector3 atWorld(0, 0, 0);
	Vector3 rightWorld(0, 0, 0);
	Vector3 upWorld(0, 0, 0);

	if (m_bIsKey['W'] || m_bIsKey['S'])
	{
		Vector3 at = Vector3::FORWARD;

		if (m_bIsKey['S'])
		{
			
		}
		
		atWorld = m_MatToWorld * at;
		
		bTranslating = true;
	}

	if (m_bIsKey['A'] || m_bIsKey['D'])
	{
		Vector3 right = Vector3::RIGHT;
		if (m_bIsKey['A'])
		{
			right *= -1;
		}

		rightWorld = m_MatToWorld * right;	
		bTranslating = true;
	}

	if (m_bIsKey[' '] || m_bIsKey['C'] || m_bIsKey['X'])
	{
		
		bTranslating = true;
	}

	//Handling rotation as a result of mouse position
	{
		// The secret formula!!! Don't give it away!
		//If you are seeing this now, then you must be some kind of elite hacker!
		m_fYaw += (m_fTargetYaw - m_fYaw) * (.35f);
		/*m_fTargetPitch = MAX(-90, MIN(90, m_fTargetPitch));*/
		m_fPitch += (m_fTargetPitch - m_fPitch) * (.35f);

		// Calculate the new rotation matrix from the camera
		// yaw and pitch.
		Matrix4 matRot;
		Quaternion quat = matRot.Rotation();
		quat.FromEulerAngles(static_cast<float>(DEGREES_TO_RADIANS(m_fPitch)), static_cast<float>(DEGREES_TO_RADIANS(m_fYaw)), 0.0f);
		matRot.SetRotation(quat.RotationMatrix());

		// Create the new object-to-world matrix, and the
		// new world-to-object matrix. 
		m_MatToWorld = m_MatPosition * matRot;
		m_Object->SetTransform(m_MatToWorld.Translation(), m_MatToWorld.Rotation());
		m_bIsMousePosUpdated = false;
	}

	if (bTranslating)
	{
		float elapsedTime = timeStep;

		Vector3 direction = atWorld + rightWorld + upWorld;
		direction.Normalize();

		// Ramp the acceleration by the elapsed time.
		float numberOfSeconds = 5.f;
		m_CurrentSpeed += m_MaxSpeed * ((timeStep * timeStep) / numberOfSeconds);
		if (m_CurrentSpeed > m_MaxSpeed)
			m_CurrentSpeed = m_MaxSpeed;

		direction *= m_CurrentSpeed;

		Vector3 pos = m_MatPosition.Translation() + direction;
		m_MatPosition.SetTranslation(pos);
		m_MatToWorld.SetTranslation(pos);

		m_Object->SetTransform(m_MatToWorld.Translation(), m_MatToWorld.Rotation());
	}
	else
	{
		m_CurrentSpeed = 0.0f;
	}
}

bool MovementController::VOnPointerMove(const Vector2& mousePos, const int radius)
{
	// There are two modes supported by this controller.

	if (m_bIsRotateWhenLButtonDown)
	{
		// Mode 1 - rotate the view only when the left mouse button is down
		// Only look around if the left button is down
		if (m_LastMousePos != mousePos && m_bIsMouseLButtonDown)
		{
			m_fTargetYaw = m_fTargetYaw + (m_LastMousePos.x_ - mousePos.x_);
			m_fTargetPitch = m_fTargetPitch + (mousePos.y_ - m_LastMousePos.y_);
			m_LastMousePos.x_ = mousePos.x_;
			m_LastMousePos.y_ = mousePos.y_;
			m_bIsMousePosUpdated = true;
		}
	}
	else if (m_LastMousePos != mousePos)
	{
		// Mode 2 - rotate the controller when the mouse buttons are up
		m_fTargetYaw = m_fTargetYaw + (m_LastMousePos.x_ - mousePos.x_);
		m_fTargetPitch = m_fTargetPitch + (mousePos.y_ - m_LastMousePos.y_);
		m_LastMousePos = mousePos;
		m_bIsMousePosUpdated = true;
	}

	return true;

}

bool MovementController::VOnPointerButtonDown(const Vector2& mousePos, const int radius, const String& buttonName)
{
	if (buttonName == "PointerLeft")
	{
		m_bIsMouseLButtonDown = true;

		// We want mouse movement to be relative to the position
		// the cursor was at when the user first presses down on
		// the left button
		m_LastMousePos = mousePos;
		return true;
	}
	return false;
}

bool MovementController::VOnPointerButtonUp(const  Vector2& mousePos, const int radius, const String& buttonName)
{
	if (buttonName == "PointerLeft")
	{
		m_bIsMouseLButtonDown = false;
		return true;
	}
	return false;
}