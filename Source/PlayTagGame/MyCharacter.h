// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "MyCharacter.generated.h"

UCLASS()
/** \brief The parent class of characters
 *
 * The parent class of ANightGuard and a child class of the implemented in Unreal Engine ACharacter class.
 */
class PLAYTAGGAME_API AMyCharacter : public ACharacter {
    GENERATED_BODY()

public:
    AMyCharacter(); ///< Constructor for a character.

    UPROPERTY(EditDefaultsOnly, Category = "Capsule", meta = (ClampMin = "0", ClampMax = "500"))
    float CapsuleHalfHeight = 100.0f; ///< The capsule's half height.
    UPROPERTY(EditDefaultsOnly, Category = "Capsule", meta = (ClampMin = "0", ClampMax = "500"))
    float CapsuleRadius = 37.5f; ///< The capsule's radius.
    UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    int CameraType = 0; ///< The value, which is used to set player's POV.
    UPROPERTY(Replicated)
    bool FlashlightOn = false; ///< The value, which is used to set current flashlight's state.
    UPROPERTY(Replicated)
    bool IsRun = false; ///< The value, which is used to set if the character is running.
    UPROPERTY(Replicated)
    bool IsTired = false; ///< The value, which is used to set if the character is tired.

    UPROPERTY(BlueprintReadOnly, Replicated)
    int Posture = 1; ///< The value, which is used to set current posture.

    UPROPERTY(EditDefaultsOnly, Category = "Stamina", meta = (ClampMin = "0", ClampMax = "2000"))
    float Stamina = 300.0f; ///< The max Stamina parameter.

    UPROPERTY(EditDefaultsOnly, Category = "Stamina")
    float MinusStamina = 1.0f; ///< The value the current stamina decreases by. Better not change.

    UPROPERTY(EditDefaultsOnly, Category = "Stamina")
    float PlusStamina = 1.0f; ///< The value the current stamina increases by. Better not change.

    UPROPERTY(Replicated, EditDefaultsOnly, Category = "Stamina")
    float CurrentStamina = Stamina; ///< Character's current stamina.

    UPROPERTY(EditDefaultsOnly, Category = "Stamina")
    float RecoveryStamina = 0.5 * Stamina; ///< The recovery condition value.

    /** \brief Getter for CurrentStamina.
     * \author DimasBirdrus
     * \version 1.0
     * \date January, 2024
     *
     * Probably not useful for a project but so on.
     * \return CurrentStamina returns character's current stamina.
     */
    const float& GetStamina();

    /** \brief Changes current camera.
     * \author DimasBirdrus
     * \version 1.0
     * \date January, 2024
     *
     * May be used for different cameras to make a character more comfortable to play.
     */
    virtual void ChangeCamera();

    /** \brief Crouch method for a character.
     * \author DimasBirdrus
     * \version 1.0
     * \date January, 2024
     *
     * Some classes as ANightGuard may need some functionality of crouching to make the game more adventurous.
     */
    virtual void CrouchFunc();

    /** \brief Changes the density of the flashlight's light.
     * \author DimasBirdrus
     * \version 1.0
     * \date January, 2024
     *
     * Some classes as ANightGuard may need some functionality with flashlights.
     */
    virtual void ChangeFlashlight();

    /** \brief Run method for a character.
     * \author DimasBirdrus
     * \version 1.0
     * \date January, 2024
     *
     * Almost every character should have this method to make the game more challenging.
     */
    virtual void Run();

    /** \brief Game-controllable Stop-Run method.
     * \author DimasBirdrus
     * \version 1.0
     * \date January, 2024
     *
     * This method is used for a permanent sprint stop due to game rules.
     */
    virtual void StopRun();

    /** \brief Player-controllable Stop-Run method.
     * \author DimasBirdrus
     * \version 1.0
     * \date January, 2024
     *
     * This method is used for player's sprint stop.
     */
    virtual void StopRunSelf();

    /** \brief The Decrease Stamina method.
     * \author DimasBirdrus
     * \version 1.0
     * \date January, 2024
     *
     * This method is used to decrease CurrentStamina by MinusStamina.
     */
    virtual void DecreaseStamina();

    /** \brief The Increase Stamina method.
     * \author DimasBirdrus
     * \version 1.0
     * \date January, 2024
     *
     * This method is used to increase CurrentStamina by PlusStamina.
     */
    virtual void IncreaseStamina();

    /** \brief The method of Interaction.
     * \author DimasBirdrus
     * \version 1.0
     * \date January, 2024
     *
     * This method is used to have an access to the methods of an other class.
     */
    virtual void Interact();

    /** \brief The replication method.
     * \author DimasBirdrus
     * \version 1.0
     * \date May, 2024
     *
     * This method sets which variables are replicated.
     */
    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
};