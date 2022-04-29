#include "PopupSpawner.h"

#include "BasePopup.h"

UPopupSpawner::UPopupSpawner(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer),
	WorldContextObject(nullptr),
	Active(false),
	Popup(nullptr)
{
}


UPopupSpawner* UPopupSpawner::PopupSpawner(const UObject* WorldContextObject, UBasePopup* popup)
{
	UPopupSpawner* Node = NewObject<UPopupSpawner>();
	Node->WorldContextObject = WorldContextObject;
	Node->Popup = popup;
	return Node;
}

void UPopupSpawner::Activate()
{
	if (nullptr == WorldContextObject)
	{
		FFrame::KismetExecutionMessage(TEXT("Invalid WorldContextObject. Cannot execute PopupSpawner."), ELogVerbosity::Error);
		return;
	}
	if (Active) 
	{
		FFrame::KismetExecutionMessage(TEXT("PopupSpawner is already running."), ELogVerbosity::Warning);
		return;
	}
	if (Popup == nullptr)
	{
		FFrame::KismetExecutionMessage(TEXT("PopupSpawner's Popup cannot be null."), ELogVerbosity::Warning);
		return;
	}

	Active = true;
	Popup->AddToViewport();
	Popup->OnPopupFinished.AddDynamic(this, &UPopupSpawner::Finish);
}

void UPopupSpawner::Finish(const bool Result)
{
	Popup->RemoveFromParent();
	Finished.Broadcast(Result);
	Active = false;
}