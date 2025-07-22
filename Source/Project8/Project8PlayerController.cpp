#include "Project8PlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "Project8Character.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AProject8PlayerController::AProject8PlayerController()
{
    MenuComponent = CreateDefaultSubobject<UMenuComponent>(TEXT("MenuComponent"));
	
	bIsTouch = false;
	bMoveToMouseCursor = false;
    bIsMovementReversed = false;

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;

}

float AProject8PlayerController::GetReverseEffectRemainingTime() const
{
	return GetWorldTimerManager().GetTimerRemaining(TimerHandle_ReverseMovement);
}

void AProject8PlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	FString CurrentMapName = GetWorld()->GetMapName();
	if (CurrentMapName.Contains(TEXT("BasicMap")))
	{
		if (MenuComponent)
		{
			MenuComponent->ShowMainMenu();
		}
	}
}

void AProject8PlayerController::StartGame()
{
	if (UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		MyGameInstance->TotalScore = 0;
		MyGameInstance->CurrentLevelIndex = 0;
		MyGameInstance->CharacterHealth = 100.f;
	}
	SetGameInputMode();
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("AdvancedLevel"));
}

UMenuComponent* AProject8PlayerController::GetMenuComponent() const
{
	if (MenuComponent)
	{
		return MenuComponent;
	}
	
	UE_LOG(LogTemp, Error, TEXT("MenuComponent is null!"));
	return nullptr;
}

void AProject8PlayerController::ActivateReverseMovement(float Duration)
{
	bIsMovementReversed = true;

	AProject8Character* PlayerCharacter = Cast<AProject8Character>(GetPawn());
	if (PlayerCharacter)
	{
		PlayerCharacter->OnReverseStatusChanged.Broadcast(true);
	}

	GetWorldTimerManager().ClearTimer(TimerHandle_ReverseMovement);

	GetWorldTimerManager().SetTimer(
	   TimerHandle_ReverseMovement, 
	   this, 
	   &AProject8PlayerController::DeactivateReverseMovement, 
	   Duration, 
	   false);
}

void AProject8PlayerController::DeactivateReverseMovement()
{
	AProject8Character* PlayerCharacter = Cast<AProject8Character>(GetPawn());
	if (PlayerCharacter)
	{
		PlayerCharacter->OnReverseStatusChanged.Broadcast(false);
	}
    bIsMovementReversed = false;
}

void AProject8PlayerController::SetGameInputMode()
{
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);
	
	SetPause(false);
}

void AProject8PlayerController::SetUIInputMode()
{
	SetInputMode(FInputModeUIOnly());
	SetPause(true);
}


void AProject8PlayerController::TogglePauseMenu()
{
	if (MenuComponent)
	{
		MenuComponent->ShowPauseMenu();
	}
}

void AProject8PlayerController::OnInputStarted()
{
	StopMovement();
}

void AProject8PlayerController::OnSetDestinationTriggered()
{
	FollowTime += GetWorld()->GetDeltaSeconds();
	
	FHitResult Hit;
	bool bHitSuccessful = false;
	if (bIsTouch)
	{
		bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
	}
	else
	{
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	}

	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}
	
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		
		if (bIsMovementReversed)
		{
			WorldDirection *= -1.0f;
		}
		
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
}

void AProject8PlayerController::OnSetDestinationReleased()
{
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
    
	FollowTime = 0.f;
	bIsTouch = false;
}


void AProject8PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &AProject8PlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &AProject8PlayerController::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &AProject8PlayerController::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &AProject8PlayerController::OnSetDestinationReleased);

		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &AProject8PlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &AProject8PlayerController::OnTouchTriggered);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &AProject8PlayerController::OnTouchReleased);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &AProject8PlayerController::OnTouchReleased);
		
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Triggered, this, &AProject8PlayerController::TogglePauseMenu);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}
void AProject8PlayerController::OnTouchTriggered()
{
	bIsTouch = true;
	OnSetDestinationTriggered();
}

void AProject8PlayerController::OnTouchReleased()
{
	bIsTouch = false;
	OnSetDestinationReleased();
}