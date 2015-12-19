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

template <class Type>
SharedPtr<Type> MakeStrongPtr(WeakPtr<Type> pWeakPtr)
{
	if (!pWeakPtr.Expired())
		return SharedPtr<Type>(pWeakPtr);
	else
		return SharedPtr<Type>();
}

template <class Type>
WeakPtr<Type> MakeWeakPtr(SharedPtr<Type> pPtr)
{
	if (pPtr)
		return WeakPtr<Type>(pPtr);
	else
		return WeakPtr<Type>();
}