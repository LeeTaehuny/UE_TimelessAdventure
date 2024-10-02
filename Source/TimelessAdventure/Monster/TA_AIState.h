#pragma once

UENUM(BlueprintType)
enum class EBossState : uint8
{
	BS_Idle UMETA(DisplayName = "Idle"),
	BS_Attack UMETA(DisplayName = "Attack"),
	BS_Chase UMETA(DisplayName = "Chase"),
};