#pragma once
#include "Fmod.h"
#include "FMOD/ToneFire.h"
#include <memory>
#include <assert.h>

//Static Helper Class. Do not instantiate. I will make the compiler yell at you.
//Note: this class is not 100% thread safe for multithreaded systems.
class AudioEngine
{
public:
	typedef std::shared_ptr<AudioEngine> Sptr;

	static void init();

	static void setPitch(const float pitch);

	static void SetEventParameters(const std::string eventName, const std::string parameterName, const float parameterValue);

	static ToneFire::FMODStudio* GetContext();

	static ToneFire::StudioSound* GetContextBanks();
private:
	//Static Singletons for use throughout the project
	inline static ToneFire::FMODStudio Studio;
	inline static ToneFire::StudioSound Banks;
};

