#include "ScoringGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "ScoreTorus.h"

void AScoringGameMode::BeginPlay()
{
	GetWorldTimerManager().SetTimer(AvailableTimeHandle, this, &AScoringGameMode::UpdateAvailableTime, 1.f, true, 1.f);
	
	BindScoreTorusOnLevel();

	GetWorldTimerManager().SetTimerForNextTick(this, &AScoringGameMode::UpdateUIOnBeginPlay);
}

void AScoringGameMode::IncreasePlayerPoints(int32 PointsReward)
{
	if (AvailableTimeSeconds <= 0)
	{
		return;
	}
	
	PlayerPoints += PointsReward;

	OnUpdatePlayerPoints.ExecuteIfBound(PlayerPoints);
}

void AScoringGameMode::UpdateAvailableTime()
{
	AvailableTimeSeconds -= 1;

	if (AvailableTimeSeconds <= 0)
	{
		GetWorldTimerManager().ClearTimer(AvailableTimeHandle);
	}

	OnUpdateAvailableTime.ExecuteIfBound(AvailableTimeSeconds);
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

void AScoringGameMode::UpdateUIOnBeginPlay()
{
	OnUpdatePlayerPoints.ExecuteIfBound(PlayerPoints);
	OnUpdateAvailableTime.ExecuteIfBound(AvailableTimeSeconds);
}
