// Fill out your copyright notice in the Description page of Project Settings.


#include "NightGuard.h"
#include "ClassicDoor.h"

//===============================================================================
//		CONSTRUCTOR
ANightGuard::ANightGuard() : Super() {

	Camera1st = CreateDefaultSubobject<UCameraComponent>(TEXT("1st_Camera"));
	Camera1st->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "NeckSocket");
	
	SpringArm1 = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent-1"));
	SpringArm1->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "NeckSocket");
	SpringArm1->TargetArmLength = SpringArmLength;

	Camera3rd_1 = CreateDefaultSubobject<UCameraComponent>(TEXT("3rd_Camera-1"));
	Camera3rd_1->SetupAttachment(SpringArm1);

	SpringArm2 = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent-2"));
	SpringArm2->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "NeckSocket");
	SpringArm2->TargetArmLength = SpringArmLength*(-1);

	Camera3rd_2 = CreateDefaultSubobject<UCameraComponent>(TEXT("3rd_Camera_2"));
	Camera3rd_2->SetupAttachment(SpringArm2);

	SpringArm1->bUsePawnControlRotation = true;
	SpringArm2->bUsePawnControlRotation = true;
	Camera3rd_1->bUsePawnControlRotation = false;
	Camera3rd_1->bUsePawnControlRotation = false;
	Camera1st->bUsePawnControlRotation = true;

	bUseControllerRotationPitch = false;

	Light = CreateDefaultSubobject<URectLightComponent>(TEXT("Flashlight"));
	Light->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "FlashlightSocket");

	FlashlightSound1 = CreateDefaultSubobject<UAudioComponent>(TEXT("Flashlightaudio1"));
	FlashlightSound1->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "FlashlightSocket");
	FlashlightSound2 = CreateDefaultSubobject<UAudioComponent>(TEXT("Flashlightaudio2"));
	FlashlightSound2->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "FlashlightSocket");
	Light->SetIntensity(0.0f);
	Material1 = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/characters/NightGuard/assets/light"));
	Material2 = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/characters/NightGuard/assets/light1"));
}

//===============================================================================
//		PLAYER INPUT COMPONENT
void ANightGuard::SetupPlayerInputComponent(UInputComponent* MyPlayerInput)
{
	Super::SetupPlayerInputComponent(MyPlayerInput);


	MyPlayerInput->BindAxis("MoveForwardBackward", this, &ANightGuard::MoveForwardBackward);
	MyPlayerInput->BindAxis("MoveLeftRight", this, &ANightGuard::MoveLeftRight);

	MyPlayerInput->BindAxis("Turn", this, &ANightGuard::AddControllerYawInput);
	MyPlayerInput->BindAxis("LookUpDown", this, &ANightGuard::AddControllerPitchInput);

	MyPlayerInput->BindAction("Run",IE_Pressed, this, &ANightGuard::Run);
	MyPlayerInput->BindAction("Run", IE_Released, this, &ANightGuard::StopRunSelf);
	MyPlayerInput->BindAction("Crouch", IE_Pressed, this, &ANightGuard::CrouchFunc);

	MyPlayerInput->BindAction("Camera", IE_Pressed, this, &ANightGuard::ChangeCamera);
	MyPlayerInput->BindAction("Flashlight", IE_Pressed, this, &ANightGuard::ChangeFlashlight);
	MyPlayerInput->BindAction("Interaction", IE_Pressed, this, &ANightGuard::Interact);
}

//===============================================================================
//		MOVEMENT
void ANightGuard::MoveForwardBackward(float Value)
{
	if (Controller) {
		const FRotator Rotation = Controller->GetControlRotation();				/// Rotation - This gets character's current rotation.
		const FRotator Yaw(0, Rotation.Yaw, 0);									/// Yaw - This gets character's Yaw rotation.
		const FVector Direction = FRotationMatrix(Yaw).GetUnitAxis(EAxis::X);	/// Direction - This defines character to move on X-axis.
		AddMovementInput(Direction, Value);
	}
}

void ANightGuard::MoveLeftRight(float Value)
{
	if (Controller) {
		const FRotator Rotation = Controller->GetControlRotation();				/// Rotation - This gets character's current rotation.
		const FRotator Yaw(0, Rotation.Yaw, 0);									/// Yaw - This gets character's Yaw rotation.
		const FVector Direction = FRotationMatrix(Yaw).GetUnitAxis(EAxis::Y);	/// Direction - This defines character to move on X-axis.
		AddMovementInput(Direction, Value);
	}
}

void ANightGuard::CrouchFunc()
{
	if (Posture) {
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("Crouched")));
		this->Crouch(true);
	} else if (GetMesh()) {
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("UnCrouched")));
		this->UnCrouch(true);
	}
	Posture == 0 ? Posture = 1 : Posture = 0;
}

void ANightGuard::Run()
{
	if (GetInputAxisValue("MoveForwardBackward") > 0) {
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Run")));
		Server_SetIsRun(true);
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Do not run")));
		Server_SetIsRun(false);
	}
}

void ANightGuard::StopRun()
{
	Server_SetIsTired(true);
	Client_SetIsTired(true);
	Update_Movement_Speed();
}

void ANightGuard::StopRunSelf()
{
	Server_SetIsRun(false);
	Client_SetIsRun(false);
	Update_Movement_Speed();
}

