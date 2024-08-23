#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ScoringGameMode.generated.h"

UCLASS()
class SKATEBOARDDEMO_API AScoringGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PointsSystem, meta = (AllowPrivateAccess = "true"))
	int32 AvailableTimeSeconds = 180;

	FTimerHandle AvailableTimeHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PointsSystem, meta = (AllowPrivateAccess = "true"))
	int32 PlayerPoints = 0;

	void IncreasePlayerPoints(int32 PointsReward);

	void UpdateAvailableTime();

	void BindScoreTorusOnLevel();
};
