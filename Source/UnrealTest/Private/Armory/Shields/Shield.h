// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Armory/ArmoryBase.h"
#include "Shield.generated.h"

/**
 * 
 */
UCLASS()
class AShield : public AArmoryBase
{
	GENERATED_BODY()
public:
	// Inherited via AArmoryBase
	void SetupArmory() override;

	void ReduceShield();

private:
	// Set this variable on the server
	UPROPERTY(EditDefaultsOnly)
	int ShieldLives{3};
};
