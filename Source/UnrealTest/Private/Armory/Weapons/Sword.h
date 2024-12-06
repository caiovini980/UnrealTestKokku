// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Armory/ArmoryBase.h"
#include "Sword.generated.h"

class UBoxComponent;

/**
 * 
 */
UCLASS()
class ASword : public AArmoryBase
{
	GENERATED_BODY()

public:
	ASword();

	UBoxComponent* GetCollider() const;
	float GetDamage() const;
	
private:	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> Collider;

	UPROPERTY(VisibleAnywhere)
	float Damage{10};
};
