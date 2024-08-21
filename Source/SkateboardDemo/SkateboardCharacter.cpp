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

	SkateboardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Skateboard"));

	if (SpringArm)
	{
		SpringArm->SetupAttachment(GetRootComponent());
	}

	if (Camera)
	{
		Camera->SetupAttachment(SpringArm);
	}

	if (SkateboardMesh)
	{
		SkateboardMesh->SetupAttachment(GetRootComponent());
	}
}

void ASkateboardCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASkateboardCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AlignSkate();
	// AddSkateMomentum();
}

void ASkateboardCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASkateboardCharacter::SteerSkateboard(const FVector2D& InputMovement)
{
	const float DeltaTime = GetWorld()->GetDeltaSeconds();

	if (InputMovement.X == 0.f)
	{
		return;
	}

	AddControllerYawInput(InputMovement.X * TurnSpeed * DeltaTime);

	PreviousTurnDirection = InputMovement.X;
	
	GetWorldTimerManager().SetTimerForNextTick(this, &ASkateboardCharacter::FixVelocityDirection);
}

void ASkateboardCharacter::SkateJump()
{
	Super::Jump();
}

void ASkateboardCharacter::PushSkateboard()
{
	if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	{
		if (!MovementComponent->IsMovingOnGround())
		{
			return;
		}
		
		MovementComponent->AddInputVector(GetActorForwardVector() * PushAccelerationScale);
	}
}

void ASkateboardCharacter::StartBraking()
{
	if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	{
		PreviousBrakeFriction = MovementComponent->BrakingDecelerationWalking;
		MovementComponent->BrakingDecelerationWalking = BrakeFriction;
	}
}

void ASkateboardCharacter::Brake()
{

}

void ASkateboardCharacter::StopBraking()
{
	if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	{
		MovementComponent->BrakingDecelerationWalking = PreviousBrakeFriction;
	}
}

void ASkateboardCharacter::FixVelocityDirection()
{
	if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	{
		if (MovementComponent->Velocity.IsNearlyZero())
		{
			return;
		}
		
		const FVector VelocityProjection = FVector::VectorPlaneProject(MovementComponent->Velocity, GetActorUpVector());
		
		if (VelocityProjection.IsNearlyZero())
		{
			return;
		}

		const FQuat Between = FQuat::FindBetweenVectors(GetActorForwardVector(), VelocityProjection);
		
		const float AngleToForward = Between.GetAngle();

		const FVector InitialPos = GetActorLocation() + GetActorUpVector().GetClampedToSize(50.f, 50.f);

		MovementComponent->Velocity = MovementComponent->Velocity.RotateAngleAxisRad(AngleToForward * PreviousTurnDirection, GetActorUpVector());
	}
}

void ASkateboardCharacter::AlignSkate()
{
	if (!SkateboardMesh)
	{
		return;
	}

	const FVector FrontWheelLocation = SkateboardMesh->GetSocketLocation(TEXT("FrontWheel"));
	const FVector BackWheelLocation = SkateboardMesh->GetSocketLocation(TEXT("BackWheel"));
	const FVector RightFrontWheelLocation = SkateboardMesh->GetSocketLocation(TEXT("RightFrontWheel"));
	const FVector LeftFrontWheelLocation = SkateboardMesh->GetSocketLocation(TEXT("LeftFrontWheel"));

	const FVector FrontHit = WheelTrace(FrontWheelLocation);
	const FVector BackHit = WheelTrace(BackWheelLocation);
	const FVector RightFrontHit = WheelTrace(RightFrontWheelLocation);
	const FVector LeftFrontHit = WheelTrace(LeftFrontWheelLocation);

	const FRotator ForwardDirectionRotation = UKismetMathLibrary::FindLookAtRotation(BackHit, FrontHit);
	const FRotator RightDirectionRotation = UKismetMathLibrary::FindLookAtRotation(RightFrontHit, LeftFrontHit);

	FRotator SkateRotation(ForwardDirectionRotation.Pitch, ForwardDirectionRotation.Yaw, RightDirectionRotation.Pitch);

	// FRotator SkateRotation = ForwardDirectionRotation;

	const float DeltaTime = GetWorld()->GetDeltaSeconds();

	SkateRotation = UKismetMathLibrary::RInterpTo(SkateboardMesh->GetComponentRotation(), SkateRotation, DeltaTime, SkateAlignSpeed);

	SkateboardMesh->SetWorldRotation(SkateRotation);
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

void ASkateboardCharacter::AddSkateMomentum()
{
	if (!SkateboardMesh)
	{
		return;
	}

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();

	if (!MovementComponent->IsMovingOnGround())
	{
		return;
	}

	const FVector SkateDirection = SkateboardMesh->GetForwardVector();

	// TODO: Improve this

	if (SkateDirection.Z < 0.f)
	{
		MovementComponent->AddInputVector(SkateDirection * MomentumAccelerationScale);
	}
	else
	{
		MovementComponent->AddInputVector(SkateDirection * MomentumAccelerationScale * -1.f);
	}
}
