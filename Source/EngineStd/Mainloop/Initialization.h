#ifndef INITIALIZATION_H
#define INITIALIZATION_H

typedef Vector<String> Resolutions;

struct GameOptions
{
	// XML file
	String m_FileName;
	SharedPtr<Urho3D::XMLFile> m_pFile;

	// Level options
	String m_Level;

	// Render options
	String m_Renderer;
	bool m_bRunFullSpeed;

	// Window options
	IntVector2 m_ScreenSize;
	bool m_bWindowMode;
	bool m_bVSync;
	int m_Multisample;
	bool m_bTripleBuffer;

	// Sound options
	int m_MasterVolume;
	int m_SoundEffectsVolume;
	int m_MusicVolume;
	int m_SpeechVolume;
	bool m_bIsEnableMusic;

	// Multiplayer options
	int m_ListenPort;
	String m_GameHost;
	bool m_IsEnableMultipayer;;

	// Platform
    PlatformOS m_CurrentPlatform;

	// resource cache options
	bool m_bUseDevelopmentDirectories;

	// Debug Hud
	bool m_bUseDebugHud;

	// Available resolutions
	Resolutions m_Resolutions;

	GameOptions();
	~GameOptions();

	bool InitGameOptions(const char* xmlFilePath);
	void InitResolutions();
	bool ChangeXML();

    bool InitRenderer(void);
    void SetPlatform(PlatformOS setPlatform);

	// Helper functions
	static IntVector2 GetResolutionValues(String res);
	Resolutions& GetResolutions() { return m_Resolutions; }
};

#endif // INITIALIZATION_H
