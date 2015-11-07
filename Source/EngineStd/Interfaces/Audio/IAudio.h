#ifndef IAUDIO_H
#define IAUDIO_H

// This interface class describes the public interface for
// a game's audio system.

#include "Resources/ResHandle.h"

class IAudioBuffer;


class IAudio : public Object
{
public:
	IAudio(Context* context);
	virtual bool VActive() = 0;

	virtual IAudioBuffer* VInitAudioBuffer(SharedPtr<ResHandle> handle) = 0;
	virtual void VReleaseAudioBuffer(IAudioBuffer* audioBuffer) = 0;

	virtual void VStopAllSounds() = 0;
	virtual void VPauseAllSounds() = 0;
	virtual void VResumeAllSounds() = 0;

	virtual void VUpdate() = 0;

	virtual bool VInitialize() = 0;
	virtual void VShutdown() = 0;
};

#endif //IAUDIO_H
