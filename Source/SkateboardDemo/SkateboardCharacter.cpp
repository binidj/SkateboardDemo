#include "SkateboardCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

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

