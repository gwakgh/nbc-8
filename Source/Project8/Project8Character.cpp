#include "Project8Character.h"

#include "MyGameState.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"


AProject8Character::AProject8Character()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));

	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->TargetArmLength = 1400.f;
	CameraBoom->SetWorldRotation(FRotator(0.f, -50.f, 45.f));
	CameraBoom->bDoCollisionTest = false;
	
	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(GetMesh());
	OverheadWidget->SetWidgetSpace(EWidgetSpace::Screen);

	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));

	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	
	MaxHealth = 100.0f;
	Health = MaxHealth;
	
}

void AProject8Character::BeginPlay()
{
	Super::BeginPlay();
	
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		OriginalWalkSpeed = MoveComp->MaxWalkSpeed;
	}
	
	UpdateOverheadHP();
}

float AProject8Character::GetSlowEffectRemainingTime() const
{
	return GetWorldTimerManager().GetTimerRemaining(TimerHandle_SlowEffect);
}

int32 AProject8Character::GetHealth() const
{
	return Health;
}

void AProject8Character::SetHealth(float NewHealth)
{
	Health = NewHealth;
}

int32 AProject8Character::GetMaxHealthBP() const
{
	return MaxHealth;
}

void AProject8Character::AddHealth(float Amount)
{
	Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);
    OnHealthChanged.Broadcast(Amount);
	UpdateOverheadHP();
}

void AProject8Character::ApplySlowEffect(float SlowAmount, float Duration)
{
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		OnSlowStatusChanged.Broadcast(true);
		MoveComp->MaxWalkSpeed = FMath::Max(0.f, OriginalWalkSpeed - SlowAmount);
		GetWorldTimerManager().ClearTimer(TimerHandle_SlowEffect);
		GetWorldTimerManager().SetTimer(
			TimerHandle_SlowEffect,
			this,
			&AProject8Character::RestoreMovementSpeed,
			Duration,
			false);
	}
}

void AProject8Character::RestoreMovementSpeed()
{
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->MaxWalkSpeed = OriginalWalkSpeed;
		OnSlowStatusChanged.Broadcast(false);
	}
}

void AProject8Character::OnDeath()
{
	AMyGameState* MyGameState = GetWorld() ? GetWorld()->GetGameState<AMyGameState>() : nullptr;
	if (MyGameState)
	{
		MyGameState->OnGameOver();
	}
}

float AProject8Character::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
	UpdateOverheadHP();
	UE_LOG(LogTemp, Warning, TEXT("Health decreased to: %f"), Health);

	if (Health <= 0.0f)
	{
		OnDeath();
	}

	return ActualDamage;
}
void AProject8Character::UpdateOverheadHP()
{
	if (!OverheadWidget) return;
	
	UUserWidget* OverheadWidgetInstance = OverheadWidget->GetUserWidgetObject();
	if (!OverheadWidgetInstance) return;
	
	if (UTextBlock* HPText = Cast<UTextBlock>(OverheadWidgetInstance->GetWidgetFromName(TEXT("OverHeadHP"))))
	{
		HPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), Health, MaxHealth)));
	}
}