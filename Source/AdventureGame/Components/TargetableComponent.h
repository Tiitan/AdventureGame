#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "TargetableComponent.generated.h"


UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class ADVENTUREGAME_API UTargetableComponent : public USceneComponent
{
	GENERATED_BODY()
	
	// a time cosines is used to animate the cursor position. this is animation frequency
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float CursorAmplitude;
	
	// a time cosines is used to animate the cursor position. this is animation frequency
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float CursorFrequency;
	
	// instantiated cursor.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AActor* CursorActorInstance;
	
	// Actor used to display on the character when equipped
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSoftClassPtr<AActor> CursorActor;
	
public:
	UTargetableComponent();
	virtual void BeginPlay() override;

	// Called when the player focus this actor. Display a Selection cursor.
	void OnAcquiredChanged(bool IsTargeted);

	// Animate cursor. Only Tick when cursor is spawned.
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
