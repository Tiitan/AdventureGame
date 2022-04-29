// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Equipment.generated.h"

/*
** Equipment: item that can be held in hand and used (weapon)
*/
UCLASS()
class ADVENTUREGAME_API UEquipment : public UItem
{
	GENERATED_BODY()
	
	// Is item currently equiped ? managed by character class. (readonly)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsEquiped;

public:
	// Actor used to display on the character when equipped
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	TSoftClassPtr<AActor> EquipedActor;

	// Socket to attach EquipMesh (usually "hand_r")
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	FName AttachSocketName;

	virtual void Activate(class AAgCharacter* Character) override;

	void SetEquiped(bool value);

	void Use(class AAgCharacter* Character);

	UFUNCTION(BlueprintImplementableEvent)
	void OnUse(class AAgCharacter* Character);
};
