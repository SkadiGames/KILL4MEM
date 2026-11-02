// Copyright Epic Games, Inc. All Rights Reserved.

#include "KILL4MEMCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapons/WeaponDataAsset.h"
#include "KILL4MEM.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

void AKILL4MEMCharacter::SetWeaponByIndex(int32 NewIndex)
{
    if (!Weapons.IsValidIndex(NewIndex))
        return;
    CurrentWeaponIndex = NewIndex;

    UWeaponDataAsset* Weapon = GetCurrentWeapon();
    if (!Weapon)
        return;

    // Меш оружия в руках
    if (UStaticMesh* WeaponMesh = Weapon->WeaponMesh.LoadSynchronous())
    {
        FirstPersonWeaponMesh->SetStaticMesh(WeaponMesh);
    }
}

UWeaponDataAsset* AKILL4MEMCharacter::GetCurrentWeapon() const
{
    return Weapons.IsValidIndex(CurrentWeaponIndex) ? Weapons[CurrentWeaponIndex] : nullptr;
}

AKILL4MEMCharacter::AKILL4MEMCharacter()
{
    GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

    // Create the Camera Component
    FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
    FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
    FirstPersonCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 64.0f));
    FirstPersonCameraComponent->SetRelativeRotation(FRotator::ZeroRotator);
    FirstPersonCameraComponent->bUsePawnControlRotation = true;
    FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
    FirstPersonCameraComponent->bEnableFirstPersonScale = true;
    FirstPersonCameraComponent->FirstPersonFieldOfView = 110.0f;
    FirstPersonCameraComponent->FirstPersonScale = 0.6f;

    FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));
    FirstPersonMesh->SetupAttachment(FirstPersonCameraComponent);
    FirstPersonMesh->SetOnlyOwnerSee(true);
    FirstPersonMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
    FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));

    FirstPersonWeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("First Person Weapon Mesh"));
    FirstPersonWeaponMesh->SetupAttachment(FirstPersonMesh, FName("hand_r")); // или другой сокет
    FirstPersonWeaponMesh->SetOnlyOwnerSee(true);
    FirstPersonWeaponMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
    FirstPersonWeaponMesh->SetCollisionProfileName(FName("NoCollision"));

    // configure the character comps
    GetMesh()->SetOwnerNoSee(true);
    GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

    GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);

    // Configure character movement
    GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
    GetCharacterMovement()->AirControl = 0.5f;
}

void AKILL4MEMCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    // Set up action bindings
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // Jumping
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AKILL4MEMCharacter::DoJumpStart);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AKILL4MEMCharacter::DoJumpEnd);

        // Moving
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AKILL4MEMCharacter::MoveInput);

        // Attack
        EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this,
                                           &AKILL4MEMCharacter::AttackInput);

        // Looking/Aiming
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AKILL4MEMCharacter::LookInput);
        EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this,
                                           &AKILL4MEMCharacter::LookInput);
    }
    else
    {
        UE_LOG(LogKILL4MEM, Error,
               TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input "
                    "system. If you intend to use the legacy system, then you will need to update this C++ file."),
               *GetNameSafe(this));
    }
}

void AKILL4MEMCharacter::MoveInput(const FInputActionValue& Value)
{
    // get the Vector2D move axis
    FVector2D MovementVector = Value.Get<FVector2D>();

    // pass the axis values to the move input
    DoMove(MovementVector.X, MovementVector.Y);
}

void AKILL4MEMCharacter::LookInput(const FInputActionValue& Value)
{
    // get the Vector2D look axis
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    // pass the axis values to the aim input
    DoAim(LookAxisVector.X, LookAxisVector.Y);
}

void AKILL4MEMCharacter::AttackInput()
{
    UE_LOG(LogTemplateCharacter, Log, TEXT("Attack pressed"));

    UWeaponDataAsset* Weapon = GetCurrentWeapon();
    if (!Weapon)
        return;

    UAnimMontage* Montage = Weapon->AttackMontage.LoadSynchronous();
    if (!Montage)
        return;

    if (FirstPersonMesh && FirstPersonMesh->GetAnimInstance())
    {
        FirstPersonMesh->GetAnimInstance()->Montage_Play(Montage);
    }

    PlayAnimMontage(Montage);
}

void AKILL4MEMCharacter::DoAim(float Yaw, float Pitch)
{
    if (GetController())
    {
        // pass the rotation inputs
        AddControllerYawInput(Yaw);
        AddControllerPitchInput(Pitch);
    }
}

void AKILL4MEMCharacter::DoMove(float Right, float Forward)
{
    if (GetController())
    {
        // pass the move inputs
        AddMovementInput(GetActorRightVector(), Right);
        AddMovementInput(GetActorForwardVector(), Forward);
    }
}

void AKILL4MEMCharacter::DoJumpStart()
{
    // pass Jump to the character
    Jump();
}

void AKILL4MEMCharacter::DoJumpEnd()
{
    // pass StopJumping to the character
    StopJumping();
}
