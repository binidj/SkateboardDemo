#include "PlayerHUD.h"
#include "Components/TextBlock.h"

void UPlayerHUD::UpdateTimeRemaining(int32 TimeRemainingSeconds)
{
	if (TimeRemaining)
	{
		const FString FormatedTime = FString::Printf(TEXT("%.2d:%.2d"), TimeRemainingSeconds / 60, TimeRemainingSeconds % 60);
		TimeRemaining->SetText(FText::FromString(FormatedTime));

		if (TimeRemainingSeconds == 0)
		{
			TimeRemaining->SetColorAndOpacity(FLinearColor::Red);
		}
	}
}

void UPlayerHUD::UpdatePlayerPoints(int32 PlayerPoints)
{
	if (TotalPlayerPoints)
	{
		TotalPlayerPoints->SetText(FText::FromString(FString::FromInt(PlayerPoints)));
	}
}
