#include "SkateboardController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

ASkateboardController::ASkateboardController()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ASkateboardController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASkateboardController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		if (InputSystem)
		{
			InputSystem->AddMappingContext(SkatingContext, 0);
		}
	}
}

void ASkateboardController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASkateboardController::Move);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASkateboardController::Jump);
	}
}

void ASkateboardController::Move(const FInputActionValue& Value)
{
	if (ACharacter* PlayerCharacter = GetCharacter())
	{
		const FVector2D InputMovement = Value.Get<FVector2D>();

		UE_LOG(LogTemp, Warning, TEXT("Movement action: (%f, %f)"), InputMovement.X, InputMovement.Y);
	}
}

void ASkateboardController::Jump()
{
	UE_LOG(LogTemp, Warning, TEXT("Jump pressed"));
}
