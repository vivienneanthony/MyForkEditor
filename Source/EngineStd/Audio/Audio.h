#ifndef AUDIO_H
#define AUDIO_H

class SoundResourceExtraData;


//////////////////////////////////////////////////////////////////////
// SoundType Description
// This is an enum that represents the different kinds of sound data
// streams the sound system can handle.
//////////////////////////////////////////////////////////////////////

enum SoundType
{
	SOUND_TYPE_FIRST,
	SOUND_TYPE_MP3 = SOUND_TYPE_FIRST,
	SOUND_TYPE_WAVE,
	SOUND_TYPE_MIDI,
	SOUND_TYPE_OGG,
	// This needs to be the last sound type
	SOUND_TYPE_COUNT,
	SOUND_TYPE_UNKNOWN,
};

extern String gSoundExtentions[];

//////////////////////////////////////////////////////////////////////
// Implements IAudio interface - but not all the way - this is
// still a base class.
//////////////////////////////////////////////////////////////////////

class SWAudio : public IAudio
{
	URHO3D_OBJECT(Audio, IAudio)
public:
	// Con/De-structors
	SWAudio(Context* context);
	virtual ~SWAudio() { }

	// A bit of an anachronism in name - but it simply returns true
	// if the sound system is active.
	static bool HasSoundCard();

	// Init, update, shutdown
	virtual void VUpdate() { }
	virtual void VShutdown();

	// Stop all sounds. Will start playing from the beggining.
	virtual void VStopAllSounds();
	// Pause all sounds. Will start from current pause.
	virtual void VPauseAllSounds();
	// Resume/Play all sounds.
	virtual void VResumeAllSounds();

	// Getters/Setters
	bool IsPaused() { return m_bIsAllPaused; }
	bool IsReadyToDelete() { return m_bIsRunning;  }
	bool SetShutDown() { m_bIsRunning = false; return true; }
protected:
	AudioBufferList m_AllBuffers;							// List of all currently allocated audio buffers
	bool m_bIsInitialized;									// Has the sound system been initialized?
    bool m_bIsAllPaused;									// Has the sound system been paused?
	bool m_bIsRunning;
};

extern SWAudio* g_pAudio;

#endif
