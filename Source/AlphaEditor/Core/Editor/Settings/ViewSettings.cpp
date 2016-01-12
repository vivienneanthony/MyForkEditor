#include "AlphaEditorStd.h"

#include "../../UI/UIGlobals.h"

#include "ViewSettings.h"


using namespace Urho3D;


// contstructor
ViewSettings::ViewSettings(Context* context) :Serializable(context)
{
    ResetToDefault();

    //URHO3D_LOGINFO(String(cameraBaseSpeed_));
}

// closing
ViewSettings::~ViewSettings()
{
}


// configurable settings for viewport
void ViewSettings::RegisterObject(Context* context)
{
    context->RegisterFactory<ViewSettings>();

    // camera movement speed
    URHO3D_ATTRIBUTE("cameraBaseSpeed_", float, cameraBaseSpeed_, 10.0f, AM_DEFAULT&&AM_FILE);
    URHO3D_ATTRIBUTE("cameraRotationSpeed_", float,cameraRotationSpeed_,.2f, AM_DEFAULT&&AM_FILE);
    URHO3D_ATTRIBUTE("cameraShiftSpeedMultiplier_", float, cameraShiftSpeedMultiplier_, 5.0f, AM_DEFAULT&&AM_FILE);

    //camera viewport
    URHO3D_ATTRIBUTE("viewNearClip_", float, viewNearClip_, 0.1f, AM_DEFAULT&&AM_FILE);
    URHO3D_ATTRIBUTE("viewFarClip_", float, viewFarClip_, 2000.0f, AM_DEFAULT&&AM_FILE);
    URHO3D_ATTRIBUTE("viewFov_", float, viewFov_, 45.0f, AM_DEFAULT&&AM_FILE);

	// move steps
    URHO3D_ATTRIBUTE("moveStep_", float, moveStep_, 0.5f, AM_DEFAULT&&AM_FILE);
    URHO3D_ATTRIBUTE("rotateStep_", float, rotateStep_, 5.0f, AM_DEFAULT&&AM_FILE);
    URHO3D_ATTRIBUTE("scaleStep_", float, scaleStep_, 0.1f, AM_DEFAULT&&AM_FILE);
    URHO3D_ATTRIBUTE("snapScale_", float, snapScale_, 1.0f, AM_DEFAULT&&AM_FILE);

}


// returns a variant map
VariantMap ViewSettings::ToVariantMap()
{
    const Vector<AttributeInfo>* attributes = GetAttributes();
    if (!attributes)
        return VariantMap();
    VariantMap variantMap;
    Variant value;

    for (unsigned i = 0; i < attributes->Size(); ++i)
    {
        const AttributeInfo& attr = attributes->At(i);
        if (!(attr.mode_ & AM_DEFAULT))
            continue;

        OnGetAttribute(attr, value);
        variantMap[attr.name_] = value;
    }
    return variantMap;
}



bool ViewSettings::SetFromVariantMap(const VariantMap & newSettings)
{
	return true;
}



