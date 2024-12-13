// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Armory/ArmoryBase.h"
#include "Axe.generated.h"

class UBoxComponent;

UCLASS()
class AAxe : public AArmoryBase
{
	GENERATED_BODY()

public:
	AAxe();

	void SetupArmory() override;

	void EnableCollider(bool bEnabled) const;
	float GetDamage() const;
	
private:
	UFUNCTION()
	void OnHitTarget(UPrimitiveComponent* Component, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY()
	TObjectPtr<UBoxComponent> Collider;

	UPROPERTY(EditDefaultsOnly)
	float Damage{10};
};
