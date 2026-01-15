// Fill out your copyright notice in the Description page of Project Settings.


#include "ThirdPersonPlayerState.h"
#include "TimerManager.h"



void AThirdPersonPlayerState::BeginPlay()
{
	Super::BeginPlay();

	currentHealth = maxHealth;
	currentShield = maxShield;

}

void AThirdPersonPlayerState::TakeDamage(float InDamageAmount)
{

	float damageLeft = InDamageAmount;
	bool bDamageOnShieldOrHealth = false;

	if (currentOverShield > 0)
	{
		currentOverShield -= damageLeft;
		if (currentOverShield < 0)
		{
			damageLeft = -currentOverShield;
			currentOverShield = 0;
		}
		else
		{
			damageLeft = 0;
		}
	}


	if (damageLeft > 0 && currentShield > 0)
	{
		currentShield -= damageLeft;
		bDamageOnShieldOrHealth = true;

		if (currentShield < 0)
		{
			damageLeft = -currentShield;
			currentShield = 0;
		}
		else
		{
			damageLeft = 0;
		}
	}

	if (damageLeft > 0)
	{
		currentHealth -= damageLeft;
		bDamageOnShieldOrHealth = true;
	}

	if (bDamageOnShieldOrHealth)
	{
		StopShieldRegen();
		RestartShieldRegenDelay();

	}

	if (currentHealth <= 0.0f)
	{
		currentHealth = 0;

		// death stuff here
		UE_LOG(LogTemp, Warning, TEXT("Player Died"));

	}
}

void AThirdPersonPlayerState::AddHealth(float InHealthAmount)
{
	currentHealth += InHealthAmount;

	if (currentHealth >= maxHealth)
	{
		currentHealth = maxHealth;
	}
}

void AThirdPersonPlayerState::StartShieldRegen()
{
	if (currentShield >= maxShield || bIsRegeneratingShield == true)
	{
		return;
	}

	bIsRegeneratingShield = true;

	GetWorld()->GetTimerManager().SetTimer(ShieldRegenTickHandle, this, &AThirdPersonPlayerState::TickShieldRegen, shieldRegenTick, true);

}

void AThirdPersonPlayerState::StopShieldRegen()
{
	GetWorld()->GetTimerManager().ClearTimer(ShieldRegenDelayHandle);
	GetWorld()->GetTimerManager().ClearTimer(ShieldRegenTickHandle);
	bIsRegeneratingShield = false;

}

void AThirdPersonPlayerState::TickShieldRegen()
{
	if (currentShield >= maxShield)
	{
		currentShield = maxShield;
		StopShieldRegen();
		return;
	}

	currentShield += shieldRegenPerTick;

	if (currentShield >= maxShield)
	{
		currentShield = maxShield;
		StopShieldRegen();
	}

}

void AThirdPersonPlayerState::RestartShieldRegenDelay()
{
	GetWorld()->GetTimerManager().ClearTimer(ShieldRegenDelayHandle);
	GetWorld()->GetTimerManager().SetTimer(ShieldRegenDelayHandle, this, &AThirdPersonPlayerState::StartShieldRegen, shieldRegenDelay, false);

}

void AThirdPersonPlayerState::AddOverShield(float InAddOverShieldAmount)
{
	currentOverShield += InAddOverShieldAmount;

	if (currentOverShield > maxOverShield)
	{
		currentOverShield = maxOverShield;
	}
}

void AThirdPersonPlayerState::EnterOverShieldZone()
{
	bInOverShieldZone = true;
	StartOverShieldRegen();
}

void AThirdPersonPlayerState::ExitOverShieldZone()
{
	bInOverShieldZone = false;
	StopOverShieldRegen();
}

void AThirdPersonPlayerState::StartOverShieldRegen()
{
	if (!bInOverShieldZone) return;

	if (!GetWorld()->GetTimerManager().IsTimerActive(OverShieldRegenTickHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(OverShieldRegenTickHandle, this, &AThirdPersonPlayerState::TickOverShieldRegen, overShieldRegenTick, true);

	}
	UE_LOG(LogTemp, Error, TEXT("%f"), overShieldRegenTick);

}

void AThirdPersonPlayerState::StopOverShieldRegen()
{
	GetWorld()->GetTimerManager().ClearTimer(OverShieldRegenTickHandle);
}

void AThirdPersonPlayerState::TickOverShieldRegen()
{
	if (!bInOverShieldZone) return;

	if (currentOverShield < maxOverShield)
	{
		currentOverShield = FMath::Min(currentOverShield + overShieldRegenPerTick, maxOverShield);

	}


}

