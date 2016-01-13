#pragma once

/*!
 * \file ViewSettings.h
 *
 * \author vitali
 * \date Februar 2015
 *
 *
 */


struct SettingMap
{
    String Name;
    Variant Value;
};

class ViewSettings : public Serializable
{
    URHO3D_OBJECT(ViewSettings,Serializable);
public:
    ViewSettings(Context* context);
    virtual ~ViewSettings();

    static void		RegisterObject(Context* context);
    virtual bool	SaveDefaultAttributes() const
    {
        return true;
    }

    VariantMap	ToVariantMap();

    bool SetFromVariantMap(Vector<SettingMap> newSettingsnewSettings);

private:
    /// Parameters
    float	cameraBaseSpeed_;
    float	cameraRotationSpeed_;
    float	cameraShiftSpeedMultiplier_;
    float	viewNearClip_;
    float	viewFarClip_;
    float   viewFov_;
    float	moveStep_;
    float	rotateStep_;
    float	scaleStep_;
    float	resourcePackages_;
    float	snapScale_;
};
