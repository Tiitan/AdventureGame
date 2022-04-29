#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Items/Equipment.h"
#include "GameplayTagContainer.h"
#include "GameplayTagAssetInterface.h"
#include "InputMappingContext.h"
#include "AgCharacter.generated.h"

UCLASS(config=Game)
class AAgCharacter : public ACharacter, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FGameplayTagContainer GameplayTagContainer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UInputMappingContext> InputMappingContext;
	
	// CameraBoom help position the camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	// Game camera, used for main viewport
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	bool bIsWalking;

	// actor currently instantiated and attached to the player's skeletalmesh. logic in EquipmentSlot1
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AActor* EquippedActor;
	
	// Equipment used when clicking and mesh attached to the character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UEquipment* EquipmentSlot1;

	bool bIsCameraLocked;
	
	// Player's target
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AActor* CurrentTarget;

	// Helper code to find the TargetableComponent and send notification.
	static void NotifyTargetAcquiredChanged(const AActor* Target, bool IsAcquired);
	
public:
	AAgCharacter();

	UPROPERTY(EditAnywhere)
	float WalkMultiplier;

	UPROPERTY(EditAnywhere)
	float TargetScanRadius;

	UPROPERTY(EditAnywhere)
	float TargetScanDistance;
	
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	// Main update. Linetrace to select target
	virtual void Tick(float DeltaTime) override;
	
protected:
	
	void StartWalkMode(const FInputActionValue& Value);
	void LockCamera(const FInputActionValue& Value);

	// Start an attack, called when action button is pressed (left mouse).
	void Action();
	
	void MoveForward(const FInputActionValue& Value);
	void MoveRight(const FInputActionValue& Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	void Turn(const FInputActionValue& Value);
	void LookUp(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void TryUpdateTarget();
	// End of APawn interface

	virtual bool CanJumpInternal_Implementation() const;
	virtual void OnWalkingOffLedge_Implementation(const FVector& PreviousFloorImpactNormal, const FVector& PreviousFloorContactNormal, const FVector& PreviousLocation, float TimeDelta) override;

public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	void Equip(UEquipment* Equipment);

	UFUNCTION(BlueprintCallable, Category = "Items")
	void ActivateItem(class UItem* Item);

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
};
