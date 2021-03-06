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


/// Message box dialog.
class ModalWindow : public Object
{
    URHO3D_OBJECT(ModalWindow, Object);

public:
    /// Construct. If layout file is not given, use the default message box layout. If style file is not given, use the default style file from root UI element.
    ModalWindow(Context* context, UIElement* content = NULL, const String& titleString = String::EMPTY, XMLFile* layoutFile = 0, XMLFile* styleFile = 0);
    /// Destruct.
    virtual ~ModalWindow();
    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Set title text. No-ops if there is no title text element.
    void SetTitle(const String& text);
    /// Set Content Container.
    void SetContent(UIElement* content);

    /// Return title text. Return empty string if there is no title text element.
    const String& GetTitle() const;
    /// Return dialog window.
    UIElement* GetWindow() const
    {
        return window_;
    }
    /// Return Content Container.
    UIElement* GetContentContainer() const;
    /// Return Content.
    UIElement* GetContent() const
    {
        return content_;
    }
private:
    /// Handle events that dismiss the message box.
    void HandleMessageAcknowledged(StringHash eventType, VariantMap& eventData);

    /// UI element containing the whole UI layout. Typically it is a Window element type.
    SharedPtr<UIElement> window_;
    /// Title text element.
    Text* titleText_;
    /// Content Container element.
    SharedPtr<UIElement> contentContainer_;
    /// Content element.
    SharedPtr<UIElement> content_;
    /// OK button element.
    Button* okButton_;
};

