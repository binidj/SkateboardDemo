#include "SkateboardCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

ASkateboardCharacter::ASkateboardCharacter()
{
 	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	SkateboardSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skateboard Mesh"));

	if (SpringArm)
	{
		SpringArm->SetupAttachment(GetRootComponent());
	}

	if (Camera)
	{
		Camera->SetupAttachment(SpringArm);
	}

	if (SkateboardSkeletalMesh)
	{
		SkateboardSkeletalMesh->SetupAttachment(GetRootComponent());
	}
}

void ASkateboardCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	MovementComponent = GetCharacterMovement();
}

void ASkateboardCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AlignSkate();
	FixVelocityDirection();
}

void ASkateboardCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASkateboardCharacter::SteerSkateboard(float InputDirection)
{
	const float DeltaTime = GetWorld()->GetDeltaSeconds();

	if (FMath::IsNearlyZero(InputDirection))
	{
		return;
	}

	if (!MovementComponent)
	{
		return;
	}

	float TurnSpeedReductionFactor = 1.f - MovementComponent->Velocity.Length() / MovementComponent->MaxWalkSpeed;
	TurnSpeedReductionFactor = FMath::Clamp(TurnSpeedReductionFactor, MinTurnSpeedReductionScale, 1.f);

	const float JumpScale = IsSkateJumping() ? TurnScaleWhenJumping : 1.f;

	AddControllerYawInput(InputDirection * TurnSpeed * DeltaTime * TurnSpeedReductionFactor * JumpScale);
}

void ASkateboardCharacter::SkateJump()
{
	if (bIsPushing || IsSkateJumping())
	{
		return;
	}

	bTriggerJumpAnimation = true;
	bIsPlayingJumpAnimation = true;
}

void ASkateboardCharacter::PushSkateboard()
{
	if (!CanPush())
	{
		return;
	}

	bIsPushing = true;
	bIsPushRecharged = false;
	GetWorldTimerManager().SetTimer(PushRechargeHandle, this, &ASkateboardCharacter::RechargePush, PushRechargeSeconds);
}

void ASkateboardCharacter::StartBraking()
{
	if (MovementComponent)
	{
		PreviousBrakeFriction = MovementComponent->BrakingDecelerationWalking;
		MovementComponent->BrakingDecelerationWalking = BrakeFriction;
	}
}

void ASkateboardCharacter::StopBraking()
{
	if (MovementComponent)
	{
		MovementComponent->BrakingDecelerationWalking = PreviousBrakeFriction;
	}
}

bool ASkateboardCharacter::IsSkateJumping() const
{
	return MovementComponent && MovementComponent->IsFalling();
}

void ASkateboardCharacter::AddPushForce() const
{
	if (!MovementComponent || !MovementComponent->IsMovingOnGround())
	{
		return;
	}

	float PushImpulseReductionScale = 1.f - MovementComponent->Velocity.Length() / MovementComponent->MaxWalkSpeed;
	PushImpulseReductionScale = FMath::Clamp(PushImpulseReductionScale, MinPushImpulseReductionScale, 1.f);

	const FVector ImpulseToAdd = GetActorForwardVector() * (ImpulseForce * PushImpulseReductionScale / MovementComponent->Mass);

	if (ImpulseToAdd.Length() + MovementComponent->Velocity.Length() < MovementComponent->MaxWalkSpeed)
	{
		MovementComponent->AddImpulse(GetActorForwardVector() * ImpulseForce * PushImpulseReductionScale);
	}
}

void ASkateboardCharacter::StopPushing()
{
	bIsPushing = false;
}

void ASkateboardCharacter::GetLegLocations(FVector& OutFrontLegLocation, FVector& OutBackLegLocation) const
{
	if (!SkateboardSkeletalMesh)
	{
		OutFrontLegLocation = FVector::ZeroVector;
		OutBackLegLocation = FVector::ZeroVector;
		return;
	}
	
	OutFrontLegLocation = SkateboardSkeletalMesh->GetSocketLocation(TEXT("FrontLeg"));

	OutBackLegLocation = SkateboardSkeletalMesh->GetSocketLocation(TEXT("BackLeg"));
}

