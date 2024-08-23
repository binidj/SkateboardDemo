#include "ScoringGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "ScoreTorus.h"

void AScoringGameMode::BeginPlay()
{
	GetWorldTimerManager().SetTimer(AvailableTimeHandle, this, &AScoringGameMode::UpdateAvailableTime, 1.f, true, 1.f);
	
	BindScoreTorusOnLevel();
}

void AScoringGameMode::IncreasePlayerPoints(int32 PointsReward)
{
	if (AvailableTimeSeconds <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Time's out: can't score points!"));
		return;
	}
	
	PlayerPoints += PointsReward;

	UE_LOG(LogTemp, Warning, TEXT("Player got %d points, total points is %d"), PointsReward, PlayerPoints);
	// UpdateUI
}

void AScoringGameMode::UpdateAvailableTime()
{
	AvailableTimeSeconds -= 1;

	UE_LOG(LogTemp, Warning, TEXT("Available time is %d"), AvailableTimeSeconds);

	if (AvailableTimeSeconds <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Time has ended!"), AvailableTimeSeconds);
		GetWorldTimerManager().ClearTimer(AvailableTimeHandle);
	}

	// UpdateUI
}

void AScoringGameMode::BindScoreTorusOnLevel()
{
	TArray<AActor*> LevelScoreTorus;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AScoreTorus::StaticClass(), LevelScoreTorus);

	for (AActor* Actor : LevelScoreTorus)
	{
		if (AScoreTorus* ScoreTorus = Cast<AScoreTorus>(Actor))
		{
			ScoreTorus->OnPlayerScorePoints.BindUObject(this, &AScoringGameMode::IncreasePlayerPoints);
		}
	}
}
