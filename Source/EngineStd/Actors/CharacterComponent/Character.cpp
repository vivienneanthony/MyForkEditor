#include "EngineStd.h"
#include "Character.h"

Character::Character(Context* context) : LogicComponent(context), m_bIsOnGround(false), m_bIsOnJump(true), m_InAirTime(0.0f)
{
	// Only the physics update event is needed: unsubscribe from the rest for optimization
	SetUpdateEventMask(USE_FIXEDUPDATE);

	m_bIsInJump = false;
}

void Character::RegisterObject(Context* context)
{
	context->RegisterFactory<Character>();

	// These macros register the class attributes to the Context for automatic load / save handling.
	// We specify the Default attribute mode which means it will be used both for saving into file, and network replication

	URHO3D_ATTRIBUTE("On Ground", bool, m_bIsOnGround, false, AM_DEFAULT);
	URHO3D_ATTRIBUTE("OK To Jump", bool, m_bIsOnJump, true, AM_DEFAULT);
	URHO3D_ATTRIBUTE("In Air Timer", float, m_InAirTime, 0.0f, AM_DEFAULT);
}

void Character::Initialize()
{
	m_pBody = GetComponent<RigidBody>();
	m_pAnimControl = GetComponent<AnimationController>();
}

void Character::InitializeAllDelegates()
{
	SubscribeToEvent(GetNode(), E_NODECOLLISION, URHO3D_HANDLER(Character, HandleNodeCollisionDelegate));
}

void Character::DestroyAllDelegates()
{
	UnsubscribeFromAllEvents();
}

void Character::Start()
{
	// Component has been inserted into its scene node. Subscribe to events now
	InitializeAllDelegates();


}

void Character::FixedUpdate(float timeStep)
{
	//m_pBody = GetComponent<RigidBody>();
	//m_pAnimControl = GetComponent<AnimationController>();

	if (m_pBody && m_pAnimControl)
	{
		// Update the in air timer. Reset if grounded
		if (!m_bIsOnGround)
		{
			m_InAirTime += timeStep;
		}
		else
		{
			m_InAirTime = 0.0f;
		}

		// When character has been in air less than 1/10 second, it's still interpreted as being on ground
		bool softGrounded = (m_InAirTime = 0.0f < INAIR_THRESHOLD_TIME) != 0;

		// Update movement & animation
		const Quaternion& rot = node_->GetRotation();
		//Vector3 moveDir = Vector3::ZERO;
		const Vector3& velocity = m_pBody->GetLinearVelocity();

		// Velocity on the XZ plane
		Vector3 planeVelocity(velocity.x_, 0.0f, velocity.z_);



		// Normalize move vector so that diagonal strafing is not faster
		if (m_MoveDir.LengthSquared() > 0.0f)
		{
			m_MoveDir.Normalize();
		}

		// If in air, allow control, but slower than when on ground
		m_pBody->ApplyImpulse(rot * m_MoveDir * (softGrounded ? MOVE_FORCE : INAIR_MOVE_FORCE));

		if (softGrounded)
		{
			// When on ground, apply a braking force to limit maximum ground velocity
			Vector3 brakeForce = -planeVelocity * BRAKE_FORCE;
			m_pBody->ApplyImpulse(brakeForce);

			// Jump. Must release jump control inbetween jumps
			if (m_bIsInJump)
			{
				if (m_bIsOnJump)
				{
					m_pBody->ApplyImpulse(Vector3::UP * JUMP_FORCE);
					m_bIsOnJump = false;
				}
			}
			else
			{
				m_bIsOnJump = true;
			}
		}

		// Play walk animation if moving on ground, otherwise fade it out
		if (softGrounded && !m_MoveDir.Equals(Vector3::ZERO))
		{
			m_pAnimControl->PlayExclusive("Characters/Jack/Animation/Jack_Walk.ani", 0, true, 0.2f);
		}
		else
		{
			m_pAnimControl->Stop("Characters/Jack/Animation/Jack_Walk.ani", 0.2f);
		}

		// Set walk animation speed proportional to velocity
		m_pAnimControl->SetSpeed("Characters/Jack/Animation/Jack_Walk.ani", planeVelocity.Length() * 2.0f);

		node_->SetRotation(Quaternion(m_Rotation.x_, Vector3::UP));

		// Reset grounded flag for next frame
		m_bIsOnGround = false;
	}
}

void Character::HandleNodeCollisionDelegate(StringHash eventType, VariantMap& eventData)
{
	// Check collision contacts and see if character is standing on ground (look for a contact that has near vertical normal)
	using namespace NodeCollision;

	MemoryBuffer contacts(eventData[P_CONTACTS].GetBuffer());

	while (!contacts.IsEof())
	{
		Vector3 contactPosition = contacts.ReadVector3();
		Vector3 contactNormal = contacts.ReadVector3();
		/*float contactDistance = */contacts.ReadFloat();
		/*float contactImpulse = */contacts.ReadFloat();

		// If contact is below node center and mostly vertical, assume it's a ground contact
		if (contactPosition.y_ < (node_->GetPosition().y_ + 1.0f))
		{
			float level = Abs(contactNormal.y_);
			if (level > 0.75)
			{
				m_bIsOnJump = true;
			}
		}
	}
}
