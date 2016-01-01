#include "EngineIDEStd.h"

#include "../../Editor.h"
#include "../../EditorData.h"
#include "../../View/EditorView.h"
#include "../../Selection/EditorSelection.h"
#include "../../Browser/ResourceBrowser.h"

#include "../../../UI/Picker/ResourcePicker.h"
#include "../../../UI/Menu/MenuBarUI.h"
#include "../../../UI/Hierarchy/HierarchyWindow.h"
#include "../../../UI/Tab/TabWindow.h"
#include "../../../UI/Attribute/AttributeContainer.h"
#include "../../../UI/Attribute/AttributeInspector.h"

#include "../../../Input/InputActionSystem.h"

#include "PluginScene3DEditor.h"
#include "../EditorPlugin.h"

//	const sInputAction AS_MOVEUP("S3D_MoveUp");
//	const sInputAction AS_MOVEDOWN("S3D_MoveDown");
sInputAction AS_MOVEFORWARD("S3D_MoveForward", Urho3D::KEY_UP);
sInputAction AS_MOVEBACKWARD("S3D_MoveBackward", Urho3D::KEY_DOWN);
sInputAction AS_MOVELEFT("S3D_MoveLeft", Urho3D::KEY_LEFT);
sInputAction AS_MOVERIGHT("S3D_MoveRight", Urho3D::KEY_RIGHT);

sInputAction AS_ROTHORI("S3D_RotHorizontal", 0, MOUSEB_RIGHT);
sInputAction AS_ROTVERT("S3D_RotVertical", 0, MOUSEB_RIGHT);

sInputAction AS_FASTER("S3D_FASTER", Urho3D::KEY_SHIFT);

PluginScene3DEditor::PluginScene3DEditor(Context* context) : EditorPlugin(context),
    initialized_(false),
    yaw_(0.0f),
    pitch_(0.0f)
{
}

PluginScene3DEditor::~PluginScene3DEditor()
{
}

void PluginScene3DEditor::RegisterObject(Context* context)
{
    context->RegisterFactory<PluginScene3DEditor>("EditorPlugin");
}

void PluginScene3DEditor::Enter()
{
}

void PluginScene3DEditor::Leave()
{
    UnsubscribeFromEvent(E_INPUTACTION);
}

bool PluginScene3DEditor::OnKeyInput(int key, bool down)
{
    return false;
}

bool PluginScene3DEditor::OnMouseButtonInput(int button, bool down)
{
    return false;
}

bool PluginScene3DEditor::HasMainFrame()
{
    return true;
}

void PluginScene3DEditor::Update(float delta)
{
    // Do not move if the UI has a focused element (the console)
    if (g_pApp->GetSubsystem<UI>()->GetFocusElement())
        return;
    float MOVE_SPEED = 20.0f;

    // Movement speed as world units per second
    if (AS_FASTER.IsActive())
        MOVE_SPEED = 60.0f;


    if (AS_ROTHORI.IsActive() && AS_ROTVERT.IsActive())
    {
        Input* input = GetSubsystem<Input>();
        // Mouse sensitivity as degrees per pixel
        const float MOUSE_SENSITIVITY = 0.1f;
        // Use this frame's mouse motion to adjust camera node yaw and pitch. Clamp the pitch between -90 and 90 degrees
        IntVector2 mouseMove = input->GetMouseMove();
        if (AS_ROTHORI.IsActive())
            yaw_ += MOUSE_SENSITIVITY * mouseMove.x_;

        if (AS_ROTVERT.IsActive())
        {
            pitch_ += MOUSE_SENSITIVITY * mouseMove.y_;
            pitch_ = Clamp(pitch_, -90.0f, 90.0f);
        }
    }
    // Construct new orientation for the camera scene node from yaw and pitch. Roll is fixed to zero
    cameraNode_->SetRotation(Quaternion(pitch_, yaw_, 0.0f));

    if (moveVector != Vector3::ZERO)
    {
        Vector3 moveVector_ = moveVector.Normalized();
        cameraNode_->Translate(moveVector_ * MOVE_SPEED * delta);
    }
}

void PluginScene3DEditor::HandleInputAction(StringHash eventType, VariantMap& eventData)
{
    using namespace InputAction;

    StringHash action = eventData[P_ACTIONID].GetStringHash();
    bool isdown = eventData[P_ISDOWN].GetBool();
    Input* input = GetSubsystem<Input>();
    if (isdown)
    {
        if (AS_MOVEFORWARD.id_ == action)
        {
            moveVector.z_ += 1.0f;
        }
        else if (AS_MOVEBACKWARD.id_ == action)
        {
            moveVector.z_ -= 1.0f;
        }
        else if (AS_MOVELEFT.id_ == action)
        {
            moveVector.x_ -= 1.0f;
        }
        else if (AS_MOVERIGHT.id_ == action)
        {
            moveVector.x_ += 1.0f;
        }

    }
    else
    {
        if (AS_MOVEFORWARD.id_ == action)
        {
            moveVector.z_ -= 1.0f;
        }
        else if (AS_MOVEBACKWARD.id_ == action)
        {
            moveVector.z_ += 1.0f;
        }
        else if (AS_MOVELEFT.id_ == action)
        {
            moveVector.x_ += 1.0f;
        }
        else if (AS_MOVERIGHT.id_ == action)
        {
            moveVector.x_ -= 1.0f;
        }

    }
}

