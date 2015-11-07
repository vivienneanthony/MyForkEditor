#include "EngineStd.h"
#include "Audio.h"



//////////////////////////////////////////////////////////////////////
// Globals
//////////////////////////////////////////////////////////////////////

String gSoundExtentions[] = { ".mp3", ".wav", ".midi", ".ogg" };

SWAudio* g_pAudio = nullptr;

SWAudio::SWAudio(Context* context) : IAudio(context), m_bIsInitialized(false), m_bIsAllPaused(false)
{

}

//
// SWAudio::VShutdown
//
void SWAudio::VShutdown()
{
	AudioBufferList::Iterator i = m_AllBuffers.Begin();

	while (i != m_AllBuffers.End())
	{
		IAudioBuffer *SWAudioBuffer = (*i);
		if(SWAudioBuffer)
		{
			SWAudioBuffer->VStop();
			SAFE_DELETE(SWAudioBuffer);
			i = m_AllBuffers.Erase(i);
		}
	}
}

void SWAudio::VStopAllSounds()
{
	IAudioBuffer *SWAudioBuffer = nullptr;

	AudioBufferList::Iterator i;
	AudioBufferList::Iterator end;
	for (i = m_AllBuffers.Begin(), end = m_AllBuffers.End(); i != end; ++i)
	{
		SWAudioBuffer = (*i);
		SWAudioBuffer->VStop();
	}

	m_bIsAllPaused = false;
}


void SWAudio::VPauseAllSounds()
{
	AudioBufferList::Iterator i;
	AudioBufferList::Iterator end;
	for (i = m_AllBuffers.Begin(), end = m_AllBuffers.End(); i != end; ++i)
	{
		IAudioBuffer *SWAudioBuffer = (*i);
		SWAudioBuffer->VPause();
	}

	m_bIsAllPaused = true;
}


void SWAudio::VResumeAllSounds()
{
	AudioBufferList::Iterator i;
	AudioBufferList::Iterator end;
	for (i = m_AllBuffers.Begin(), end = m_AllBuffers.End(); i != end; ++i)
	{
		IAudioBuffer *SWAudioBuffer = (*i);
		SWAudioBuffer->VResume();
	}

	m_bIsAllPaused = false;
}

bool SWAudio::HasSoundCard()
{
	return (g_pApp->GetContext()->GetSubsystem<SWAudio>() && g_pApp->GetContext()->GetSubsystem<SWAudio>()->VActive());
}


