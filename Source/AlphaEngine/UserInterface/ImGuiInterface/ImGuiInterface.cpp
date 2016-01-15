#include <AlphaEngineStd.h>



#include <Urho3D/Core/StringUtils.h>

#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/GraphicsDefs.h>
#include <Urho3D/Graphics/VertexBuffer.h>
#include <Urho3D/Graphics/IndexBuffer.h>
#include <Urho3D/Resource/ResourceCache.h>



// Load Imgui Related
#include <Urho3D/ThirdParty/imgui/imgui.h>

#include "ImGuiSettings.h"
#include "ImGuiInterface.h"


ImVec2 ToImVec2(const String& source)
{
    ImVec2 ret;

    unsigned elements = source.Split(' ').Size();
    if (elements < 2)
        return ret;

    char* ptr = (char*)source.CString();
    ret.x = (float)strtod(ptr, &ptr);
    ret.y = (float)strtod(ptr, &ptr);

    return ret;
};

ImVec4 ToImVec4(const String& source)
{
    ImVec4 ret;

    unsigned elements = source.Split(' ').Size();
    if (elements < 4)
        return ret;

    char* ptr = (char*)source.CString();
    ret.x = (float)strtod(ptr, &ptr);
    ret.y = (float)strtod(ptr, &ptr);
    ret.z = (float)strtod(ptr, &ptr);
    ret.w = (float)strtod(ptr, &ptr);

    return ret;
};

String ToString(const ImVec2& vec2)
{
    char tempBuffer[256];
    sprintf(tempBuffer, "%.g %.g", vec2.x, vec2.y);
    return String(tempBuffer);
};

String ToString(const ImVec4& vec4)
{
    char tempBuffer[256];
    sprintf(tempBuffer, "%.g %.g %.g %.g", vec4.x, vec4.y, vec4.z, vec4.w);
    return String(tempBuffer);
};


static const char* ImGui_GetClipboardText()
{
    return SDL_GetClipboardText();
};

static void ImGui_SetClipboardText(const char* text)
{
    SDL_SetClipboardText(text);
};



using namespace Urho3D;

static ImGuiInterface * m_pImGuiInterface = NULL;

static void ImGui_RenderDrawLists(ImDrawData* data)
{
    if (m_pImGuiInterface)
    {
        m_pImGuiInterface->RenderDrawLists(data);
    }
};

// Constructor
ImGuiInterface::ImGuiInterface(Context * context):Object(context)
   , bInitialized(false)
    ,iniFilename(String::EMPTY)
    ,logFilename(String::EMPTY)
    ,screenSize(IntRect::ZERO)
    ,fontTexture(NULL)
    ,m_pRenderer(NULL)
    ,m_pGraphics(NULL)
    ,pVertexBuffer(NULL)
    ,pIndexBuffer(NULL)
{

    assert(!m_pImGuiInterface);

    // set interface to this
    m_pImGuiInterface = this;
    bInitialized = false;

    // get grpahics system
    m_pRenderer = g_pApp->GetRenderer();
    m_pGraphics = g_pApp->GetGraphics();

    //m_pFileSystem = g_pApp->GetFileSystem();

    Initialize();

    return;
}

// Deconstructor
ImGuiInterface::~ImGuiInterface(void)
{
    // set interface to null
    m_pImGuiInterface = NULL;

    return;
}

// RegisterObject
void ImGuiInterface::RegisterObject(Context * context)
{
    context->RegisterFactory<ImGuiInterface>();
    return;
}

