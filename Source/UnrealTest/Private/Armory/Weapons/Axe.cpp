// Fill out your copyright notice in the Description page of Project Settings.


#include "Armory/Weapons/Axe.h"

#include "Components/BoxComponent.h"

AAxe::AAxe()
{}


void AAxe::SetupArmory()
{
	Collider = GetComponentByClass<UBoxComponent>();
	
	if (!Collider)
	{
		UE_LOG(LogTemp, Error, TEXT("Axe couldn't set up correctly: Collider is missing."))
	}
}

void AAxe::EnableCollider(bool bEnabled) const
{
	if (Collider.Get() != nullptr && HasAuthority())
	{
		if (bEnabled)
		{
			Collider.Get()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			DrawDebugBox(
				GetWorld(),
				GetActorLocation(),
				Collider.Get()->Bounds.BoxExtent,
				FColor::Green,
				true,
				2.0f,
				0,
				2.0f);
			return;
		}

		Collider.Get()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		DrawDebugBox(
			GetWorld(),
			GetActorLocation(),
			Collider.Get()->Bounds.BoxExtent,
			FColor::Red,
			true,
			2.0f,
			0,
			2.0f);
	}
}

float AAxe::GetDamage() const
{
	return Damage;
}
