#include "SkateboardController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "SkateboardCharacter.h"

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

	SkateboardCharacter = Cast<ASkateboardCharacter>(GetCharacter());

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
		EnhancedInputComponent->BindAction(PushAction, ETriggerEvent::Triggered, this, &ASkateboardController::Push);
		EnhancedInputComponent->BindAction(BreakAction, ETriggerEvent::Started, this, &ASkateboardController::StartBraking);
		EnhancedInputComponent->BindAction(BreakAction, ETriggerEvent::Completed, this, &ASkateboardController::StopBraking);
	}
}

void ASkateboardController::Move(const FInputActionValue& Value)
{
	if (!SkateboardCharacter)
	{
		return;
	}

	FVector2D InputMovement = Value.Get<FVector2D>();
	InputMovement.Y = FMath::Max(InputMovement.Y, 0.f);
	InputMovement.Normalize();
	SkateboardCharacter->SteerSkateboard(InputMovement);
}

void ASkateboardController::Jump()
{
	if (SkateboardCharacter)
	{
		SkateboardCharacter->SkateJump();
	}
}

void ASkateboardController::Push()
{
	if (SkateboardCharacter)
	{
		SkateboardCharacter->PushSkateboard();
	}
}

void ASkateboardController::StartBraking()
{
	if (SkateboardCharacter)
	{
		SkateboardCharacter->StartBraking();
	}
}

void ASkateboardController::StopBraking()
{
	if (SkateboardCharacter)
	{
		SkateboardCharacter->StopBraking();
	}
}