// Initialize
bool ImGuiInterface::Initialize(void)
{
    // Verify a renderer and graphics system
    if(m_pRenderer==NULL||m_pGraphics==NULL||bInitialized)
    {
        return false;
    }

    // Get Screen Dimensions
    screenSize = IntRect(0, 0, m_pGraphics->GetWidth(), m_pGraphics->GetHeight());
    pVertexBuffer = new VertexBuffer(context_);
    pIndexBuffer = new IndexBuffer(context_);

    /// init imgui
    ImGuiIO& io = ImGui::GetIO();

    io.UserData = (void*) this;

    io.KeyMap[ImGuiKey_Tab] = SCANCODE_TAB;                 // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array.
    io.KeyMap[ImGuiKey_LeftArrow] = SCANCODE_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = SCANCODE_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = SCANCODE_UP;
    io.KeyMap[ImGuiKey_DownArrow] = SCANCODE_DOWN;
    io.KeyMap[ImGuiKey_Home] = SCANCODE_HOME;
    io.KeyMap[ImGuiKey_End] = SCANCODE_END;
    io.KeyMap[ImGuiKey_Delete] = SCANCODE_DELETE;
    io.KeyMap[ImGuiKey_Backspace] = SCANCODE_BACKSPACE;
    io.KeyMap[ImGuiKey_Enter] = SCANCODE_RETURN;
    io.KeyMap[ImGuiKey_Escape] = SCANCODE_ESCAPE;
    io.KeyMap[ImGuiKey_A] = SCANCODE_A;
    io.KeyMap[ImGuiKey_C] = SCANCODE_C;
    io.KeyMap[ImGuiKey_V] = SCANCODE_V;
    io.KeyMap[ImGuiKey_X] = SCANCODE_X;
    io.KeyMap[ImGuiKey_Y] = SCANCODE_Y;
    io.KeyMap[ImGuiKey_Z] = SCANCODE_Z;

    // setup callback functions
    io.RenderDrawListsFn = ImGui_RenderDrawLists;
    io.SetClipboardTextFn = ImGui_SetClipboardText;
    io.GetClipboardTextFn = ImGui_GetClipboardText;

    // Set to true
    bInitialized = true;
    SetSettings(GuiSettings);

    URHO3D_LOGINFO("ImGui::Initialized");

    return true;

}

// save xml style element
bool ImGuiInterface::SaveStyleXML(XMLElement& dest, ImGuiStyle* outStyle) const
{
    ImGuiStyle* style = outStyle;
    // if no output style defined use default style
    if (!style)
        style = &ImGui::GetStyle();

    dest.CreateChild("Alpha").SetFloat("value", style->Alpha);
    dest.CreateChild("WindowPadding").SetAttribute("value", ToString(style->WindowPadding));
    dest.CreateChild("WindowMinSize").SetAttribute("value", ToString(style->WindowMinSize));
    dest.CreateChild("WindowRounding").SetFloat("value", style->WindowRounding);
    dest.CreateChild("WindowTitleAlign").SetInt("value", style->WindowTitleAlign);
    dest.CreateChild("ChildWindowRounding").SetFloat("value", style->ChildWindowRounding);
    dest.CreateChild("FramePadding").SetAttribute("value", ToString(style->FramePadding));
    dest.CreateChild("FrameRounding").SetFloat("value", style->FrameRounding);
    dest.CreateChild("ItemSpacing").SetAttribute("value", ToString(style->ItemSpacing));
    dest.CreateChild("ItemInnerSpacing").SetAttribute("value", ToString(style->ItemInnerSpacing));
    dest.CreateChild("TouchExtraPadding").SetAttribute("value", ToString(style->TouchExtraPadding));
    dest.CreateChild("WindowFillAlphaDefault").SetFloat("value", style->WindowFillAlphaDefault);
    dest.CreateChild("IndentSpacing").SetFloat("value", style->IndentSpacing);
    dest.CreateChild("ColumnsMinSpacing").SetFloat("value", style->ColumnsMinSpacing);
    dest.CreateChild("ScrollbarSize").SetFloat("value", style->ScrollbarSize);
    dest.CreateChild("ScrollbarRounding").SetFloat("value", style->ScrollbarRounding);
    dest.CreateChild("GrabMinSize").SetFloat("value", style->GrabMinSize);
    dest.CreateChild("GrabRounding").SetFloat("value", style->GrabRounding);
    dest.CreateChild("DisplayWindowPadding").SetAttribute("value", ToString(style->DisplayWindowPadding));
    dest.CreateChild("DisplaySafeAreaPadding").SetAttribute("value", ToString(style->DisplaySafeAreaPadding));
    dest.CreateChild("AntiAliasedLines").SetFloat("value", style->AntiAliasedLines);
    dest.CreateChild("AntiAliasedShapes").SetFloat("value", style->AntiAliasedShapes);
    dest.CreateChild("CurveTessellationTol").SetFloat("value", style->CurveTessellationTol);
    dest.CreateChild("WindowFillAlphaDefault").SetFloat("value", style->WindowFillAlphaDefault);

    XMLElement colors =dest.CreateChild("Colors");
    for (unsigned i = 0; i < ImGuiCol_COUNT; i++)
    {
        colors.CreateChild(ImGui::GetStyleColName(i)).SetAttribute("value", ToString(style->Colors[i]));
    }

    return true;
}

