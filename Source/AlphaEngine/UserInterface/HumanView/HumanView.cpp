#include "AlphaEngineStd.h"
#include "GameLogic/BaseGameLogic.h"
#include "Mainloop/Activity/ActivityManager.h"
#include "Audio/Audio.h"
#include "Resources/ResourceCache.h"
#include "Resources/Loading.h"
#include "HumanView.h"

#include "../Elements/LetterBox.h"

// ----------------------------------------------------------
// Constructors.
// ----------------------------------------------------------


const GameViewId INVALID_GAME_VIEW_ID = -1;

HumanView::HumanView(Context *context, Renderer* renderer) : IGameView(context)
{
    m_pRenderer = renderer ? renderer :  nullptr;
    InitAudio();
    if (m_pRenderer)
    {
        m_pRenderer->AddRef();
    }

    m_pKeyboardHandler.Reset();
    m_pPointerHandler.Reset();

    m_PointerRadius = 30;
    m_pActivityManager = new ActivityManager(context);

    VInitializeAllDelegates();
}

HumanView::~HumanView()
{
    SAFE_DELETE(m_pActivityManager);

    if (m_pRenderer)
    {
        m_pRenderer->ReleaseRef();
    }

    VDestroyAllDelegates();
}

// ----------------------------------------------------------
// Initializtion, update, shutdown.
// ----------------------------------------------------------

void HumanView::VOnAttach(GameViewId vid, GameNodeId aid)
{
    m_ViewId = vid;
    m_GameNodeId= aid;
}

bool HumanView::VOnRestore()
{
    for (ScreenElementList::Iterator i = m_ScreenElements.Begin(); i != m_ScreenElements.End(); ++i)
    {
        (*i)->VOnRestore();
    }

    return true;
}

bool HumanView::VOnLostDevice()
{
    for (ScreenElementList::Iterator i = m_ScreenElements.Begin(); i != m_ScreenElements.End(); ++i)
    {
        (*i)->VOnLostDevice();
    }

    return true;
}

void HumanView::VOnUpdate(float timeStep)
{
    m_pActivityManager->UpdateActivities(timeStep);

    // This section of code was added post-press. It runs through the screenlist
    // and calls VOnUpdate. Some screen elements need to update every frame, one
    // example of this is a 3D scene attached to the human view.
    for (ScreenElementList::Iterator i = m_ScreenElements.Begin(); i != m_ScreenElements.End(); ++i)
    {
        (*i)->VOnUpdate(timeStep);
    }

}

bool HumanView::VOnMsgProc(AppMsg message)
{
    // Iterate through the screen layers first
    // In reverse order since we'll send input messages to the
    // screen on top
    for (ScreenElementList::Iterator i = m_ScreenElements.Begin(); i != m_ScreenElements.End(); ++i)
    {
        if ((*i)->VIsVisible())
        {
            if ((*i)->VOnMsgProc(message))
            {
                return 1;
            }
        }
    }

    bool result = false;

    switch (message.uEvent)
    {
    case APP_EVENT::KEY_DOWN:
    {
        if (m_pKeyboardHandler.NotNull())
        {
            result = m_pKeyboardHandler->VOnKeyDown(message.lParam.m_keycode);
        }
    }
    break;

    case APP_EVENT::KEY_UP:
    {
        if (m_pKeyboardHandler.NotNull())
        {
            result = m_pKeyboardHandler->VOnKeyUp(message.lParam.m_keycode);
        }
    }
    break;

    case MOUSE_MOVE:
    {
        if (m_pPointerHandler.NotNull())
        {
            result = m_pPointerHandler->VOnPointerMove(message.lParam.m_position, 1);
        }
    }
    break;

    case MOUSE_LBUTTONDOWN:
    {
        if (m_pPointerHandler.NotNull())
        {
            result = m_pPointerHandler->VOnPointerButtonDown(message.lParam.m_position, 1, "PointerLeft");
        }
    }
    break;

    case MOUSE_LBUTTONUP:
    {
        if (m_pPointerHandler.NotNull())
        {
            result = m_pPointerHandler->VOnPointerButtonUp(message.lParam.m_position, 1, "PointerLeft");
        }
    }
    break;

    case MOUSE_RBUTTONDOWN:
    {
        if (m_pPointerHandler.NotNull())
        {
            result = m_pPointerHandler->VOnPointerButtonDown(message.lParam.m_position, 1, "PointerRight");
        }
    }
    break;

    case MOUSE_RBUTTONUP:
    {
        if (m_pPointerHandler.NotNull())
        {
            result = m_pPointerHandler->VOnPointerButtonUp(message.lParam.m_position, 1, "PointerRight");
        }
    }
    break;

    default:
        return false;
    }
    return result;
}

void HumanView::VShutdown()
{
    if (g_pAudio)
    {
        if (m_pRenderer)
        {
            g_pAudio->VShutdown();
            SAFE_DELETE(g_pAudio);
        }
    }
}


void HumanView::VPushElement(SharedPtr<IScreenElement> pElement)
{
    m_ScreenElements.PushFront(pElement);
}

void HumanView::VRemoveElement(SharedPtr<IScreenElement> pElement)
{
    ScreenElementList::Iterator it = m_ScreenElements.Find(pElement);
    if (it->NotNull())
    {
        m_ScreenElements.Erase(it);
    }
}

bool HumanView::LoadGame(pugi::xml_node pLevelData)
{
    // call the delegate method
	return VLoadGameDelegate(pLevelData);
}


bool HumanView::VLoadGameDelegate(pugi::xml_node pLevelData)
{
	// Default action. Connect HumanView's scene with GameLogic's scene.
	m_pScene = g_pApp->GetGameLogic()->GetScene();
	
	// Create the camera (not included in the scene file)
	m_pCameraNode = m_pScene->GetChild("MainCamera");
	if (m_pCameraNode == nullptr)
	{
		m_pCameraNode = m_pScene->CreateChild("MainCamera");
		m_pCameraNode->CreateComponent<Camera>();

		// Set an initial position for the camera scene node above the plane
		m_pCameraNode->SetPosition(Vector3(0.0f, 10.0f, 0.0f));
	}

	m_pViewport = SharedPtr<Viewport>(new Viewport(context_, m_pScene, m_pCameraNode->GetComponent<Camera>()));
	m_pRenderer->SetViewport(1, m_pViewport);
    return true;
}



bool HumanView::InitAudio()
{
    if (m_pRenderer)
    {
        if (g_pAudio)
        {
            return true;
        }


        if (g_pAudio)
        {
            return g_pAudio->VInitialize();
        }
        else
        {
            return false;
        }

        return true;
    }

    return false;
}

void HumanView::VInitializeAllDelegates()
{


}

void HumanView::VDestroyAllDelegates()
{
    UnsubscribeFromAllEvents();

}
