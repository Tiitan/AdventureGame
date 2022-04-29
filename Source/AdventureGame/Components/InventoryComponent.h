#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ADVENTUREGAME_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

	// Initial list of item (for testing)
	UPROPERTY(EditDefaultsOnly, Instanced)
	TArray<class UItem*> DefaultItems;

	// Refresh UI delegate
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdated OnInventoryUpdated;

public:	
	UInventoryComponent();

	bool AddItem(class UItem* Item);
	bool RemoveItem(class UItem* Item);

	// Readonly list of items in inventory
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items")
	TArray<class UItem*> Items;

protected:
	virtual void BeginPlay() override;
		
};