//===============================================================================
//		INTERACTION
void ANightGuard::Interact()
{
	FVector StartLocation = GetActorLocation();			/// StartLocation - The begin of the LineTrace.
	FVector EndLocation = StartLocation + GetActorForwardVector() * LineTraceLength;	/// EndLocation - The end of the LineTrace.

	FHitResult HitResult;		/// HitResult - The pointer to what we hit with the LineTrace.
	FCollisionQueryParams CollisionParams;		/// CollisionParams - The params of collision.
	CollisionParams.AddIgnoredActor(this);
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Interacted")));
	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionParams)) {
		AClassicDoor* InteractableObject = Cast<AClassicDoor>(HitResult.GetActor());	/// InteractableObject - The cast of the door we hit with LineTrace.
		if (InteractableObject) {				// May be added more new conditions for more interactable objects.
	        InteractableObject->Actor = this;
	        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("Interacted with door")));
	        InteractableObject->Interaction();
		}
	}
}

//===============================================================================
//		FLASHLIGHT
void ANightGuard::ChangeFlashlight()
{
	if (FlashlightOn) {
		Light->SetIntensity(0.0f);
		GetMesh()->SetMaterial(1, Material2);
	    FlashlightSound1->Play();
	}
	else {
		Light->SetIntensity(FlashlightBrightness);
		GetMesh()->SetMaterial(1, Material1);
		FlashlightSound2->Play();
	}
	FlashlightOn = not FlashlightOn;
}

//===============================================================================
//		CHANGE CAMERA
void ANightGuard::ChangeCamera() {
	switch (CameraType)
	{
	case 0:
		CameraType = 1;
		Camera3rd_1->SetActive(true);
		Camera1st->SetActive(false);
		break;
	case 1:
		CameraType = 2;
		Camera3rd_2->SetActive(true);
		Camera3rd_1->SetActive(false);
		break;
	case 2:
		CameraType = 0;
		Camera1st->SetActive(true);
		Camera3rd_2->SetActive(false);
		break;
	default:
		CameraType = 0;
		Camera1st->SetActive(true);
		break;
	}
}

//===============================================================================
//		UNREAL ENGINE TICK
void ANightGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsRun && CurrentStamina > 0.0f && this->GetInputAxisValue("MoveForwardBackward") > 0) {
		DecreaseStamina();
	}
	else {
		if (!IsRun && CurrentStamina < Stamina) {
			IncreaseStamina();
		}
		if (IsRun and this->GetInputAxisValue("MoveForwardBackward") <= 0 and this->GetInputAxisValue("MoveLeftRight")) {
			StopRunSelf();
		}
		if (CurrentStamina < 1.0f) {
			Server_SetCurrentStamina(0.0f);
			StopRun();
		}
		else if (CurrentStamina == RecoveryStamina) {
			Server_SetIsTired(false);
			Server_SetCurrentStamina(RecoveryStamina);
		}
	}
}

//===============================================================================
//		STAMINA
void ANightGuard::DecreaseStamina()
{
	CurrentStamina -= MinusStamina;
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Stamina decreased -- %f"), CurrentStamina));
}


void ANightGuard::IncreaseStamina()
{
	CurrentStamina += PlusStamina;
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("Stamina increased -- %f"), CurrentStamina));
}

//===============================================================================
//		REPLICATION
void ANightGuard::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ANightGuard, IsRun, COND_None);
	DOREPLIFETIME_CONDITION(ANightGuard, IsTired, COND_None);
	DOREPLIFETIME_CONDITION(ANightGuard, CurrentStamina, COND_None);
}

//===============================================================================
//		MOVEMENT SPEED
void ANightGuard::Update_Movement_Speed()
{
	if (IsRun) {
		if (!IsTired) {
			GetCharacterMovement()->MaxWalkSpeed = 400.0f;
			GetCharacterMovement()->MaxWalkSpeedCrouched = 250.0f;
		}
		else {
			GetCharacterMovement()->MaxWalkSpeed = 125.0f;
			GetCharacterMovement()->MaxWalkSpeedCrouched = 75.0f;
		}
	}
	else {
		GetCharacterMovement()->MaxWalkSpeed = 100.0f;
		GetCharacterMovement()->MaxWalkSpeedCrouched = 50.0f;
	}
}

//===============================================================================
//		IS_RUN REPLICATION

void ANightGuard::Server_SetIsRun_Implementation(bool bNewIsRun)
{
	IsRun = bNewIsRun;
	Client_SetIsRun(bNewIsRun);
	Update_Movement_Speed();
}

bool ANightGuard::Server_SetIsRun_Validate(bool bNewIsRun)
{
	return true;
}

void ANightGuard::Client_SetIsRun_Implementation(bool bNewIsRun)
{
	IsRun = bNewIsRun;
	Update_Movement_Speed();
}

//===============================================================================
//		IS_TIRED REPLICATION

void ANightGuard::Server_SetIsTired_Implementation(bool bNewIsTired)
{
	IsTired = bNewIsTired;
	Client_SetIsTired(IsTired);
	Update_Movement_Speed();
}

bool ANightGuard::Server_SetIsTired_Validate(bool bNewIsTired)
{
	return true;
}

void ANightGuard::Client_SetIsTired_Implementation(bool bNewIsTired)
{
	IsTired = bNewIsTired;
	Update_Movement_Speed();
}

//===============================================================================
//		CURRENT_STAMINA REPLICATION

void ANightGuard::Server_SetCurrentStamina_Implementation(float fNewCurrentStamina)
{
	CurrentStamina = fNewCurrentStamina;
	Client_SetCurrentStamina(CurrentStamina);
}

bool ANightGuard::Server_SetCurrentStamina_Validate(float fNewCurrentStamina)
{
	return true;
}

void ANightGuard::Client_SetCurrentStamina_Implementation(float fNewCurrentStamina)
{
	CurrentStamina = fNewCurrentStamina;
}