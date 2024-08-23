#include "ScoreTorus.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

AScoreTorus::AScoreTorus()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	TorusMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Torus Mesh"));

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));

	if (TorusMesh)
	{
		TorusMesh->SetupAttachment(GetRootComponent());
	}

	if (CollisionBox)
	{
		CollisionBox->SetupAttachment(TorusMesh);
	}
}

void AScoreTorus::BeginPlay()
{
	Super::BeginPlay();
	
	if (CollisionBox)
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AScoreTorus::OnEnterCollisionBox);
	}
}

void AScoreTorus::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AScoreTorus::OnEnterCollisionBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!CanScorePoints(OtherActor))
	{
		return;
	}

	OnPlayerScorePoints.ExecuteIfBound(PointsReward);
	SetActorHiddenInGame(true);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

bool AScoreTorus::CanScorePoints(AActor* OtherActor) const
{
	if (!OtherActor)
	{
		return false;
	}

	const bool IsPlayer = OtherActor->ActorHasTag(TEXT("PlayerCharacter"));

	if (!IsPlayer)
	{
		return false;
	}
	
	const FVector PlayerForward = OtherActor->GetActorForwardVector();
	FVector TorusForward = GetActorForwardVector();
	const float DotProduct = TorusForward.Dot(PlayerForward);

	if (FMath::IsNearlyZero(DotProduct))
	{
		return false;
	}

	if (DotProduct < 0.f)
	{
		TorusForward *= -1.f;
	}

	const FQuat Between = FQuat::FindBetweenVectors(PlayerForward, TorusForward);
	const float PlayerInDegree = FMath::Abs(FMath::RadiansToDegrees(Between.GetAngle()));

	const bool HasAcceptableInDegree = PlayerInDegree <= AcceptablePlayerInAngle;

	if (!HasAcceptableInDegree)
	{
		return false;
	}

	return true;
}

