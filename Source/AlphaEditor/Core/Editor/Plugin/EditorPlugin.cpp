#include "AlphaEditorStd.h"

#include "EditorPlugin.h"


EditorPlugin::EditorPlugin(Context* context) : Object(context),
    visible_(false)

{

}

EditorPlugin::~EditorPlugin()
{

}

// Scene start update
void EditorPlugin::StartSceneUpdate()
{
}

// Scene stop update
void EditorPlugin::StopSceneUpdate()
{
}


