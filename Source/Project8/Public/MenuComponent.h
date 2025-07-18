#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MenuComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT8_API UMenuComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMenuComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> HUDClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	UUserWidget* HUDInstance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainMenu")
	TSubclassOf<UUserWidget> MainMenuClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MainMenu")
	UUserWidget* MainMenuInstance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PauseMenu")
	TSubclassOf<UUserWidget> PauseMenuClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PauseMenu")
	UUserWidget* PauseMenuInstance;
	
	UFUNCTION(BlueprintPure, Category = "UI")
	UUserWidget* GetHUDWidget() const;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowPauseMenu();
	
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowGameHUD();
	
	UFUNCTION(BlueprintCallable, Category = "MainMenu")
	void ShowMainMenu();
	
	UFUNCTION(BlueprintCallable, Category = "PauseMenu")
	void ResumeGame();
	
	UFUNCTION(BlueprintCallable, Category = "PauseMenu")
	void GobackToMainMenu();
protected:
	virtual void BeginPlay() override;
	
	void ClearAllWidgets();
		
};