void ASkateboardCharacter::TriggerJumpMovement()
{
	Super::Jump();

	if (SkateboardSkeletalMesh)
	{
		SkateboardSkeletalMesh->PlayAnimation(SkateboardFlip, false);
	}
}

void ASkateboardCharacter::FixVelocityDirection()
{
	if (!MovementComponent)
	{
		return;
	}

	if (MovementComponent->Velocity.IsNearlyZero())
	{
		return;
	}

	FVector VelocityProjection = FVector::VectorPlaneProject(MovementComponent->Velocity, FVector::UpVector);

	if (VelocityProjection.IsNearlyZero())
	{
		return;
	}

	VelocityProjection = GetActorForwardVector() * VelocityProjection.Length();
	MovementComponent->Velocity.X = VelocityProjection.X;
	MovementComponent->Velocity.Y = VelocityProjection.Y;
	//const FVector VelocityProjection = FVector::VectorPlaneProject(MovementComponent->Velocity, GetActorUpVector());

	//if (VelocityProjection.IsNearlyZero())
	//{
	//	return;
	//}

	//const FQuat Between = FQuat::FindBetweenVectors(GetActorForwardVector(), VelocityProjection);

	//const float AngleToForward = Between.GetAngle();

	//const FVector InitialPos = GetActorLocation() + GetActorUpVector().GetClampedToSize(50.f, 50.f);

	//MovementComponent->Velocity = MovementComponent->Velocity.RotateAngleAxisRad(AngleToForward * PreviousTurnDirection, GetActorUpVector());
}

void ASkateboardCharacter::AlignSkate()
{
	if (!SkateboardSkeletalMesh)
	{
		return;
	}

	if (bIsPlayingJumpAnimation)
	{
		return;
	}

	const FVector FrontWheelLocation = SkateboardSkeletalMesh->GetSocketLocation(TEXT("FrontWheel"));
	const FVector BackWheelLocation = SkateboardSkeletalMesh->GetSocketLocation(TEXT("BackWheel"));
	const FVector RightFrontWheelLocation = SkateboardSkeletalMesh->GetSocketLocation(TEXT("RightFrontWheel"));
	const FVector LeftFrontWheelLocation = SkateboardSkeletalMesh->GetSocketLocation(TEXT("LeftFrontWheel"));

	const FVector FrontHit = WheelTrace(FrontWheelLocation);
	const FVector BackHit = WheelTrace(BackWheelLocation);
	const FVector RightFrontHit = WheelTrace(RightFrontWheelLocation);
	const FVector LeftFrontHit = WheelTrace(LeftFrontWheelLocation);

	const FRotator ForwardDirectionRotation = UKismetMathLibrary::FindLookAtRotation(BackHit, FrontHit);
	const FRotator RightDirectionRotation = UKismetMathLibrary::FindLookAtRotation(RightFrontHit, LeftFrontHit);

	FRotator SkateRotation(ForwardDirectionRotation.Pitch, ForwardDirectionRotation.Yaw, RightDirectionRotation.Pitch);

	const float DeltaTime = GetWorld()->GetDeltaSeconds();

	SkateRotation = UKismetMathLibrary::RInterpTo(SkateboardSkeletalMesh->GetComponentRotation(), SkateRotation, DeltaTime, SkateAlignSpeed);

	SkateboardSkeletalMesh->SetWorldRotation(SkateRotation);
}

FVector ASkateboardCharacter::WheelTrace(const FVector& WheelLocation)
{
	const FVector RayOffset(0.f, 0.f, 25.f);

	FHitResult HitResult;

	bool bHasHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		WheelLocation + RayOffset,
		WheelLocation - RayOffset,
		ECC_Visibility
	);

	if (bHasHit)
	{
		return HitResult.Location;
	}

	return WheelLocation;
}

bool ASkateboardCharacter::CanPush() const
{
	return !bIsPushing && bIsPushRecharged && !IsSkateJumping() && !bIsPlayingJumpAnimation;
}

void ASkateboardCharacter::RechargePush()
{
	bIsPushRecharged = true;
}
