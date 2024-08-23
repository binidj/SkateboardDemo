#include "ScoreTorus.h"
#include "Components/BoxComponent.h"

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
	

}

void AScoreTorus::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

