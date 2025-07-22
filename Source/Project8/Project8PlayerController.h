#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "MenuComponent.h"
#include "Project8PlayerController.generated.h"

class UNiagaraSystem;
class UInputMappingContext;
class UInputAction;
class UAudioComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

enum class EIntroState : uint8
{
	None,       
	Blending,   
	ViewingPaused
};

UCLASS(abstract)
class AProject8PlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SetDestinationClickAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SetDestinationTouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	UInputAction* PauseAction;
	
	uint32 bMoveToMouseCursor : 1;
	uint32 bIsTouch : 1;
	FVector CachedDestination;
	float FollowTime = 0.0f;

	bool bIsMovementReversed;
	
    void DeactivateReverseMovement();

	FTimerHandle TimerHandle_ReverseMovement;
public:

	AProject8PlayerController();
	
	UFUNCTION(BlueprintPure, Category = "Status")
	float GetReverseEffectRemainingTime() const;
	
	UFUNCTION(BlueprintCallable, Category = "MainMenu")
	void StartGame();

	UFUNCTION(BlueprintCallable, Category = "UI")
	UMenuComponent* GetMenuComponent() const;
	
    void ActivateReverseMovement(float Duration);
	
	void SetGameInputMode();

	void SetUIInputMode();
	
	
protected:
	UPROPERTY(VisibleAnywhere, Category="UI")
	UMenuComponent* MenuComponent;
	
	virtual void SetupInputComponent() override;
	virtual void BeginPlay();
	
	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();
	void OnTouchTriggered();
	void OnTouchReleased();
	void TogglePauseMenu();
};


