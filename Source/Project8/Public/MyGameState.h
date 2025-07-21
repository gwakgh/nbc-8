#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MyGameState.generated.h"

UCLASS()
class PROJECT8_API AMyGameState : public AGameState
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
	void StartWave();
	void EndWave();
	void OnWaveTimeUp();
public:
	AMyGameState();

	FTimerHandle LevelTimerHandle;
	FTimerHandle WaveTimerHandle;
	FTimerHandle HUDUpdateHandle;


	UPROPERTY(VisibleAnywhere, Category = "Wave")
	int32 CurrentWave;

	UPROPERTY(EditDefaultsOnly, Category = "Wave")
	int32 MaxWaves;
	
	UPROPERTY(EditDefaultsOnly, Category = "Wave")
	float WaveDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coin")
	int32 SpawnedCoinCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coin")
	int32 CollectedCoinCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	int32 CurrentLevelIndex;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	int32 MaxLevels;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	float LevelDuration;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	TArray<FName> LevelNames;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
	int32 Score;

	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetScore() const;

	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnGameOver();
	
	void StartLevel();
	void EndLevel();

	void OnLevelTimeUp();
	void OnCoinCollected();

	void UpdateHUD();

protected:
};
