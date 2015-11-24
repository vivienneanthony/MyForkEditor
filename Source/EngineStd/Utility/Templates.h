#pragma once



template <class BaseType, class SubType>
BaseType* GenericObjectCreationFunction() { return new SubType(); }

template <class BaseClass, class IdType>
class GenericObjectFactory
{
	typedef BaseClass* (*ObjectCreationFunction)();
	HashMap<IdType, ObjectCreationFunction> m_CreationFunctions;

public:
	template <class SubClass>
	bool Register(IdType id)
	{
		auto findIt = m_CreationFunctions.Find(id);
		if (findIt == m_CreationFunctions.End())
		{
			m_CreationFunctions[id] = &GenericObjectCreationFunction<BaseClass, SubClass>;  // insert() is giving me compiler errors
			return true;
		}

		return false;
	}

	BaseClass* Create(IdType id)
	{
		auto findIt = m_CreationFunctions.Find(id);
		if (findIt != m_CreationFunctions.End())
		{
			ObjectCreationFunction pFunc = findIt->second_;
			return pFunc();
		}

		return NULL;
	}
};

