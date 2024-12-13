// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "UnrealTestCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

class AAxe;
class AShield;

UENUM(BlueprintType)
enum PlayerActionState
{
	PAS_Neutral UMETA(DisplayName = "Neutral"),
	PAS_Block UMETA(DisplayName = "Block"),
	PAS_Attack UMETA(DisplayName = "Attack"),
	PAS_Hit UMETA(DisplayName = "Hit"),
	PAS_Dead UMETA(DisplayName = "Dead"),
};

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AUnrealTestCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AUnrealTestCharacter();

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;

	/* Server Variables */
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	TEnumAsByte<PlayerActionState> ActionState;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
	UPROPERTY(EditDefaultsOnly, Category = Input)
	TObjectPtr<UInputAction> AttackAction;
	UPROPERTY(EditDefaultsOnly, Category = Input)
	TObjectPtr<UInputAction> BlockAction;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	TObjectPtr<UAnimMontage> AttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	TObjectPtr<UAnimMontage> BlockMontage;
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	TObjectPtr<UAnimMontage> BlockOngoingMontage;
	
	UPROPERTY(VisibleDefaultsOnly, Category = Armory)
	TObjectPtr<AAxe> AxeRef;
	UPROPERTY(VisibleDefaultsOnly, Category = Armory)
	TObjectPtr<AShield> ShieldRef;
	
	/* Methods */
	void InitMontageNotifies();

	void InitAttackNotifies();
	void InitBlockNotifies() const;
	void AttackEndedNotifyImplementation();
	void DisableWeaponColliderNotifyImplementation();
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Attack(const FInputActionValue& Value);
	void Block(const FInputActionValue& Value);
	
	/* Server Methods */
	/* Multicasts */
	UFUNCTION(NetMulticast, reliable)
	void HeavyAttack();

	UFUNCTION(NetMulticast, reliable)
	void LightAttack();

	UFUNCTION(NetMulticast, reliable)
	void ExecuteBlock();

	UFUNCTION(NetMulticast, reliable)
	void ExitBlock();
	
	/* Run On Server */
	UFUNCTION(Server, Reliable)
	void Server_LightAttack();
	
	UFUNCTION(Server, Reliable)
	void Server_HeavyAttack();

	UFUNCTION(Server, Reliable)
	void Server_UseShield(bool IsPressingToBlock);
	
};

