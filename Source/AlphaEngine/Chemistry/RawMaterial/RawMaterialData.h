#pragma once

struct RawMaterialData
{
	RawMaterialData()
	{
		m_ElementName = String::EMPTY;
		m_Symbol = String::EMPTY;
		m_Density = 0.0f;
	}

	RawMaterialData(String elementName, String symbol, float density)
	{
		m_ElementName = elementName;
		m_Symbol = symbol;
		m_Density = density;
	}

	RawMaterialData(const RawMaterialData& other)
	{
		m_ElementName = other.m_ElementName;
		m_Symbol = other.m_Symbol;
		m_Density = other.m_Density;
	}

	String m_ElementName;
	String m_Symbol;
	float m_Density;
};