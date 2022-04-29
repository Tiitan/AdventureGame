#include "BasePopup.h"
#include "Engine/World.h"

void UBasePopup::NativeConstruct()
{
	Super::NativeConstruct();
	StartTime = Delay > 0 ? GetWorld()->GetRealTimeSeconds() : -1;
	InitialDelay = Delay;
}

void UBasePopup::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (StartTime > 0) // popup use a countdown
	{
		const float ElapsedTime = GetWorld()->GetRealTimeSeconds() - StartTime;
		const float TimeLeft = InitialDelay - ElapsedTime;
		if (Delay > TimeLeft)
			TimerTick();
	}
}

void UBasePopup::TimerTick()
{
	Delay -= 1;
	OnTimerTicked();
	if (Delay <= 0)
		OnPopupFinished.Broadcast(false);
}