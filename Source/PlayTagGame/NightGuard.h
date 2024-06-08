// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/RectLightComponent.h"
#include "Components/AudioComponent.h"
#include "Materials/Material.h"
#include "Components/CapsuleComponent.h"
#include "ClassicDoor.h"
#include "NightGuard.generated.h"

/**
 *
 */
UCLASS()
/** \brief ANightGuard class
* 
* The class of a playable character.
*/
class PLAYTAGGAME_API ANightGuard : public AMyCharacter
{
public:
	GENERATED_BODY()

	ANightGuard();			///< Constructor for a character.

	UPROPERTY(EditAnywhere, Category = "SpringArm", meta = (ClampMin = "0", ClampMax = "400"))
	    float SpringArmLength = 300.0f;		///< The parameter of the Spring Arms.

	UPROPERTY(VisibleAnywhere)
	    USpringArmComponent* SpringArm1;	///< The pointer to Spring Arm 1.

	UPROPERTY(VisibleAnywhere)
        USpringArmComponent* SpringArm2;	///< The pointer to Spring Arm 2.

	UPROPERTY(VisibleAnywhere)
	    UCameraComponent* Camera3rd_1;		///< The pointer to 3rd person Camera 1.

	UPROPERTY(VisibleAnywhere)
	    UCameraComponent* Camera3rd_2;		///< The pointer to 3rd person Camera 2.

	UPROPERTY(VisibleAnywhere)
	    UCameraComponent* Camera1st;		///< The pointer to 1st person Camera.

	UPROPERTY(VisibleAnywhere, Replicated)
	    URectLightComponent* Light;			///< The pointer to the flashlight Rect Light.

	UPROPERTY(EditAnywhere, Category = "Flashlight", meta = (ClampMin = "0", ClampMax = "2000"))
	    float FlashlightBrightness = 1000.0f;	///< The value of flashlight's brightness.

	UPROPERTY(VisibleAnywhere, Category = "Flashlight")
	    UAudioComponent* FlashlightSound1;		///< The pointer to flashlight's sound 1.
	UPROPERTY(VisibleAnywhere, Category = "Flashlight")
	    UAudioComponent* FlashlightSound2;		///< The pointer to flashlight's sound 2.
	UPROPERTY(Replicated)
	    UMaterialInterface* Material1;			///< The pointer to the flashlight's light material 1.
	UPROPERTY(Replicated)
	    UMaterialInterface* Material2;			///< The pointer to the flashlight's light material 2.
	UPROPERTY(EditAnywhere, Category = "LineTrace")
	    float LineTraceLength = 300;			///< The length of the Line Trace, which appears with the Interaction().

	/** \brief Sets the inputs for actions.
	* \author DimasBirdrus
	* \version 1.0
	* \date January, 2024
	* 
	* It is used to set the function to run after the key being pressed.
	*/
	void SetupPlayerInputComponent(class UInputComponent* MyPlayerInput) override;

	/** \brief Makes character run forward and backward.
	* \author DimasBirdrus
	* \version 1.0
	* \date January, 2024
	*
	* This is used to make character go the direction as it says.
	* \param[in] Value The value of the ForwardBackward Axis.
	*/
	void MoveForwardBackward(float Value);

	/** \brief Makes character run left and right.
	* \author DimasBirdrus
	* \version 1.0
	* \date January, 2024
	* 
	* This is used to make charater go sideways.
	* \param[in] Value The value of the LeftRight Axis.
	*/
	void MoveLeftRight(float Value);

	/** \brief Makes character crouch/uncrouch.
	* \author DimasBirdrus
	* \version 1.0
	* \date April, 2024
	*
	* This makes character crouch or uncrouch with functions of an AActor.
	*/
	void CrouchFunc() override;

	UFUNCTION(Server, Reliable, WithValidation)
	/** \brief Sets IsRun on the server.
	* \author DimasBirdrus
	* \version 1.0
	* \date May, 2024
	* 
	* This is a server method to make a character run on the server. It is defined by Unreal Engine.
	* The only thing should be made is giving definitions to _Implemntation and _Validate methods.
	* \param[in] bNewIsRun The new state of the IsRun variable.
	*/
	void Server_SetIsRun(bool bNewIsRun);

	UFUNCTION(Client, Reliable)
	/** \brief Sets IsRun on a client.
	* \author DimasBirdrus
	* \version 1.0
	* \date May, 2024
	* 
	* This is a client method to make a character run on a client. It is defined by Unreal Engine.
	* The only thing should be made is giving definitions to _Implemntation and _Validate methods.
	* \param[in] bNewIsRun The new state of the IsRun variable.
	*/
	void Client_SetIsRun(bool bNewIsRun);

