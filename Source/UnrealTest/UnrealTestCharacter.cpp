// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealTestCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "Private/Armory/Weapons/Axe.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Armory/Shields/Shield.h"
#include "Net/UnrealNetwork.h"

#include "Private/Characters/Common/Animations/AnimNotifies/AttackEndNotify.h"
#include "Private/Characters/Common/Animations/AnimNotifies/DisableWeaponColliderNotify.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AUnrealTestCharacter
AUnrealTestCharacter::AUnrealTestCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

void AUnrealTestCharacter::InitAttackNotifies()
{
	if (AttackMontage)
	{
		const auto NotifyEvents = AttackMontage->Notifies;

		for (const auto& EventNotify : NotifyEvents)
		{
			if (const auto AttackEndedNotify = Cast<UAttackEndNotify>(EventNotify.Notify))
			{
				AttackEndedNotify->OnNotified.AddUObject(this, &AUnrealTestCharacter::AttackEndedNotifyImplementation);
			}

			if (const auto DisableWeaponColliderNotify = Cast<UDisableWeaponColliderNotify>(EventNotify.Notify))
			{
				DisableWeaponColliderNotify->OnNotified.AddUObject(this, &AUnrealTestCharacter::DisableWeaponColliderNotifyImplementation);

			}
		}
	}
}

void AUnrealTestCharacter::InitBlockNotifies() const
{
	if (BlockMontage)
	{
		// const auto NotifyEvents = BlockMontage->Notifies;
		//
		// for (const auto& EventNotify : NotifyEvents)
		// {
		// }
	}
}

void AUnrealTestCharacter::InitMontageNotifies()
{
	InitAttackNotifies();
	InitBlockNotifies();
}

void AUnrealTestCharacter::AttackEndedNotifyImplementation()
{
	// Reset caracter to neutral stage
	if (HasAuthority())
	{
		ActionState = PAS_Neutral;
	}
}

void AUnrealTestCharacter::DisableWeaponColliderNotifyImplementation()
{
	// TODO Send this to the server
	if (HasAuthority() && AxeRef.Get())
	{
		AxeRef.Get()->EnableCollider(false);
	}
}

void AUnrealTestCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	ActionState = PAS_Neutral;

	// Get Axe and Shield
	TArray<AActor*> ChildActors;
	GetAllChildActors(ChildActors);

	for (auto Child : ChildActors)
	{
		if (AShield* ShieldObject = Cast<AShield>(Child))
		{
			ShieldRef = ShieldObject;
			ShieldRef->SetupArmory();
		}
		else if (AAxe* AxeObject = Cast<AAxe>(Child))
		{
			AxeRef = AxeObject;
			AxeRef->SetupArmory();
		}
	}
	
	// Setup animation notifies
	InitMontageNotifies();
}

//////////////////////////////////////////////////////////////////////////
// Input

void AUnrealTestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUnrealTestCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AUnrealTestCharacter::Look);

		// Attacking
		EnhancedInputComponent->BindAction(AttackAction.Get(), ETriggerEvent::Triggered, this, &AUnrealTestCharacter::Attack);

		// Blocking
		EnhancedInputComponent->BindAction(BlockAction.Get(), ETriggerEvent::Triggered, this, &AUnrealTestCharacter::Block);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AUnrealTestCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AUnrealTestCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AUnrealTestCharacter::Attack(const FInputActionValue& Value)
{
	if (AttackMontage.Get() != nullptr && ActionState == PAS_Neutral)
	{
		if (Value.Get<bool>())
		{
			Server_HeavyAttack();
			return;
		}

		Server_LightAttack();
	}
}

void AUnrealTestCharacter::Block(const FInputActionValue& Value)
{
	bool bIsPressing = Value.Get<bool>();
	//UE_LOG(LogTemp, Warning, TEXT("Block check: %i"), bIsPressing)

	if (BlockMontage.Get() != nullptr)
	{
		if (ActionState == PAS_Neutral && bIsPressing)
		{
			ExecuteBlock_Implementation();
		}

		// if when check the player is blocking, it must stop using it
		else if (ActionState == PAS_Block && !bIsPressing)
		{
			ExitBlock_Implementation();
		}

		Server_UseShield(bIsPressing);
	}
}

/* Multiplayer Methods */
/* Client Methods */
/* Multicasts */
void AUnrealTestCharacter::ExecuteBlock_Implementation()
{
	bUseControllerRotationYaw = true;
}

void AUnrealTestCharacter::ExitBlock_Implementation()
{
	bUseControllerRotationYaw = false;
}

void AUnrealTestCharacter::HeavyAttack_Implementation()
{
	PlayAnimMontage(AttackMontage.Get(), 1, FName("HeavyAttack"));
	AxeRef.Get()->EnableCollider(true);
}

void AUnrealTestCharacter::LightAttack_Implementation()
{
	PlayAnimMontage(AttackMontage.Get(), 1.5, FName("LightAttack"));
	AxeRef.Get()->EnableCollider(true);
}

/* Server Methods */
/* Replicated props */
void AUnrealTestCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AUnrealTestCharacter, ActionState);
}

/* Run On Server */
void AUnrealTestCharacter::Server_LightAttack_Implementation()
{
	if (!HasAuthority() || ActionState != PAS_Neutral)
	{
		return;
	}

	LightAttack();
	ActionState = PAS_Attack;
}

void AUnrealTestCharacter::Server_HeavyAttack_Implementation()
{
	if (!HasAuthority() || ActionState != PAS_Neutral)
	{
		return;
	}

	HeavyAttack();
	ActionState = PAS_Attack;
}

void AUnrealTestCharacter::Server_UseShield_Implementation(bool IsPressingToBlock)
{
	if (!HasAuthority())
	{
		return;
	}

	if (ActionState == PAS_Neutral && IsPressingToBlock)
	{
		ExecuteBlock_Implementation();
		ActionState = PAS_Block;
		return;		
	}

	if (ActionState == PAS_Block && !IsPressingToBlock)
	{
		ExitBlock_Implementation();
		ActionState = PAS_Neutral;
	}
}
