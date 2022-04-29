#pragma once

#include "GameFramework/GameUserSettings.h"
#include "AgGameUserSettings.generated.h"

UCLASS(config=GameUserSettings)
class UAgGameUserSettings: public UGameUserSettings
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure)
	static UAgGameUserSettings* GetUserSettings();

	UFUNCTION(BlueprintPure, Category = Settings)
	float GetMasterVolume() const;

	UFUNCTION(BlueprintPure, Category = Settings)
	float GetEffectVolume() const;

	UFUNCTION(BlueprintPure, Category = Settings)
	float GetMusicVolume() const;

	UFUNCTION(BlueprintCallable, Category = Settings)
	void SetMasterVolume(const float Value);

	UFUNCTION(BlueprintCallable, Category = Settings)
	void SetEffectVolume(const float Value);
	
	UFUNCTION(BlueprintCallable, Category = Settings)
	void SetMusicVolume(const float Value);

protected:
	UPROPERTY(config)
	float MasterVolume;
	
	UPROPERTY(config)
	float EffectVolume;
	
	UPROPERTY(config)
	float MusicVolume;
};
