#ifndef IAUDIO_BUFFER_H
#define IAUDIO_BUFFER_H

// The interface class that defines the public API for audio buffers.
// An audio buffer maps to one instance of a sound being played,
// which ISNT the sound data. Two different sounds can be played from
// the same source data - such as two explosions in two different places.

class IAudioBuffer : public Object
{
public:
	IAudioBuffer(Context* context);
	virtual ~IAudioBuffer() { }

	virtual void *VGet() = 0;
	/*virtual shared_ptr<ResHandle> VGetResource()=0;*/
	virtual bool VOnRestore() = 0;

	virtual bool VPlay(int volume, bool looping) = 0;
	virtual bool VPause() = 0;
	virtual bool VStop() = 0;
	virtual bool VResume() = 0;

	virtual bool VTogglePause() = 0;
	virtual bool VIsPlaying() = 0;
	virtual bool VIsLooping() const = 0;
	virtual void VSetVolume(int volume) = 0;
	virtual void VSetPosition(unsigned long newPosition) = 0;
	virtual int VGetVolume() const = 0;
	virtual float VGetProgress() = 0;
};

#endif // IAUDIO_BUFFER_H
