#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SkateboardController.generated.h"

struct FInputActionValue;

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
	class UInputAction* MoveAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DefaultInput, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DefaultInput, meta = (AllowPrivateAccess = "true"))
	class UInputAction* PushAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DefaultInput, meta = (AllowPrivateAccess = "true"))
	class UInputAction* BreakAction = nullptr;

	void Move(const FInputActionValue& Value);
	void Jump();
	void Push();
	void StartBreaking();
	void Break();
	void StopBreaking();
};
