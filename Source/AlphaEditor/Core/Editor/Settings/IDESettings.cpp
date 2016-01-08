#include "AlphaEditorStd.h"

#include "../../UI/UIGlobals.h"

#include "../../UI/UIUtils.h"
#include "../../UI/Hierarchy/HierarchyWindow.h"

#include "IDESettings.h"


using namespace Urho3D;


IDESettings::IDESettings(Context* context) :Serializable(context)

{
    ResetToDefault();
}

IDESettings::~IDESettings()
{
}

void IDESettings::RegisterObject(Context* context)
{
    context->RegisterFactory<IDESettings>();

    URHO3D_ATTRIBUTE("WindowTitle", String, windowTitle_, String("EngineStd IDE"), AM_FILE);
    URHO3D_ATTRIBUTE("WindowIcon", String, windowIcon_, String("Textures/EngineIDE.png"), AM_FILE);
    URHO3D_ATTRIBUTE("WindowWidth", int, windowWidth_, 1024, AM_FILE);
    URHO3D_ATTRIBUTE("WindowHeight", int, windowHeight_, 768, AM_FILE);

    URHO3D_ATTRIBUTE("ResourcePaths", String, resourcePaths_, String("Data;CoreData;GameData"), AM_FILE);
    URHO3D_ATTRIBUTE("ResourcePackages", String, resourcePackages_, String("Data.pak;CoreData.pak;GameData.pak"), AM_FILE);
    URHO3D_ATTRIBUTE("AutoloadPaths", String, autoloadPaths_, String("Extra"), AM_FILE);

    URHO3D_ATTRIBUTE("WindowResizable", bool, windowResizable_, true, AM_FILE);
    URHO3D_ATTRIBUTE("FullScreen", bool, fullScreen_, false, AM_FILE);

}

VariantMap IDESettings::ToVariantMap()
{
    const Vector<AttributeInfo>* attributes = GetAttributes();
    if (!attributes)
        return VariantMap();
    VariantMap variantMap;
    Variant value;

    for (unsigned i = 0; i < attributes->Size(); ++i)
    {
        const AttributeInfo& attr = attributes->At(i);
        if (!(attr.mode_ & AM_FILE))
            continue;

        OnGetAttribute(attr, value);
        variantMap[attr.name_] = value;
    }
    return variantMap;
}

Urho3D::String IDESettings::GetPreferencesFullPath()
{
    // Changed to use Urho3D GameSystem - Engine not fully initalized yet
    FileSystem* fs = GetSubsystem<FileSystem>();

    // change data to have the IDESettings
    String filepath = fs->GetProgramDir()+"Data/";
    filepath += "EngineIDESettings.xml";

    return filepath;
}


bool IDESettings::Save()
{
    // Changed to use Urho3D GameSystem - Engine not fully initalized yet
    FileSystem* fileSystem = GetSubsystem<FileSystem>();

    File saveFile(context_, GetPreferencesFullPath(), FILE_WRITE);
    XMLFile xmlFile(context_);
    XMLElement rootElem = xmlFile.CreateRoot("EngineIDESettings");
    SaveXML(rootElem);
    return xmlFile.Save(saveFile);
}

bool IDESettings::Load()
{
    FileSystem* fileSystem = GetSubsystem<FileSystem>();

    if (fileSystem->FileExists(GetPreferencesFullPath()))
    {
        File loadFile(context_, GetPreferencesFullPath(), FILE_READ);
        XMLFile loadXML(context_);
        loadXML.BeginLoad(loadFile);
        LoadXML(loadXML.GetRoot("EngineIDESettings"));
        return true;
    }
    else
        return false;
}

void IDESettings::LoadConfigFile()
{
    if (!Load())
    {
        /// file does not exist so create default file
        Save();
    }
}
