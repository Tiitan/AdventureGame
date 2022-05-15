#include "AgGameUserSettings.h"
#include "AudioMixer.h"

UAgGameUserSettings* UAgGameUserSettings::GetUserSettings()
{
	auto test = UGameUserSettings::GetGameUserSettings();
	return Cast<UAgGameUserSettings>(UGameUserSettings::GetGameUserSettings());
}

float UAgGameUserSettings::GetMasterVolume() const
{
	return MasterVolume;
}

float UAgGameUserSettings::GetEffectVolume() const
{
	return EffectVolume;
}

float UAgGameUserSettings::GetMusicVolume() const
{
	return MusicVolume;
}

void UAgGameUserSettings::SetMasterVolume(const float Value)
{
	MasterVolume = Value;
}

void UAgGameUserSettings::SetEffectVolume(const float Value)
{
	EffectVolume = Value;
}

void UAgGameUserSettings::SetMusicVolume(const float Value)
{
	MusicVolume = Value;
}

void UAgGameUserSettings::ApplyNonResolutionSettings()
{
	ApplyAudioSettings();
	Super::ApplyNonResolutionSettings();
}

void UAgGameUserSettings::ApplyAudioSettings()
{
	/*static ConstructorHelpers::FObjectFinder<Audio::IAudioMixer> AudioMixerFinder(TEXT("/Game/Audio/SoundMix"));
	if (AudioMixerFinder.Succeeded()) {
		//AudioMixerFinder->
	}*/
}