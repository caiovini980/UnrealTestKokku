// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "DisableWeaponColliderNotify.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnNotified);

UCLASS()
class UDisableWeaponColliderNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;

	FOnNotified OnNotified;

};
