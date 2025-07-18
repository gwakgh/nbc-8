#include "MenuComponent.h"
#include "Project8PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

UMenuComponent::UMenuComponent()
	: HUDInstance(nullptr),
	  HUDClass(nullptr),
	  MainMenuInstance(nullptr),
	  MainMenuClass(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UMenuComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UMenuComponent::ShowGameHUD()
{
	ClearAllWidgets();

	if (!HUDClass) return;
	APlayerController* PC = Cast<APlayerController>(GetOwner());
	if (PC)
	{
		AProject8PlayerController* P8Controller = Cast<AProject8PlayerController>(PC);
		if (P8Controller)
		{
			HUDInstance = CreateWidget<UUserWidget>(P8Controller, HUDClass);
			if (HUDInstance)
			{
				HUDInstance->AddToViewport();
				P8Controller->SetInputMode(FInputModeGameOnly());
			}
			
		}
	}
}

void UMenuComponent::ShowMainMenu()
{
	ClearAllWidgets();

	if (!MainMenuClass) return;
	APlayerController* PC = Cast<APlayerController>(GetOwner());
	if (PC)
	{
		AProject8PlayerController* P8Controller = Cast<AProject8PlayerController>(PC);
		if (P8Controller)
		{
			MainMenuInstance = CreateWidget<UUserWidget>(P8Controller, MainMenuClass);
			if (!MainMenuInstance) return;
			MainMenuInstance->AddToViewport();
			P8Controller->SetPause(true);
			P8Controller->SetInputMode(FInputModeUIOnly());
		}
	}
}
void UMenuComponent::ShowPauseMenu()
{
	ClearAllWidgets();

	if (!PauseMenuClass) return;
	
	APlayerController* PC = Cast<APlayerController>(GetOwner());
	if (PC)
	{
		AProject8PlayerController* P8Controller = Cast<AProject8PlayerController>(PC);
		if (P8Controller)
		{
			PauseMenuInstance = CreateWidget<UUserWidget>(P8Controller, PauseMenuClass);
			if (!PauseMenuInstance) return;

			PauseMenuInstance->AddToViewport();
			P8Controller->SetPause(true);
			P8Controller->SetInputMode(FInputModeUIOnly());
		}
	}
}

void UMenuComponent::ResumeGame()
{
	ClearAllWidgets();
	APlayerController* PC = Cast<APlayerController>(GetOwner());
	if (PC)
	{
		AProject8PlayerController* P8Controller = Cast<AProject8PlayerController>(PC);
		if (P8Controller)
		{
			P8Controller->SetPause(false);
			P8Controller->SetInputMode(FInputModeGameOnly());
			ShowGameHUD();
		}
	}
}

void UMenuComponent::GobackToMainMenu()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("BasicMap"));
	ShowMainMenu();
}

UUserWidget* UMenuComponent::GetHUDWidget() const
{
	return HUDInstance;
}

void UMenuComponent::ClearAllWidgets()
{
	if (MainMenuInstance) { MainMenuInstance->RemoveFromParent(); MainMenuInstance = nullptr; }
	if (PauseMenuInstance) { PauseMenuInstance->RemoveFromParent(); PauseMenuInstance = nullptr; }
	if (HUDInstance) { HUDInstance->RemoveFromParent(); HUDInstance = nullptr; }
}