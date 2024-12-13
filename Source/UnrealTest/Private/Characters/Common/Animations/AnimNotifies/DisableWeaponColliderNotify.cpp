// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Common/Animations/AnimNotifies/DisableWeaponColliderNotify.h"

void UDisableWeaponColliderNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	OnNotified.Broadcast();
	Super::Notify(MeshComp, Animation, EventReference);
}
