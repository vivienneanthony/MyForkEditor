#pragma onnce


class CameraInfoImGuiUI : public Object
{
    URHO3D_OBJECT(CameraInfoImGuiUI, Object);
public:
    CameraInfoImGuiUI(Context * context);
    virtual ~CameraInfoImGuiUI();

    static bool * ShowCameraInfo(const Node * NodeCamera);


protected:
private:
  static bool initialized;
};
