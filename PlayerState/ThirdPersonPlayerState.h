// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ThirdPersonPlayerState.generated.h"

UCLASS()
class TEAM3GP4_API AThirdPersonPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void TakeDamage(float InDamageAmount);

	UFUNCTION(BlueprintCallable)
	void AddHealth(float InHealthAmount);

	UFUNCTION(BlueprintCallable)
	float GetCurrentHealth() { return currentHealth; }
	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() { return maxHealth; }

	UFUNCTION(BlueprintCallable)
	void SetMaxHealth(float InNewMaxHealth) { maxHealth = InNewMaxHealth; }

	UFUNCTION(BlueprintCallable)
	float GetCurrentShield() { return currentShield; }

	UFUNCTION(BlueprintCallable)
	float GetMaxShield() { return maxShield; }	

	UFUNCTION(BlueprintCallable)
	void SetMaxShield(float InNewMaxShield) { maxShield = InNewMaxShield; }

	UFUNCTION(BlueprintCallable)
	float GetCurrentOverShield() { return currentOverShield; }

	UFUNCTION(BlueprintCallable)
	float GetMaxOverShield() { return maxOverShield; }

	UFUNCTION(BlueprintCallable)
	void SetMaxOverShield(float InNewMaxOverShield) { maxOverShield = InNewMaxOverShield; }

	UFUNCTION(BlueprintCallable)
	void SetOverShieldRegenPerTick(float InNewOverShieldRegenPerTick) { overShieldRegenPerTick = InNewOverShieldRegenPerTick; }

	UFUNCTION(BlueprintCallable)
	void SetOverShieldTick(float InNewOverShieldRegenTick) { overShieldRegenTick = InNewOverShieldRegenTick; }

	UFUNCTION(BlueprintCallable)
	void AddOverShield(float InAddOverShieldAmount);

	UFUNCTION(BlueprintCallable)
	void EnterOverShieldZone();

	UFUNCTION(BlueprintCallable)
	void ExitOverShieldZone();

	UFUNCTION(BlueprintCallable)
	void StartShieldRegen();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float maxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere, Category = "Health")
	float currentHealth;
	
	UPROPERTY(EditDefaultsOnly, Category = "Shield")
	float maxShield = 50.0f;

	UPROPERTY(VisibleAnywhere, Category = "Shield")
	float currentShield = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Shield")
	float shieldRegenDelay = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Shield")
	float shieldRegenTick = 0.05f;

	UPROPERTY(EditDefaultsOnly, Category = "Shield")
	float shieldRegenPerTick = 0.1f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shield")
	bool bIsRegeneratingShield = false;

	UPROPERTY(EditDefaultsOnly, Category = "OverShield")
	float maxOverShield = 50.0f;

	UPROPERTY(VisibleAnywhere, Category = "OverShield")
	float currentOverShield = 0;

	UPROPERTY(EditDefaultsOnly, Category = "OverShield")
	float overShieldRegenTick = 0.05f;

	UPROPERTY(EditDefaultsOnly, Category = "OverShield")
	float overShieldRegenPerTick = 0.2f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "OverShield")
	bool bInOverShieldZone = false;


private:
	FTimerHandle ShieldRegenDelayHandle;
	FTimerHandle ShieldRegenTickHandle;

	FTimerHandle OverShieldRegenTickHandle;

	void StopShieldRegen();
	void TickShieldRegen();
	void RestartShieldRegenDelay();

	void StartOverShieldRegen();
	void StopOverShieldRegen();
	void TickOverShieldRegen();
};
