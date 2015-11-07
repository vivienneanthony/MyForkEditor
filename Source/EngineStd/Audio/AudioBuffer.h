#ifndef AUDIO_BUFFER_H
#define AUDIO_BUFFER_H

class AudioBuffer : public IAudioBuffer
{
	URHO3D_OBJECT(AudioBuffer, IAudioBuffer)

protected:
	// Disable public constructions
	AudioBuffer(Context* context);
public:
	virtual ~AudioBuffer() { }

	// Getters/Setters
	virtual bool VIsLooping() const { return m_bIsLooping; }
	virtual int VGetVolume() const { return m_Volume; }
protected:
	bool m_bIsPaused;					// Is the sound paused
	bool m_bIsLooping;					// Is the sound looping
	int m_Volume;						// the volume
};

#endif
