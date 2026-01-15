// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerManager.h"

// Sets default values
ASpawnerManager::ASpawnerManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawnerManager::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle temp;
	GetWorldTimerManager().SetTimer(temp, this, &ASpawnerManager::RefreshSpawnPoints, 5.0f, false);
	ApplyDifficulty();

}

void ASpawnerManager::RefreshSpawnPoints()
{
	Points.Reset();
	TArray<AActor*> Found;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemySpawner::StaticClass(), Found);
	for (AActor* Actor : Found)
	{
		if (AEnemySpawner* Point = Cast<AEnemySpawner>(Actor))
		{
			Points.Add(Point);
		}
	}
}

void ASpawnerManager::ApplyDifficulty()
{
	float intesityScale = FMath::Pow(1.0f + intesityIncrease, (float)loopIndex);
	currentSpawnInteval = baseSpawnInterval / intesityScale;
	currentMaxSpawnedCap = baseMaxSpawnedCap * intesityScale;
}

void ASpawnerManager::IncreaseTotalSpawnedCap()
{
	float intesityScale = FMath::Pow(1.0f + intesityIncrease, (float)loopIndex);
	float increaseAmount = baseMaxSpawnedCap * intesityScale;
	currentMaxSpawnedCap += increaseAmount;
}


void ASpawnerManager::StartSpawning()
{
	if (!GetWorld()->GetTimerManager().IsTimerActive(ManagerTimer) && insideZones > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(ManagerTimer, this, &ASpawnerManager::TickSpawn, currentSpawnInteval, true, 0);

	}

}

void ASpawnerManager::StopSpawning()
{
	if (insideZones <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(ManagerTimer);

	}
}

bool ASpawnerManager::IsPlayerInRange(const AEnemySpawner* Point) const
{
	if (!Point) return false;
	APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	return FVector::Dist2D(Player->GetActorLocation(), Point->GetActorLocation()) <= spawnRadius;

}

const FEnemyToSpawn* ASpawnerManager::ChooseEnemy() const
{
	if (SpawnArray.Num() == 0) return nullptr;

	float weightSum = 0.0f;
	for (const FEnemyToSpawn& Enemy : SpawnArray)
	{
		if (Enemy.EnemyType && Enemy.SpawnWeight > 0.0f) weightSum += Enemy.SpawnWeight;
	}

	const float pick = FMath::FRandRange(0.0f, weightSum);
	float totalWeight = 0.0f;

	for (const FEnemyToSpawn& Enemy : SpawnArray)
	{
		totalWeight += Enemy.SpawnWeight;
		if (pick <= totalWeight)
		{
			return &Enemy;
		}
	}
	return nullptr;
}

AEnemySpawner* ASpawnerManager::PickSpawnPoint() const
{
	TArray<AEnemySpawner*> SpawnPoints;
	for (auto& p : Points)
	{
		if (AEnemySpawner* Point = p.Get())
		{
			if (IsPlayerInRange(Point))
			{
				SpawnPoints.Add(Point);
			}
		}
	}
	if (SpawnPoints.Num() == 0) return nullptr;
	return SpawnPoints[FMath::RandRange(0, SpawnPoints.Num() - 1)];
}

//void ASpawnerManager::TrackIfEnemyDestroyed(AActor* DestroyedActor)
//{
//	totalAlive = FMath::Max(0, totalAlive - 1);
//}

void ASpawnerManager::TickSpawn()
{
	if (totalSpawned >= currentMaxSpawnedCap) return;

	AEnemySpawner* SpawnPoint = PickSpawnPoint();
	if (!SpawnPoint) return;

	const FEnemyToSpawn* Entry = ChooseEnemy();

	const FVector SpawnLocation = SpawnPoint->GetActorLocation() + FVector(0, 0, spawnZOffset);
	const FRotator SpawnRotation = SpawnPoint->GetActorRotation();

	const FTransform  SpawnTransform(SpawnRotation, SpawnLocation, FVector::OneVector);

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	ACharacter* Enemy = GetWorld()->SpawnActor<ACharacter>(Entry->EnemyType, SpawnTransform, Params);
	if (!Enemy) return;

	totalSpawned++;
	UE_LOG(LogTemp, Error, TEXT("Total Alive: %d out of: %d possible"), totalSpawned, currentMaxSpawnedCap);

}


