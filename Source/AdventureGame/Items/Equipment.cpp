#include "Equipment.h"
#include <AdventureGame/AgCharacter.h> // TODO: equipment shouldn't know about character

void UEquipment::Activate(class AAgCharacter* Character)
{
	Super::Activate(Character);
	if (Character)
	{
		Character->Equip(bIsEquiped ? nullptr : this);
	}
}

void UEquipment::SetEquiped(bool value)
{
	bIsEquiped = value;
	OnItemUpdated.Broadcast();
}

void UEquipment::Use(class AAgCharacter* Character)
{
	OnUse(Character);
}

