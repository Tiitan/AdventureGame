#include "Item.h"

UItem::UItem()
{
	ActivateActionText = FText::FromString("Use");
	DisplayName = FText::FromString("Item");
}

void UItem::Activate(AAgCharacter* Character)
{
	OnActivate(Character);
}
