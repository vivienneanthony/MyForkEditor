#include "EngineStd.h"

// Pickup components
#include "Components/Pickup/Ammo/AmmoComponent.h"
#include "Components/Pickup/Health/HealthComponent.h"

// GameAssets components
#include "Components/EngineObject/GameAssetEngineObject.h"
#include "Components/Source/GameAssetPowerSourceUnit.h"
#include "Components/LightUnit/GameAssetLightUnit.h"

// Engine specific assets
#include "Components/EngineCamera/GameAssetEngineCamera.h"
#include "Components/EngineLight/GameAssetEngineLight.h"
#include "Components/EngineSkybox/GameAssetEngineSkybox.h"

#include "GameAssetFactory.h"


GameAssetFactory::GameAssetFactory(Context* context_) : Object(context_)
    ,m_pGameAssetManager(NULL)
{
    // Set invalid game node flag
    m_LastGameNodeId = 10;

    // needed
    m_pGameAssetManager = NULL;

    // There should be registered game asset components for GameAssetFactory and Urho3D Factory.
    // Pickup
    m_ComponentFactory.Register<AmmoPickup>((unsigned int)AmmoPickup::g_Type);
    context_->RegisterFactory<AmmoPickup>();

    m_ComponentFactory.Register<HealthPickup>((unsigned int)HealthPickup::g_Type);
    context_->RegisterFactory<HealthPickup>();

    // Game Assets Components
    m_ComponentFactory.Register<GameAssetEngineObject>((unsigned int)GameAssetEngineObject::g_Type);
    context_->RegisterFactory<GameAssetEngineObject>();

    // Game Assets Engine Specific
    m_ComponentFactory.Register<GameAssetEngineLight>((unsigned int)GameAssetEngineLight::g_Type);
    context_->RegisterFactory<GameAssetEngineLight>();

    m_ComponentFactory.Register<GameAssetEngineCamera>((unsigned int)GameAssetEngineCamera::g_Type);
    context_->RegisterFactory<GameAssetEngineCamera>();

    m_ComponentFactory.Register<GameAssetEngineSkybox>((unsigned int)GameAssetEngineSkybox::g_Type);
    context_->RegisterFactory<GameAssetEngineSkybox>();


}

GameAssetFactory::~GameAssetFactory()
{

}

StrongNodePtr GameAssetFactory::CreateNode(const GameAsset* gameAsset, const GameNodeId serversId)
{
    GameNodeId nextGameNodeId = serversId;
    if (nextGameNodeId == INVALID_GAME_NODE_ID)
    {
        nextGameNodeId = GetNextGameNodeId();
    }

    StrongNodePtr pGameNode(new Node(context_));

    pGameNode->SetID(nextGameNodeId);

    ResourceCache* cache = g_pApp->GetConstantResCache();

    // Create root component
    // Loop through each game asset child element and load the component

    StrongComponentPtr component = VCreateComponent(gameAsset);

    if (component.NotNull())
    {
        // *ITISSCAN* 23.11.2015.
        // Not to good cast from GameAssetType structure to unsigned int...
        // Maybe in future better to make StringHash instead?
        pGameNode->SetID(nextGameNodeId);
        pGameNode->AddComponent(component, nextGameNodeId, Urho3D::CreateMode::LOCAL);

        // Initialize after it's added
        component->Initialize();
    }
    else
    {
        // If an error occurs, we kill the game node and bail. We could keep going, but the game node is will only be
        // partially complete so it's not worth it. Note that the pGameNode instance will be destroyed because it
        // will fall out of scope with nothing else pointing to it.
        return StrongNodePtr();
    }

    // Create childs components
    Vector<GameAsset*> childs = gameAsset->GetChilds();
    Vector<GameAsset*>::Iterator it = childs.Begin();
    for (; it != childs.End(); it++)
    {
        StrongComponentPtr component = VCreateComponent(gameAsset);

        if (component)
        {
            // *ITISSCAN* 23.11.2015.
            // Not to good cast from GameAssetType structure to unsigned int...
            // Maybe in future better to make StringHash instead?

            // If you want to know GameAssetTypeId.
            // Simply do substract abs(ComponentId - NodeId)
            pGameNode->SetID(nextGameNodeId);
            pGameNode->AddComponent(component, nextGameNodeId, component->GetCreateMode());

            // Initialize after it's added
            component->Initialize();
        }
        else
        {
            // If an error occurs, we kill the game node and bail. We could keep going, but the game node is will only be
            // partially complete so it's not worth it. Note that the pGameNode instance will be destroyed because it
            // will fall out of scope with nothing else pointing to it.
            return StrongNodePtr();
        }
    }

    // *ITISSCAN* 23.11.2015.
    // Not the best solution.
    // Send POST INIT event to game assets, in order to finish initialization properly.
    // May be some components depend on others.
    // In future we should to override Urho3D::Node class and implement our GameNode class.

    SendEvent("Game_Asset_Factory_Post_Init");

    return pGameNode;
}

