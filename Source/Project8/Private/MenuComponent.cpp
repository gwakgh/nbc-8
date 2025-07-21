#include "MenuComponent.h"

#include "MyGameInstance.h"
#include "Project8PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Project8Character.h"

UMenuComponent::UMenuComponent()
	: HUDInstance(nullptr),
	  HUDClass(nullptr),
	  MainMenuInstance(nullptr),
	  MainMenuClass(nullptr),
	  GameOverMenuClass(nullptr),
	  GameOverMenuInstance(nullptr),
	  BlindEffectWidgetClass(nullptr),
	  BlindEffectWidgetInstance(nullptr)
{
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
				P8Controller->SetGameInputMode();
			}
		}
	}
}

void UMenuComponent::ShowBlindMenu(float Duration)
{
	if (!BlindEffectWidgetClass)	return;
	if (BlindEffectWidgetClass && !BlindEffectWidgetInstance)
	{
		APlayerController* PC = Cast<APlayerController>(GetOwner());
		if (PC)
		{
			AProject8PlayerController* P8Controller = Cast<AProject8PlayerController>(PC);
			if (P8Controller)
			{
		
				AProject8Character* PlayerCharacter = Cast<AProject8Character>(P8Controller->GetPawn());
				if (PlayerCharacter)
				{
					PlayerCharacter->OnBlindStatusChanged.Broadcast(true);
				}
				BlindEffectWidgetInstance = CreateWidget<UUserWidget>(P8Controller, BlindEffectWidgetClass);
				BlindEffectWidgetInstance->AddToViewport();

				FTimerHandle TimerHandle;
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UMenuComponent::DeactivateBlindEffect, Duration, false);
			}
		}
	}
}

void UMenuComponent::DeactivateBlindEffect()
{
	if (BlindEffectWidgetInstance)
	{
		APlayerController* PC = Cast<APlayerController>(GetOwner());
		if (PC)
		{
			AProject8PlayerController* P8Controller = Cast<AProject8PlayerController>(PC);
			if (P8Controller)
			{
				AProject8Character* PlayerCharacter = Cast<AProject8Character>(P8Controller->GetPawn());
				if (PlayerCharacter)
				{
					PlayerCharacter->OnBlindStatusChanged.Broadcast(false);
				}
			}
		}
		BlindEffectWidgetInstance->RemoveFromParent();
		BlindEffectWidgetInstance = nullptr;
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
			P8Controller->SetUIInputMode();
		}
	}
}
void UMenuComponent::ShowGameOverMenu()
{
	ClearAllWidgets();

	if (!GameOverMenuClass) return;
	APlayerController* PC = Cast<APlayerController>(GetOwner());
	if (PC)
	{
		AProject8PlayerController* P8Controller = Cast<AProject8PlayerController>(PC);
		if (P8Controller)
		{
			GameOverMenuInstance = CreateWidget<UUserWidget>(P8Controller, GameOverMenuClass);
			if (!GameOverMenuInstance) return;

			GameOverMenuInstance->AddToViewport();
			P8Controller->SetUIInputMode();
			
			UFunction* PlayAnimFunc = GameOverMenuInstance->FindFunction("PlayGameOverAnim");
			if (PlayAnimFunc)
			{
				GameOverMenuInstance->ProcessEvent(PlayAnimFunc, nullptr);
			}
			UTextBlock* TotalScoreText = Cast<UTextBlock>(GameOverMenuInstance->GetWidgetFromName(TEXT("TotalScoreText")));
			if (TotalScoreText)
			{
				if (UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(P8Controller)))
				{
					TotalScoreText->SetText(FText::FromString(FString::Printf(TEXT("Total Score: %d"), MyGameInstance->TotalScore)));
				}
			}
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
			P8Controller->SetUIInputMode();
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
			P8Controller->SetGameInputMode();
			ShowGameHUD();
		}
	}
}

void UMenuComponent::GobackToMainMenu()
{
	ShowMainMenu();
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("BasicMap"));
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

void UMenuComponent::Initialize(AProject8PlayerController* Controller)
{
    CachedController = Controller;
}