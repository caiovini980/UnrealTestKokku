#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AttackEndNotify.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNotified);

UCLASS()
class UAttackEndNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;
	
	FOnNotified OnNotified;
	
};
