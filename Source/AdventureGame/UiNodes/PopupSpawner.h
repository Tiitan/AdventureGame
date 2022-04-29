#pragma once

#include "BasePopup.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "PopupSpawner.generated.h"

UCLASS()
class UPopupSpawner : public UBlueprintAsyncActionBase
{
	GENERATED_UCLASS_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnPopupFinished Finished;


	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Popup")
	static UPopupSpawner* PopupSpawner(const UObject* WorldContextObject, UBasePopup* Popup);
	
	virtual void Activate() override;

private:
	const UObject* WorldContextObject;
	bool Active;
	
	UPROPERTY()
	UBasePopup* Popup;
	
	UFUNCTION()
	void Finish(const bool Result);
};