#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SkateboardController.generated.h"

struct FInputActionValue;
class UInputAction;
class UPlayerHUD;

UCLASS()
class SKATEBOARDDEMO_API ASkateboardController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ASkateboardController();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

private:
	UPROPERTY()
	class ASkateboardCharacter* SkateboardCharacter = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DefaultInput, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* SkatingContext = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DefaultInput, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DefaultInput, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DefaultInput, meta = (AllowPrivateAccess = "true"))
	UInputAction* PushAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DefaultInput, meta = (AllowPrivateAccess = "true"))
	UInputAction* BreakAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = PlayerHUD, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UPlayerHUD> PlayerHUDClass = nullptr;

	UPROPERTY()
	UPlayerHUD* PlayerHUD = nullptr;

	UPROPERTY()
	class AScoringGameMode* GameMode = nullptr;

	void SteerSkateboard(const FInputActionValue& Value);
	void Jump();
	void Push(const FInputActionValue& Value);
	void StartBraking();
	void StopBraking();
	void SetupInputContext();
	void SetupPlayerHUD();
};
