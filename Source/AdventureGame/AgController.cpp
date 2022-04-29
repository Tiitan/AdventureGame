#include "AgController.h"
#include "Components/InventoryComponent.h"

AAgController::AAgController()
{
	Inventory = CreateDefaultSubobject<UInventoryComponent>("Inventory");
}
