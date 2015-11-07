#include "EngineStd.h"
#include "Initialization.h"

GameOptions::GameOptions()
{
	// set all the options to decent default values
	m_Name = "Mr.Scan";
	m_Level = "";
	m_Renderer = "OpenGL";
	m_bRunFullSpeed = false;
	m_SoundEffectsVolume = 100;
	m_MusicVolume = 100;
	m_SpeechVolume = 100;
	m_ExpectedPlayers = 1;
	m_ListenPort = -1;
	m_GameHost = "MrMike-m1710";
	m_NumAIs = 1;
	m_MaxAIs = 4;
	m_MaxPlayers = 4;
	m_ScreenSize = IntVector2(1024, 768);
	m_bUseDevelopmentDirectories = false;
	m_bWindowMode = true;
	m_bUseDebugHud = false;
	m_Multisample = 1;	// no multisampling
	m_bTripleBuffer = false; // does not use tripple buffer
}

GameOptions::~GameOptions()
{
	pugi::xml_document* document = m_pFile->GetDocument();
	if (document)
	{
		document->save_file("GameData\\System\\PlayerOptions.xml");
	}
}

bool GameOptions::Init(const char* xmlFilePath)
{
	//// read the XML file
	//// if needed, override the XML file with options passed in on the command line.
	m_FileName = String(xmlFilePath);

	m_pFile = new Urho3D::XMLFile(g_pApp->GetContext());

	pugi::xml_document* document = m_pFile->GetDocument();

	document->load_file(xmlFilePath);

	if (!document)
	{
		return false;
	}

	pugi::xml_node root = document->root();
	if (!root)
	{
		return false;
	}

	// Loop through each child element and load the component
	pugi::xml_node node;
	node = root.first_child();
	if (String(node.attribute("name").as_string()).Compare("options") == 0)
	{
		node = node.child("Graphics");
		if (node)
		{
			String attribute = node.attribute("renderer").as_string();
			int comp = attribute.Compare("OpenGL", false);

			if (attribute.Compare("OpenGL", false) == 0 || attribute.Compare("DirectX11", false) == 0 || attribute.Compare("DirectX9", false) == 0)
			{
				m_Renderer = attribute;
			}
			else
			{
				return false;
			}


			m_ScreenSize.x_ =node.attribute("width").as_int();
			if (m_ScreenSize.x_ < 800) m_ScreenSize.x_ = 800;
			if (m_ScreenSize.x_ > 3840) m_ScreenSize.x_ = 3840;


			m_ScreenSize.y_ = node.attribute("height").as_int();
			if (m_ScreenSize.y_ < 600) m_ScreenSize.y_ = 600;
			if (m_ScreenSize.y_ > 2160) m_ScreenSize.y_ = 2160;


			attribute = String(node.attribute("runfullspeed").as_string());
			m_bRunFullSpeed = (attribute == "yes") ? true : false;

			attribute = String(node.attribute("vsync").as_string());
			m_bVSync = (attribute == "yes") ? true : false;

			attribute = String(node.attribute("windowMode").as_string());
			m_bWindowMode = (attribute == "yes") ? true : false;

			m_Multisample = node.attribute("multisample").as_int();

			attribute = String(node.attribute("triplebuffer").as_string());
			m_bTripleBuffer = (attribute == "yes") ? true : false;
		}

		node = node.next_sibling("Sound");
		if (node)
		{
			m_MasterVolume = node.attribute("masterVolume").as_int();
			m_MusicVolume = node.attribute("musicVolume").as_int();
			m_SoundEffectsVolume = node.attribute("sfxVolume").as_int();
			m_SpeechVolume = node.attribute("speechVolume").as_int();
		}

		node = node.next_sibling("Multiplayer");
		if (node)
		{
			m_Name = node.attribute("playerName").as_string();
			m_ExpectedPlayers = node.attribute("expectedPlayers").as_int();
			m_NumAIs = node.attribute("numAIs").as_int();
			m_MaxAIs = node.attribute("maxAIs").as_int();
			m_MaxPlayers = node.attribute("maxPlayers").as_int();

			m_ListenPort = node.attribute("listenPort").as_int();
			m_GameHost = String(node.attribute("gameHost").as_string());
		}

		node = node.next_sibling("ResCache");
		if (node)
		{
			String attribute(node.attribute("useDevelopmentDirectories").as_string());
			m_bUseDevelopmentDirectories = ((attribute == "yes") ? (true) : (false));
		}

		node = node.next_sibling("Debugging");
		if (node)
		{
			String attribute(node.attribute("useDebugHud").as_string());
			m_bUseDebugHud = ((attribute == "yes") ? (true) : (false));
		}
	}

	InitResolutions();

	URHO3D_LOGINFO("Successfully loaded player options");
	return true;
}

bool GameOptions::ChangeXML()
{
	pugi::xml_document* document = m_pFile->GetDocument();

	if (!document)
	{
		URHO3D_LOGERROR("Failed to change GameOptions xml file");
		return false;
	}

	pugi::xml_node root = document->root();
	if (!root)
	{
		return false;
	}
	pugi::xml_node node;
	node = root.first_child();
	if (String(node.attribute("name").as_string()).Compare("options") == 0)
	{
		node = node.find_child_by_attribute("name", "sound");
		if (node)
		{
			node.attribute("masterVolume").set_value(m_MasterVolume);
			node.attribute("sfxVolume").set_value(m_SoundEffectsVolume);
			node.attribute("musicVolume").set_value(m_MusicVolume);
			node.attribute("speechVolume").set_value(m_SpeechVolume);
		}

		node = root.first_child();
		node = node.find_child_by_attribute("name", "graphics");
		if (node)
		{
			IntVector2 size = g_pApp->GetCurrentWindowSize();
			node.attribute("width").set_value(size.x_);
			node.attribute("height").set_value(size.y_);
			node.attribute("vsync").set_value((m_bVSync == true ? "yes" : "no"));
			node.attribute("windowMode").set_value(((m_bWindowMode == true) ? "yes" : "no"));

			node.attribute("multisample").set_value(m_Multisample);
			node.attribute("triplebuffer").set_value(((m_bTripleBuffer == true) ? "yes" : "no"));
		}

	}


	return true;
}

IntVector2 GameOptions::GetResolutionValues(String res)
{
	int offset = res.Find("x");
	int length = res.Length();
	String ystr;
	ystr.Append(res.CString() + offset + 1);
	int x = atoi(String(res.CString(), offset).CString());
	int y = atoi(ystr.CString());
	return IntVector2(x, y);
}

void GameOptions::InitResolutions()
{
	pugi::xml_document* document = m_pFile->GetDocument();

	if (!document)
	{
		URHO3D_LOGERROR("Failed to init resolutions in GameOptions xml file");
	}

	pugi::xml_node root = document->root();
	if (!root)
	{
		URHO3D_LOGERROR("Failed to find root in GameOptions xml file");
	}

	pugi::xml_node node;
	node = root.find_child_by_attribute("name", "resolution");
	if (node)
	{
		for (pugi::xml_node res = node.child("Resolution-1"); res; res = res.next_sibling())
		{
			String width = res.attribute("width").as_string();
			String height = res.attribute("height").as_string();
			m_Resolutions.Push(width + String("x") + height);
		}
	}
}
