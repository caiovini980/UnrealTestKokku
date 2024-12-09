// Fill out your copyright notice in the Description page of Project Settings.


#include "Armory/Weapons/Axe.h"

#include "Components/BoxComponent.h"

AAxe::AAxe()
{
	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
}

UBoxComponent* AAxe::GetCollider() const
{
	return Collider.Get();
}

float AAxe::GetDamage() const
{
	return Damage;
}
