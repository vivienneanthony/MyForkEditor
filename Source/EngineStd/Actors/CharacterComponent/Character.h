#ifndef CHARACTER_COMPONENT_H
#define CHARACTER_COMPONENT_H

const int CTRL_FORWARD = 1;
const int CTRL_BACK = 2;
const int CTRL_LEFT = 4;
const int CTRL_RIGHT = 8;
const int CTRL_JUMP = 16;

const float MOVE_FORCE = 0.3f;
const float INAIR_MOVE_FORCE = 0.02f;
const float BRAKE_FORCE = 0.2f;
const float JUMP_FORCE = 7.0f;
const float YAW_SENSITIVITY = 0.1f;
const float INAIR_THRESHOLD_TIME = 0.1f;

// Character component, responsible for physical movement according to controls, as well as animation.
class Character : public LogicComponent
{
	URHO3D_OBJECT(Character, LogicComponent)

public:
	// Construct.
	Character(Context* context);

	// Register object factory and attributes.
	static void RegisterObject(Context* context);
	void Initialize();

	// Handle startup. Called by LogicComponent base class.
	virtual void Start();
	// Handle physics world update. Called by LogicComponent base class.
	virtual void FixedUpdate(float timeStep);

	// Getters/Setters
	inline void SetMoveDir(Vector3& move) { m_MoveDir.x_ = move.x_; m_MoveDir.y_ = move.y_; m_MoveDir.z_ = move.z_; }
	inline void SetInJump(bool jump) { m_bIsInJump = jump; }
	inline void SetRotation(Vector3& rot) { m_Rotation.x_ = rot.x_; m_Rotation.y_ = rot.y_; m_Rotation.z_ = rot.z_; }
	// Delegates
private:
	void InitializeAllDelegates();				// Register all delegates
	void DestroyAllDelegates();					// Unsubscribe from all events

	void HandleNodeCollisionDelegate(StringHash eventType, VariantMap& eventData);

private:
	RigidBody* m_pBody;														// Cache the components for faster access instead of finding them each frame
	AnimationController* m_pAnimControl;									// Cache the components for faster access instead of finding them each frame
	Vector3 m_MoveDir;

	bool m_bIsOnGround;														// Grounded flag for movement.

	bool m_bIsInJump;
	bool m_bIsOnJump;														// Jump flag.
	float m_InAirTime;														// In air timer. Due to possible physics inaccuracy,
																			// character can be off ground for max. 1/10 second and still be allowed to move.
	Vector3 m_Rotation;
};

#endif // CHARACTER_COMPONENT_H
