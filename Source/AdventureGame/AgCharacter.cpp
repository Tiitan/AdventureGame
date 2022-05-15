#include "AgCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/TargetableComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "EnhancedActionKeyMapping.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

AAgCharacter::AAgCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	WalkMultiplier = 0.3;

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	TargetScanDistance = 1000;
	TargetScanRadius = 300;
	
	SetActorTickEnabled(true);
}

const UInputAction* FindAction(FString ActionName, const UInputMappingContext* Context)
{
	const TArray<FEnhancedActionKeyMapping>& keyMappings = Context->GetMappings();
	for (const FEnhancedActionKeyMapping& KeyMapping: keyMappings)
		if (KeyMapping.Action->GetName() == ActionName)
			return KeyMapping.Action;
	return nullptr;
}

void AAgCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(InputMappingContext)
	const UInputMappingContext* Context = InputMappingContext.Get();
	
	const APlayerController* PlayerController = Cast<APlayerController>(Controller);
	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	SubSystem->AddMappingContext(Context, 0);
	

	UEnhancedInputComponent* PlayerEnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	const UInputAction* JumpAction = FindAction("Jump", Context);
	PlayerEnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
	PlayerEnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

	const UInputAction* WalkAction = FindAction("Walk", Context);
	PlayerEnhancedInputComponent->BindAction(WalkAction, ETriggerEvent::Started, this, &AAgCharacter::StartWalkMode);
	PlayerEnhancedInputComponent->BindAction(WalkAction, ETriggerEvent::Completed, this, &AAgCharacter::StartWalkMode);

	const UInputAction* LockCameraAction = FindAction("LockCamera", Context);
	PlayerEnhancedInputComponent->BindAction(LockCameraAction, ETriggerEvent::Started, this, &AAgCharacter::LockCamera);
	PlayerEnhancedInputComponent->BindAction(LockCameraAction, ETriggerEvent::Completed, this, &AAgCharacter::LockCamera);

	PlayerEnhancedInputComponent->BindAction(FindAction("MoveForward", Context), ETriggerEvent::Triggered, this, &AAgCharacter::MoveForward);
	PlayerEnhancedInputComponent->BindAction(FindAction("MoveRight", Context), ETriggerEvent::Triggered, this, &AAgCharacter::MoveRight);
	
	PlayerEnhancedInputComponent->BindAction(FindAction("Turn", Context), ETriggerEvent::Triggered, this, &AAgCharacter::Turn);
	//PlayerInputComponent->BindAxis("TurnRate", this, &ATpsTestCharacter::TurnAtRate);
	PlayerEnhancedInputComponent->BindAction(FindAction("LookUp", Context), ETriggerEvent::Triggered, this, &AAgCharacter::LookUp);
	//PlayerInputComponent->BindAxis("LookUpRate", this, &ATpsTestCharacter::LookUpAtRate);

	PlayerEnhancedInputComponent->BindAction(FindAction("Action", Context), ETriggerEvent::Started, this, &AAgCharacter::Action);
}

void AAgCharacter::NotifyTargetAcquiredChanged(const AActor* Target, bool IsAcquired)
{
	if (Target != nullptr)
	{
		TArray<UTargetableComponent*> Targetables;
		Target->GetComponents<UTargetableComponent>(Targetables);
		for (const auto& Targetable : Targetables)
			Targetable->OnAcquiredChanged(IsAcquired);
	}
}

void AAgCharacter::TryUpdateTarget()
{
	auto TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);

	TArray<FHitResult> HitResults;
	const FTransform CameraTransform = FollowCamera->GetComponentTransform();
	const FVector Start = CameraBoom->GetComponentTransform().GetLocation();
	const FVector End = Start + CameraTransform.GetRotation().Vector() * TargetScanDistance;
	
	UKismetSystemLibrary::SphereTraceMulti(
		GetWorld(), Start, End, TargetScanRadius, TraceTypeQuery1, false,
		{this}, EDrawDebugTrace::None, HitResults, true);

	AActor* BestTarget = nullptr;
	for (const auto& HitResult : HitResults)
	{
		// Check if hit actor is Targetable
		const auto TagInterface = Cast<IGameplayTagAssetInterface>(HitResult.GetActor());
		static auto TargetTag = FGameplayTag::RequestGameplayTag("Targetable");
		if (TagInterface && TagInterface->HasMatchingGameplayTag(TargetTag))
		{
			BestTarget = HitResult.GetActor();
			break; // TODO: multi target priority
		}
	}

	// Update target
	if (CurrentTarget != BestTarget)
	{
		NotifyTargetAcquiredChanged(CurrentTarget, false);
		CurrentTarget = BestTarget;
		NotifyTargetAcquiredChanged(CurrentTarget, true);
			
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Target update"));
	}
}

void AAgCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsCameraLocked)
		TryUpdateTarget();
	else if (CurrentTarget)
	{
		// Lock camera on target
		const FVector TargetPosition = CurrentTarget->GetActorLocation();
		const FVector CameraPosition = CameraBoom->GetComponentTransform().GetLocation();
		const FVector Direction = TargetPosition - CameraPosition;
		const FRotator RotationToTarget = UKismetMathLibrary::MakeRotFromXZ(Direction, FVector::UpVector);
		const FRotator AppliedRotation =
			FMath::RInterpTo(Controller->GetControlRotation(), RotationToTarget, 12 * DeltaTime, 1);
		Controller->SetControlRotation(AppliedRotation);
	}
		
}

void AAgCharacter::StartWalkMode(const FInputActionValue& Value)
{
	bIsWalking = Value.GetMagnitude() > 0.5f;
}

void AAgCharacter::LockCamera(const FInputActionValue& Value)
{
	bIsCameraLocked = Value.GetMagnitude() > 0.5f;
}

void AAgCharacter::TurnAtRate(float Rate)
{
	Turn(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AAgCharacter::LookUpAtRate(float Rate)
{
	LookUp(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AAgCharacter::Turn(const FInputActionValue& Value)
{
	if (!bIsCameraLocked)
		APawn::AddControllerYawInput(Value.GetMagnitude());
}

void AAgCharacter::LookUp(const FInputActionValue& Value)
{
	if (!bIsCameraLocked)
		APawn::AddControllerPitchInput(Value.GetMagnitude());
}

void AAgCharacter::MoveForward(const FInputActionValue& Value)
{
	float NewMoveSpeed = Value.GetMagnitude();
	if ((Controller != nullptr) && (NewMoveSpeed != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		if (bIsWalking)
			NewMoveSpeed *= WalkMultiplier;
		AddMovementInput(Direction, NewMoveSpeed);
	}
}

void AAgCharacter::MoveRight(const FInputActionValue& Value)
{
	float NewMoveSpeed = Value.GetMagnitude();
	if ( (Controller != nullptr) && (NewMoveSpeed != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		if (bIsWalking)
			NewMoveSpeed *= WalkMultiplier;
		// add movement in that direction
		AddMovementInput(Direction, NewMoveSpeed);
	}
}

void AAgCharacter::OnWalkingOffLedge_Implementation(const FVector& PreviousFloorImpactNormal,
	const FVector& PreviousFloorContactNormal, const FVector& PreviousLocation, float TimeDelta)
{
	Super::OnWalkingOffLedge_Implementation(
		PreviousFloorImpactNormal, PreviousFloorContactNormal, PreviousLocation, TimeDelta);
	if (!bIsWalking)
	{
		Jump();
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("JUMP"));
	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("WALK FALL"));
	}
}

bool AAgCharacter::CanJumpInternal_Implementation() const
{
	return true;
}

void AAgCharacter::Equip(UEquipment* Equipment)
{
	if (EquipmentSlot1 != nullptr)
	{
		EquipmentSlot1->SetEquiped(false);
		if (EquippedActor != nullptr)
		{
			EquippedActor->Destroy();
			EquippedActor = nullptr;
		}
		
	}
	EquipmentSlot1 = Equipment;
	if (Equipment)
	{
		Equipment->SetEquiped(true);

		const FVector Location(0.0f, 0.0f, 0.0f);
		const FRotator Rotation(0.0f, 0.0f, 0.0f);
		EquippedActor = GetWorld()->SpawnActor<AActor>(Equipment->EquipedActor.Get(), Location, Rotation);
		if (EquippedActor != nullptr)
			EquippedActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
				Equipment->AttachSocketName);
		else if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
				TEXT("EquippedActor could not be to instanciated."));
		}
	}


	if (GEngine)
	{
		FString DebugText = TEXT("Unequip");
		if (Equipment)
			DebugText = FString::Printf(TEXT("Equip %s"), *Equipment->DisplayName.ToString());
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, DebugText);
	}
}

// TODO: move this in inventory system
void AAgCharacter::ActivateItem(UItem* Item)
{
	Item->Activate(this);
}

void AAgCharacter::Action()
{
	if (EquipmentSlot1)
	{
		EquipmentSlot1->Use(this);
	}

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Attack"));
}

// IGameplayTagAssetInterface implementation
void AAgCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer = GameplayTagContainer;
}