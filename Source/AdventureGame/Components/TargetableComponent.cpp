#include "TargetableComponent.h"
#include "GenericPlatform/GenericPlatformMath.h"

UTargetableComponent::UTargetableComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	CursorFrequency = 2;
	CursorAmplitude = 30;
}

void UTargetableComponent::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(false);
}

void UTargetableComponent::OnAcquiredChanged(bool IsTargeted)
{
	if (IsTargeted && CursorActor)
	{
		CursorActorInstance = GetWorld()->SpawnActor<AActor>(CursorActor.Get(), GetComponentTransform());
		CursorActorInstance->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale);

		SetComponentTickEnabled(true);
	}
	else if (!IsTargeted && CursorActorInstance)
	{
		CursorActorInstance->Destroy();
		CursorActorInstance = nullptr;
		SetComponentTickEnabled(false);
	}
}

void UTargetableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (CursorActorInstance)
	{
		const float VerticalOffsetAnimation =
			CursorAmplitude * FGenericPlatformMath::Cos(GetWorld()-> GetTimeSeconds() * CursorFrequency);
		CursorActorInstance->SetActorRelativeLocation(FVector(0, 0, VerticalOffsetAnimation));
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("UTargetableComponent tick without CursorActorInstance"));
}

