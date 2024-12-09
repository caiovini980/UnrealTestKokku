// Fill out your copyright notice in the Description page of Project Settings.


#include "Armory/Shields/Shield.h"

void AShield::ReduceShield()
{
	if (ShieldLives > 0)
	{
		ShieldLives -= 1;
		return;
	}

	// Destroy on server and replicate on clients
	Destroy();
}
