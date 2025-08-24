// Copyright Epic Games, Inc. All Rights Reserved.

#include "GASCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GASPlayerState.h"
#include "GAS.h"
#include "GASAbilitySystemLibrary.h"
#include "CharacterClassInfo.h"
#include "GASAttributeSet.h"
#include "GASAbilitySystemComponent.h"


AGASCharacter::AGASCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AGASCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	// Server: Initialize the Ability System for the Player State
	if(HasAuthority())
	{
		InitAbilityActorInfo();
	}
}

void AGASCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	// Client: Initialize the Ability System for the Player State
	InitAbilityActorInfo();
}

void AGASCharacter::InitAbilityActorInfo()
{
	if(AGASPlayerState* GASPlayerState = GetPlayerState<AGASPlayerState>())
	{
		GASAbilitySystemComponent = GASPlayerState->GetGASAbilitySystemComponent();
		GASAttributeSet = GASPlayerState->GetGASAttributeSet();

		if(IsValid(GASAbilitySystemComponent))
		{
			GASAbilitySystemComponent->InitAbilityActorInfo(GASPlayerState, this);
			BindCallbacksToDependencies();

			if(HasAuthority()){
				InitClassDefaults();
			}
		}
		else
		{
			UE_LOG(LogGAS, Error, TEXT("'%s' Failed to find Ability System Component from Player State '%s'!"), *GetNameSafe(this), *GetNameSafe(GASPlayerState));
		}
	}
}

void AGASCharacter::InitClassDefaults()
{
	if(!CharacterTag.IsValid())
	{
		UE_LOG(LogGAS, Warning, TEXT("'%s' CharacterTag is not set! Please set a CharacterTag for this character class."), *GetNameSafe(this));
	}
	else if (UCharacterClassInfo* ClassInfo = UGASAbilitySystemLibrary::GetCharacterDefaultInfo(this)) 
	{
		if (FCharacterClassDefaultInfo* SelectedClassInfo = ClassInfo->ClassDefaultInfoMap.Find(CharacterTag)) 
		{
			if (IsValid(GASAbilitySystemComponent)) 
			{
				GASAbilitySystemComponent->AddCharacterAbilities(SelectedClassInfo->StartingAbilities);
				GASAbilitySystemComponent->AddCharacterPassives(SelectedClassInfo->StartingPassives);
				GASAbilitySystemComponent->InitializeDefaultAttributes(SelectedClassInfo->DefaultAttributes);
			}
		}
	}
}

void AGASCharacter::BindCallbacksToDependencies()
{
	if(IsValid(GASAbilitySystemComponent) && IsValid(GASAttributeSet))
	{
		GASAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GASAttributeSet->GetHealthAttribute()).AddLambda(
			[&](const FOnAttributeChangeData& Data)
			{
				const float CurrentHealth = Data.NewValue;
				const float MaxHealth = GASAttributeSet->GetMaxHealth();
				OnHealthChanged(CurrentHealth, MaxHealth);
			}
		);

		GASAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GASAttributeSet->GetManaAttribute()).AddLambda(
			[&](const FOnAttributeChangeData& Data)
			{
				const float CurrentMana = Data.NewValue;
				const float MaxMana = GASAttributeSet->GetMaxMana();
				OnManaChanged(CurrentMana, MaxMana);
			}
		);
	}
	else
	{
		UE_LOG(LogGAS, Error, TEXT("'%s' Failed to bind Ability System callbacks! Ability System Component or Attribute Set is invalid."), *GetNameSafe(this));
	}
}

void AGASCharacter::BroadcastInitialValues()
{
	if(IsValid(GASAttributeSet))
	{
		OnHealthChanged(GASAttributeSet->GetHealth(), GASAttributeSet->GetMaxHealth());
		OnManaChanged(GASAttributeSet->GetMana(), GASAttributeSet->GetMaxMana());
	}
	else
	{
		UE_LOG(LogGAS, Error, TEXT("'%s' Failed to broadcast initial values! Attribute Set is invalid."), *GetNameSafe(this));
	}
}


void AGASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGASCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AGASCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGASCharacter::Look);
	}
	else
	{
		UE_LOG(LogGAS, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AGASCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AGASCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AGASCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void AGASCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AGASCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void AGASCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}
