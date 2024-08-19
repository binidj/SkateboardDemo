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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DefaultInput, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* SkatingContext = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DefaultInput, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DefaultInput, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction = nullptr;

	void Move(const FInputActionValue& Value);

	void Jump();
};
