#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScoreTorus.generated.h"

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

};
