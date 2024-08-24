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
	USkeletalMeshComponent* SkateboardSkeletalMesh = nullptr;

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
	float TurnScaleWhenJumping = 0.15f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MovementSettings, meta = (AllowPrivateAccess = "true"))
	float ImpulseForce = 5000.f;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MovementSettings, meta = (AllowPrivateAccess = "true"))
	float MinTurnSpeedReductionScale = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MovementSettings, meta = (AllowPrivateAccess = "true"))
	float MinPushImpulseReductionScale = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MovementSettings, meta = (AllowPrivateAccess = "true"))
	float SlowDownSpeedOnAir = 15.f;

	FTimerHandle PushRechargeHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MovementSettings, meta = (AllowPrivateAccess = "true"))
	UAnimationAsset* SkateboardFlip = nullptr;

	bool bTriggerJumpAnimation = false;
	bool bIsPlayingJumpAnimation = false;

	void FixVelocityDirection();
	void AlignSkate();
	FVector WheelTrace(const FVector& WheelLocation);
	bool CanPush() const;
	void RechargePush();

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SteerSkateboard(float InputDirection);
	void SkateJump();
	void PushSkateboard();
	void StartBraking();
	void SlowDownOnAir();
	void StopBraking();

private:
	bool IsSkateJumping() const;

public:
	UFUNCTION(BlueprintCallable)
	bool GetIsPushing() const { return bIsPushing; };

	UFUNCTION(BlueprintCallable)
	bool GetTriggerJumpAnimation() const { return bTriggerJumpAnimation; };

	UFUNCTION(BlueprintCallable)
	void UnsetTriggerJumpAnimation() { bTriggerJumpAnimation = false; };

	UFUNCTION(BlueprintCallable)
	void AddPushForce() const;

	UFUNCTION(BlueprintCallable)
	void StopPushing();

	UFUNCTION(BlueprintCallable)
	void GetLegLocations(FVector& OutFrontLegLocation, FVector& OutBackLegLocation) const;

	UFUNCTION(BlueprintCallable)
	void TriggerJumpMovement();

	UFUNCTION(BlueprintCallable)
	void UnsetIsPlayingJumpAnimation() { bIsPlayingJumpAnimation = false; };
};
