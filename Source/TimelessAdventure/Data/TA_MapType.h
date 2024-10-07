#pragma once

#include "CoreMinimal.h"
#include "TA_MapType.generated.h"

UENUM(BlueprintType)
enum class EMapType : uint8
{
	MT_Prairie, // 초원
	MT_Ruins,	// 유적
	MT_Cave,	// 동굴
};