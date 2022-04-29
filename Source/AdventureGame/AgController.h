#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AgController.generated.h"

/*
** CharacterPlayerController:
** - Pass input to character (todo)
** - Manage player inventory for persistance across respawn
*/
UCLASS()
class ADVENTUREGAME_API AAgController : public APlayerController
{
	GENERATED_BODY()

	// Player inventory
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* Inventory;

public:
	AAgController();
};
