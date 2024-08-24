#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScoreTorus.generated.h"

DECLARE_DELEGATE_OneParam(FOnPlayerScorePoints, int32)

UCLASS()
class SKATEBOARDDEMO_API AScoreTorus : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TorusMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* CollisionBox = nullptr;

public:	
	AScoreTorus();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	FOnPlayerScorePoints OnPlayerScorePoints;

private:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PointsSystem, meta = (AllowPrivateAccess = "true"))
	int32 PointsReward = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PointsSystem, meta = (AllowPrivateAccess = "true"))
	float AcceptablePlayerInAngle = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PointsSystem, meta = (AllowPrivateAccess = "true"))
	class USoundCue* GrabSFX = nullptr;

	UFUNCTION()
	void OnEnterCollisionBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool CanScorePoints(AActor* OtherActor) const;
};
