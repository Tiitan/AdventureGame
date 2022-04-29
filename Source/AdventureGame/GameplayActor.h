#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "GameplayTagAssetInterface.h"
#include "GameplayActor.generated.h"

/*
** AGameplayActor:
** Base class for all actors that use a GameplayTag. (not character/pawn)
*/
UCLASS()
class ADVENTUREGAME_API AGameplayActor : public AActor, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FGameplayTagContainer GameplayTagContainer;
	
public:	
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
};
