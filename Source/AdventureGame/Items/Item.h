#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Item.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemUpdated);

/*
** Item: base class for anything that can be picked up and put in inventory
*/
UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class ADVENTUREGAME_API UItem : public UPrimaryDataAsset
{
	GENERATED_BODY()

protected:
	// Refresh UI delegate
	UPROPERTY(BlueprintAssignable, Category = "Item")
	FOnItemUpdated OnItemUpdated;
	
public:

	UItem();

	// Text to activate item (Use, Equip, Eat, etc)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	FText ActivateActionText;

	// Mesh used to display the item  when it is on the ground
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	TSoftObjectPtr<UStaticMesh> PickupMesh;

	// Image used to display the item  when it is in inventory
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	TSoftObjectPtr<UTexture2D> Thumbnail;

	// Name displayed on pickup and in inventory
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	FText DisplayName;

	// Item description
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (MultiLine = true))
	FText Description;

	UFUNCTION(BlueprintCallable)
	virtual void Activate(class AAgCharacter* Character);

	UFUNCTION(BlueprintImplementableEvent)
	void OnActivate(class AAgCharacter* Character);
};
