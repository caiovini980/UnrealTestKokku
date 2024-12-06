// Fill out your copyright notice in the Description page of Project Settings.


#include "Armory/Weapons/Sword.h"

#include "Components/BoxComponent.h"

ASword::ASword()
{
	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
}

UBoxComponent* ASword::GetCollider() const
{
	return Collider.Get();
}

float ASword::GetDamage() const
{
	return Damage;
}
