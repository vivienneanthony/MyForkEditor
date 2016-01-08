#include "AlphaEngineStd.h"
#include "BaseSocketManager.h"

BaseSocketManager* g_pSocketManager = nullptr;

BaseSocketManager::BaseSocketManager(Context* context) : Object(context)
{
	m_pNetwork = GetSubsystem<Network>();
	m_Inbound = 0;
	m_Outbound = 0;
	m_MaxOpenSockets = 0;
	m_SubnetMask = 0;
	m_Subnet = 0xffffffff;
	m_NextSocketId = 0;

	g_pSocketManager = this;
}

BaseSocketManager::~BaseSocketManager()
{
	Shutdown();
}

bool BaseSocketManager::Init()
{
	VInitializeDelegates();
	return true;
}


void BaseSocketManager::Shutdown()
{
	SocketList::Iterator it = m_SocketsList.Begin();
	while (!m_SocketsList.Empty())
	{
		SAFE_DELETE(*it);
		it = m_SocketsList.Erase(it);
	}

	VDestroyAllDelegates();
}

int BaseSocketManager::AddSocket(NetSocket* socket)
{
	socket->m_UniqueId = m_NextSocketId;
	m_SocketsMap[m_NextSocketId] = socket;
	++m_NextSocketId;
	m_SocketsList.Push(socket);
	if (m_SocketsList.Size() > m_MaxOpenSockets)
	{
		m_MaxOpenSockets++;
	}

	return socket->m_UniqueId;
}

void BaseSocketManager::RemoveSocket(NetSocket* socket)
{
	m_SocketsList.Erase(m_SocketsList.Find(socket));
	m_SocketsMap.Erase(socket->m_UniqueId);
	SAFE_DELETE(socket);
}

bool BaseSocketManager::Send(unsigned int sockId, SharedPtr<BasePacket> packet)
{
	NetSocket* sock = FindSocket(sockId);
	if (!sock)
	{
		return false;
	}
	sock->Send(packet);

	return true;
}

NetSocket* BaseSocketManager::FindSocket(unsigned int sockId)
{
	SocketIdMap::Iterator it = m_SocketsMap.Find(sockId);
	if (it == m_SocketsMap.End())
	{
		return nullptr;
	}

	return (*it).second_;
}

void BaseSocketManager::DoSelect(bool handleInput)
{

	for (SocketList::Iterator it = m_SocketsList.Begin(); it != m_SocketsList.End(); it++)
	{
		if ((*it)->VHasOutput())
		{
			(*it)->VHandleOutput();
		}

		if (handleInput)
		{
			(*it)->VHandleInput();
		}
	}

	SocketList::Iterator it = m_SocketsList.Begin();
	while (it != m_SocketsList.End())
	{
		if ((*it)->m_DeleteFlags)
		{
			g_pSocketManager->RemoveSocket((*it));
		}
		it++;
	}
}


// -------------- DELEGATES -----------------


void BaseSocketManager::VInitializeDelegates()
{

}

void BaseSocketManager::VDestroyAllDelegates()
{

}