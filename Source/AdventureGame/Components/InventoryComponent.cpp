#include "InventoryComponent.h"

UInventoryComponent::UInventoryComponent()
{
}


void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	for (auto& Item : DefaultItems)
		AddItem(Item);
}

bool UInventoryComponent::AddItem(UItem* Item)
{
	Items.Add(Item);
	OnInventoryUpdated.Broadcast();
	return true;
}

bool UInventoryComponent::RemoveItem(UItem* Item)
{
	Items.Remove(Item);
	OnInventoryUpdated.Broadcast();
	return false;
}