void GameAssetFactory::ModifyNode(StrongNodePtr node, const GameAsset* gameAsset)
{


}

// Used to modify node attributes from pgi XMl - Modify Node
void GameAssetFactory::ModifyNode(StrongNodePtr pChildNode, const GameAsset* gameAsset, const pugi::xml_node GameAssetChild)
{
    // Use additional flags based on type
    if(gameAsset->GetAssetType()==GAType_EngineLight)
    {
        // Available additional flags
        float SetBrightness = GameAssetChild.attribute("Brightness").as_float();
        float SetSpecular = GameAssetChild.attribute("Specular").as_float();
        float SetRange = GameAssetChild.attribute("Range").as_float();
        float SetFov = GameAssetChild.attribute("Fov").as_float();
        float SetLookAtXPos = GameAssetChild.attribute("LookAtX").as_float();
        float SetLookAtYPos = GameAssetChild.attribute("LookAtY").as_float();
        float SetLookAtZPos = GameAssetChild.attribute("LookAtZ").as_float();
        unsigned int SetViewMask = GameAssetChild.attribute("ViewMask").as_uint();
        unsigned int SetLightMask = GameAssetChild.attribute("LightMask").as_uint();
        unsigned int SetShadowMask = GameAssetChild.attribute("ShadowMask").as_uint();
        unsigned int SetZoneMask = GameAssetChild.attribute("ZoneMask").as_uint();
        unsigned int SetCastShadows = GameAssetChild.attribute("CastShadows").as_uint();

        // Check if Light component actually exist
        GameAssetEngineLight * EngineLight = (GameAssetEngineLight *) pChildNode->GetComponent("GameAssetEngineLight");

        // If game asset has a Engine Light component
        if(EngineLight)
        {
            if(SetBrightness)
            {
                EngineLight->SetBrightness(SetBrightness);
            }
            if(SetSpecular)
            {
                EngineLight->SetSpecularIntensity(SetSpecular);
            }
            if(SetRange)
            {
                EngineLight->SetRange(SetRange);
            }
            if(SetFov)
            {
                EngineLight->SetFOV(SetFov);
            }
            if(SetLookAtXPos!=0.0f||SetLookAtYPos!=0.0f||SetLookAtZPos!=0.0f)
            {
                EngineLight->SetLookAt(Vector3(SetLookAtXPos,SetLookAtYPos,SetLookAtZPos));
            }
             if(SetViewMask)
            {
                EngineLight->SetViewMask(SetViewMask);
            }
            if(SetLightMask)
            {
                EngineLight->SetLightMask(SetLightMask);
            }
            if(SetShadowMask)
            {
                EngineLight->SetShadowMask(SetShadowMask);
            }
            if(SetZoneMask)
            {
                EngineLight->SetZoneMask(SetZoneMask);
            }
        }
    }

    // Use additional flags based on type
    if(gameAsset->GetAssetType() == GAType_EngineObject)
    {
        // Available additional flags
        unsigned int SetViewMask = GameAssetChild.attribute("ViewMask").as_uint();
        unsigned int SetLightMask = GameAssetChild.attribute("LightMask").as_uint();
        unsigned int SetShadowMask = GameAssetChild.attribute("ShadowMask").as_uint();
        unsigned int SetZoneMask = GameAssetChild.attribute("ZoneMask").as_uint();
        
		unsigned int value = GameAssetChild.attribute("CastShadows").as_uint();
		bool SetCastShadows = ((value >= 1) ? (true) : (false));
        float SetScale = GameAssetChild.attribute("Scale").as_float();

        // Check if Light component actually exist
        GameAssetEngineObject * EngineObject = (GameAssetEngineObject *) pChildNode->GetComponent("GameAssetEngineObject");

        // If game asset has a Engine Light component
        if(EngineObject)
        {
            if(SetViewMask)
            {
                EngineObject->SetViewMask(SetViewMask);
            }
            if(SetLightMask)
            {
                EngineObject->SetLightMask(SetLightMask);
            }
            if(SetShadowMask)
            {
                EngineObject->SetShadowMask(SetShadowMask);
            }
            if(SetZoneMask)
            {
                EngineObject->SetZoneMask(SetZoneMask);
            }
            if(SetCastShadows)
            {
                EngineObject->SetCastShadow(SetCastShadows);
            }

            if(SetScale)
            {
                EngineObject->SetScale(SetScale);
            }
        }
    }

     // Use additional flags based on type
    if(gameAsset->GetAssetType()==GAType_EngineSkybox)
    {
        // Available additional flags
        float SetScale = GameAssetChild.attribute("Scale").as_float();

        // Check if Light component actually exist
        GameAssetEngineSkybox * EngineSkybox = (GameAssetEngineSkybox *) pChildNode->GetComponent("GameAssetEngineSkybox");

        // If game asset has a Engine Light component
        if(EngineSkybox)
        {

            if(SetScale)
            {
                EngineSkybox->SetScale(SetScale);
            }
        }
    }

    return;
}

