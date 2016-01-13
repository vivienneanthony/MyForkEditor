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

    float GetCameraBaseSpeed(void)
    {
        return cameraBaseSpeed_;
    }

    float GetCameraRotationSpeed(void)
    {
        return	cameraRotationSpeed_;
    }

    float GetCameraShiftSpeedMultiplier(void)
    {
        return cameraShiftSpeedMultiplier_;
    }


    float GetViewNearClip(void)
    {
        return viewNearClip_;

    }

    float GetViewFarClip(void)
    {
        return viewFarClip_;

    }

    float GetViewFov(void)
    {
        return viewFov_;
    }


    float GetMoveStep(void)
    {
        return moveStep_;
    }


    float GetRotateStep(void)
    {
        return rotateStep_;
    }


    float GetScaleStep(void)
    {
        return scaleStep_;
    }


    float GetSnapScale(void)
    {
        return snapScale_;
    }





private:    /// Parameters
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
