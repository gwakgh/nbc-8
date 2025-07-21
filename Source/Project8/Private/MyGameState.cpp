#include "MyGameState.h"
#include "MyGameInstance.h"
#include "SpawnVolume.h"
#include "CoinItem.h"
#include "Project8PlayerController.h"

#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"


AMyGameState::AMyGameState()
{
	Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	
	LevelDuration = 30.f;
	CurrentLevelIndex = 0;
	MaxLevels = 3;

	WaveDuration = 10.f;
	CurrentWave = 0;
	MaxWaves = 3;
	
	PrimaryActorTick.bCanEverTick = true;
}

void AMyGameState::BeginPlay()
{
	Super::BeginPlay();
	StartLevel();
	GetWorldTimerManager().SetTimer(
		HUDUpdateHandle,
		this,
		&AMyGameState::UpdateHUD,
		0.1f, true);
}

void AMyGameState::StartWave()
{
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (AProject8PlayerController* MyPlayerController = Cast<AProject8PlayerController>(PC))
		{
			MyPlayerController->GetMenuComponent()->ShowGameHUD();
		}
	}
	
	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	const int32 ItemToSpawn = (CurrentLevelIndex + 1) * 15;
	for (int32 i = 0; i < ItemToSpawn; i++)
	{
		if (FoundVolumes.Num() > 0)
		{
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
			if (SpawnVolume)
			{
				AActor* SpawndActor = SpawnVolume->SpawnRandomItem();
				if (SpawndActor && SpawndActor->IsA(ACoinItem::StaticClass()))
				{
					SpawnedCoinCount++;
				}
			}
		}
	}

	GetWorldTimerManager().SetTimer(
	   WaveTimerHandle,
	   this,
	   &AMyGameState::OnWaveTimeUp,
	   WaveDuration, false
	);
    
	UpdateHUD();
}

void AMyGameState::EndWave()
{
	GetWorldTimerManager().ClearTimer(WaveTimerHandle);
	CurrentWave++;

	if (CurrentWave > MaxWaves)
	{
		EndLevel();
	}
	else
	{
		StartWave();
	}
}

void AMyGameState::OnWaveTimeUp()
{
	EndWave();
}
void AMyGameState::StartLevel()
{
	CurrentWave = 1;
	StartWave();
	
	if (UGameInstance* GameInstance = GetWorld()->GetGameInstance())
	{
		UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
		if (MyGameInstance)
		{
			Score = MyGameInstance->TotalScore;
			CurrentLevelIndex = MyGameInstance->CurrentLevelIndex;
		}
	}
}

void AMyGameState::EndLevel()
{
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);
	if (UGameInstance* GameInstance = GetWorld()->GetGameInstance())
	{
		
		UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
		if (MyGameInstance)
		{
			AddScore(Score);
			CurrentLevelIndex++;
			MyGameInstance->CurrentLevelIndex = CurrentLevelIndex;
		}
		
	}

	if (CurrentLevelIndex >= MaxLevels)
	{
		OnGameOver();
		return;
	}
	
	if (LevelNames.IsValidIndex(CurrentLevelIndex))
	{
		UGameplayStatics::OpenLevel(GetWorld(), LevelNames[CurrentLevelIndex]);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Game Over! No more levels!"));
		OnGameOver();
	}
}

void AMyGameState::OnLevelTimeUp()
{
	EndLevel();
}

int32 AMyGameState::GetScore() const
{
	return Score;
}

void AMyGameState::AddScore(int32 Amount)
{
	if (UGameInstance* GameInstance = GetWorld()->GetGameInstance())
	{
		
		UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
		if (MyGameInstance)
		{
			MyGameInstance->AddToScore(Amount);
		}
		
	}
}

void AMyGameState::OnGameOver()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AProject8PlayerController* MyPlayerController = Cast<AProject8PlayerController>(PlayerController))
		{
			MyPlayerController->SetPause(true);
			MyPlayerController->GetMenuComponent()->ShowGameOverMenu();
		}
	}
}

void AMyGameState::OnCoinCollected()
{
	CollectedCoinCount++;
	UE_LOG(LogTemp, Warning, TEXT("Collected Coins: %d / %d"), CollectedCoinCount, SpawnedCoinCount);

	if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
	{
		EndWave();
	}
}

void AMyGameState::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AProject8PlayerController* MyPlayerController = Cast<AProject8PlayerController>(PlayerController))
		{
			
			if (UUserWidget* HUDWidget = MyPlayerController->GetMenuComponent()->GetHUDWidget())
			{
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("TimeValue"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(WaveTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
				}
				
				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("ScoreValue"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						if (UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance))
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), MyGameInstance->TotalScore)));
						}
					}
				}
				
				
				if (UTextBlock* LevelText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
				{
					LevelText->SetText(FText::FromString(FString::Printf(TEXT("Level %d"), CurrentLevelIndex + 1)));
				}
				
				if (UTextBlock* WaveText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("WaveValue"))))
				{
					WaveText->SetText(FText::FromString(FString::Printf(TEXT("Wave: %d / %d"), CurrentWave, MaxWaves)));
				}
			}
			
		}
	}
}