// Create component
StrongComponentPtr GameAssetFactory::VCreateComponent(const GameAsset* gameAsset)
{
    GameAssetType GA_Type = gameAsset->GetAssetType();

    StrongComponentPtr pComponent(m_ComponentFactory.Create((unsigned int)GA_Type));

    // initialize the component if we found one
    if (pComponent)
    {
        if (!pComponent->VInit(gameAsset))
        {
            URHO3D_LOGDEBUG("Component failed to initialize: " + String((unsigned int)GA_Type));
            return StrongComponentPtr();
        }
    }
    else
    {
        URHO3D_LOGDEBUG("Couldn't find GameAssetComponent id " + String((unsigned int)GA_Type));
        return StrongComponentPtr();
    }

    // pComponent will be NULL if the component wasn't found.
    // This isn't necessarily an error since you might have a
    // custom CreateComponent() function in a sub class.
    // For now it is ERROR.

    return pComponent;
}

// createnode
StrongNodePtr GameAssetFactory::CreateNodeRecursive(const GameAsset* gameAsset, const GameNodeId serversId, Node * node = NULL, bool recursive = true)
{
    ResourceCache* cache = g_pApp->GetConstantResCache();

    GameNodeId nextGameNodeId = serversId;


    if (nextGameNodeId == INVALID_GAME_NODE_ID)
    {
        nextGameNodeId = GetNextGameNodeId();
    }

    StrongNodePtr pGameNode(new Node(context_));

    pGameNode->SetID(nextGameNodeId);

    // Create root component then create a component type
    StrongComponentPtr component = VCreateComponent(gameAsset);

    // If component creation failed exit
    if (component.NotNull())
    {
        // *ITISSCAN* 23.11.2015.
        // Not to good cast from GameAssetType structure to unsigned int... Maybe in future better to make StringHash instead?

        pGameNode->SetID(nextGameNodeId);
        pGameNode->AddComponent(component, nextGameNodeId, component->GetCreateMode());

        // Initialize after it's added
        component->Initialize();

        // Is Physical
        if(gameAsset->IsPhysical())
        {
            // Create a model and string
            String ModelFile = String("GameData/Models/")+gameAsset->GetPhysicalModel()+String(".mdl");

            // create a static model
            StaticModel* m_pGameNodeModel = pGameNode->CreateComponent<StaticModel>();
            m_pGameNodeModel->SetModel(cache->GetResource<Model>(ModelFile));
        }
    }
    else
    {
        // If an error occurs, we kill the game node and bail. We could keep going, but the game node is will only be
        // partially complete so it's not worth it. Note that the pGameNode instance will be destroyed because it
        // will fall out of scope with nothing else pointing to it.
        return StrongNodePtr();
    }

    // Recursive is always default to true - Makes sure first run of the function
    if(recursive)
    {
        // Create childs components
        Vector<GameAsset*> childs = gameAsset->GetChilds();
        Vector<GameAsset*>::Iterator it = childs.Begin();

        // Loop through each game asset child element and load the component
        for (Vector<GameAsset*>::Iterator it = childs.Begin(); it != childs.End(); it++)
        {
            // if it isn't a linked asset
            if((*it)->IsLinkedGameAsset() == false)
            {
                StrongComponentPtr component = VCreateComponent((*it));

                if (component.NotNull())
                {
                    // *ITISSCAN* 23.11.2015.
                    // Not to good cast from GameAssetType structure to unsigned int...
                    // Maybe in future better to make StringHash instead?
                    pGameNode->SetID(nextGameNodeId);
                    pGameNode->AddComponent(component, nextGameNodeId, component->GetCreateMode());

                }
                else
                {
                    // If an error occurs, we kill the game node and bail. We could keep going, but the game node is will only be
                    // partially complete so it's not worth it. Note that the pGameNode instance will be destroyed because it
                    // will fall out of scope with nothing else pointing to it.
                    return StrongNodePtr();
                }
            }
            else
            {
                // Check if a Game Asset Manager is set to find linked game asset by symbol
                if(m_pGameAssetManager)
                {
                    // If linked game asset is found using the symbol
                    // The create a child node for it
                    GameAsset* pLinkedGameAsset = NULL;
                    pLinkedGameAsset = m_pGameAssetManager->FindGameAssetBySymbol((*it)->GetSymbol());

                    if(pLinkedGameAsset)
                    {
                        // Build a new child using game asset linked
                        // make recursive to false stopping a child creation
                        StrongNodePtr childNode = CreateNodeRecursive(pLinkedGameAsset, serversId, pGameNode, false);
                        if(childNode.NotNull())
                        {

                            // *ITISSCAN* may be add it to the root node as child ?
                            pGameNode->AddChild(childNode);

                            // Make any adjustments to created node
                            // For example positioning

                        }
                        else
                        {
                            // Problem occurred creating the child node
                            // Error leave node creation
                            return StrongNodePtr();
                        }
                    }
                    else
                    {
                        // Symbol was not found for linked game asset - Error occurred exit
                        // Error leave node creation
                        return StrongNodePtr();
                    }
                }
                else
                {
                    // The Game Asset Manager was not found so it could not complete building the node
                    // Error leave node creation
                    return StrongNodePtr();
                }
            }
        }
    }

    // send int
    SendEvent("Game_Asset_Factory_Post_Init");

    return pGameNode;
}


StrongNodePtr GameAssetFactory::CreateNodeEmpty(const GameNodeId serversId)
{
    GameNodeId nextGameNodeId = serversId;
    if (nextGameNodeId == INVALID_GAME_NODE_ID)
    {
        nextGameNodeId = GetNextGameNodeId();
    }

    StrongNodePtr pGameNode(new Node(context_));

    pGameNode->SetID(nextGameNodeId);

    SendEvent("Game_Asset_Factory_Post_Init");

    return pGameNode;
}
