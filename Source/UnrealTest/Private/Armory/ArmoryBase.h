// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArmoryBase.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class AArmoryBase : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMesh;
	
public:	
	// Sets default values for this actor's properties
	AArmoryBase();

	virtual void SetupArmory();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool bIsActive{ true };

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
