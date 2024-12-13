// Fill out your copyright notice in the Description page of Project Settings.


#include "Armory/Weapons/Axe.h"

#include "Components/BoxComponent.h"

AAxe::AAxe()
{
	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
}

void AAxe::EnableCollider(bool bEnabled) const
{
	if (Collider.Get() != nullptr)
	{
		if (bEnabled)
		{
			Collider.Get()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			return;
		}

		Collider.Get()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

float AAxe::GetDamage() const
{
	return Damage;
}