	UFUNCTION(Server, Reliable, WithValidation)
	/** \brief Sets IsTired on the server.
	* \author DimasBirdrus
	* \version 1.0
	* \date May, 2024
	*
	* This is a server method to make a character be tired on the server. It is defined by Unreal Engine.
	* The only thing should be made is giving definitions to _Implemntation and _Validate methods.
	* \param[in] bNewIsRun The new state of the IsTired variable.
	*/
	void Server_SetIsTired(bool bNewIsTired);

	UFUNCTION(Client, Reliable)
	/** \brief Sets IsTired on a client.
	* \author DimasBirdrus
	* \version 1.0
	* \date May, 2024
	*
	* This is a client method to make a character be tired on a client. It is defined by Unreal Engine.
	* The only thing should be made is giving definitions to _Implemntation and _Validate methods.
	* \param[in] bNewIsRun The new state of the IsTired variable.
	*/
	void Client_SetIsTired(bool bNewIsTired);

	UFUNCTION(Server, Reliable, WithValidation)
	/** \brief Sets CurrentStamina on the server.
	* \author DimasBirdrus
	* \version 1.0
	* \date May, 2024
	*
	* This is a server method to set CurrentStamina on the server. It is defined by Unreal Engine.
	* The only thing should be made is giving definitions to _Implemntation and _Validate methods.
	* \param[in] bNewIsRun The new value of the CurrentStamina variable.
	*/
	void Server_SetCurrentStamina(float fNewCurrentStamina);

	UFUNCTION(Client, Reliable)
	/** \brief Sets CurrentStamina on a client.
	* \author DimasBirdrus
	* \version 1.0
	* \date May, 2024
	*
	* This is a server method to set CurrentStamina on a client. It is defined by Unreal Engine.
	* The only thing should be made is giving definitions to _Implemntation and _Validate methods.
	* \param[in] bNewIsRun The new value of the CurrentStamina variable.
	*/
	void Client_SetCurrentStamina(float fNewCurrentStamina);

	/** \brief Run method.
	* \author DimasBirdrus
	* \version 1.1
	* \date May, 2024
	*
	* This method is used to set the logic of character movement.
	*/
	void Run() override;

	/** \brief StopRun method.
	* \author DimasBirdrus
	* \version 1.0
	* \date May, 2024
	*
	* This method is used to stop character from running permanently.
	*/
	void StopRun() override;

	/** \brief StopRunSelf method.
	* \author DimasBirdrus
	* \version 1.1
	* \date May, 2024
	*
	* This method is used to stop the character from running whenever the player wants.
	*/
	void StopRunSelf() override;

	/** \brief DecreaseStamina method.
	* \author DimasBirdrus
	* \version 1.0
	* \date January, 2024
	*
	* This method is used to decrease CurrentStamina by 1.0f.
	*/
	void DecreaseStamina() override;

	/** \brief IncreaseStamina method.
	* \author DimasBirdrus
	* \version 1.0
	* \date January, 2024
	*
	* This method is used to increase CurrentStamina by 1.0f.
	*/
	void IncreaseStamina() override;

	/** \brief Interact method.
	* \author DimasBirdrus
	* \version 1.0
	* \date April, 2024
	*
	* This method is used to cast the action from ANightGuard to the interactable object (in this case this is the AClassicDoor).
	*/
	void Interact() override;

	/** \brief ChangeFlashlight method.
	* \author DimasBirdrus
	* \version 1.0
	* \date April, 2024
	*
	* This method is used to change flashlight's light.
	*/
	void ChangeFlashlight() override;

	/** \brief ChangeCamera method.
	* \author DimasBirdrus
	* \version 1.0
	* \date April, 2024
	*
	* This method is used to change character's POV.
	*/
	void ChangeCamera() override;

	/** \brief Overriden Unreal Engine Tick method
	* \author DimasBirdrus
	* \version 1.1
	* \date May, 2024
	*
	* This method calls functions every game tick. It is used for Stamina functionality.
	* \param[in] Time The time to work in a cycle.
	*/
	void Tick(float Time) override;

	/** \brief Sets replication conditions.
	* \author DimasBirdrus
	* \version 1.0
	* \date May, 2024
	*
	* This method sets the conditions for replicated variables.
	* \param[out] OutLifetimeProps An array which is set inside this method with replication rules.
	*/
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** \brief Update_Movement_Speed method.
	* \author DimasBirdrus
	* \version 1.0
	* \date May, 2024
	*
	* This method updates speed of a character.
	*/
	void Update_Movement_Speed();
};