// Save User Interace XML Style
bool ImGuiInterface::SaveStyleXML(Serializer& dest, ImGuiStyle* outStyle, const String& indentation) const
{
    // Create a XML File
    SharedPtr<XMLFile> xml(new XMLFile(context_));
    XMLElement rootElem = xml->CreateRoot("ImGuiStyle");

    // save root
    if (!SaveStyleXML(rootElem))
        return false;

    return xml->Save(dest, indentation);
}


void ImGuiInterface::SetSettings(const ImGuiSettings& settings)
{
    // set GuiSettings
    GuiSettings = settings;

    // Get resiource cache
    ResourceCache * m_pResCache = g_pApp->GetConstantResCache();

    if(bInitialized)
    {
        ImGuiIO& io = ImGui::GetIO();

        String m_CurrentWorkDirectory = g_pApp->GetFileSystem()->GetProgramDir();
        m_CurrentWorkDirectory += String("Data");

        String dataAssetPath;

        dataAssetPath.Append(m_CurrentWorkDirectory);

        // setup core imgui settings
        io.IniSavingRate = settings.mIniSavingRate;
        io.MouseDoubleClickTime = settings.mMouseDoubleClickTime;
        io.MouseDoubleClickMaxDist = settings.mMouseDoubleClickMaxDist;
        io.MouseDragThreshold = settings.mMouseDragThreshold;
        io.KeyRepeatDelay = settings.mKeyRepeatDelay;
        io.KeyRepeatRate = settings.mKeyRepeatRate;
        io.FontGlobalScale = settings.mFontGlobalScale;
        io.FontAllowUserScaling = settings.mFontAllowUserScaling;
        io.DisplayFramebufferScale = settings.mDisplayFramebufferScale;
        io.DisplayVisibleMin = settings.mDisplayVisibleMin;
        io.DisplayVisibleMax = settings.mDisplayVisibleMax;
        iniFilename = dataAssetPath + settings.mIniFilename;
        logFilename = dataAssetPath + settings.mLogFilename;
        io.IniFilename = iniFilename.CString();
        io.LogFilename = logFilename.CString();

        // setup fonts
        ImFontAtlas* fontAtlas = ImGui::GetIO().Fonts;
        fontAtlas->Clear();
        fonts_.Clear();

        // if no fonts added, add default font
        if (settings.mFonts.Empty())  // does not work if font provied by settings are all in merge mode, need one font already loaded
        io.Fonts->AddFontDefault();

        const HashMap< StringHash, ImGuiSettings::FontConfig >& fontconfigs_map = settings.getFonts();
        HashMap< StringHash, ImGuiSettings::FontConfig >::ConstIterator it;

        for (it = fontconfigs_map.Begin(); it != fontconfigs_map.End(); it++)
        {
            String name = GetFileName(it->second_.filename_);
            fonts_[name] = io.Fonts->AddFontFromFileTTF(m_pResCache->GetResourceFileName(it->second_.filename_).CString(),
                           it->second_.size_, &it->second_.config_, it->second_.glyphRanges_.Empty() ? NULL : &it->second_.glyphRanges_[0]);
        }

        // setup font Texture
        if (fontTexture)
        {
            delete fontTexture;
            fontTexture = NULL;
        }
        unsigned char* pixels;
        int width, height;
        io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
        fontTexture = new Texture2D(context_);

        fontTexture->SetNumLevels(1);
        fontTexture->SetSize(width, height, Graphics::GetRGBAFormat());
        fontTexture->SetData(0, 0, 0, width, height, pixels);
        fontTexture->SetFilterMode(FILTER_NEAREST);

        // Store our identifier
        io.Fonts->TexID = (void *)(intptr_t)fontTexture;

        io.Fonts->ClearTexData();

        // set style
        const ImGuiStyle& style = settings.getStyle();
        ImGuiStyle& imGuiStyle = ImGui::GetStyle();
        imGuiStyle.Alpha = style.Alpha;
        imGuiStyle.WindowPadding = style.WindowPadding;
        imGuiStyle.WindowMinSize = style.WindowMinSize;
        imGuiStyle.WindowRounding = style.WindowRounding;
        imGuiStyle.WindowTitleAlign = style.WindowTitleAlign;
        imGuiStyle.ChildWindowRounding = style.ChildWindowRounding;
        imGuiStyle.FramePadding = style.FramePadding;
        imGuiStyle.FrameRounding = style.FrameRounding;
        imGuiStyle.ItemSpacing = style.ItemSpacing;
        imGuiStyle.ItemInnerSpacing = style.ItemInnerSpacing;
        imGuiStyle.TouchExtraPadding = style.TouchExtraPadding;
        imGuiStyle.WindowFillAlphaDefault = style.WindowFillAlphaDefault;
        imGuiStyle.IndentSpacing = style.IndentSpacing;
        imGuiStyle.ColumnsMinSpacing = style.ColumnsMinSpacing;
        imGuiStyle.ScrollbarSize = style.ScrollbarSize;
        imGuiStyle.ScrollbarRounding = style.ScrollbarRounding;
        imGuiStyle.GrabMinSize = style.GrabMinSize;
        imGuiStyle.GrabRounding = style.GrabRounding;
        imGuiStyle.DisplayWindowPadding = style.DisplayWindowPadding;
        imGuiStyle.DisplaySafeAreaPadding = style.DisplaySafeAreaPadding;
        imGuiStyle.AntiAliasedLines = style.AntiAliasedLines;
        imGuiStyle.AntiAliasedShapes = style.AntiAliasedShapes;

        // set colors
        for (int i = 0; i < ImGuiCol_COUNT; i++)
            imGuiStyle.Colors[i] = style.Colors[i];
    }
}


