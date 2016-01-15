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

class ImGuiInterface;

// Settings class
struct ImGuiSettings
{
    friend class ImGuiInterface;

public:
    struct FontConfig
    {
        String filename_;
        float size_;
        Vector<ImWchar> glyphRanges_;
        ImFontConfig config_;
    };

    /// defaults to using the current window, the basic ImGui font and the dark theme
    ImGuiSettings();

    ///  Maximum time between saving positions/sizes to .ini file, in seconds.
    ImGuiSettings& IniSavingRate(float value);
    /// Ini Filename path.
    ImGuiSettings& IniFilename(const String& value);
    /// Log Filename path.
    ImGuiSettings& LogFilename(const String& value);
    /// Time for a double-click, in seconds.
    ImGuiSettings& MouseDoubleClickTime( float value);
    // Distance threshold to stay in to validate a double-click, in pixels.
    ImGuiSettings& MouseDoubleClickMaxDist(float value);
    // Distance threshold before considering we are dragging
    ImGuiSettings& MouseDragThreshold(float value);
    // When holding a key/button, time before it starts repeating, in seconds. (for actions where 'repeat' is active)
    ImGuiSettings& KeyRepeatDelay(float value);
    // When holding a key/button, rate at which it repeats, in seconds.
    ImGuiSettings& KeyRepeatRate(float value);
    // Global scale all fonts
    ImGuiSettings& FontGlobalScale(float value);
    // Allow user scaling text of individual window with CTRL+Wheel.
    ImGuiSettings& FontAllowUserScaling(bool value);
    // For retina display or other situations where window coordinates are different from framebuffer coordinates. User storage only, presently not used by ImGui.
    ImGuiSettings& DisplayFramebufferScale(const Vector2& value);
    // If you use DisplaySize as a virtual space larger than your screen, set DisplayVisibleMin/Max to the visible area.
    ImGuiSettings& DisplayVisibleMin(const Vector2& value);
    // If the values are the same, we defaults to Min=(0.0f) and Max=DisplaySize
    ImGuiSettings& DisplayVisibleMax(const Vector2& value);

    /// sets the font to use in ImGui
    ImGuiSettings& font(const String& fontPath, float size, bool MergeMode = false);
    /// sets the font config to use in ImGui
    /// @param PixelSnapH; // false    // Align every character to pixel boundary (if enabled, set OversampleH/V to 1)
    /// @param MergeMode; // false    // Merge into previous ImFont, so you can combine multiple inputs font into one ImFont (e.g. ASCII font + icons + Japanese glyphs).
    /// @param MergeGlyphCenterV; // false    // When merging (multiple ImFontInput for one ImFont), vertically center new glyphs instead of aligning their baseline
    /// @param OversampleH, OversampleV;   // 3, 1     // Rasterize at higher quality for sub-pixel positioning. We don't use sub-pixel positions on the Y axis.
    ImGuiSettings& fontConfig(const String& name, bool PixelSnapH = false, bool MergeMode = false, bool MergeGlyphCenterV= false,int OversampleH = 3, int OversampleV = 1);
    /// sets the font to use in ImGui
    ImGuiSettings& fontGlyphRanges(const String &name, const Vector<ImWchar> &glyphRanges);

