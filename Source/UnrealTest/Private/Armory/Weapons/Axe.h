// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Armory/ArmoryBase.h"
#include "Axe.generated.h"

class UBoxComponent;

/**
 * 
 */
UCLASS()
class AAxe : public AArmoryBase
{
	GENERATED_BODY()

public:
	AAxe();

	void EnableCollider(bool bEnabled) const;
	float GetDamage() const;
	
private:	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> Collider;

	UPROPERTY(VisibleAnywhere)
	float Damage{10};
};
