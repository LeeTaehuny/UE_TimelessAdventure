#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "TA_ItemData.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
    Consumable,     // 소비
    Miscellaneous,  // 기타
};

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = "Item", BlueprintReadWrite)
    int32 ItemID;

    UPROPERTY(EditAnywhere, Category = "Item", BlueprintReadWrite)
    EItemType EItemType;

    UPROPERTY(EditAnywhere, Category = "Item", BlueprintReadWrite)
    FText ItemName;

    UPROPERTY(EditAnywhere, Category = "Item", BlueprintReadWrite)
    FText ItemDescription;

    UPROPERTY(EditAnywhere, Category = "Item", BlueprintReadWrite)
    UTexture2D* ItemThumbnail;

    UPROPERTY(EditAnywhere, Category = "Item", BlueprintReadWrite)
    int32 Price;

    UPROPERTY(EditAnywhere, Category = "Item", BlueprintReadWrite)
    bool bIsStackable;

    UPROPERTY(EditAnywhere, Category = "Item", BlueprintReadWrite)
    int32 MaxStackCount;

    UPROPERTY(EditAnywhere, Category = "Item", BlueprintReadWrite)
    float AddHPAmount;

    UPROPERTY(EditAnywhere, Category = "Item", BlueprintReadWrite)
    float AddStaminaAmount;
};