#pragma once

#include "CoreMinimal.h"
#include "TA_SlotType.generated.h"

UENUM(BlueprintType)
enum class ESlotType : uint8
{
    ST_Inventory_C,
    ST_Inventory_M,
    ST_QuickSlot,
    ST_Shop,
};