bool ImGuiInterface::LoadStyleXML(Deserializer& source, ImGuiStyle* outStyle)
{
    SharedPtr<XMLFile> xml(new XMLFile(context_));
    if (!xml->Load(source))
        return false;
    return LoadStyleXML(xml->GetRoot());
}

bool ImGuiInterface::LoadStyleXML(const XMLElement& source, ImGuiStyle* outStyle, bool setInstanceDefault)
{
    ImGuiStyle* style = outStyle;
    // if no output style defined use default style
    if (!style)
        style = &ImGui::GetStyle();

    style->Alpha = source.GetChild("Alpha").GetFloat("value");
    style->WindowPadding = ToImVec2(source.GetChild("WindowPadding").GetAttribute("value"));
    style->WindowMinSize = ToImVec2(source.GetChild("WindowMinSize").GetAttribute("value"));
    style->WindowRounding = source.GetChild("WindowRounding").GetFloat("value");
    style->WindowTitleAlign = source.GetChild("WindowTitleAlign").GetInt("value");
    style->ChildWindowRounding = source.GetChild("ChildWindowRounding").GetFloat("value");
    style->FramePadding = ToImVec2(source.GetChild("FramePadding").GetAttribute("value"));
    style->FrameRounding = source.GetChild("FrameRounding").GetFloat("value");
    style->ItemSpacing = ToImVec2(source.GetChild("ItemSpacing").GetAttribute("value"));
    style->ItemInnerSpacing = ToImVec2(source.GetChild("ItemInnerSpacing").GetAttribute("value"));
    style->TouchExtraPadding = ToImVec2(source.GetChild("TouchExtraPadding").GetAttribute("value"));
    style->WindowFillAlphaDefault = source.GetChild("WindowFillAlphaDefault").GetFloat("value");
    style->IndentSpacing = source.GetChild("IndentSpacing").GetFloat("value");
    style->ColumnsMinSpacing = source.GetChild("ColumnsMinSpacing").GetFloat("value");
    style->ScrollbarSize = source.GetChild("ScrollbarSize").GetFloat("value");
    style->ScrollbarRounding = source.GetChild("ScrollbarRounding").GetFloat("value");
    style->GrabMinSize = source.GetChild("GrabMinSize").GetFloat("value");
    style->GrabRounding = source.GetChild("GrabRounding").GetFloat("value");
    style->DisplayWindowPadding = ToImVec2(source.GetChild("DisplayWindowPadding").GetAttribute("value"));
    style->DisplaySafeAreaPadding = ToImVec2(source.GetChild("DisplaySafeAreaPadding").GetAttribute("value"));
    style->AntiAliasedLines = source.GetChild("AntiAliasedLines").GetFloat("value");
    style->AntiAliasedShapes = source.GetChild("AntiAliasedShapes").GetFloat("value");
    style->CurveTessellationTol = source.GetChild("CurveTessellationTol").GetFloat("value");
    style->WindowFillAlphaDefault = source.GetChild("WindowFillAlphaDefault").GetFloat("value");


    XMLElement colors = source.GetChild("Colors");
    for (unsigned i = 0; i < ImGuiCol_COUNT; i++)
    {
        style->Colors[i] = ToImVec4(colors.GetChild(ImGui::GetStyleColName(i)).GetAttribute("value"));
    }

    return true;
}


