#include "SkateboardController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "SkateboardCharacter.h"
#include "PlayerHUD.h"
#include "ScoringGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

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

	GameMode = Cast<AScoringGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	SetupInputContext();

	SetupPlayerHUD();
}

void ASkateboardController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASkateboardController::SteerSkateboard);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASkateboardController::Jump);
		EnhancedInputComponent->BindAction(PushAction, ETriggerEvent::Triggered, this, &ASkateboardController::Push);
		EnhancedInputComponent->BindAction(BreakAction, ETriggerEvent::Started, this, &ASkateboardController::StartBraking);
		EnhancedInputComponent->BindAction(BreakAction, ETriggerEvent::Triggered, this, &ASkateboardController::SlowDownOnAir);
		EnhancedInputComponent->BindAction(BreakAction, ETriggerEvent::Completed, this, &ASkateboardController::StopBraking);
		EnhancedInputComponent->BindAction(ResetLevelAction, ETriggerEvent::Triggered, this, &ASkateboardController::ResetLevel);
		EnhancedInputComponent->BindAction(ExitGameAction, ETriggerEvent::Triggered, this, &ASkateboardController::ExitGame);
	}
}

void ASkateboardController::SteerSkateboard(const FInputActionValue& Value)
{
	if (!SkateboardCharacter)
	{
		return;
	}

	const float InputDirection = Value.Get<float>();
	SkateboardCharacter->SteerSkateboard(InputDirection);
}

void ASkateboardController::Jump()
{
	if (SkateboardCharacter)
	{
		SkateboardCharacter->SkateJump();
	}
}

void ASkateboardController::Push(const FInputActionValue& Value)
{
	if (Value.Get<float>() < 0.f)
	{
		return;
	}

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

void ASkateboardController::SlowDownOnAir()
{
	if (SkateboardCharacter)
	{
		SkateboardCharacter->SlowDownOnAir();
	}
}

void ASkateboardController::StopBraking()
{
	if (SkateboardCharacter)
	{
		SkateboardCharacter->StopBraking();
	}
}

void ASkateboardController::SetupInputContext()
{
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		if (InputSystem)
		{
			InputSystem->AddMappingContext(SkatingContext, 0);
		}
	}
}

void ASkateboardController::SetupPlayerHUD()
{
	if (!PlayerHUDClass)
	{
		return;
	}

	PlayerHUD = CreateWidget<UPlayerHUD>(this, PlayerHUDClass);

	if (!PlayerHUD)
	{
		return;
	}

	if (GameMode)
	{
		GameMode->OnUpdateAvailableTime.BindUObject(PlayerHUD, &UPlayerHUD::UpdateTimeRemaining);
		GameMode->OnUpdatePlayerPoints.BindUObject(PlayerHUD, &UPlayerHUD::UpdatePlayerPoints);
	}

	PlayerHUD->AddToPlayerScreen();
}

void ASkateboardController::ResetLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(*GetWorld()->GetName()), false);
}

void ASkateboardController::ExitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), this, EQuitPreference::Quit, false);
}
