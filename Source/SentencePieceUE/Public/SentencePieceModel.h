#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SentencePieceModel.generated.h"

/**
 * Represents a binary .model file imported into Unreal.
 */
UCLASS(BlueprintType)
class SENTENCEPIECEUE_API USentencePieceModel : public UObject
{
    GENERATED_BODY()

public:
    // The raw binary data of the .model file
    UPROPERTY()
    TArray<uint8> ModelData;

    // Optional: Store the JSON config here too so you have one asset for both
    UPROPERTY(EditAnywhere, Category = "Config", meta = (MultiLine = true))
    FString TokenizerConfigJson;
};