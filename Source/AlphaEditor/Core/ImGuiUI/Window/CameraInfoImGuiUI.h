#pragma onnce


class CameraInfoImGuiUI : public Object
{
    URHO3D_OBJECT(CameraInfoImGuiUI, Object);
public:
    CameraInfoImGuiUI(Context * context);
    virtual ~CameraInfoImGuiUI();

    static void ShowCameraInfo(const Node * NodeCamera);

    static bool initialized;
protected:
private:
};
