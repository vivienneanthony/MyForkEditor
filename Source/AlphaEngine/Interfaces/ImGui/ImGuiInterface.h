//
// Copyright (c) 2008-2015 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#pragma once

#include "ImGuiSettings.h"

struct ImGuiStyle;
struct ImDrawData;

class ImGuiSettings;


using namespace Urho3D;

// TODO: Input handler for Touch, Gesture, Joystick ?
//		 for now simple one touch input handling implemented.
// TODO: Replace font handling with Urho3d font ?
// TODO: imgui saves window positions and sizes to an ini file, use urho3d file system for that ?
class ImGuiInterface : public LogicComponent
{
    friend class Editor;
    friend class AlphaEngineApp;

    URHO3D_OBJECT(ImGuiInterface, LogicComponent);
public:
    ImGuiInterface(Context* context);
    virtual	~ImGuiInterface();

    /// Set ImGui Settings if already Initialize.
    void SetSettings(const ImGuiSettings &settings );

    /// Load Style from an XML file. Return true if successful.
    bool LoadStyleXML(Deserializer& source, ImGuiStyle* outStyle = NULL);
    /// Save Style to an XML file. Return true if successful.
    bool SaveStyleXML(Serializer& dest, ImGuiStyle* outStyle = NULL,const String& indentation = "\t") const;
    /// Load from XML data. Return true if successful.
    bool LoadStyleXML(const XMLElement& source, ImGuiStyle* outStyle = NULL, bool setInstanceDefault = false);
    /// Save as XML data. Return true if successful.
    bool SaveStyleXML(XMLElement& dest, ImGuiStyle* outStyle = NULL) const;

    /// Push font
    void PushFont(const String& name);
    /// Pop font
    void PopFont();

    /// Return whether focusing a text edit will show the on-screen keyboard.
    bool GetUseScreenKeyboard() const
    {
        return useScreenKeyboard_;
    }

    /// imgui render
    void RenderDrawLists(ImDrawData* data);

    /// Register
    static void RegisterObject(Context * context);

protected:
    /// Initialize when screen mode initially set.
    void Initialize();

    /// Shutdown.
    void Shutdown();
    /// Handle screen mode event.
    void HandleScreenMode(StringHash eventType, VariantMap& eventData);
    /// Handle frame begin event.
    void HandleBeginFrame(StringHash eventType, VariantMap& eventData);
    /// Handle end rendering event.
    void HandleEndRendering(StringHash eventType, VariantMap& eventData);
    /// Handle key event.
    void HandleKeyUp(StringHash eventType, VariantMap& eventData);
    /// Handle key event.
    void HandleKeyDown(StringHash eventType, VariantMap& eventData);
    /// Handle Text Input event.
    void HandleTextInput(StringHash eventType, VariantMap& eventData);
    /// Handle touch begin event.
    void HandleTouchBegin(StringHash eventType, VariantMap& eventData);
    /// Handle touch end event.
    void HandleTouchEnd(StringHash eventType, VariantMap& eventData);
    /// Handle touch move event.
    void HandleTouchMove(StringHash eventType, VariantMap& eventData);
    /// Initialized flag.
    bool initialized_;
    /// show debug ui menu bar.
    bool debugMenu_;
    /// Graphics subsystem.
    Graphics* graphics_;
    /// Input subsystem.
    Input* input_;
    /// screen size
    IntRect screenSize_;
    /// imgui font Texture
    Texture2D* fontTexture_;
    /// UI vertex buffer.
    SharedPtr<VertexBuffer> vertexBuffer_;
    /// UI index buffer.
    SharedPtr<IndexBuffer> indexBuffer_;
    /// Profiler timer.
    Timer profilerTimer_;
    /// Touch id
    int touchId_;
    /// Touch Position
    IntVector2 touchPos_;
    /// Touch
    bool touch_;
    /// Flag for showing the on-screen keyboard on focusing a text edit.
    bool useScreenKeyboard_;
    /// Map of used fonts.
    HashMap<StringHash, ImFont* > fonts_;
    /// ImGui settings.
    ImGuiSettings settings_;
    /// ini filename
    String iniFilename_;
    /// log filename
    String logFilename_;
private:
};