void ImGuiInterface::RenderDrawLists(ImDrawData* data)
{
    ImDrawList** const cmd_lists = data->CmdLists;
    int cmd_lists_count = data->CmdListsCount;

    // Engine does not render when window is closed or device is lost
    assert(m_pGraphics && m_pGraphics->IsInitialized() && !m_pGraphics->IsDeviceLost());

    if (cmd_lists_count == 0)
        return;

    Vector2 invScreenSize(1.0f / (float)m_pGraphics->GetWidth(), 1.0f / (float)m_pGraphics->GetHeight());
    Vector2 scale(2.0f * invScreenSize.x_, -2.0f * invScreenSize.y_);
    Vector2 offset(-1.0f, 1.0f);

    Matrix4 projection(Matrix4::IDENTITY);
    projection.m00_ = scale.x_;
    projection.m03_ = offset.x_;
    projection.m11_ = scale.y_;
    projection.m13_ = offset.y_;
    projection.m22_ = 1.0f;
    projection.m23_ = 0.0f;
    projection.m33_ = 1.0f;

    m_pGraphics->ClearParameterSources();
    m_pGraphics->SetColorWrite(true);
    m_pGraphics->SetCullMode(CULL_NONE);
    m_pGraphics->SetDepthTest(CMP_ALWAYS);
    m_pGraphics->SetDepthWrite(false);
    m_pGraphics->SetFillMode(FILL_SOLID);
    m_pGraphics->SetStencilTest(false);
    m_pGraphics->ResetRenderTargets();
    m_pGraphics->SetBlendMode(BLEND_ALPHA);

    ShaderVariation* noTextureVS = m_pGraphics->GetShader(VS, "IMGUI", "VERTEXCOLOR");
    ShaderVariation* diffTextureVS = m_pGraphics->GetShader(VS, "IMGUI", "DIFFMAP VERTEXCOLOR");
    ShaderVariation* noTexturePS = m_pGraphics->GetShader(PS, "IMGUI", "VERTEXCOLOR");
    ShaderVariation* diffTexturePS = m_pGraphics->GetShader(PS, "IMGUI", "DIFFMAP VERTEXCOLOR");
    ShaderVariation* diffMaskTexturePS = m_pGraphics->GetShader(PS, "IMGUI", "DIFFMAP ALPHAMASK VERTEXCOLOR");
    ShaderVariation* alphaTexturePS = m_pGraphics->GetShader(PS, "IMGUI", "ALPHAMAP VERTEXCOLOR");

    unsigned alphaFormat = Graphics::GetAlphaFormat();

    /// resize buffers
#if defined(URHO3D_D3D11)
    // TODO: for D3D11 : cannot set vertex/index buffer to dynamic. bug ? does it need to be dynamic ?
    if ((int)VertexBuffer->GetVertexCount() < data->TotalVtxCount || (int)VertexBuffer->GetVertexCount() > data->TotalVtxCount * 2)
        VertexBuffer->SetSize(data->TotalVtxCount, MASK_POSITION | MASK_COLOR | MASK_TEXCOORD1, false);

    if ((int)pIndexBuffer->GetIndexCount() < data->TotalIdxCount || (int)pIndexBuffer->GetIndexCount() > data->TotalIdxCount * 2)
        pIndexBuffer->SetSize(data->TotalIdxCount, false, false);
#else
    if ((int)pVertexBuffer->GetVertexCount() < data->TotalVtxCount || (int)pVertexBuffer->GetVertexCount() > data->TotalVtxCount * 2)
        pVertexBuffer->SetSize(data->TotalVtxCount, MASK_POSITION | MASK_COLOR | MASK_TEXCOORD1, true);

    if ((int)pIndexBuffer->GetIndexCount() < data->TotalIdxCount || (int)pIndexBuffer->GetIndexCount() > data->TotalIdxCount * 2)
        pIndexBuffer->SetSize(data->TotalIdxCount, false, true);
#endif

    // Copy and convert all vertices into a single contiguous buffer

    // 		int vtx_list_offset = 0;
    // 		int idx_list_offset = 0;
    // 		for (int n = 0; n < data->CmdListsCount; n++)
    // 		{
    // 			const ImDrawList* cmd_list = data->CmdLists[n];
    //
    // 			VertexBuffer->SetDataRange(&cmd_list->VtxBuffer[0], vtx_list_offset, cmd_list->VtxBuffer.size());
    // 			pIndexBuffer->SetDataRange(&cmd_list->IdxBuffer[0], idx_list_offset, cmd_list->IdxBuffer.size());
    //
    // 			vtx_list_offset += cmd_list->VtxBuffer.size();
    // 			idx_list_offset += cmd_list->IdxBuffer.size();
    // 		}

#ifndef GL_ES_VERSION_2_0
    ImDrawVert* vtx_dst = (ImDrawVert*)pVertexBuffer->Lock(0, data->TotalVtxCount);
    ImDrawIdx* idx_dst = (ImDrawIdx*)pIndexBuffer->Lock(0, data->TotalIdxCount);
    for (int n = 0; n < data->CmdListsCount; n++)
    {
        const ImDrawList* cmd_list = data->CmdLists[n];
        memcpy(vtx_dst, &cmd_list->VtxBuffer[0], cmd_list->VtxBuffer.size() * sizeof(ImDrawVert));
        memcpy(idx_dst, &cmd_list->IdxBuffer[0], cmd_list->IdxBuffer.size() * sizeof(ImDrawIdx));
        vtx_dst += cmd_list->VtxBuffer.size();
        idx_dst += cmd_list->IdxBuffer.size();
    }
    pVertexBuffer->Unlock();
    pIndexBuffer->Unlock();
#endif

    m_pGraphics->SetVertexBuffer(pVertexBuffer);
    m_pGraphics->SetIndexBuffer(pIndexBuffer);

    // Render command lists
    int vtx_offset = 0;
    int idx_offset = 0;
    for (int n = 0; n < data->CmdListsCount; n++)
    {
        const ImDrawList* cmd_list = data->CmdLists[n];
#ifdef GL_ES_VERSION_2_0

        VertexBuffer->SetDataRange(&cmd_list->VtxBuffer[0], 0, cmd_list->VtxBuffer.size());
        pIndexBuffer->SetDataRange(&cmd_list->IdxBuffer[0], 0, cmd_list->IdxBuffer.size());

        idx_offset = 0;
#endif

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.size(); cmd_i++)
        {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback)
            {
                pcmd->UserCallback(cmd_list, pcmd);
            }
            else
            {
                Texture* texture = (Urho3D::Texture*)(intptr_t)pcmd->TextureId;
                ShaderVariation* ps;
                ShaderVariation* vs;
                if (!texture)
                {
                    ps = noTexturePS;
                    vs = noTextureVS;
                }
                else
                {
                    vs = diffTextureVS;
                    // If texture contains only an alpha channel, use alpha shader (for fonts)
                    if (texture->GetFormat() == alphaFormat)
                        ps = alphaTexturePS;
                    // 						else if (blendMode_ != BLEND_ALPHA && batch.blendMode_ != BLEND_ADDALPHA && batch.blendMode_ != BLEND_PREMULALPHA)
                    //			 						ps = diffMaskTexturePS;
                    else
                        ps = diffTexturePS;
                }

                m_pGraphics->SetShaders(vs, ps);

                if (m_pGraphics->NeedParameterUpdate(SP_OBJECT, this))
                    m_pGraphics->SetShaderParameter(VSP_MODEL, Matrix3x4::IDENTITY);
                if (m_pGraphics->NeedParameterUpdate(SP_CAMERA, this))
                    m_pGraphics->SetShaderParameter(VSP_VIEWPROJ, projection);
                if (m_pGraphics->NeedParameterUpdate(SP_MATERIAL, this))
                    m_pGraphics->SetShaderParameter(PSP_MATDIFFCOLOR, Color(1.0f, 1.0f, 1.0f, 1.0f));

                m_pGraphics->SetScissorTest(true, IntRect((int)pcmd->ClipRect.x, (int)(pcmd->ClipRect.y),
                                            (int)(pcmd->ClipRect.z), (int)(pcmd->ClipRect.w)));

                m_pGraphics->SetTexture(0, texture);

#if defined(URHO3D_OPENGL)

#ifdef GL_ES_VERSION_2_0
                m_pGraphics->Draw(TRIANGLE_LIST, idx_offset, pcmd->ElemCount, 0, cmd_list->VtxBuffer.size());
#else
    //            m_pGraphics->Draw(TRIANGLE_LIST, idx_offset, pcmd->ElemCount, vtx_offset, 0, cmd_list->VtxBuffer.size());                m_pGraphics->Draw(TRIANGLE_LIST, idx_offset, pcmd->ElemCount, 0, cmd_list->VtxBuffer.size());
          m_pGraphics->Draw(TRIANGLE_LIST, idx_offset, pcmd->ElemCount, 0, cmd_list->VtxBuffer.size());
#endif
#else
                m_pGraphics->Draw(TRIANGLE_LIST, idx_offset, pcmd->ElemCount, vtx_offset, 0, cmd_list->VtxBuffer.size());
#endif
            }
            idx_offset += pcmd->ElemCount;
        }
        vtx_offset += cmd_list->VtxBuffer.size();
    }
}

void ImGuiInterface::PushFont(const String& name)
{
    HashMap<StringHash, ImFont* >::ConstIterator it =  fonts_.Find(name);
    if(it == fonts_.End())
    {
        //URHO3D_LOGERRORF("IMUI::PushFont: Font %s not found!! ",name.CString());  will be called multiple times -> use assert ?
        return;
    }

    ImGui::PushFont(it->second_);
}

void ImGuiInterface::PopFont()
{
    ImGui::PopFont();
}


// Ex. Main menu view doesn't have any actors.
void ImGuiInterface::VOnAttach(GameViewId vid, GameNodeId aid) {};

// Initialize humam view.
bool ImGuiInterface::VOnRestore() {};

// Lost device/input.
bool ImGuiInterface::VOnLostDevice() {};

// Update view.
void ImGuiInterface::VOnUpdate(float timeStep) {};

// Handle input message from devices
bool ImGuiInterface::VOnMsgProc(AppMsg message) {};

// Shutdown view.
void ImGuiInterface::VShutdown() {};
