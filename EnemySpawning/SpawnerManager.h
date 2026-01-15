// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "EnemySpawner.h"
//#include "NavigationSystem.h" cant build if this is included
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "SpawnerManager.generated.h"



USTRUCT(BlueprintType)
struct FEnemyToSpawn
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ACharacter> EnemyType = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0"))
	float SpawnWeight = 1.0f;

};

UCLASS()
class TEAM3GP4_API ASpawnerManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnerManager();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	float managerInteval = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	float spawnRadius = 2500;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	int baseMaxSpawnedCap = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "spawning")
	float baseSpawnInterval = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Intensity")
	float intesityIncrease = 0.05f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	TArray<FEnemyToSpawn> SpawnArray;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	float spawnZOffset = 0.f;

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SetLoopIndex(int InLoopIndex) { loopIndex = InLoopIndex; ApplyDifficulty(); }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loop")
	int loopIndex = 0.f;

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void RefreshSpawnPoints();

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void StartSpawning();

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void StopSpawning();

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void DecreaseTotalSpawned() { totalSpawned = FMath::Max(0, totalSpawned - 1); }

	// for special Spawn zone
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void IncreaseTotalSpawned() { totalSpawned++; }

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	float GetTotalSpawned() { return totalSpawned; }

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	float GetMaxAliveCap() { return currentMaxSpawnedCap; }

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	float GetSpawnInterval() { return currentSpawnInteval; }


	UFUNCTION(BlueprintCallable, Category = "Zones")
	void IncreaseZoneTotal() { insideZones++; StartSpawning(); }

	UFUNCTION(BlueprintCallable, Category = "Zones")
	void DecreaseZoneTotal() { insideZones--; StopSpawning(); }

	UFUNCTION(BlueprintCallable, Category = "Zones")
	void IncreaseTotalSpawnedCap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private:

	TArray<TWeakObjectPtr<AEnemySpawner>> Points;
	FTimerHandle ManagerTimer;

	int totalSpawned = 0;
	float currentSpawnInteval = 1.0f;
	int currentMaxSpawnedCap = 15;
	
	int insideZones = 0;

	void ApplyDifficulty();
	void TickSpawn();
	AEnemySpawner* PickSpawnPoint() const;
	const FEnemyToSpawn* ChooseEnemy() const;

	bool IsPlayerInRange(const AEnemySpawner* Point) const;
};
