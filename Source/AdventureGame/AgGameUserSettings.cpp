#include "AgGameUserSettings.h"

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
