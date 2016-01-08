#ifndef MOVEMENT_CONTROLLER_H
#define MOVEMENT_CONTROLLER_H

// Implements a WASD style movement controller


class MovementController : public IPointerHandler, public IKeyboardHandler
{
	URHO3D_OBJECT(MovementController, IKeyboardHandler)
public:
	MovementController(Context* context, SharedPtr<Node> object, float initialYaw, float initialPitch, bool rotateWhenLButtonDown);
	virtual ~MovementController();

	void SetObject(SharedPtr<Node> newObject);
	void OnUpdate(float timeStep);

	bool VOnPointerMove(const Vector2& mousePos, const int radius);
	bool VOnPointerButtonDown(const Vector2& mousePos, const int radius, const String& buttonName);
	bool VOnPointerButtonUp(const  Vector2& mousePos, const int radius, const String& buttonName);

	inline bool VOnKeyDown(const int c) { if(c >= 0 && c <= 255) m_bIsKey[c] = true; return true; }
	inline bool VOnKeyUp(const int c) { if (c >= 0 && c <= 255) m_bIsKey[c] = false; return true; }

	// Getter/Setters
	const Matrix3x4* GetToWorld() { return &m_MatToWorld; }
	const Matrix3x4* GetFromWorld() { return &m_MatFromWorld; }

public:
	Vector2	   m_LastMousePos;
	bool	   m_bIsKey[256];			// Which keys are up and down

	// Orientation Controls
	float		m_fTargetYaw;
	float		m_fTargetPitch;
	float		m_fYaw;
	float		m_fPitch;
	float		m_fPitchOnDown;
	float		m_fYawOnDown;
	float		m_MaxSpeed;
	float		m_CurrentSpeed;

	bool		m_bIsMouseLButtonDown;
	bool		m_bIsRotateWhenLButtonDown;

	Matrix3x4     m_MatToWorld;
	Matrix3x4	  m_MatFromWorld;
	Matrix3x4	  m_MatPosition;

	SharedPtr<Node> m_Object;
	bool		  m_bIsMousePosUpdated;
};

#endif
