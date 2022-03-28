#include "AudioEngine.h"

void AudioEngine::init()
{
	assert(&Studio != nullptr);
	Studio.LoadBank("Master.bank");
	Studio.LoadBank("Master.strings.bank");
	Studio.LoadBank("Level1.bank");
}
void AudioEngine::SetEventParameters(const std::string eventName, const std::string parameterName, float parameterValue) {
	Banks.SetEventParameter(eventName, parameterName, parameterValue);
}

void AudioEngine::setPitch(const float pitch) {
	
}

ToneFire::FMODStudio* AudioEngine::GetContext()
{
	return &Studio;
}
ToneFire::StudioSound* AudioEngine::GetContextBanks()
{
	return &Banks;
}