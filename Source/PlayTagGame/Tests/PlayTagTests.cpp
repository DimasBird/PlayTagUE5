// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayTagTests.h"
#include "PlayTagGame/Tests/PlayTagTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "PlayTagGame/NightGuard.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Tests/AutomationCommon.h"
#include "Tests/AutomationEditorCommon.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Editor/EditorEngine.h"
#include "EditorLevelUtils.h"
#include "LevelEditor.h"
#include "Net/UnrealNetwork.h"



/** \brief namespace of GetTestWorld()
*
* Testing method to get TestWorld
*/
namespace
{
    UWorld* GetTestGameWorld()
    {
        const TIndirectArray<FWorldContext>& WorldContexts = GEngine->GetWorldContexts();
        for (const FWorldContext& Context : WorldContexts)
        {
            if ((Context.WorldType == EWorldType::PIE) && Context.World())
            {
                return Context.World();
            }
        }

        return nullptr;
    }
}

/** \brief Level open class
*
* This class is used to find optional Level
*/
class LevelScope
{
public:
    LevelScope(const FString& MapName) { AutomationOpenMap(MapName); }
    ~LevelScope() { ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand); }
};


IMPLEMENT_SIMPLE_AUTOMATION_TEST(ANightGuardConstructorTest, "NightGuard.Constructor", EAutomationTestFlags::ApplicationContextMask
    | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);
IMPLEMENT_SIMPLE_AUTOMATION_TEST(ANightGuardCrouchTest1, "NightGuard.Crouch.Great", EAutomationTestFlags::ApplicationContextMask
    | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);
IMPLEMENT_SIMPLE_AUTOMATION_TEST(ANightGuardCrouchTest2, "NightGuard.Crouch.BadValue", EAutomationTestFlags::ApplicationContextMask
    | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::LowPriority);
IMPLEMENT_SIMPLE_AUTOMATION_TEST(ANightGuardFlashlightTest, "NightGuard.Flashlight", EAutomationTestFlags::ApplicationContextMask
    | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);
IMPLEMENT_SIMPLE_AUTOMATION_TEST(ANightGuardStaminaTest, "NightGuard.Stamina", EAutomationTestFlags::ApplicationContextMask
    | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);
