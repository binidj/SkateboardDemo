#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

class UTextBlock;

UCLASS()
class SKATEBOARDDEMO_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

private:

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TimeRemaining = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TotalPlayerPoints = nullptr;

public:
	
	void UpdateTimeRemaining(int32 TimeRemainingSeconds);
	void UpdatePlayerPoints(int32 PlayerPoints);
};
