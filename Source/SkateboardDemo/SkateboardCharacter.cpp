#include "SkateboardCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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

		MovementComponent->AddInputVector(GetActorForwardVector() * PushAcceleration);
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

