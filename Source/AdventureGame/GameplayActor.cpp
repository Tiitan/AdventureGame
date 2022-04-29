#include "GameplayActor.h"

// IGameplayTagAssetInterface implementation
void AGameplayActor::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer = GameplayTagContainer;
}