    /// Global alpha applies to everything in ImGui
    ImGuiSettings& alpha(float a);
    /// Padding within a window
    ImGuiSettings& windowPadding(const Vector2 &padding);
    /// Minimum window size
    ImGuiSettings& windowMinSize(const Vector2 &minSize);
    /// Radius of window corners rounding. Set to 0.0f to have rectangular windows
    ImGuiSettings& windowRounding(float rounding);
    /// Alignment for title bar text
    ImGuiSettings& windowTitleAlign(ImGuiAlign  align);
    /// Radius of child window corners rounding. Set to 0.0f to have rectangular windows
    ImGuiSettings& childWindowRounding(float rounding);
    /// Padding within a framed rectangle (used by most widgets)
    ImGuiSettings& framePadding(const Vector2 &padding);
    /// Radius of frame corners rounding. Set to 0.0f to have rectangular frame (used by most widgets).
    ImGuiSettings& frameRounding(float rounding);
    /// Horizontal and vertical spacing between widgets/lines
    ImGuiSettings& itemSpacing(const Vector2 &spacing);
    /// Horizontal and vertical spacing between within elements of a composed widget (e.g. a slider and its label)
    ImGuiSettings& itemInnerSpacing(const Vector2 &spacing);
    /// Expand bounding box for touch-based system where touch position is not accurate enough (unnecessary for mouse inputs). Unfortunately we don't sort widgets so priority on overlap will always be given to the first widget running. So dont grow this too much!
    ImGuiSettings& touchExtraPadding(const Vector2 &padding);
    /// Default alpha of window background, if not specified in ImGui::Begin()
    ImGuiSettings& windowFillAlphaDefault(float defaultAlpha);
    /// Horizontal spacing when entering a tree node
    ImGuiSettings& indentSpacing(float spacing);
    /// Minimum horizontal spacing between two columns
    ImGuiSettings& columnsMinSpacing(float minSpacing);
    /// Width of the vertical scroll bar, Height of the horizontal scrollbar
    ImGuiSettings& scrollBarSize(float size);
    /// Radius of grab corners for scrollbar
    ImGuiSettings& scrollbarRounding(float rounding);
    /// Minimum width/height of a grab box for slider/scrollbar
    ImGuiSettings& grabMinSize(float minSize);
    /// Radius of grabs corners rounding. Set to 0.0f to have rectangular slider grabs.
    ImGuiSettings& grabRounding(float rounding);
    /// Window positions are clamped to be visible within the display area by at least this amount. Only covers regular windows.
    ImGuiSettings& displayWindowPadding(const Vector2 &padding);
    /// If you cannot see the edge of your screen (e.g. on a TV) increase the safe area padding. Covers popups/tooltips as well regular windows.
    ImGuiSettings& displaySafeAreaPadding(const Vector2 &padding);
    /// Enable anti-aliasing on lines/borders. Disable if you are really tight on CPU/GPU.
    ImGuiSettings& antiAliasedLines(bool antiAliasing);
    /// Enable anti-aliasing on filled shapes (rounded rectangles, circles, etc.)
    ImGuiSettings& antiAliasedShapes(bool antiAliasing);
    /// Tessellation tolerance. Decrease for highly tessellated curves (higher quality, more polygons), increase to reduce quality.
    ImGuiSettings& curveTessellationTol(float tessTolerance);

    /// sets imgui original theme
    ImGuiSettings& defaultTheme();
    /// sets the dark theme
    ImGuiSettings& darkTheme();
    /// sets theme colors
    ImGuiSettings& color(ImGuiCol  option, const Color &color);

    /// returns the list of available fonts to use in ImGui
    const HashMap< StringHash, FontConfig >& getFonts() const
    {
        return mFonts;
    }
    /// returns the window that will be use to connect the signals and render ImGui
    const ImGuiStyle& getStyle() const
    {
        return mStyle;
    }


protected:
    ImGuiStyle mStyle;
    HashMap< StringHash, FontConfig > mFonts;

    float mIniSavingRate;            // = 5.0f               // Maximum time between saving positions/sizes to .ini file, in seconds.
    String mIniFilename;              // = "imgui.ini"        // Path to .ini file. NULL to disable .ini saving.
    String mLogFilename;              // = "imgui_log.txt"    // Path to .log file (default parameter to ImGui::LogToFile when no file is specified).
    float mMouseDoubleClickTime;     // = 0.30f              // Time for a double-click, in seconds.
    float mMouseDoubleClickMaxDist;  // = 6.0f               // Distance threshold to stay in to validate a double-click, in pixels.
    float mMouseDragThreshold;       // = 6.0f               // Distance threshold before considering we are dragging
    float mKeyRepeatDelay;           // = 0.250f             // When holding a key/button, time before it starts repeating, in seconds. (for actions where 'repeat' is active)
    float mKeyRepeatRate;            // = 0.020f             // When holding a key/button, rate at which it repeats, in seconds.

    float mFontGlobalScale;          // = 1.0f               // Global scale all fonts
    bool mFontAllowUserScaling;     // = false              // Allow user scaling text of individual window with CTRL+Wheel.
    ImVec2 mDisplayFramebufferScale;  // = (1.0f,1.0f)        // For retina display or other situations where window coordinates are different from framebuffer coordinates. User storage only, presently not used by ImGui.
    ImVec2 mDisplayVisibleMin;        // <unset> (0.0f,0.0f)  // If you use DisplaySize as a virtual space larger than your screen, set DisplayVisibleMin/Max to the visible area.
    ImVec2 mDisplayVisibleMax;        // <unset> (0.0f,0.0f)  // If the values are the same, we defaults to Min=(0.0f) and Max=DisplaySize

};
