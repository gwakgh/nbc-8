#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Project8Character.generated.h"

class UWidgetComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, ChangedAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlowStatusChangedSignature, bool, bIsSlowed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReverseStatusChangedSignature, bool, bIsReversed);

UCLASS(abstract)
class AProject8Character : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	void RestoreMovementSpeed();

	FTimerHandle TimerHandle_SlowEffect;
	
	float OriginalWalkSpeed;
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;
	
	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual void OnDeath();
	
	void UpdateOverheadHP();
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
public:

	AProject8Character();

	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* OverheadWidget;
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnSlowStatusChangedSignature OnSlowStatusChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnSlowStatusChangedSignature OnReverseStatusChanged;
	
	UFUNCTION(BlueprintPure, Category = "Status")
	float GetSlowEffectRemainingTime() const;
	
	UFUNCTION(BlueprintPure, Category = "Health")
	int32 GetHealth() const;
	
	UFUNCTION(BlueprintPure, Category = "Health")
	int32 GetMaxHealthBP() const;
	
	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddHealth(float Amount);
	
	UFUNCTION(BlueprintCallable, Category = "Speed")
    void ApplySlowEffect(float SlowAmount, float Duration);
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthChangedSignature OnHealthChanged;

	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	

};

