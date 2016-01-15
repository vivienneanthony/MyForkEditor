#pragma once

using  namespace Urho3D;

// ImGui Interface for the editor
class ImGuiInterface : public Object
{
    URHO3D_OBJECT(ImGuiInterface, Object);
public:
    // Constructors
    ImGuiInterface(Context *context);
    virtual ~ImGuiInterface();

    // Ex. Main menu view doesn't have any actors.
    virtual void VOnAttach(GameViewId vid, GameNodeId aid);
    // Initialize humam view.
    virtual bool VOnRestore();
    // Lost device/input.
    virtual bool VOnLostDevice();
    // Update view.
    virtual void VOnUpdate(float timeStep);
    // Handle input message from devices
    virtual bool VOnMsgProc(AppMsg message);
    // Shutdown view.
    virtual void VShutdown();

    // REgister
    static void RegisterObject(Context * context);

    // Initialize
    bool Initialize(void);

    // Set ImGui Settings if already Initialize.
    void SetSettings(const ImGuiSettings &settings );

    // Save Style to an XML file. Return true if successful.
    bool SaveStyleXML(Serializer& dest, ImGuiStyle* outStyle = NULL,const String& indentation = "\t") const;
    // Save as XML data. Return true if successful.
    bool SaveStyleXML(XMLElement& dest, ImGuiStyle* outStyle = NULL) const;
    // Load Style from an XML file. Return true if successful.
    bool LoadStyleXML(Deserializer& source, ImGuiStyle* outStyle = NULL);
    // Load from XML data. Return true if successful.
    bool LoadStyleXML(const XMLElement& source, ImGuiStyle* outStyle = NULL, bool setInstanceDefault = false);

    /// imgui render
    void RenderDrawLists(ImDrawData* data);


    /// Push font
    void PushFont(const String& name);
    /// Pop font
    void PopFont();

    /// Handle screen mode event.
    void HandleScreenMode(StringHash eventType, VariantMap& eventData);
    /// Handle frame begin event.
    void HandleBeginFrame(StringHash eventType, VariantMap& eventData);
    /// Handle end rendering event.
    void HandleEndRendering(StringHash eventType, VariantMap& eventData);


private:

    WeakPtr<ImGuiInterface> m_pImGuiInterface;

    // Gui Settings
    ImGuiSettings GuiSettings;

    // Initialized State
    bool bInitialized;

    // Map of used fonts.
    HashMap<StringHash, ImFont* > fonts_;

    // ini filename
    String iniFilename;

    // log filename
    String logFilename;

    // screen size
    IntRect screenSize;

    // imgui font Texture
    Texture2D* fontTexture;

    // Subsystemssss
    SharedPtr<Graphics> m_pGraphics;
    SharedPtr<Renderer> m_pRenderer;
    SharedPtr<Input> input_;


    SharedPtr<VertexBuffer> pVertexBuffer;
    SharedPtr<IndexBuffer> pIndexBuffer;

};
