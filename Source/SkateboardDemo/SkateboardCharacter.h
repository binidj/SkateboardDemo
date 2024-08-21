#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SkateboardCharacter.generated.h"

UCLASS()
class SKATEBOARDDEMO_API ASkateboardCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* SkateboardMesh = nullptr;

	UPROPERTY()
	class UCharacterMovementComponent* MovementComponent = nullptr;

public:
	ASkateboardCharacter();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MovementSettings, meta = (AllowPrivateAccess = "true"))
	float TurnSpeed = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MovementSettings, meta = (AllowPrivateAccess = "true"))
	float PushAccelerationScale = 5.f;

	float PreviousTurnDirection = 0.f;
	float PreviousBrakeFriction = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MovementSettings, meta = (AllowPrivateAccess = "true"))
	float BrakeFriction = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MovementSettings, meta = (AllowPrivateAccess = "true"))
	float SkateAlignSpeed = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MovementSettings, meta = (AllowPrivateAccess = "true"))
	float MomentumAccelerationScale = 1.f;

	bool bIsPushing = false;
	bool bIsPushRecharged = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MovementSettings, meta = (AllowPrivateAccess = "true"))
	float PushRechargeSeconds = 2.5f;

	FTimerHandle PushRechargeHandle;

	void FixVelocityDirection();
	void AlignSkate();
	void AddSkateMomentum();
	FVector WheelTrace(const FVector& WheelLocation);
	bool CanPush() const;
	void RechargePush();

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SteerSkateboard(const FVector2D& InputMovement);
	void SkateJump();
	void PushSkateboard();
	void StartBraking();
	void Brake();
	void StopBraking();

private:
	bool IsSkateJumping() const;

public:
	UFUNCTION(BlueprintCallable)
	bool GetIsPushing() const { return bIsPushing; };

	UFUNCTION(BlueprintCallable)
	void AddPushForce() const;

	UFUNCTION(BlueprintCallable)
	void StopPushing();
};
