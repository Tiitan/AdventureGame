#pragma once
#include "Blueprint/UserWidget.h"
#include "EngineMinimal.h"

#include "BasePopup.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPopupFinished, bool, Result);

UCLASS()
class UBasePopup: public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "Default", meta = (ExposeOnSpawn = "true"))
	int Delay;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnPopupFinished OnPopupFinished;

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnTimerTicked();

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
private:
	float StartTime;
	int InitialDelay;
	
	// decrease delay and call OnTimerTicked every second then broadcast OnPopupFinished(false) when delay reach 0
	// Called from NativeTick using real time countdown instead of a timer because the game can be paused.
	UFUNCTION()
	void TimerTick();
};
