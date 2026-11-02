// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "KILL4MEMCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UWeaponDataAsset;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A basic first person character
 */
UCLASS(abstract)
class AKILL4MEMCharacter : public ACharacter
{
    GENERATED_BODY()

    /** Pawn mesh: first person view (arms; seen only by self) */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USkeletalMeshComponent* FirstPersonMesh;

    /** First person camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UCameraComponent* FirstPersonCameraComponent;

protected:
    /** Jump Input Action */
    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* JumpAction;

    /** Move Input Action */
    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* MoveAction;

    /** Attack Input Action */
    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* AttackAction;

    /** Look Input Action */
    UPROPERTY(EditAnywhere, Category = "Input")
    class UInputAction* LookAction;

    /** Mouse Look Input Action */
    UPROPERTY(EditAnywhere, Category = "Input")
    class UInputAction* MouseLookAction;

    // --- Weapons ---
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TArray<TObjectPtr<UWeaponDataAsset>> Weapons;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Weapon")
    int32 CurrentWeaponIndex = 0;

    // Компонент для меша оружия в FP-виде (если ещё нет)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    TObjectPtr<UStaticMeshComponent> FirstPersonWeaponMesh;

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void SetWeaponByIndex(int32 NewIndex);

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    UWeaponDataAsset* GetCurrentWeapon() const;

public:
    AKILL4MEMCharacter();

protected:
    void MoveInput(const FInputActionValue& Value);

    void LookInput(const FInputActionValue& Value);

    void AttackInput();

    /** Handles aim inputs from either controls or UI interfaces */
    UFUNCTION(BlueprintCallable, Category = "Input")
    virtual void DoAim(float Yaw, float Pitch);

    /** Handles move inputs from either controls or UI interfaces */
    UFUNCTION(BlueprintCallable, Category = "Input")
    virtual void DoMove(float Right, float Forward);

    /** Handles jump start inputs from either controls or UI interfaces */
    UFUNCTION(BlueprintCallable, Category = "Input")
    virtual void DoJumpStart();

    /** Handles jump end inputs from either controls or UI interfaces */
    UFUNCTION(BlueprintCallable, Category = "Input")
    virtual void DoJumpEnd();

protected:
    /** Set up input action bindings */
    virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

public:
    /** Returns the first person mesh **/
    USkeletalMeshComponent* GetFirstPersonMesh() const { return FirstPersonMesh; }

    /** Returns first person camera component **/
    UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
};
