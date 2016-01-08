#pragma once

struct MaterialData
{
	MaterialData()
	{
		m_Restitution = 0;
		m_Friction = 0;
	}

	MaterialData(float restitution, float friction)
	{
		m_Restitution = restitution;
		m_Friction = friction;
	}

	MaterialData(const MaterialData& other)
	{
		m_Restitution = other.m_Restitution;
		m_Friction = other.m_Friction;
	}

	float m_Restitution;
	float m_Friction;
};