IMPLEMENT_SIMPLE_AUTOMATION_TEST(ANightGuardCameraTest1, "NightGuard.Camera.Great", EAutomationTestFlags::ApplicationContextMask
    | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(ANightGuardCameraTest2, "NightGuard.Camera.BadValue", EAutomationTestFlags::ApplicationContextMask
    | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::LowPriority)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(ANightGuardMapTest, "NightGuard.MapCanBeOpened", EAutomationTestFlags::ApplicationContextMask
    | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FNightGuardTickTest1, "NightGuard.TickTest.FullCharge", EAutomationTestFlags::ApplicationContextMask
    | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FNightGuardTickTest2, "NightGuard.TickTest.TiredCharge", EAutomationTestFlags::ApplicationContextMask
    | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FNightGuardOnMapTest, "NightGuard.CheckOnMap", EAutomationTestFlags::ApplicationContextMask
    | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)


/** \brief Constructor Test
*
* This test checks if the objects were successfully created
*/
bool ANightGuardConstructorTest::RunTest(const FString& Parameters)
{
    ANightGuard* NightGuard = NewObject<ANightGuard>();

    if (!NightGuard) {
        return false;
    }

    TestNotNull(TEXT("Check if Character created"), NightGuard);

    TestNotNull(TEXT("Check if SpringArm1 created"), NightGuard->SpringArm1);
    TestNotNull(TEXT("Check if SpringArm2 created"), NightGuard->SpringArm2);
    TestNotNull(TEXT("Check if Camera1 created"), NightGuard->Camera1st);
    TestNotNull(TEXT("Check if Camera2 created"), NightGuard->Camera3rd_1);
    TestNotNull(TEXT("Check if Camera3 created"), NightGuard->Camera3rd_2);
    TestNotNull(TEXT("Check if RectLight created"), NightGuard->Light);
    TestNotNull(TEXT("Check if SoundObject1 created"), NightGuard->FlashlightSound1);
    TestNotNull(TEXT("Check if SoundObject2 created"), NightGuard->FlashlightSound2);
    TestNotNull(TEXT("Check if Material1 created"), NightGuard->Material1);
    TestNotNull(TEXT("Check if Material2 created"), NightGuard->Material2);
    return true;
}

/** \brief Crouch Test 1
*
* This test checks if Crouch method works great
*/
bool ANightGuardCrouchTest1::RunTest(const FString& Parameters)
{
    ANightGuard* NightGuard = NewObject<ANightGuard>();

    if (!NightGuard) {
        return false;
    }

    TestTrue(TEXT("Initial Posture should be true"), NightGuard->Posture == 1);

    NightGuard->CrouchFunc();
    TestTrue(TEXT("Posture should be false after first CrouchFunc call"), NightGuard->Posture == 0);

    NightGuard->CrouchFunc();
    TestTrue(TEXT("Posture should be true again after second CrouchFunc call"), NightGuard->Posture == 1);

    return true;
}

/** \brief Crouch Test 2
*
* This test checks if wrong values become correct
*/
bool ANightGuardCrouchTest2::RunTest(const FString& Parameters)
{
    ANightGuard* NightGuard = NewObject<ANightGuard>();

    if (!NightGuard) {
        return false;
    }

    NightGuard->Posture = 2;

    NightGuard->CrouchFunc();

    TestEqual(TEXT("Is current posture equal 0?"), NightGuard->Posture, 0);

    return true;
}

/** \brief Flashlight Test
*
* This test checks if the flashlight works great
*/
bool ANightGuardFlashlightTest::RunTest(const FString& Parameters)
{
    ANightGuard* NightGuard = NewObject<ANightGuard>();

    if (!NightGuard) {
        return false;
    }

    TestEqual(TEXT("Is current flashlight's off?"), NightGuard->Light->Intensity, 0.0f);

    NightGuard->ChangeFlashlight();
    TestEqual(TEXT("Is current flashlight's off?"), NightGuard->Light->Intensity, NightGuard->FlashlightBrightness);

    NightGuard->ChangeFlashlight();
    TestEqual(TEXT("Is current flashlight's back to original?"), NightGuard->Light->Intensity, 0.0f);

    return true;
}

/** \brief Stamina Test
*
* This test checks if Stamina add/remove system works great
*/
bool ANightGuardStaminaTest::RunTest(const FString& Parameters)
{
    ANightGuard* NightGuard = NewObject<ANightGuard>();

    TestTrue(TEXT("PlusStamina equals 1"), NightGuard->PlusStamina == 1.0f);
    TestTrue(TEXT("MinusStamina is integer"), static_cast<int>(NightGuard->MinusStamina) == NightGuard->MinusStamina);

    if (!NightGuard) {
        return false;
    }

    NightGuard->CurrentStamina = 150;

    for (int i = 0; i < 100; ++i) { NightGuard->DecreaseStamina(); }
    TestEqual(TEXT("Is CurrentStamina equals to 50.0f?"), NightGuard->CurrentStamina, 50.0f);

    for (int i = 0; i < 80; ++i) { NightGuard->IncreaseStamina(); }
    TestEqual(TEXT("Is CurrentStamina equals to 50.0f?"), NightGuard->CurrentStamina, 130.0f);

    return true;
}

/** \brief Camera Test 1
*
* This test checks if the camera works great
*/
bool ANightGuardCameraTest1::RunTest(const FString& Parameters)
{
    ANightGuard* NightGuard = NewObject<ANightGuard>();

    if (!NightGuard) {
        return false;
    }

    TestEqual(TEXT("Current camera is 0"), NightGuard->CameraType, 0);

    NightGuard->ChangeCamera();
    TestEqual(TEXT("Current camera is 0"), NightGuard->CameraType, 1);

    NightGuard->ChangeCamera();
    TestEqual(TEXT("Current camera is 2"), NightGuard->CameraType, 2);

    NightGuard->ChangeCamera();
    TestEqual(TEXT("Current camera is back to 0"), NightGuard->CameraType, 0);

    return true;
}

/** \brief Camera Test 2
*
* This test checks if the wrong values become correct
*/
bool ANightGuardCameraTest2::RunTest(const FString& Parameters)
{
    ANightGuard* NightGuard = NewObject<ANightGuard>();

    if (!NightGuard) {
        return false;
    }

    NightGuard->CameraType = 100;
    NightGuard->ChangeCamera();

    TestEqual(TEXT("Current camera is 0 as it should"), NightGuard->CameraType, 0);

    return true;
}

/** \brief Open Map Test
*
* This test checks if the map can be opened
*/
bool ANightGuardMapTest::RunTest(const FString& Parameters)
{
    LevelScope("/Game/maps/plain");
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.0f))
    return true;
}

/** \brief Tick Test 1
*
* This test checks if the Tick system works with not tired character
*/
bool FNightGuardTickTest1::RunTest(const FString& Parameters)
{
    ANightGuard* NightGuard = NewObject<ANightGuard>();

    TestEqual(TEXT("Initial Stamina should be equal to max Stamina"), NightGuard->CurrentStamina, NightGuard->Stamina);
    TestFalse(TEXT("Initial Tired state should be false"), NightGuard->IsTired);

    NightGuard->IsRun = true;
    NightGuard->CurrentStamina = NightGuard->RecoveryStamina + 10.0f;
    NightGuard->Tick(1.0f);
    TestEqual(TEXT("Stamina shouldn't increase when Run method is active"), NightGuard->CurrentStamina, NightGuard->RecoveryStamina + 10.0f);
    TestFalse(TEXT("Tired state should not change while running"), NightGuard->IsTired);

    NightGuard->IsRun = false;
    NightGuard->CurrentStamina = NightGuard->Stamina - 10.0f;
    NightGuard->Tick(1.0f);
    TestEqual(TEXT("Stamina should increase when not running"), NightGuard->CurrentStamina, NightGuard->Stamina - 10.0f + NightGuard->PlusStamina);
    TestFalse(TEXT("Tired state should not change while not running"), NightGuard->IsTired);

    return true;
}

/** \brief Tick Test 2
*
* This test checks if the Tick system works with tyred character
*/
bool FNightGuardTickTest2::RunTest(const FString& Parameters)
{
    ANightGuard* NightGuard = NewObject<ANightGuard>();

    NightGuard->CurrentStamina = NightGuard->RecoveryStamina + 10.0f;
    NightGuard->Tick(1.0f);
    TestFalse(TEXT("Tired state should be false when stamina is equal RecoveryStamina"), NightGuard->IsTired);

    return true;
}

/** \brief
*
*
*/
bool FNightGuardOnMapTest::RunTest(const FString& Parameters)
{
    LevelScope("/Game/maps/plain");
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.0f));
    GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Green, TEXT("You have 10 seconds to make tests"));
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(10.0f));
    return true;
}